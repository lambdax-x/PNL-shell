#include <linux/uaccess.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <common/def.h>
#include <cmd/protos.h>
#include <linux/slab.h>

static char *envp[] = {
	"HOME=/",
	"TERM=linux",
	"PATH=/sbin:/usr/sbin:/bin:/usr/bin",
	NULL
};

int cmd_exec_handler(struct cmd_exec_args *args, struct cmd_exec_res *res)
{
	__label__ _free_name;
	struct subprocess_info *info;
	char *name;
	char *argv[2];

	name = kmalloc(sizeof(char) * (args->sizeName), GFP_KERNEL);
	if (copy_from_user(name, args->name, args->sizeName) != 0)
		return -EFAULT;

	name[args->sizeName] = 0;
	pr_debug("name : %s\n", name);
	argv[0] = name;
	argv[1] = NULL;

	info = call_usermodehelper_setup(argv[0], argv, envp, GFP_ATOMIC
			, NULL, NULL, NULL);
	if (!info)
		goto _free_name;

	return call_usermodehelper_exec(info, UMH_WAIT_PROC);

_free_name:
	kfree(name);
	return -ENOMEM;
}
