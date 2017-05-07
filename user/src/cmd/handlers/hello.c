#include <cmd/protos.h>
#include <utils/parse.h>
#include <utils/io.h>

ssize_t parse_cmd_hello(const char line[],
			const size_t n,
			struct cmd_hello_args *args)
{
	ssize_t read;
	size_t count = 0;
	int a;

	read = parse_some_space(line, n);
	if (read < 1) {
		pr_debug("spaces expected but '%s' found", line);
		return -P_UNEXP_CHAR;
	}
	count += read;

	read = parse_int(line + count, n - count, &a);
	if (read <= 0) {
		pr_debug("integer expected but '%s' found", line + count);
		return -P_UNEXP_CHAR;
	}
	count += read;

	args->val = a;

	return count;
}

void show_cmd_hello(const int code, const struct cmd_hello_res *res)
{
	if (code == 0)
		printf("hello: %d\n", res->val);
	else
		pr_error("oops, something went wrong");
}
