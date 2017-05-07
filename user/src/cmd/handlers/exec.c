#include <stdlib.h>
#include <cmd/protos.h>
#include <utils/parse.h>
#include <utils/io.h>
#include <string.h>

char *name;

ssize_t parse_cmd_exec(const char line[],
			const size_t n,
			struct cmd_exec_args *args)
{
	ssize_t read;
	size_t count = 0;

	read = parse_some_space(line, n);

	if (read < 1) {
		pr_debug("spaces expected but '%s' found", line);
		return -P_UNEXP_CHAR;
	}
	count += read;

	name = malloc(sizeof(char) * (n-read));
	for (int i = 0 ; i < n - read ; i++)
		name[i] = line[i + read];
	name[n - read] = 0;
	count += n - read;

	args->name = name;
	args->sizeName = (count-read);

	return count;
}

void show_cmd_exec(const int code, const struct cmd_exec_res *res)
{
	free(name);
}
