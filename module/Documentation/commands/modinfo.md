# Command modinfo

Shows information about a Linux Kernel module. This command extracts information from the Linux Kernel module given on the command line.
By default this command lists each attribute of the module in form "param x : value". 

## Definition of the command

This command is defined in `include/cmd/def.h`.
- input : 
	- a char pointer that represents the name of the module. 
	- size of this name.
	- a char pointer for the result.
	- the size of this result.
- output : return value of the command.

## Command execution

Just type "modinfo <name>" in the shell.

## Example of output : 

[root@vm-nmv ~]# insmod helloWorld.ko 
[12250.384424] (0) Hello, default
[root@vm-nmv ~]# ./shell 
λ. modinfo helloWorld
[MODINFO] Status code: 0
[MODINFO]
name           : helloWorld
version        : (null)
load address   : ffff88003db87ba0
number param   : 2
param 0        : howmany
param 1        : whom
λ. 

