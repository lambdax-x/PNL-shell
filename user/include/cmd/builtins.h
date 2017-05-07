#ifndef CMD_BUILTINS_H
#define CMD_BUILTINS_H

#include <sys/types.h>

ssize_t parse_exec_builtin(const char line[], const size_t n);

#endif
