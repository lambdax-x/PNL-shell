#ifndef UTILS_PARSE_H
#define UTILS_PARSE_H

#include <sys/types.h>

enum parse_error_t {
	P_UNEXP_END_OF_BUFFER,
	P_UNEXP_CHAR
};

ssize_t parse_char(const char line[], const size_t n, const char c);

ssize_t parse_space(const char line[], const size_t n);

ssize_t parse_string(const char line[], const size_t n, const char str[]);

ssize_t parse_many_space(const char line[], const size_t n);

ssize_t parse_some_space(const char line[], const size_t n);

ssize_t parse_int(const char line[], const size_t n, int *number);

#endif
