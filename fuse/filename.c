/*
 * Copyright 2014-2017 Katherine Flavel
 *
 * See LICENCE for the full copyright terms.
 */

#include <assert.h>
#include <string.h>

#include "filename.h"

const char *
filename(const char *path)
{
	const char *p;

	assert(path != NULL);

	p = strrchr(path, '/');
	if (p == NULL) {
		return path;
	}

	return p + 1;
}

