#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/list.h>
#include <cmd/def.h>
#include <cmd/protos.h>
#include <cmd/work.h>

static inline void show_cmd_work_infos(const struct cmd_work_infos *infos)
{
	pr_debug("uid=%u type=%u state=%u\n", infos->uid, infos->type,
			infos->state);
}

int cmd_list_handler(struct cmd_list_args *args, struct cmd_list_res *res)
{
	struct cmd_work *work;
	int r = 0;

	lock_cmd_works();
	if (works_count > args->n) {
		r = -ENOMEM;
		goto _unlock;
	}
	list_for_each_entry(work, &works_list, list) {
		show_cmd_work_infos(&work->infos);
		if (copy_to_user(args->infos + r, &work->infos,
					sizeof(struct cmd_work_infos))) {
			r = -EFAULT;
			break;
		}
		++r;
	}
_unlock:
	unlock_cmd_works();
	return r;
}
