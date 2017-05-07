#include <sys/ioctl.h>
#include <stdio.h>
#include <errno.h>
#include <cmd/protos.h>
#include <cmd/exec.h>
#include <utils/io.h>
#include <common/ioc.h>


int exec_cmd(const int fd, const struct cmd_params *params)
{
	int r;

	r = ioctl(fd, IOC_CMD, params);
	if (r != 0) {
		perror("ioctl");
		return errno;
	}

	if (params->asynchronous)
		return 0;

	switch (params->type) {
#define CMD(name, in, out)						\
	case CMD_TYPE(name):						\
		SHOW_CMD(name)(params->status->code,			\
				&params->status->res.name		\
		);							\
		break;
	CMD_TABLE
#undef CMD
	default :
		pr_error("something went wrong");
	}

	return 0;
}
