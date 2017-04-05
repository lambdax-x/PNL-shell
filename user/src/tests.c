#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cmd/def.h>

void hello(int fd)
{
	struct cmd_params params;
	struct cmd_status status;
	int r;

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
}

void list(int fd)
{
	struct cmd_params params;
	struct cmd_status status;
	cmdid_t uid[10];
	int r;

	params.asynchronous = 0;
	params.status = &status;
	params.args.list.uid = uid;
	params.args.list.size = 10;
	r = ioctl(fd, IOC_list, &params);
	if (r == -1) {
		perror("ioctl list");
		exit(errno);
	}
	printf("status code: %d\n", status.code);
	if (status.code < 0) {
		printf("error\n");
		exit(status.code);
	}
	printf("%u commands:\n", status.res.list.size);
	for (unsigned int i = 0 ; i < status.res.list.size ; ++i)
		printf("uid=%u\n", uid[i]);
}

void modinfo(int fd, char *name){

	struct cmd_params params;
	struct cmd_status status;
	int result;
	int r;
	params.asynchronous = 0;
	params.status = &status;
	params.args.modinfo.name = malloc(sizeof(char)*strlen(name));
	strcpy(params.args.modinfo.name,name);
	result = ioctl(fd, IOC_modinfo, &params);
	if ( r == -1 ) {
		perror("ioctl modinfo");
		exit(errno);
	}
	printf("status code: %d\n", status.code);
	if ( status.code < 0 ){
		printf("error\n");
		exit(status.code);
	}
	printf("result : %s\n", status.res.modinfo.info);
}

int main(int argc, char *argv[])
{
	int fd, r;
		fd = open("/dev/playground", O_RDWR);
	if (fd == -1) {
		perror("open");
		exit(errno);
	}
	hello(fd);
	list(fd);
	modinfo(fd,"helloWorld");
	close(fd);

	return EXIT_SUCCESS;
}
