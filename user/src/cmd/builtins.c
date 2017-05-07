#include <stdio.h>
#include <utils/parse.h>
#include <cmd/builtins.h>
#include <cmd/protos.h>
#include <common/def.h>

void show_help_usage()
{
	printf("help <cmd>\n");
}

void show_help()
{
	show_help_usage();
#define CMD(name, in, out)						\
	SHOW_CMD_USAGE(name)();
	CMD_TABLE
#undef CMD
}

ssize_t parse_exec_help(const char line[], size_t n)
{
#define _PARSE_FAILED_LABEL(name) _parse_failed_cmd_ ## name
#define CMD(name, in, out)						\
	__label__  _PARSE_FAILED_LABEL(name);
	CMD_TABLE
#undef CMD

	ssize_t read;
	size_t count = 0;

	read = parse_string(line, n, "help");
	if (read < 0)
		return read;
	count += read;

	read = parse_many_space(line + count, n - count);
	count += read;

	if (count == n) {
		show_help();
		return count;
	}

#define CMD(name, in, out)						\
	read = parse_string(line + count, n - count, #name);		\
	if (read < 0)							\
		goto _PARSE_FAILED_LABEL(name);				\
	count += read;							\
	SHOW_CMD_USAGE(name)();						\
	SHOW_CMD_HELP(name)();						\
	return count;							\
_PARSE_FAILED_LABEL(name) :
	CMD_TABLE
#undef CMD
	return -P_UNEXP_CHAR;
}

ssize_t parse_exec_builtin(const char line[], size_t n)
{
	return parse_exec_help(line, n);
}

