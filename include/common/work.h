#ifndef COMMON_WORK_H
#define COMMON_WORK_H

#include <common/def.h>

enum work_state_t {
	work_registered,
	work_running,
	work_terminated
};

struct work_infos {
	workid_t uid;
	enum work_state_t state;
	int asynchronous;
	enum cmd_type_t type;
};

#endif
