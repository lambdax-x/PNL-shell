#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cmd/def.h>

void hello_cmd(int fd)
{
	struct cmd_params params;
	struct cmd_status status;
	int r;

	params.asynchronous = 0;
	params.status = &status;
	params.args.hello.val = 666;
	r = ioctl(fd, IOC_hello, &params);
	if (r != 0) {
		perror("ioctl hello");
		exit(errno);
	}
}

void hello_cmd_async(int fd)
{
	struct cmd_params params;
	struct cmd_status fg_status;
	struct cmd_status hello_status;
	int r;

	params.asynchronous = 1;
	params.status = NULL;
	params.args.hello.val = 666;
	r = ioctl(fd, IOC_hello, &params);
	if (r != 0) {
		perror("ioctl hello");
		exit(errno);
	}

	params.asynchronous = 0;
	params.status = &fg_status;
	params.args.fg.uid = 0;
	params.args.fg.status = &hello_status;
	r = ioctl(fd, IOC_fg, &params);
	if (r != 0) {
		perror("ioctl fg");
		exit(errno);
	}
	if (fg_status.code != 0) {
		printf("command fg");
		exit(fg_status.code);
	}
	printf("%d\n", hello_status.res.hello.val);
}

void sleep_cmd(int fd)
{
	struct cmd_params params;
	struct cmd_status status;
	int r;

	params.asynchronous = 1;
	params.status = NULL;
	params.args.sleep.seconds = 8;
	r = ioctl(fd, IOC_sleep, &params);
	if (r != 0) {
		perror("ioctl sleep");
		exit(errno);
	}
}

int main(int argc, char *argv[])
{
	int fd, r;
		fd = open("/dev/playground", O_RDWR);
	if (fd == -1) {
		perror("open");
		exit(errno);
	}
	sleep_cmd(fd);
	close(fd);

	return EXIT_SUCCESS;
}
