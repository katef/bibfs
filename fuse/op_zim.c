/*
 * Copyright 2014-2017 Katherine Flavel
 *
 * See LICENCE for the full copyright terms.
 */

#define _XOPEN_SOURCE 500
#define FUSE_USE_VERSION 26

#include <sys/types.h>
#include <sys/stat.h>

#include <fuse.h>

#include <assert.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <bib/bib.h>
#include <bib/out.h>
#include <bib/find.h>

#include "internal.h"
#include "sread.h"
#include "op.h"

/* TODO: name= from .bib source file */
static const char *notebook =
	"[Notebook]\n"
	"name=Something\n"
	"version=0.4\n"
	"endofline=unix\n"
	"profile=None\n";

static int
zim_getattr(struct bibfs_state *b,
	struct stat *st,
	const char *key, const char *name, const char *ext)
{
	struct bib_entry *e;
	off_t z;

	assert(b != NULL);
	assert(st != NULL);
	assert(name == NULL && ext == NULL);

	(void) name;
	(void) ext;

	if (!b->zim) {
		return -ENOENT;
	}

	if (key == NULL) {
		z = strlen(notebook);
		if (z < 0) {
			return -EINVAL;
		}

		st->st_mode  = S_IFREG | 0444;
		st->st_nlink = 1;
		st->st_size  = z;

		return 0;
	}

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	if (e->zim == NULL) {
		e->zim = bib_outs(e, out_zim);
		if (e->zim == NULL) {
			return -errno;
		}
	}

	z = strlen(e->zim);
	if (z < 0) {
		return -EINVAL;
	}

	st->st_mode  = S_IFREG | 0444;
	st->st_nlink = 1;
	st->st_size  = z;

	return 0;
}

static int
zim_open(struct bibfs_state *b,
	struct fuse_file_info *fi,
	const char *key, const char *name, const char *ext)
{
	struct bib_entry *e;

	assert(b != NULL);
	assert(fi != NULL);
	assert(name == NULL && ext == NULL);

	(void) name;
	(void) ext;

	if (!b->zim) {
		return -ENOENT;
	}

	(void) fi;

	if (key == NULL) {
		if ((fi->flags & 03) != O_RDONLY) {
			return -EACCES;
		}

		return 0;
	}

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	if ((fi->flags & 03) != O_RDONLY) {
		return -EACCES;
	}

	return 0;
}

static int
zim_read(struct bibfs_state *b,
	char *buf, size_t size, size_t offset, struct fuse_file_info *fi,
	const char *key, const char *name, const char *ext)
{
	struct bib_entry *e;

	assert(b != NULL);
	assert(fi != NULL);
	assert(buf != NULL);
	assert(offset <= size);
	assert(name == NULL && ext == NULL);

	(void) fi;
	(void) name;
	(void) ext;

	if (!b->zim) {
		return -ENOENT;
	}

	if (key == NULL) {
		return sread(notebook, buf, size, offset);
	}

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	if (e->zim == NULL) {
		e->zim = bib_outs(e, out_zim);
		if (e->zim == NULL) {
			return -errno;
		}
	}

	return sread(e->zim, buf, size, offset);
}

struct bibfs_op op_zim = {
	zim_getattr,
	NULL,
	NULL,
	zim_open,
	zim_read
};

