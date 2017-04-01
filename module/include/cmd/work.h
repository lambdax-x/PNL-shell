#ifndef WORK_H
#define WORK_H

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
	struct cmd_work first;
};

int schedule_cmd_work(const cmdid_t uid, const enum cmd_type type,
		const struct cmd_params *user_params_addr);
#endif
