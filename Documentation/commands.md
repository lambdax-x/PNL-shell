# Commands

Commands are tasks scheduled in the Linux kernel. They can be blocking or
executed asynchronously. A user program can schedule a command from a `ioctl`
call.

## Define a new command

Command data structures are mostly defined in `include/cmd/def.h`. They are
generated by the preprocessor. A command is defined by four parts:
- a name
- some inputs - packed in a `struct`
- some outputs - packed in a `struct`
- a handler

The file `common/def.h` contains X-macros that generates the first three.
In order to generate them for a new command, a specific entry must be added to
the `CMD_TABLE` table. Such an entry looks like the following:

```C
#define CMD_TABLE       \
    ...                 \
    CMD(add             \
    , _(int a) _(int b) \
    , _(int sum)        \
    )                   \
    ...
```

Two major data structures will be generated from this definition. The argument
(or input) data structure:

```C
struct cmd_add_args {
    int a;
    int b;
};
```

and the result (or output) data structure:

```C
struct cmd_add_res {
    int sum;
};
```

`struct cmd_*_args` and `struct cmd_*_res` can contain an arbitrary number of
fields. They must be declared within the `_(field)` helper and not separated
with a comma or anything.

Command handler's prototype are generated by the preprocessor too (see
`include/cmd/protos.h`). They return an `int` value which may be a status code
and have two arguments: pointers to the `struct cmd_*_args` and `struct
cmd_*_res`. For example, given the previous `add` command:

```C
int cmd_add_handler(struct cmd_add_args *args, struct cmd_add_res *res);
```

The code of the handler should be written in the `src/cmd/handlers/add.c`. This
file should include at least the following header:

```C
#include <cmd/protos.h>
```

Handler's `args` pointer refers to address in kernel space so it does not need
to copy it from user space *unless it contains an adress to a data structure (an
array for example) the handler needs*. It's `res` pointer is also in kernel
space but the handler does not need to copy back it in user space. This is made
possible because handlers are wrapped in a function that does this boring job.

```C
int cmd_add_handler(struct cmd_add_args *args, struct cmd_add_res *res)
{
    res->sum = args->a + args->b;
    return 0;
}
```

## Command execution

Commands have been unified with two data structures: `struct cmd_params` and
`struct cmd_status`.

A `struct cmd_params` contains all the parameters to schedule and execute a
command, includings its arguments. The field `status` is the user space address
where the result should be stored. If the result is not required, NULL can be
given.

If the field `asynchronous` is zero then the command will be blocking. *A
command executed asynchronously must be collected in order to get it's
result(s)*.

Arguments of a command are stored in the `args` field which is a union of all
`struct cmd_*_args`.

```C
struct cmd_params {
	int asynchronous;
	struct cmd_status *status;
	union cmd_args args;
};
```

For example, parameters and arguments for the `add` command can be defined as
the following:

```C
struct cmd_params params;
struct cmd_status status;

params.asynchronous = 0;
params.status = &status;
args.add.a = 666;
args.add.b = 42;
```

The command can now be scheduled: `ioctl(fd, IOC_CMD, &params);`.

The result will be copied in the `status` struct. It contains a `code` field
which is the return value of the command's handler, and a `res` field which is a
union of all `struct cmd_*_res`:

```C
struct cmd_status {
	int code;
	union cmd_res res;
};
```

The result of the execution of a command should be checked in three steps:
1. result of the call to `ioctl`: it will return 0 if the command have been
   correctly scheduled
2. the command handler may fail: it's status code should be checked (`code`
   field)
3. access the result(s) of the command

```C
int r;

r = ioctl(fd, IOC_add, &params);
if (r) {
	perror("ioctl");
        exit(errno);
}
if (status.code != 0) {
	fprintf(stderr, "non reachable code: add command never fails!\n");
} else {
	printf("%d + %d = %d\n", args.add.a, args.add.b, status.res.add.sum);
}
```
