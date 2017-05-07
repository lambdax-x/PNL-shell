# You shall not use this Linux module

## About this project

Linux kernel programming project at UPMC. The goal is to schedule "commands" in
the kernel, from userspace.

### License

This project has been contaminated by the GPL.

## Features

- [x] generic implementation of commands (see documentation)
- [x] synchronous execution using wait queues
- [x] asynchronous execution
- [ ] slab allocator
- [x] user program (shell) interfacing the module

Implemented commands:

- hello
- sleep
- list
- fg
- kill
- wait
- meminfo
- modinfo
- exec
- lsmod

## Documentation

- [commands explained](module/Documentation/commands.md)
- [commands' documentation](module/Documentation/commands/)

## Building

Dependencies:
- GNU readline for the user program.
- Linux 4.2.3 patched (submodule)

```bash
# fetch kernel sources
git submodule init
git submodule update

# build everything:
make

# or build the module:
make module

# or build the user shell:
make user
```

It will produce:
- the module: `module/playground.ko`
- the shell: `user/bin/shell`
