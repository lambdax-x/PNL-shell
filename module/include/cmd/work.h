#ifndef WORK_H
#define WORK_H

#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/workqueue.h>

struct cmd_work {
	cmdid_t uid;
	enum cmd_type type;
	struct cmd_params params;
	struct cmd_status status;

	struct work_struct ws;
	struct list_head list;
};

struct cmd_works {
	struct mutex mutex;
	struct list_head first;
};
extern struct cmd_works cmd_works;

int schedule_cmd_work(const cmdid_t uid, const enum cmd_type type,
		const struct cmd_params *user_params_addr);
#endif
