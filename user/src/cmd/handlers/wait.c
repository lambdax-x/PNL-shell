#include <cmd/protos.h>
#include <utils/parse.h>
#include <utils/io.h>

#define MAX_PIDS 10

pid_t pid[MAX_PIDS];

ssize_t parse_cmd_wait(const char line[],
			const size_t n,
			struct cmd_wait_args *args)
{
	__label__ _parsed;
	ssize_t read;
	size_t count = 0;
	size_t i;

	read = parse_some_space(line, n);
	if (read < 1) {
		pr_debug("spaces expected but '%s' found", line);
		return -P_UNEXP_CHAR;
	}
	count += read;

	read = parse_uint(line + count, n - count, (unsigned int *) &pid[0]);
	if (read < 1) {
		pr_debug("integer expected but '%s' found", line + count);
		return -P_UNEXP_CHAR;
	}
	count += read;

	for (i = 1 ; i < MAX_PIDS ; ++i) {
		read = parse_some_space(line + count, i - count);
		if (read < 1)
			goto _parsed;
		count += read;

		read = parse_uint(line + count,
					i - count,
					(unsigned int *) &pid[i]
		);
		if (read < 1)
			goto _parsed;
		count += read;
	}

_parsed:
	args->pid = pid;
	args->n = i;
	return count;
}

void show_cmd_wait(const int code, const struct cmd_wait_res *res)
{
}

void show_cmd_wait_usage()
{
	printf("wait PID [PID ...]\n");
}

void show_cmd_wait_help()
{
}
