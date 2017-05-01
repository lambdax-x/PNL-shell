#ifndef CMD_PROTOS_H
#define CMD_PROTOS_H

#include <common/def.h>

/* Definition of handlers of commands:
 * int cmd_"name"_handler(
 *	struct cmd_"name"_args *args,
 *	struct cmd_"name"_res *res
 * );
 */
#define CMD_HANDLER(name, in, out) cmd_ ## name ## _handler(in, out)
#define CMD(name, in, out) int CMD_HANDLER(name,			\
				struct CMD_ARGS(name) *args,		\
				struct CMD_RES(name) *res		\
);
CMD_TABLE
#undef CMD

#endif
