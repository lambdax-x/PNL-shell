#ifndef CMD_PROTOS_H
#define CMD_PROTOS_H

#include <cmd/def.h>

/* Definition of handlers for synchronous commands:
 * int cmd_"name"_handler(
 *	struct cmd_"name"_args *args,
 *	struct cmd_"name"_res *res
 * );
 */
#define CMD_HANDLER(name, in, out) cmd_ ## name ## _handler(in, out)

#define CMD(name, in, out) int CMD_HANDLER(name,			\
				struct cmd_ ## name ## _args *args,	\
				struct cmd_ ## name ## _res *res	\
				);
CMD_TABLE
#undef CMD

#endif
