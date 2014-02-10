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
#include "op.h"

/* TODO: name= from .bib source file */
const char *notebook =
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

	assert(b != NULL);
	assert(st != NULL);
	assert(name == NULL && ext == NULL);

	if (!b->zim) {
		return -ENOENT;
	}

	if (key == NULL) {
		st->st_mode  = S_IFREG | 0444;
		st->st_nlink = 1;
		st->st_size  = strlen(notebook);

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

	st->st_mode  = S_IFREG | 0444;
	st->st_nlink = 1;
	st->st_size  = strlen(e->zim);

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
	char *buf, size_t size, off_t offset, struct fuse_file_info *fi,
	const char *key, const char *name, const char *ext)
{
	struct bib_entry *e;

	assert(b != NULL);
	assert(fi != NULL);
	assert(buf != NULL);
	assert(offset <= size);
	assert(name == NULL && ext == NULL);

	(void) fi;

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

