#include <utils/io.h>
#include <utils/parse.h>
#include <cmd/parse.h>
#include <cmd/protos.h>

#define _PARSE_FAILED_LABEL(name) _parse_failed_cmd_ ## name

ssize_t parse_command(const char line[],
			const size_t n,
			struct cmd_params *params
)
{
#define CMD(name, in, out)						\
	__label__ _PARSE_FAILED_LABEL(name);
	CMD_TABLE
#undef CMD
	__label__ _success;

#define CMD(name, in, out)						\
	struct CMD_ARGS(name) *name ## _args;
	CMD_TABLE
#undef CMD
	ssize_t read;
	size_t count = 0;
	size_t remaining = n;

	params->type = invalid_cmd;
	if (n == 0)
		return 0;

#define CMD(name, in, out)						\
	read = parse_string(line + count, remaining, #name );		\
	if (read <= 0)							\
		goto _PARSE_FAILED_LABEL(name);				\
	count += read;							\
	remaining -= read;						\
									\
	pr_debug("command '" #name "' matched, parsing arguments...");	\
	name ## _args = (struct CMD_ARGS(name) *) &params->args.name;	\
	read = PARSE_CMD(name)(line + count, remaining, name ## _args);	\
	if (read < 0) {							\
		pr_debug("failed");					\
		goto _PARSE_FAILED_LABEL(name);				\
	}								\
	count += read;							\
	remaining -= read;						\
	params->type = CMD_TYPE(name);					\
	pr_debug("parsing command '" #name "' succeed");		\
	goto _success;							\
_PARSE_FAILED_LABEL(name):
	CMD_TABLE
#undef CMD

	return -P_UNEXP_CHAR;

_success:
	read = parse_many_space(line + count, remaining);
	count += read;
	remaining -= read;

	/* check if command should be executed asynchronously
	 */
	read = parse_char(line + count, remaining, '&');
	if (read == 1) {
		pr_debug("command has to be executed asynchronously");
		params->asynchronous = 1;
		count += read;
		remaining -= read;
	}

	return count;
}
