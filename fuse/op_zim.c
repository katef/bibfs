#define _XOPEN_SOURCE 500
#define FUSE_USE_VERSION 26

#include <sys/types.h>
#include <sys/stat.h>

#include <fuse.h>

#include <assert.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <bib/bib.h>
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
	const char *key, const char *name)
{
	struct bib_entry *e;
	struct bib_field *f;
	struct bib_value *v;
	const char *n;
	size_t i;

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

	st->st_mode  = S_IFREG | 0444;
	st->st_nlink = 1;
	st->st_size  = 57; /* TODO: size of zim .txt file */

	{
		FILE *f;
		size_t n;
		struct stat st_zim;
		int fd;

		f = tmpfile();
		if (f == NULL) {
			return -errno;
		}

		/* TODO: error-check */
		out_zim(f, e, 0);

		fflush(f);

		if (-1 == fstat(fileno(f), &st_zim)) {
			fclose(f);
			return -errno;
		}

		st->st_size = st_zim.st_size;

		fclose(f);
	}

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

	{
		FILE *f;
		size_t n;

		f = tmpfile();
		if (f == NULL) {
			return -errno;
		}

		/* TODO: error-check */
		out_zim(f, e, 0);

		if (-1 == fseek(f, offset, SEEK_SET)) {
			(void) fclose(f);
			return -errno;
		}

		n = 0;

		while (n < size) {
			int r;

			r = fread(buf + n, 1, size - n, f);
			if (r == -1) {
				return -errno;
			}

			if (r == 0) {
				break;
			}

			n += r;
		}

		fclose(f);

		return n;
	}
}

struct bibfs_op op_zim = {
	zim_getattr,
	NULL,
	NULL,
	zim_open,
	zim_read
};

