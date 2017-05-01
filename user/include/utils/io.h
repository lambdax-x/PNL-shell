#ifndef UTILS_IO_H
#define UTILS_IO_H

#include <stdio.h>

#ifdef DEBUG
#define pr_debug(str, ...) fprintf(stderr, "debug: " str "\n", ##__VA_ARGS__)
#else
#define pr_debug
#endif

#define pr_error(str, ...) fprintf(stderr, "error: " str "\n", ##__VA_ARGS__)
#endif
