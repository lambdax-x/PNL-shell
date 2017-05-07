#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <utils/io.h>
#include <utils/parse.h>
#include <cmd/parse.h>
#include <cmd/builtins.h>
#include <cmd/exec.h>

#define PROMPT "λ. "
#define DEVICE "/dev/playground"

int open_io_dev();
void close_io_dev(const int fd);
void help();

int main(int argc, char *argv[])
{
	__label__ _loop, _free, _out;
	
	struct cmd_params params;
	struct cmd_status status;
	ssize_t read;
	size_t length, count;
	char *line = NULL;
	int fd;

	fd = open_io_dev();

	params.status = &status;
_loop:
	line = readline(PROMPT);
	if (line == NULL)
		goto _out;
	length = strlen(line);
	if (length == 0)
		goto _free;
	add_history(line);
	count = 0;

	read = parse_exec_builtin(line, length);
	if (read > 0)
		goto _free;
	
	params.asynchronous = 0;
	read = parse_command(line, length, &params);
	if (read <= 0) {
		pr_error("invalid command");
		goto _free;
	}
	count += read;

	read = parse_many_space(line + count, length);
	if (count + read != length) {
		pr_error("'%s' has not been consumed", line + count);
		goto _free;
	}

	int r = exec_cmd(fd, &params);
	if (r != 0) {
		pr_error("oops, something went wrong");
		goto _free;
	}

_free:
	free(line);
	line = NULL;
	goto _loop;
_out:
	close_io_dev(fd);
	return EXIT_SUCCESS;
}

int open_io_dev()
{
	int fd;

	fd = open(DEVICE, O_RDWR);
	if (fd == -1) {
		perror("open");
		exit(errno);
	}

	return fd;
}

void close_io_dev(const int fd)
{
	if (close(fd) != 0) {
		perror("close");
		exit(errno);
	}
}

void help(){
        printf("LIST \n\tDisplay ids of running processes\n");
        printf("FG <id>\n\tBring process <id> in foreground\n");
        printf("KILL <signal> <pid>\n\tSend a signal <signal> to process <pid>\n");
        printf("WAIT <pid> [<pid> ...]\n\tWait until one of the processes finishes\n");
        printf("MEMINFO\n\tShows the memory state and usage\n");
        printf("MODINFO <name>\n\tShow information about the module <name>\n");
        printf("EXEC <path> \n\tExecute a user space program from the kernel space\n\n");
}
