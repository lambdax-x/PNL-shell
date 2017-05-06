# Command meminfo

meminfo command reports some valuable information about the Linux system’s memory. 
Here is the brief description of what each value stands for:

MemTotal: Total usable RAM in kilobytes (i.e. physical memory minus a few reserved bytes and the kernel binary code)
MemFree: The amount of physical RAM left unused by the system.
Buffers: The amount of physical RAM used for file buffers.
SwapTotal: Total amount of physical swap memory.
SwapFree: Total amount of swap memory free.
SharedMem: Total used shared memory.
Memory unit: Memory unit size in kB. 

## Definition of the command

This command is defined in `include/cmd/def.h`.
- input : a sysinfo pointer
- output : return value of the command

## Command execution

Just type "meminfo" in the shell.

## Example of output : 

λ. meminfo
[MEMINFO] MemTotal:	   1012648 kB
[MEMINFO] MemFree:	    922204 kB
[MEMINFO] Buffers:	      8724 kB
[MEMINFO] SwapTotal:	         0 kB
[MEMINFO] SwapFree:	         0 kB
[MEMINFO] SharedMem:	      6732 kB
[MEMINFO] Memory unit:	     16384 kB
λ. 
