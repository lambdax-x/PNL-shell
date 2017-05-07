#include <stdlib.h>
#include <cmd/protos.h>
#include <utils/parse.h>
#include <utils/io.h>
#include <string.h>

char *name;
char *result;

ssize_t parse_cmd_modinfo(const char line[],
		const size_t n,
		struct cmd_modinfo_args *args)
{
	ssize_t read;
	size_t count = 0;

	read = parse_many_space(line, n);
	if (read < 1) {
		pr_debug("spaces expected but '%s' found", line);
		return -P_UNEXP_CHAR;
	}
	count += read;

	name = malloc(sizeof(char) * (n - count));
	for (int i = 0 ; i < n - count ; i++)
		name[i] = line[i+count];
	name[n - count] = 0;
	count += n - count;

	args->name = name;
	args->sizeName = (count-read);
	result = malloc(sizeof(char) * 512);
	args->result = result;
	args->sizeRes = 512;

	return count;
}

void show_cmd_modinfo(const int code, const struct cmd_modinfo_res *res)
{
	printf("[MODINFO] Status code: %d\n", code);
	if (code < 0) {
		printf("error\n");
		return;
	}
	printf("[MODINFO]\n");
	printf("%s", result);

	free(name);
	free(result);
}

void show_cmd_modinfo_usage()
{
	printf("modinfo MODULE_NAME\n");
}

void show_cmd_modinfo_help()
{
}
