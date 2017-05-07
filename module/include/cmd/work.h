#ifndef CMD_WORK_H
#define CMD_WORK_H

#include <linux/workqueue.h>
#include <linux/wait.h>
#include <common/def.h>
#include <common/work.h>

struct cmd_work {
	workid_t uid;
	enum work_state_t state;

	struct cmd_params params;
	struct cmd_status status;

	struct work_struct ws;
	wait_queue_head_t wq_state;

	struct list_head list;

	atomic_t cleaners;
};
extern struct list_head works_list;
extern size_t works_count;

void destroy_cmd_works(void);
struct cmd_work *alloc_cmd_work(void);
void free_cmd_work(struct cmd_work *work);

int schedule_cmd_work(const struct cmd_params *user_params_addr);

void lock_cmd_works(void);
void unlock_cmd_works(void);

struct cmd_work *find_cmd_work_unsafe(const workid_t uid);

void register_cmd_work_cleaner_unsafe(struct cmd_work *work);
void unregister_cmd_work_cleaner_unsafe(struct cmd_work *work);

void flush_cmd_work(struct cmd_work *work);

#endif
