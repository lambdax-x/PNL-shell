# Command kill

Send signal to a process. It's like the kill() system call. 
It can be used to send any signal to any process.

## Command

- input: 
	- integer signal, the signal 
	- pid_t pid, the pid of the process
- output: return value of the kernel handler.

## Execution

kill SIGNAL PID

## Example of output

```sh
[root@vm-nmv ~]# sleep 2000 &
[1] 313
[root@vm-nmv ~]# ./shell 
λ. kill 9 313
λ. 
[1]+  Killed                  sleep 2000
[root@vm-nmv ~]# 
```
