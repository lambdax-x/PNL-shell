#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <cmd/def.h>
#include <cmd/work.h>
#include <cmd/protos.h>

LIST_HEAD(works_list);
DEFINE_MUTEX(works_lock);

void lock_cmd_works()
{
	mutex_lock(&works_lock);
}

void unlock_cmd_works()
{
	mutex_unlock(&works_lock);
}

static inline void insert_work(struct cmd_work *work)
{
	lock_cmd_works();
	list_add(&work->list, &works_list);
	unlock_cmd_works();
	pr_debug("command %u inserted\n", work->uid);
}

static inline void clean_work_unsafe(struct kref *kref)
{
	struct cmd_work *work = container_of(kref, struct cmd_work, cleaners);
	pr_debug("cleaning and freeing command %u\n", work->uid);
	list_del(&work->list);
	kfree(work);
}

struct cmd_work *find_cmd_work_unsafe(const cmdid_t uid)
{
	struct cmd_work *work;
	list_for_each_entry(work, &works_list, list)
		if (work->uid == uid)
			return work;
	return NULL;
}

void flush_cmd_work(struct cmd_work *work)
{
	pr_debug("waiting for command %u to terminate\n", work->uid);
	wait_event(work->wait_queue, work->state == work_terminated);
	pr_debug("command %u terminated\n", work->uid);
}

void register_cmd_work_cleaner_unsafe(struct cmd_work *work)
{
	kref_get(&work->cleaners);
}

void unregister_cmd_work_cleaner_unsafe(struct cmd_work *work)
{
	kref_put(&work->cleaners, clean_work_unsafe);
}

#define _CMD_HANDLER(name) _cmd_ ## name ## _handler
#define CMD(name, in, out)						\
static void _CMD_HANDLER(name)(struct work_struct *ws)			\
{									\
	struct cmd_work *work;						\
	union cmd_args *_args;						\
	union cmd_res *_res;						\
	struct cmd_ ## name ## _args *args;				\
	struct cmd_ ## name ## _res *res;				\
									\
	work = container_of(ws, struct cmd_work, ws);			\
	work->state = work_running;					\
	_args = &work->params.args;					\
	_res = &work->status.res;					\
	args = (struct cmd_ ## name ## _args *) &_args->name;		\
	res = (struct cmd_ ## name ## _res *) &_res->name;		\
									\
	pr_debug("starting command " #name " with uid %u\n", work->uid);\
	work->status.code = CMD_HANDLER(name, args, res);		\
	pr_debug("command " #name " with uid %u executed\n", work->uid);\
	work->state = work_terminated;					\
	wake_up(&work->wait_queue);					\
}
CMD_TABLE
#undef CMD

int schedule_cmd_work(const cmdid_t uid, const enum cmd_type type,
		const struct cmd_params *user_params_addr)
{
	struct cmd_work *work;

	work = kmalloc(sizeof(struct cmd_work), 0);
	if (!work)
		return -ENOMEM;

	if (copy_from_user(&work->params, user_params_addr,
				sizeof(struct cmd_params))) {
		kfree(work);
		return -EFAULT;
	}

	switch (type) {
#define CMD(name, in, out)						\
		case cmd_ ## name:					\
			INIT_WORK(&work->ws, _CMD_HANDLER(name));	\
			break;
		CMD_TABLE
#undef CMD
		default:
			kfree(work);
			return -EINVAL;
	}

	work->uid = uid;
	work->type = type;
	work->state = work_registered;
	init_waitqueue_head(&work->wait_queue);
	if (work->params.asynchronous)
		atomic_set(&work->cleaners.refcount, 0);
	else
		kref_init(&work->cleaners);

	insert_work(work);
	schedule_work(&work->ws);

	if (work->params.asynchronous)
		return 0;
	
	flush_cmd_work(work);
	if (work->params.status != NULL) {
		copy_to_user(&work->status,
					work->params.status,
					sizeof(struct cmd_status)
				);
	}
	lock_cmd_works();
	unregister_cmd_work_cleaner_unsafe(work);
	unlock_cmd_works();
	return 0;
}
