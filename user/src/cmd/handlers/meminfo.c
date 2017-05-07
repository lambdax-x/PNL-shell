#include <cmd/protos.h>
#include <utils/parse.h>
#include <utils/io.h>
#include <sys/sysinfo.h>

struct sysinfo info;

ssize_t parse_cmd_meminfo(const char line[],
				const size_t n,
				struct cmd_meminfo_args *args)
{
	args->info = &info;
	return 0;
}

void show_cmd_meminfo(const int code, const struct cmd_meminfo_res *res)
{
	if (code == 0) {
		printf("[MEMINFO] MemTotal:	  %8lu kB\n", info.totalram);
		printf("[MEMINFO] MemFree:	  %8lu kB\n", info.freeram);
		printf("[MEMINFO] Buffers:	  %8lu kB\n", info.bufferram);
		/* printf("[MEMINFO] HighTotal:	  %8lu kB\n", info.totalhigh);
		 * printf("[MEMINFO] FreeHigh:	  %8lu kB\n", info.freehigh);
		 */
		printf("[MEMINFO] SwapTotal:	  %8lu kB\n", info.totalswap);
		printf("[MEMINFO] SwapFree:	  %8lu kB\n", info.freeswap);
		printf("[MEMINFO] SharedMem:	  %8lu kB\n", info.sharedram);
		printf("[MEMINFO] Memory unit:	  %8u kB\n", info.mem_unit);
	} else {
		pr_error("meminfo error");
	}
}

void show_cmd_meminfo_usage()
{
	printf("meminfo\n");
}

void show_cmd_meminfo_help()
{
}
