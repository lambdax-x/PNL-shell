#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <cmd/def.h>
#include <cmd/work.h>
#include <cmd/protos.h>

struct cmd_works cmd_works; 

static inline void insert_work(struct cmd_work *work)
{
	static int initialized = 0;

	if (unlikely(!initialized)) {
		mutex_init(&cmd_works.mutex);
		INIT_LIST_HEAD(&cmd_works.first);
		cmd_works.count = 0;
		initialized = 1;
	}

	mutex_lock(&cmd_works.mutex);
	list_add(&work->list, &cmd_works.first);
	cmd_works.count++;
	mutex_unlock(&cmd_works.mutex);
}

static inline void collect_work(struct cmd_work *work)
{
	mutex_lock(&cmd_works.mutex);
	list_del(&work->list);
	cmd_works.count--;
	mutex_unlock(&cmd_works.mutex);
}

static int exec_work(struct cmd_work *work)
{
	union cmd_args *args = &work->params.args;
	union cmd_res *res = &work->status.res;
	int code;

#define CMD(name, in, out)						\
	struct cmd_ ## name ## _args *name ## _args;			\
	struct cmd_ ## name ## _res *name ## _res;
	CMD_TABLE
#undef CMD

	work->state = work_running;
	switch (work->type) {
#define CMD(name, in, out)						\
		case cmd_ ## name:					\
			name ## _args = (struct cmd_ ## name ## _args *) &args->name;\
			name ## _res = (struct cmd_ ## name ## _res *) &res->name;\
			code = CMD_HANDLER(name,				\
					name ## _args,			\
					name ## _res			\
			);						\
		break;
		CMD_TABLE
		default:
			return -EINVAL;
#undef CMD
	};
	work->state = work_terminated;
	work->status.code = code;
	return 0;
}

int schedule_cmd_work(const cmdid_t uid, const enum cmd_type type,
		const struct cmd_params *user_params_addr)
{
	struct cmd_work *work;
	int r;

	work = kmalloc(sizeof(struct cmd_work), 0);
	if (!work)
		return -ENOMEM;

	work->uid = uid;
	work->type = type;
	work->state = work_registered;
	if (copy_from_user(&work->params, user_params_addr,
				sizeof(struct cmd_params))) {
		kfree(work);
		return -EFAULT;
	}
	insert_work(work);

	if (work->params.asynchronous) {
		/* asynchronous scheduling is not implemented yet
		 */
		return -EINVAL;
	}
	
	r = exec_work(work);
	if (r == 0 && work->params.status != NULL) {
		copy_to_user(work->params.status,
				&work->status,
				sizeof(struct cmd_status)
			);
	}
	collect_work(work);
	kfree(work);
	return 0;
}
