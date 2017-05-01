#include <linux/delay.h>
#include <cmd/protos.h>

int cmd_sleep_handler(struct cmd_sleep_args *args, struct cmd_sleep_res *res)
{
	ssleep(args->seconds);
	return 0;
}
