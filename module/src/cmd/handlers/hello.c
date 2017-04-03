#include <asm/uaccess.h>
#include <linux/types.h>
#include <cmd/def.h>
#include <cmd/protos.h>

int cmd_hello_handler(struct cmd_hello_args *args, struct cmd_hello_res *res)
{
	pr_debug("begin of cmd_hello_handler\n");
	pr_info("hello world, arg.val=%d\n", args->val);
	res->val = 666;
	pr_debug("end of cmd_hello_handler\n");
	return 42;
}
