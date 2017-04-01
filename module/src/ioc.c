#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/mutex.h>
#include <ioc.h>
#include <cmd/def.h>
#include <cmd/work.h>

static inline cmdid_t atomic_next_uid(void)
{
	static int initialized = 0;
	static cmdid_t next_uid = 0;
	static struct mutex uid_lock;
	cmdid_t uid;

	if (unlikely(!initialized)) {
		mutex_init(&uid_lock);
		initialized = 1;
	}

	mutex_lock(&uid_lock);
	uid = next_uid;
	++next_uid;
	mutex_unlock(&uid_lock);
	return uid;
}

long ioc(struct file *file, unsigned int seq, unsigned long addr)
{
	struct cmd_params *params;
	enum cmd_type type;
	cmdid_t uid; 

	uid = atomic_next_uid();
	params = (struct cmd_params *) addr;

	switch (seq)
	{
#define CMD(name, in, out)						\
		case IOC_ ## name:					\
			type = cmd_ ## name;				\
			break;
		CMD_TABLE
#undef CMD
		default:
			return -EINVAL;
	}

	return schedule_cmd_work(uid, type, params);
}

