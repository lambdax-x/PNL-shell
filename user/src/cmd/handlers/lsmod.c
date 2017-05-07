#include <cmd/protos.h>
#include <utils/parse.h>
#include <utils/io.h>
#include <sys/sysinfo.h>

ssize_t parse_cmd_lsmod(const char line[],
			const size_t n,
			struct cmd_lsmod_args *args)
{
	return 0;
}

void show_cmd_lsmod(const int code, const struct cmd_lsmod_res *res)
{
}
