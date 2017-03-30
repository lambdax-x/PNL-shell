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
	, _(cmdid_t *cmd)						\
	, _(size_t size)						\
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
#define CMD(name, in, out)			\
	struct cmd_## name ## _args {		\
		int asynchronous;		\
		struct cmd_status *status;	\
		in				\
	};
#define _(field) field;
CMD_TABLE
#undef _
#undef CMD

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
	enum cmd_type type;
	int code;
	union cmd_res res;
};

/* ioctl(fd, code, args):
 * code: IOC_"name"
 * args: struct cmd_"name"_args *
 */
#define CMD(name, in, out) IOC_ ## name = \
	_IOWR(IOC_CMD_MAGIC, cmd_ ## name, struct cmd_ ## name ## _args *),
enum {
	CMD_TABLE
	IOC_max_code
};
#undef CMD

#endif
