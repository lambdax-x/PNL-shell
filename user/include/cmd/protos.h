#ifndef CMD_PROTOS_H
#define CMD_PROTOS_H

#include <sys/types.h>
#include <common/def.h>

/* Parse the "name" command from line (length n) and fill its corresponding args
 * struct:
 * ssize_t parse_cmd_"name"(const char line[],
 *				const size_t n,
 *				struct cmd_"name"_args *args
 * );
 * it should return a negative integer on failure, and zero or a positive value
 * on success indicating the number of consumed characters
 */

/* Show the result of the "name" command:
 * void show_cmd_"name"(const int code, struct cmd_"name"_res *res);
 */

#define PARSE_CMD(name) parse_cmd_ ## name
#define SHOW_CMD(name) show_cmd_ ## name
#define CMD(name, in, out)						\
	ssize_t PARSE_CMD(name)(const char line[],			\
				const size_t n,				\
				struct CMD_ARGS(name) *args		\
	);								\
	void SHOW_CMD(name)(const int code, const struct CMD_RES(name) *res);
CMD_TABLE
#undef CMD

#endif
