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

static size_t
readall(FILE *f, char *buf, size_t size)
{
	size_t n;

	n = 0;

	assert(f != NULL);
	assert(buf != NULL);
	assert(size > 0);

	while (n < size) {
		int r;

		r = fread(buf + n, 1, size - n, f);
		if (r == -1) {
			return 0;
		}

		if (r == 0) {
			break;
		}

		n += r;
	}

	buf[n] = '\0';

	return n;
}

/* TODO: move to <bib/out.h> */
char *
outs(const struct bib_entry *e,
	void (*out)(FILE *f, const struct bib_entry *, int))
{
	struct stat st;
	FILE *f;
	char *s;

	assert(e != NULL);
	assert(out != NULL);

	f = tmpfile();
	if (f == NULL) {
		return NULL;
	}

	/* TODO: error-check */
	out(f, e, 0);

	fflush(f);

	if (-1 == fstat(fileno(f), &st)) {
		fclose(f);
		return NULL;
	}

	s = malloc(st.st_size + 1);
	if (s == NULL) {
		fclose(f);
		return NULL;
	}

	rewind(f);

	if (0 == readall(f, s, st.st_size + 1)) {
		fclose(f);
		free(s);
		return NULL;
	}

	fclose(f);

	return s;
}

static int
zim_getattr(struct bibfs_state *b,
	struct stat *st,
	const char *key, const char *name)
{
	struct bib_entry *e;
	struct bib_field *f;
	struct bib_value *v;

	assert(b != NULL);
	assert(st != NULL);
	assert(name == NULL);

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
		e->zim = outs(e, out_zim);
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
	const char *key, const char *name)
{
	struct bib_entry *e;
	size_t i;

	assert(b != NULL);
	assert(fi != NULL);
	assert(name == NULL);

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
	const char *key, const char *name)
{
	struct bib_entry *e;
	size_t i;
	size_t l;
	int n;

	assert(b != NULL);
	assert(fi != NULL);
	assert(buf != NULL);
	assert(offset <= size);
	assert(name == NULL);

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
		e->zim = outs(e, out_zim);
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

