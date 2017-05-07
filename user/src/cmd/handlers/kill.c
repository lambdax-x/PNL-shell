#include <cmd/protos.h>
#include <utils/parse.h>
#include <utils/io.h>

ssize_t parse_cmd_kill(const char line[],
			const size_t n,
			struct cmd_kill_args *args)
{
	ssize_t read;
	size_t count = 0;
	pid_t pid;
	int signal;

	read = parse_some_space(line, n);
	if (read < 1) {
		pr_debug("spaces expected but '%s' found", line);
		return -P_UNEXP_CHAR;
	}
	count += read;

	read = parse_uint(line + count, n - count, (unsigned int *) &signal);
	if (read <= 0) {
		pr_debug("int expected but '%s' found", line + count);
		return -P_UNEXP_CHAR;
	}
	count += read;

	read = parse_some_space(line, n);
	if (read < 1) {
		pr_debug("spaces expected but '%s' found", line + count);
		return -P_UNEXP_CHAR;
	}
	count += read;

	read = parse_uint(line + count, n - count, (unsigned int *) &pid);
	if (read <= 0) {
		pr_debug("int expected but '%s' found", line + count);
		return -P_UNEXP_CHAR;
	}
	count += read;

	args->signal = signal;
	args->pid = pid;

	return count;
}

void show_cmd_kill(const int code, const struct cmd_kill_res *res)
{
	if (code != 0)
		pr_error("no such pid");
}

void show_cmd_kill_usage()
{
	printf("kill SIGNAL PID\n");
}

void show_cmd_kill_help()
{
	printf("Send SIGNAL to PID.\n");
}
