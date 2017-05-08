#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <cmd/work.h>
#include <cmd/protos.h>

static struct kmem_cache *cmd_work_memcache;
static struct mutex works_lock;

LIST_HEAD(works_list);
size_t works_count;

static inline workid_t atomic_next_uid(void)
{
	static int initialized;
	static workid_t next_uid;
	static struct mutex uid_lock;
	workid_t uid;

	if (unlikely(!initialized)) {
		mutex_init(&uid_lock);
		next_uid = 0;
		initialized = 1;
	}

	mutex_lock(&uid_lock);
	uid = next_uid;
	++next_uid;
	mutex_unlock(&uid_lock);
	return uid;
}

void lock_cmd_works(void)
{
	static int mutex_initialized;

	if (unlikely(!mutex_initialized)) {
		mutex_init(&works_lock);
		mutex_initialized = 1;
	}

	mutex_lock(&works_lock);
}

void unlock_cmd_works(void)
{
	mutex_unlock(&works_lock);
}

static inline void insert_work(struct cmd_work *work)
{
	lock_cmd_works();
	list_add(&work->list, &works_list);
	works_count++;
	unlock_cmd_works();
	pr_debug("command %u inserted\n", work->uid);
}

static inline void clean_work_unsafe(struct cmd_work *work)
{
	pr_debug("cleaning and freeing command %u\n", work->uid);
	list_del(&work->list);
	works_count--;
	free_cmd_work(work);
}

struct cmd_work *find_cmd_work_unsafe(const workid_t uid)
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
	wait_event(work->wq_state, work->state == work_terminated);
	pr_debug("command %u terminated\n", work->uid);
}

void register_cmd_work_cleaner_unsafe(struct cmd_work *work)
{
	atomic_inc(&work->cleaners);
}

void unregister_cmd_work_cleaner_unsafe(struct cmd_work *work)
{
	if (atomic_sub_and_test(1, &work->cleaners))
		clean_work_unsafe(work);
}

/* void _cmd_"name"_handler(struct work_struct *ws)
 * is the cmd_"name"'s work_struct handler: it unwraps the cmd_work struct from
 * it and calls the cmd_"name"_handler
 */
#define _CMD_HANDLER(name) _cmd_ ## name ## _handler
#define CMD(name, in, out)						\
static void _CMD_HANDLER(name)(struct work_struct *ws)			\
{									\
	struct cmd_work *work;						\
	struct CMD_ARGS(name) *args;					\
	struct CMD_RES(name) *res;					\
									\
	work = container_of(ws, struct cmd_work, ws);			\
	work->state = work_running;					\
	args = (struct CMD_ARGS(name) *) &work->params.args.name;	\
	res = (struct CMD_RES(name) *) &work->status.res.name;		\
									\
	pr_debug("starting command '" #name "' with uid %u\n",		\
			work->uid);					\
	work->status.code = CMD_HANDLER(name, args, res);		\
	pr_debug("command '" #name "' with uid %u terminated with code %u\n",\
			work->uid,					\
			work->status.code				\
	);								\
	work->state = work_terminated;					\
	wake_up(&work->wq_state);					\
}
CMD_TABLE
#undef CMD

static inline void init_cmd_work_allocator(void)
{
	cmd_work_memcache = KMEM_CACHE(cmd_work, 0);
}

void destroy_cmd_works(void)
{
}

struct cmd_work *alloc_cmd_work(void)
{
	return kmem_cache_alloc(cmd_work_memcache, 0);
}

void free_cmd_work(struct cmd_work *work)
{
	kmem_cache_free(cmd_work_memcache, work);
}

int schedule_cmd_work(const struct cmd_params *user_params_addr)
{
	static int initialized;
	struct cmd_work *work;

	if (unlikely(!initialized)) {
		init_cmd_work_allocator();
		initialized = 1;
	}

	work = alloc_cmd_work();
	if (!work)
		return -ENOMEM;

	if (copy_from_user(&work->params, user_params_addr,
				sizeof(struct cmd_params))) {
		free_cmd_work(work);
		return -EFAULT;
	}

	switch (work->params.type) {
#define CMD(name, in, out)						\
	case CMD_TYPE(name):						\
		INIT_WORK(&work->ws, _CMD_HANDLER(name));		\
		break;
	CMD_TABLE
#undef CMD
	default:
		free_cmd_work(work);
		return -EINVAL;
	}

	work->uid = atomic_next_uid();
	work->state = work_registered;
	init_waitqueue_head(&work->wq_state);

	if (work->params.asynchronous)
		atomic_set(&work->cleaners, 0);
	else
		atomic_set(&work->cleaners, 1);

	insert_work(work);
	schedule_work(&work->ws);

	if (work->params.asynchronous)
		return 0;

	flush_cmd_work(work);
	if (work->params.status != NULL) {
		copy_to_user(work->params.status,
					&work->status,
					sizeof(struct cmd_status)
				);
	}
	lock_cmd_works();
	unregister_cmd_work_cleaner_unsafe(work);
	unlock_cmd_works();
	return 0;
}
