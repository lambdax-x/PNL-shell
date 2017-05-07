#include <linux/uaccess.h>
#include <common/def.h>
#include <common/work.h>
#include <cmd/protos.h>
#include <cmd/work.h>

int cmd_fg_handler(struct cmd_fg_args *args, struct cmd_fg_res *res)
{
	int r;
	struct cmd_work *work;
	union cmd_args *this_args;
	struct cmd_params *this_params;
	struct cmd_work *this_work;

	/* fg command should not wait for itself
	 */
	this_args = container_of(args, union cmd_args, fg);
	this_params = container_of(this_args, struct cmd_params, args);
	this_work = container_of(this_params, struct cmd_work, params);
	if (args->uid == this_work->uid)
		return -EINVAL;

	/* find command and register to it as a cleaner
	 */
	lock_cmd_works();
	work = find_cmd_work_unsafe(args->uid);
	if (work == NULL) {
		unlock_cmd_works();
		pr_debug("no command with uid %u\n", args->uid);
		return -ESRCH;
	}
	register_cmd_work_cleaner_unsafe(work);
	unlock_cmd_works();

	/* wait for the command to terminate and copy its output
	 */
	flush_cmd_work(work);
	if (args->status != NULL) {
		r = copy_to_user(args->status,
					&work->status,
					sizeof(struct cmd_status)
				);
		if (r)
			r = -EFAULT;
	}
	res->code = work->status.code;

	lock_cmd_works();
	unregister_cmd_work_cleaner_unsafe(work);
	unlock_cmd_works();
	return 0;
}
