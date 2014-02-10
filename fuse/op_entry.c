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
#include <bib/find.h>

#include "internal.h"
#include "field.h"
#include "op.h"

static int
entry_getattr(struct bibfs_state *b,
	struct stat *st,
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

	st->st_mode  = S_IFDIR | 0755;
	st->st_nlink = 57; /* TODO */

	return 0;
}

static int
entry_readdir(struct bibfs_state *b,
	void *buf, fuse_fill_dir_t fill, off_t offset, struct fuse_file_info *fi,
	const char *key, const char *name, const char *ext)
{
	const struct bib_entry *e;
	const struct bib_field *f;
	size_t i;

	assert(b != NULL);
	assert(buf != NULL);
	assert(fill != NULL);
	assert(fi != NULL);
	assert(key != NULL && name == NULL && ext == NULL);

	(void) offset;
	(void) fi;
	(void) name;
	(void) ext;

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	if (1 == fill(buf, "index.bib", NULL, 0)) {
		return -ENOBUFS;
	}

	for (i = 0; i < sizeof fields / sizeof *fields; i++) {
		f = find_field(e->field, fields[i].name);
		if (f == NULL) {
			continue;
		}

		if (1 == fill(buf, fields[i].path, NULL, 0)) {
			return -ENOBUFS;
		}
	}

	f = find_field(e->field, "file");
	if (f != NULL) {
		struct bib_value *v;
		const char *n;

		for (v = f->value; v != NULL; v = v->next) {
			n = filename(v->text);

			if (!strchr(n, '.')) {
				continue;
			}

			if (1 == fill(buf, n, NULL, 0)) {
				return -ENOBUFS;
			}
		}
	}

	return 0;
}

struct bibfs_op op_entry = {
	entry_getattr,
	entry_readdir,
	NULL,
	NULL,
	NULL
};

