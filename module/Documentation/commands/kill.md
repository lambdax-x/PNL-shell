# Command kill

Send signal to a process. It's like the kill() system call. 
It can be used to send any signal to any process.

## Definition of the command

This command is defined in `include/cmd/def.h`.
- input : 
	- integer signal, the signal 
	- pid_t pid, the pid of the process
- output : return value of the kernel handler.

## Command execution

Just type "kill signal pid" in the shell.

## Example of output : 

```sh
[root@vm-nmv ~]# sleep 2000 &
[1] 313
[root@vm-nmv ~]# ./shell 
λ. kill 9 313
λ. 
[1]+  Killed                  sleep 2000
[root@vm-nmv ~]# 
```


