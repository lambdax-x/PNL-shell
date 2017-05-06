# Command lsmod
 
Show the status of modules in the linux Kernel.
Currently this command just print the module name, state and version.

## Definition of the command

This command is defined in `include/cmd/def.h`.
- input : nothing
- output : nothing

## Command execution

Just type "lsmod" in the shell.

## Example of output : 

λ. lsmod
[ 1346.016263] Module		State	Version
[ 1346.016402] playground	LIVE	1
[ 1346.016556] helloioctl	LIVE	(null)
[ 1346.016681] helloWorld	LIVE	(null)
[ 1346.016801] hellosysfs	LIVE	(null)
λ. 

