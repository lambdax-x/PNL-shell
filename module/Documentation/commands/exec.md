# Command exec

Execute a user space program from the kernel space.

## Definition of the command

- input: 
	- a char pointer that represents the absolute path of the program
	- size of this path.
- output: nothing.

## Execution

exec PATH

## Example of output

```sh
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
```
