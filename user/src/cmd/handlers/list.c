#include <cmd/protos.h>
#include <utils/parse.h>
#include <utils/io.h>
#include <common/work.h>

#define SIZE 10

struct work_infos infos[SIZE];

ssize_t parse_cmd_list(const char line[],
		const size_t n,
		struct cmd_list_args *args)
{
	args->infos = infos;
	args->size = SIZE;
	return 0;
}

static inline void pr_work_header();
static inline void pr_work(struct work_infos *infos);

void show_cmd_list(const int code, const struct cmd_list_res *res)
{
	if (code < 0) {
		pr_error("could not get work list");
		return;
	}

	pr_work_header();
	for (unsigned int i = 0 ; i < code ; ++i)
		pr_work(infos + i);
}

static inline void pr_work_header()
{
	printf("uid\tasync\ttype\tstate\n");
}

static inline void pr_work(struct work_infos *infos)
{
	printf("%u\t", infos->uid);
	printf("%s\t", infos->asynchronous ? "true" : "false");

	switch (infos->type) {
#define CMD(name, in, out)						\
	case CMD_TYPE(name):						\
		printf(#name "\t");					\
		break;
	CMD_TABLE
#undef CMD
	default :
		printf("-\t");
	}

	switch (infos->state) {
	case work_registered:
		printf("registered");
		break;
	case work_running:
		printf("running");
		break;
	case work_terminated:
		printf("terminated");
		break;
	}

	printf("\n");
}
