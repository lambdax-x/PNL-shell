#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cmd/def.h>
#include <sys/wait.h>

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

void modinfo(int fd){
	struct cmd_params params;
	struct cmd_status status;
	int result;
	int r;
	char name[50];

	printf("[MODINFO] module : \n");
	read(STDIN_FILENO, name, 50);

	params.asynchronous = 0;
	params.status = &status;
	params.args.modinfo.name = malloc(sizeof(char)*strlen(name));
	strcpy(params.args.modinfo.name,name);
	printf("[MODINFO] Nom du module : %s\n",params.args.modinfo.name);
	result = ioctl(fd, IOC_modinfo, &params);
	if ( result == -1 ) {
		perror("ioctl modinfo");
		exit(errno);
	}
	printf("[MODINFO] status code: %d\n", status.code);
	if ( status.code < 0 ){
		printf("error\n");
		exit(status.code);
	}
	printf("[MODINFO] result : %s\n",status.res.modinfo.info);

}

void kill_fct(int fd){

	struct cmd_params params;
	struct cmd_status status;
	int result;
	int pid;
	int signal;
	int r;
	
	printf("[KILL] signum : \n");
	scanf("%d",&signal);

	params.asynchronous = 0;
	params.status = &status;

	if((pid = fork()) != 0 ){
		params.args.kill.pid = pid;
		params.args.kill.sig = signal;
		result = ioctl(fd, IOC_kill, &params);
		if ( result == -1 ) {
			perror("ioctl modinfo");
			exit(errno);
		}
	}else{
		sleep(10);
		exit(0);
	}
	printf("[KILL] status code: %d\n", status.code);
	if ( status.code < 0 ){
		printf("[KILL] error\n");
		exit(status.code);
	}
	wait(NULL);
	printf("[KILL] result : %d\n",status.res.kill.status);

}

int main(int argc, char *argv[])
{
	int fd, r;

	fd = open("/dev/playground", O_RDWR);
	if (fd == -1) {
		perror("open");
		exit(errno);
	}

	hello(fd);	//Simple test
	list(fd);	//List test
	modinfo(fd);	//modinfo
	kill_fct(fd);	//kill

	close(fd);
	return EXIT_SUCCESS;
}
