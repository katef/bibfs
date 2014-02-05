#define _XOPEN_SOURCE 500
#define FUSE_USE_VERSION 26

#include <sys/types.h>
#include <sys/stat.h>

#include <fuse.h>

#include <assert.h>
#include <stdio.h>
#include <errno.h>

#include <bib/bib.h>
#include <bib/find.h>

#include "internal.h"
#include "op.h"

static int
index_getattr(struct bibfs_state *b, struct stat *st,
	const char *key, const char *name)
{
	struct bib_entry *e;
	struct bib_field *f;
	size_t i;

	assert(b != NULL);
	assert(st != NULL);
	assert(key != NULL && name == NULL);

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	(void *) e;

	st->st_mode  = S_IFREG | 0444;
	st->st_nlink = 1;
	st->st_size  = 57; /* TODO: size of formatted entry */

	return 0;
}

static int
index_open(struct bibfs_state *b,
	struct fuse_file_info *fi,
	const char *key, const char *name)
{
	struct bib_entry *e;
	struct bib_field *f;
	struct bib_value *v;
	size_t i;

	assert(b != NULL);
	assert(fi != NULL);
	assert(key != NULL && name == NULL);

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	(void *) e;

	goto done;

done:

	if ((fi->flags & 03) != O_RDONLY) {
		return -EACCES;
	}

	return 0;
}

static int
index_read(struct bibfs_state *b,
	char *buf, size_t size, off_t offset, struct fuse_file_info *fi,
	const char *key, const char *name)
{
	struct bib_entry *e;
	struct bib_field *f;
	const char *s;
	size_t i;
	size_t l;
	int n;

	assert(b != NULL);
	assert(fi != NULL);
	assert(buf != NULL);
	assert(offset <= size);
	assert(key != NULL && name == NULL);

	(void) fi;

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	(void) e;

	/* TODO: use out() to write a single entry to memory */

	s = "@TODO {TODO\n\ttodo = {TODO}\n}\n";

	return sread(s, buf, size, offset);
}

struct bibfs_op op_index = {
	index_getattr,
	NULL,
	NULL,
	index_open,
	index_read
};

