#ifndef COMMON_DEF_H
#define COMMON_DEF_H

#include <linux/types.h>

typedef unsigned int workid_t;

/* Definition of commands:
 * CMD(name
 * , _(input arg 1) _(input arg 2) ...
 * , _(output val 1) _(output val 2) ...
 * )
 */
#define CMD_TABLE							\
									\
	CMD(hello							\
	, _(int val)							\
	, _(int val)							\
	)								\
									\
	CMD(sleep							\
	, _(unsigned int seconds)					\
	,								\
	)								\
									\
	CMD(list							\
	, _(struct work_infos *infos) _(size_t size)			\
	,								\
	)								\
	CMD(modinfo							\
	, _(char *name) _(int sizeName) _(char *result) _(int sizeRes)	\
	,       	                                         	\
	)								\
									\
	CMD(meminfo							\
	, _(struct sysinfo *info) 					\
	, _(int val)							\
	)                                                               \
									\
	CMD(fg								\
	, _(workid_t uid) _(struct cmd_status *status)			\
	, _(int code)				                        \
	)								\
									\
									\
	CMD(kill							\
	, _(int signal) _(pid_t pid)					\
	,								\
	)                                                               \
	                                                                \
	CMD(exec                                                        \
	, _(char *name) _(int sizeName)                                 \
	,                                                               \
	)

#define CMD_TYPE(name) cmd_ ## name
/* Command identifier:
 * enum cmd_type_t {
 *	invalid_cmd,
 *	cmd_"name"
 *	...
 *	cmd_max_type
 */
enum cmd_type_t {
	invalid_cmd,
#define CMD(name, in, out) CMD_TYPE(name),
	CMD_TABLE
#undef CMD
	cmd_max_type
};

/* Command specific arguments:
 * struct cmd_"name"_args {
 *	int asynchronous;
 *	struct cmd_status *status;
 *	[type field;]
 * };
 */
#define CMD_ARGS(name) cmd_ ## name ## _args
#define CMD(name, in, out)						\
	struct CMD_ARGS(name) {						\
		in							\
	};
#define _(field) field;
CMD_TABLE
#undef _
#undef CMD

/* Union of possible command args:
 * union cmd_args {
 *	[struct cmd_"name"_args "name";]
 * };
 */
union cmd_args {
#define CMD(name, in, out) struct CMD_ARGS(name) name;
	CMD_TABLE
#undef CMD
};

/* Wrapper of struct cmd_"name"_args
 */
struct cmd_params {
	enum cmd_type_t type;
	int asynchronous;
	struct cmd_status *status;
	union cmd_args args;
};

/* Command result:
 * struct cmd_"name"_res {
 *	[type field;]
 * };
 */
#define CMD_RES(name) cmd_ ## name ## _res
#define CMD(name, in, out)						\
	struct CMD_RES(name) {						\
		out							\
	};
#define _(field) field;
CMD_TABLE
#undef _
#undef CMD

/* Union of possible command results:
 * union cmd_res {
 *	[struct cmd_"name"_res "name";]
 * };
 */
#define CMD(name, in, out) struct CMD_RES(name) name;
union cmd_res {
	CMD_TABLE
};
#undef CMD

/* Status of a command:
 */
struct cmd_status {
	int code;
	union cmd_res res;
};

#endif
