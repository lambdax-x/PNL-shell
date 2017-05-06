#include <string.h>
#include <utils/parse.h>

ssize_t parse_char(const char line[], const size_t n, const char c)
{
	if (n > 0)
		return line[0] == c;
	else
		return -P_UNEXP_END_OF_BUFFER;
}

ssize_t parse_space(const char line[], const size_t n)
{
	if (n > 0)
		return line[0] == ' ' || line[0] == '\t';
	else
		return -P_UNEXP_END_OF_BUFFER;
}

ssize_t parse_string(const char line[], const size_t n, const char str[])
{
	unsigned int i;

	for (i = 0 ; str[i] != '\0' ; ++i) {
		if (line[i] == '\0')
			return -P_UNEXP_END_OF_BUFFER;

		if (line[i] != str[i])
			return -P_UNEXP_CHAR;
	}
	return i;
}

ssize_t parse_many_space(const char line[], const size_t n)
{
	ssize_t read;
	unsigned int i;

	for (i = 0 ; i < n ; ++i) {
		read = parse_space(line + i, n - i);
		if (read != 1)
			break;
	}
	return i;
}

ssize_t parse_some_space(const char line[], const size_t n)
{
	ssize_t read;

	read = parse_space(line, n);
	if (read != 1)
		return read;
	read = parse_many_space(line + 1, n - 1);
	return read + 1;
}

static inline int is_digit(const char c)
{
	return c >= '0' && c <= '9';
}

ssize_t parse_digit(const char line[], const size_t n, int *d)
{
	if (n == 0)
		return -P_UNEXP_END_OF_BUFFER;
	if (!is_digit(line[0]))
		return -P_UNEXP_CHAR;
	*d = line[0] - '0';
	return 1;
}

ssize_t parse_int(const char line[], const size_t n, int *number)
{
	ssize_t read;
	unsigned int i;
	int _number = 0, digit;
	int sign;

	if (n == 0)
		return -P_UNEXP_END_OF_BUFFER;

	read = parse_char(line, n, '-');
	sign = read == 1 ? -1 : 1;

	if (read == 1 && n < 2)
		return -P_UNEXP_END_OF_BUFFER;

	if (!is_digit(line[read]))
		return -P_UNEXP_CHAR;

	for (i = read ; i < n ; ++i) {
		read = parse_digit(line + i, n - i, &digit);
		if (read != 1)
			break;
		_number *= 10;
		_number += digit;
	}
	*number = sign * _number;
	return i;
}

ssize_t parse_uint(const char line[], const size_t n, unsigned int *number)
{
	ssize_t read;
	unsigned int i;
	unsigned int _number = 0;
	int digit;

	if (n == 0)
		return -P_UNEXP_END_OF_BUFFER;
	if (!is_digit(line[0]))
		return -P_UNEXP_CHAR;

	for (i = 0 ; i < n ; ++i) {
		read = parse_digit(line + i, n - i, &digit);
		if (read != 1)
			break;
		_number *= 10;
		_number += digit;
	}
	*number = _number;
	return i;
}
