/*
 * Copyright 2014-2017 Katherine Flavel
 *
 * See LICENCE for the full copyright terms.
 */

#include <sys/types.h>

#include <assert.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "sread.h"

int
sread(const char *s,
	char *buf, size_t size, size_t offset)
{
	size_t l;

	assert(s != NULL);
	assert(buf != NULL);
	assert(offset <= size);

	if (size + offset < size) {
		errno = ERANGE;
		return 0;
	}

	l = strlen(s);
	if (offset >= l) {
		return 0;
	}

	if (offset + size > l) {
		size = l - offset;
	}

	/* callers providing read(2) are required to return int */
	if (size > INT_MAX) {
		errno = EINVAL;
		return 0;
	}

	memcpy(buf, s + offset, size);

	return size;
}

