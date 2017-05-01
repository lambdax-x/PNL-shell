#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/list.h>
#include <common/def.h>
#include <common/work.h>
#include <cmd/protos.h>
#include <cmd/work.h>

static inline void show_cmd_work_infos(const struct work_infos *infos)
{
	pr_debug("uid=%u type=%u state=%u\n", infos->uid, infos->type,
			infos->state);
}

int cmd_list_handler(struct cmd_list_args *args, struct cmd_list_res *res)
{
	__label__ _unlock;
	struct cmd_work *work;
	struct work_infos infos;
	int r = 0;

	lock_cmd_works();
	if (works_count > args->size) {
		r = -ENOMEM;
		goto _unlock;
	}
	list_for_each_entry(work, &works_list, list) {
		infos.uid = work->uid;
		infos.state = work->state;
		infos.asynchronous = work->params.asynchronous;
		infos.type = work->params.type;
		show_cmd_work_infos(&infos);
		if (copy_to_user(args->infos + r, &infos,
					sizeof(struct work_infos))) {
			r = -EFAULT;
			break;
		}
		++r;
	}
_unlock:
	unlock_cmd_works();
	return r;
}
