# Command fg

Put a command to the foreground and get its result. It can be used to wait for
a synchonous command too, `fg` acts like a synchronization tool.

## Definition of the command

- input:
    - uid of the command to collect
    - address of the status struct where the result should be copied
- output : 
    - status code of the collected command

## Execution

fg UID
