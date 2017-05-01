#include <linux/fs.h>
#include <common/def.h>
#include <common/ioc.h>
#include <cmd/work.h>

long ioc(struct file *file, unsigned int seq, unsigned long addr)
{
	struct cmd_params *params;
	params = (struct cmd_params *) addr;
	return schedule_cmd_work(params);
}
