#include <linux/slab.h>
#include <linux/pid.h>
#include <linux/completion.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <common/def.h>
#include <cmd/protos.h>

/* packed vector of struct completion
 */
struct completions {
	struct completion **v;
	size_t size;
};

/* Initialize a vector of wait_queue_t
 */
#define DECLARE_WAIT_QUEUES(name, task, n)				\
	wait_queue_t name[n];						\
	do {								\
		for (unsigned int i = 0 ; i < n ; ++i)			\
			init_waitqueue_entry(&name[i], task);		\
	} while (0)

/* check that at least n completions are done
 */
static int completions_done(const struct completions *completions, size_t n)
{
	size_t count = 0;

	for (unsigned int i = 0 ; i < n ; ++i) {
		struct completion *c = completions->v[i];

		if (c->done)
			++count;
	}
	return count >= n;
}

static void __sched __wait_for_completions(
		const struct completions *completions,
		const size_t n,
		const long timeout,
		const int state,
		long (*action)(long))
{
	might_sleep();
	long remains = timeout;
	DECLARE_WAIT_QUEUES(wait, current, n);

	for (unsigned int i = 0 ; i < n ; ++i) {
		struct completion *c = completions->v[i];

		spin_lock_irq(&c->wait.lock);
		__add_wait_queue_tail_exclusive(&c->wait, &wait[i]);
		spin_unlock_irq(&c->wait.lock);
	}

	do {
		if (signal_pending_state(state, current)) {
			remains = -ERESTARTSYS;
			break;
		}
		__set_current_state(state);
		remains = action(remains);
	} while (!completions_done(completions, n) && remains);

	for (unsigned int i = 0 ; i < n ; ++i) {
		struct completion *c = completions->v[i];

		spin_lock_irq(&c->wait.lock);
		__remove_wait_queue(&c->wait, &wait[i]);
		spin_unlock_irq(&c->wait.lock);
	}
}

static void __sched _wait_for_completions(const struct completions *completions,
						const size_t n,
						const long timeout,
						const int state)
{
	__wait_for_completions(completions,
				n,
				timeout,
				state,
				schedule_timeout
	);
}

static void __sched wait_for_completions(const struct completions *completions,
						const size_t n)
{
	_wait_for_completions(completions,
				n,
				MAX_SCHEDULE_TIMEOUT,
				TASK_UNINTERRUPTIBLE
	);
}

static void __sched wait_for_1_completion(const struct completions *completions)
{
	wait_for_completions(completions, 1);
}

int cmd_wait_handler(struct cmd_wait_args *args, struct cmd_wait_res *res)
{
	__label__ _put_task, _free_completions, _free_task, _free_pid;

	pid_t *pid;
	size_t pid_bytes;
	struct task_struct **task;
	struct completions completions;
	unsigned int i, j;
	int r;

	pid_bytes = sizeof(pid_t) * args->n;
	pid = kmalloc(pid_bytes, 0);
	if (!pid) {
		pr_debug("couldn't allocate memory for pids\n");
		return -ENOMEM;
	}

	task = kmalloc_array(args->n, sizeof(struct task_struct *), 0);
	if (!task) {
		r = -ENOMEM;
		goto _free_pid;
	}

	completions.v = kmalloc_array(args->n, sizeof(struct completion *), 0);
	if (!completions.v) {
		r = -ENOMEM;
		goto _free_task;
	}

	if (copy_from_user(pid, args->pid, pid_bytes)) {
		pr_debug("couldn't copy pid array from user to kernel\n");
		r = -EFAULT;
		goto _free_completions;
	}

	completions.size = args->n;
	for (i = 0 ; i < args->n ; i++) {
		rcu_read_lock();
		pr_debug("looking for task associated with pid %u\n", pid[i]);
		task[i] = find_task_by_vpid(pid[i]);
		if (task[i] == NULL) {
			pr_debug("no task found for pid=%u\n", pid[i]);
			rcu_read_unlock();
			r = -EINVAL;
			goto _put_task;
		}
		get_task_struct(task[i]);
		completions.v[i] = &task[i]->dead;
		rcu_read_unlock();
	}

	wait_for_1_completion(&completions);
	r = 0;
_put_task:
	for (j = 0 ; j < i ; j++)
		put_task_struct(task[j]);
_free_completions:
	kfree(completions.v);
_free_task:
	kfree(task);
_free_pid:
	kfree(pid);
	return r;
}
