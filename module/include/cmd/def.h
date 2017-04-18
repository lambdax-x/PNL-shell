#ifndef IOCTL_H
#define IOCTL_H

#define IOC_CMD_MAGIC 'N'
typedef unsigned int cmdid_t;

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
	CMD(list							\
	, _(cmdid_t *uid) _(size_t size)				\
	, _(size_t size)						\
	)								\
									\
	CMD(kill							\
	, _(int sig) _(int pid)						\
	, _(int status)							\
	)								\
									\
	CMD(modinfo							\
	, _(char *name) _(int sizeName) _(char *result) _(int sizeRes)	\
	, _(int code)	                                         	\
	)								\
									\
	CMD(meminfo							\
	, _(struct sysinfo *info) 					\
	, _(int val)							\
	)                                                               \
	                                                                \
        CMD(wait                                                        \
	, _(int nbPid) _(int *PIDS)                                     \
	, _(int valRet) _(int PID)                                      \
	)                                                       
/* Command identifier:
 * enum cmd_type {
 *	cmd_"name"
 *	...
 *	cmd_max_type
 */
#define CMD(name, in, out) cmd_ ## name,
enum cmd_type {
	CMD_TABLE
	cmd_max_type
};
#undef CMD

/* Command specific arguments:
 * struct cmd_"name"_args {
 *	int asynchronous;
 *	struct cmd_status *status;
 *	[type field;]
 * };
 */
#define CMD(name, in, out)						\
	struct cmd_## name ## _args {					\
		int asynchronous;					\
		struct cmd_status *status;				\
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
#define CMD(name, in, out) struct cmd_ ## name ## _args name;
	CMD_TABLE
#undef CMD
};

/* Wrapper of struct cmd_"name"_args
 */
struct cmd_params {
	int asynchronous;
	struct cmd_status *status;
	union cmd_args args;
};

/* Command result:
 * struct cmd_"name"_res {
 *	[type field;]
 * };
 */
#define CMD(name, in, out)		\
	struct cmd_## name ## _res {	\
		out			\
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
#define CMD(name, in, out) struct cmd_ ## name ## _res name;
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

/* ioctl(fd, code, params):
 * code: IOC_"name"
 * params: struct cmd_params *
 */
#define CMD(name, in, out) IOC_ ## name = \
	_IOWR(IOC_CMD_MAGIC, cmd_ ## name, struct cmd_params*),
enum {
	CMD_TABLE
	IOC_max_code
};
#undef CMD

#endif
