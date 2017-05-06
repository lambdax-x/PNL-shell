# Command kill

Execute a user space program from the kernel space.

## Definition of the command

This command is defined in `include/cmd/def.h`.
- input : 
	- a char pointer that represents the absolute path of the program
	- size of this path.
- output : nothing.

## Command execution

Just type "exec <path>" in the shell.

## Example of output : 

[root@vm-nmv ~]# cat /tmp/log
cat: /tmp/log: No such file or directory
[root@vm-nmv ~]# cat test.sh 
#!/bin/sh
echo "EXEC FROM KERNEL SPACE" > /tmp/log
[root@vm-nmv ~]# ./shell 
λ. exec /root/test.sh
λ. 
[root@vm-nmv ~]# cat /tmp/log 
EXEC FROM KERNEL SPACE



