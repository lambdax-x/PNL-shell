# Do no use this Linux module

## About this project

Linux kernel programming project at UPMC. The goal is to schedule "commands" in
the kernel, from userspace.

## Features

- [x] generic implementation of commands (see documentation)
- [x] synchronous execution using wait queues
- [x] asynchronous execution
- [ ] slab allocator
- [x] user program (shell) interfacing the module

Implemented commands:

- hello
- sleep
- list
- fg
- kill

## Documentation

- commands explained: module/Documentation/commands.md
- implemented commands are described in module/Documentation/commands/

## Building

Dependencies: GNU readline for the user program.

```bash
# build everything:
make

# build the module:
make module

# build the user shell:
make user
```

It will produce:
- the module: `module/playground.ko`
- the shell: `user/bin/shell`
