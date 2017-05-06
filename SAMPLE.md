## Example for list command

```sh
λ. sleep 20 &
λ. sleep 25 &
λ. list
uid	async	type	state
20	false	list	running
19	true	sleep	running
18	true	sleep	running
λ. 
```

## Example for fg command

```sh
soon
```


## Example for kill command

```sh
[root@vm-nmv ~]# sleep 2000 &
[1] 313
[root@vm-nmv ~]# ./shell
λ. kill 9 313
λ.
[1]+  Killed                  sleep 2000
[root@vm-nmv ~]#
```

## Example for wait command

```sh
soon
```


## Example for meminfo command

```sh
λ. meminfo
[MEMINFO] MemTotal:        1012648 kB
[MEMINFO] MemFree:          922204 kB
[MEMINFO] Buffers:            8724 kB
[MEMINFO] SwapTotal:             0 kB
[MEMINFO] SwapFree:              0 kB
[MEMINFO] SharedMem:          6732 kB
[MEMINFO] Memory unit:       16384 kB
λ.
```

## Example for modinfo command

```sh
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
```

## Example for sleep command

```sh
λ. sleep 1
debug: command 'sleep' matched, parsing arguments...
debug: parsing command 'sleep' succeed
[ 2412.098509] command 2 inserted
[ 2412.098926] waiting for command 2 to terminate
[ 2412.099423] starting command 'sleep' with uid 2
[ 2413.102177] command 'sleep' with uid 2 terminated with code 0
[ 2413.102883] command 2 terminated
[ 2413.103147] cleaning and freeing command 2
λ. 
```

## Example for lsmod command

```sh
λ. lsmod
[ 1346.016263] Module           State   Version
[ 1346.016402] playground       LIVE    1
[ 1346.016556] helloioctl       LIVE    (null)
[ 1346.016681] helloWorld       LIVE    (null)
[ 1346.016801] hellosysfs       LIVE    (null)
λ.
```

## Example for exec command

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
