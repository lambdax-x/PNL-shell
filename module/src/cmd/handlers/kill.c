#include <linux/sched.h>
#include <asm/uaccess.h>
#include <common/def.h>
#include <cmd/protos.h>

int cmd_kill_handler(struct cmd_kill_args *args, struct cmd_kill_res *res)
{
	struct pid *pid;

	pid = find_get_pid(args->pid);
	if (pid == NULL)
		return -ESRCH;
	kill_pid(pid, args->signal, 1);
	put_pid(pid);
	return 0;
}
