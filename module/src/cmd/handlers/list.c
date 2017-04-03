#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <cmd/def.h>
#include <cmd/protos.h>
#include <cmd/work.h>

int cmd_list_handler(struct cmd_list_args *args, struct cmd_list_res *res)
{
	cmdid_t *uid;
	unsigned int i;
	struct cmd_work *w;
	size_t count, size;
	int r;
	
	pr_debug("begin of cmd_list_handler\n");
	mutex_lock(&cmd_works.mutex);
	if (cmd_works.count > args->size) {
		r = -ENOMEM;
		goto _unlock;
	}

	count = cmd_works.count;
	size = sizeof(cmdid_t) * count;
	uid = kmalloc(size, 0);
	if (!uid) {
		r = -ENOMEM;
		goto _unlock;
	}

	i = 0;
	list_for_each_entry(w, &cmd_works.first, list)
		uid[i++] = w->uid;
	mutex_unlock(&cmd_works.mutex);
	if (copy_to_user(args->uid, uid, size)) {
		r = -EFAULT;
	} else {
		r = 0;
		res->size = count;
	}
	kfree(uid);
	return r;
_unlock:
	mutex_unlock(&cmd_works.mutex);
	pr_debug("end of cmd_list_handler\n");
	return r;
}
