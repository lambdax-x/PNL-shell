#ifndef CMD_PARSE_H
#define CMD_PARSE_H

#include <sys/types.h>
#include <common/def.h>

ssize_t parse_command(const char line[],
			const size_t n,
			struct cmd_params *params
);

#endif
