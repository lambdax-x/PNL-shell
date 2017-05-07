#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <modinfo.h>
#include <ioc.h>

#define DEV_MAJOR 0
#define DEV_NAME "playground"

const struct file_operations f_ops = {
	.unlocked_ioctl = ioc
};

int major;

static int __init playground_init(void)
{
	int r;

	pr_debug("playground module initialization\n");

	r = register_chrdev(DEV_MAJOR, DEV_NAME, &f_ops);
	if (r < 0) {
		pr_err("register_chrdev failed with code %d\n", r);
		return r;
	}
	major = r;

	pr_debug("playground module initialized\n");
	return 0;
}
module_init(playground_init);

static void __exit playground_exit(void)
{
	unregister_chrdev(major, DEV_NAME);
	pr_debug("playground_exit\n");
}
module_exit(playground_exit);
