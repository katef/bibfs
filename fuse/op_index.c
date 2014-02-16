#define _XOPEN_SOURCE 500
#define FUSE_USE_VERSION 26

#include <sys/types.h>
#include <sys/stat.h>

#include <fuse.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <bib/bib.h>
#include <bib/out.h>
#include <bib/find.h>

#include "internal.h"
#include "op.h"

static int
index_getattr(struct bibfs_state *b, struct stat *st,
	const char *key, const char *name, const char *ext)
{
	struct bib_entry *e;

	assert(b != NULL);
	assert(st != NULL);
	assert(key != NULL && name == NULL && ext == NULL);

	(void) name;
	(void) ext;

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	if (e->bib == NULL) {
		e->bib = bib_outs(e, out_bibtex);
		if (e->bib == NULL) {
			return -errno;
		}
	}

	st->st_mode  = S_IFREG | 0444;
	st->st_nlink = 1;
	st->st_size  = strlen(e->bib);

	return 0;
}

static int
index_open(struct bibfs_state *b,
	struct fuse_file_info *fi,
	const char *key, const char *name, const char *ext)
{
	struct bib_entry *e;

	assert(b != NULL);
	assert(fi != NULL);
	assert(key != NULL && name == NULL && ext == NULL);

	(void) name;
	(void) ext;

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	(void) e;

	goto done;

done:

	if ((fi->flags & 03) != O_RDONLY) {
		return -EACCES;
	}

	return 0;
}

static int
index_read(struct bibfs_state *b,
	char *buf, size_t size, size_t offset, struct fuse_file_info *fi,
	const char *key, const char *name, const char *ext)
{
	struct bib_entry *e;

	assert(b != NULL);
	assert(fi != NULL);
	assert(buf != NULL);
	assert(offset <= size);
	assert(key != NULL && name == NULL && ext == NULL);

	(void) fi;
	(void) name;
	(void) ext;

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	if (e->bib == NULL) {
		e->bib = bib_outs(e, out_bibtex);
		if (e->bib == NULL) {
			return -errno;
		}
	}

	return sread(e->bib, buf, size, offset);
}

struct bibfs_op op_index = {
	index_getattr,
	NULL,
	NULL,
	index_open,
	index_read
};

