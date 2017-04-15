#include <asm/uaccess.h>
#include <cmd/def.h>
#include <cmd/protos.h>

int cmd_hello_handler(struct cmd_hello_args *args, struct cmd_hello_res *res)
{
	pr_info("hello world, val=%d\n", args->val);
	res->val = args->val + 1;
	return 0;
}
