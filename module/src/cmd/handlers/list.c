#include <asm/uaccess.h>
#include <linux/types.h>
#include <cmd/def.h>
#include <cmd/protos.h>
#include <cmd/work.h>

int cmd_list_handler(struct cmd_list_args *args, struct cmd_list_res *res)
{
	struct cmd_work *w;	
	pr_debug("in cmd_list_handler\n");

	list_for_each_entry(w, &cmd_works.first, list) {
		pr_debug("work uid=%u type=%d asynch=%d\n",
				w->uid, w->type, w->params.asynchronous);
	}

	return 0;
}
