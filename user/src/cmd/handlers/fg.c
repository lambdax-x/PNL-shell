#include <cmd/protos.h>
#include <utils/parse.h>
#include <utils/io.h>

struct cmd_status status;

ssize_t parse_cmd_fg(const char line[],
			const size_t n,
			struct cmd_fg_args *args)
{
	ssize_t read;
	size_t count = 0;
	workid_t uid;

	read = parse_some_space(line, n);
	if (read < 1) {
		pr_debug("spaces expected but '%s' found", line);
		return -P_UNEXP_CHAR;
	}
	count += read;

	read = parse_uint(line + count, n - count, &uid);
	if (read <= 0) {
		pr_debug("int expected but '%s' found", line + count);
		return -P_UNEXP_CHAR;
	}
	count += read;

	args->uid = uid;
	args->status = &status;

	return count;
}

void show_cmd_fg(const int code, const struct cmd_fg_res *res)
{
	if (code != 0) {
		pr_error("something went wrong");
		return;
	}
}

void show_cmd_fg_usage()
{
	printf("fg cmdid\n");
}

void show_cmd_fg_help()
{
	printf("Move command to the foreground.\n");
}
