/*
 * Copyright 2014-2017 Katherine Flavel
 *
 * See LICENCE for the full copyright terms.
 */

#ifndef BIB_FUSE_SREAD_H
#define BIB_FUSE_SREAD_H

int
sread(const char *s,
	char *buf, size_t size, size_t offset);

#endif

