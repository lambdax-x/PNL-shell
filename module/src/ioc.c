#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/mutex.h>
#include <ioc.h>
#include <cmd/def.h>
#include <cmd/protos.h>

static inline cmdid_t atomic_next_uid(void)
{
	static int initialized = 0;
	static cmdid_t next_uid = 0;
	static struct mutex uid_lock;
	cmdid_t uid;

	if (unlikely(!initialized))
		mutex_init(&uid_lock);

	mutex_lock(&uid_lock);
	uid = next_uid;
	++next_uid;
	mutex_unlock(&uid_lock);
	return uid;
}

long ioc(struct file *file, unsigned int seq, unsigned long addr)
{
	struct cmd_status status;
	struct cmd_status *user_status;
	int r;
	cmdid_t uid; 

	uid = atomic_next_uid();

	switch (seq)
	{
#define CMD(name, in, out)						\
		case IOC_ ## name: ;{					\
			struct cmd_ ## name ## _args args;		\
			r = copy_from_user(&args,			\
				(struct cmd_ ## name ## _args *) addr,	\
				sizeof(struct cmd_##name##_args)	\
			);						\
			if (r)						\
				return -EFAULT;				\
			user_status = args.status;			\
			status.type = cmd_##name;			\
			status.code = CMD_HANDLER(name,			\
					&args, &status.res.name		\
			);}						\
			break;
		CMD_TABLE
#undef CMD
		default:
			return -EINVAL;
	}
	r = copy_to_user(user_status, &status, sizeof(struct cmd_status));
	return 0;
}

