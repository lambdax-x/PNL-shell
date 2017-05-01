#include <cmd/protos.h>
#include <utils/parse.h>
#include <utils/io.h>

ssize_t parse_cmd_sleep(const char line[],
			const size_t n,
			struct cmd_sleep_args *args)
{
	ssize_t read;
	size_t count = 0;

	read = parse_some_space(line, n);
	if (read < 1) {
		pr_debug("spaces expected but '%s' found", line);
		-P_UNEXP_CHAR;
	}
	count += read;

	read = parse_int(line + count, n - count, &args->seconds);
	if (read < 1) {
		pr_debug("int expected but '%s' found", line);
		return -P_UNEXP_CHAR;
	}
	count += read;

	return count;
}

void show_cmd_sleep(const int code, const struct cmd_sleep_res *res)
{
}
