#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cmd/def.h>

int main(int argc, char *argv[])
{
	int fd, r;
	struct cmd_params params;
	struct cmd_status status;

	fd = open("/dev/playground", O_RDWR);
	if (fd == -1) {
		perror("open");
		exit(errno);
	}
	
	params.asynchronous = 0;
	params.status = &status;
	params.args.hello.val = 23;
	r = ioctl(fd, IOC_hello, &params);
	if (r == -1) {
		perror("ioctl hello");
		exit(errno);
	}

	printf("status code: %d\n", status.code);
	printf("hello ret: %d\n", status.res.hello.val);

	close(fd);

	return EXIT_SUCCESS;
}
