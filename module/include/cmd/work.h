#ifndef WORK_H
#define WORK_H

#include <linux/workqueue.h>
#include <linux/wait.h>
#include <linux/kref.h>

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
	wait_queue_head_t wait_queue;
	struct list_head list;

	struct kref cleaners;
};

int schedule_cmd_work(const cmdid_t uid, const enum cmd_type type,
		const struct cmd_params *user_params_addr);

void lock_cmd_works();
void unlock_cmd_works();

struct cmd_work *find_cmd_work_unsafe(const cmdid_t uid);

void register_cmd_work_cleaner_unsafe(struct cmd_work *work);
void unregister_cmd_work_cleaner_unsafe(struct cmd_work *work);

void flush_cmd_work(struct cmd_work *work);

#endif
