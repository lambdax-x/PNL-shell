#ifndef WORK_H
#define WORK_H

#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/workqueue.h>

enum work_state {
	work_registered,
	work_running,
	work_terminated
};

struct cmd_work {
	cmdid_t uid;
	enum cmd_type type;
	enum work_state state;

	struct cmd_params params;
	struct cmd_status status;

	struct work_struct ws;
	struct list_head list;
};

struct cmd_works {
	size_t count;
	struct list_head first;
	struct mutex mutex;
};
extern struct cmd_works cmd_works;

int schedule_cmd_work(const cmdid_t uid, const enum cmd_type type,
		const struct cmd_params *user_params_addr);
#endif
