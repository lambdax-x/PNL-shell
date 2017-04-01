#include <asm/uaccess.h>
#include <linux/types.h>
#include <cmd/def.h>
#include <cmd/protos.h>

int cmd_list_handler(struct cmd_list_args *args, struct cmd_list_res *res)
{
	pr_debug("in cmd_list_handler\n");
	return 0;
}
