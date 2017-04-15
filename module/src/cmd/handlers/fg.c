#include <asm/uaccess.h>
#include <cmd/def.h>
#include <cmd/protos.h>
#include <cmd/work.h>

int cmd_fg_handler(struct cmd_fg_args *args, struct cmd_fg_res *res)
{
	int r;
	struct cmd_work *work;

	lock_cmd_works();
	work = find_cmd_work_unsafe(args->uid);
	if (work == NULL) {
		unlock_cmd_works();
		pr_debug("no command with uid %u\n", args->uid);
		return -ESRCH;
	}
	register_cmd_work_cleaner_unsafe(work);
	unlock_cmd_works();

	flush_cmd_work(work);
	if (args->status != NULL) {
		r = copy_to_user(&work->status,
					args->status,
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
