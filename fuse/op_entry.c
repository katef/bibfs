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

int
op_getattr_entry(struct bibfs_state *b, struct stat *st,
	const char *key)
{
	struct bib_entry *e;

	assert(b != NULL);
	assert(st != NULL);
	assert(key != NULL);

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	st->st_mode  = S_IFDIR | 0755;
	st->st_nlink = 57; /* TODO */

	/* TODO: time etc from b->st */

	return 0;
}

int
op_readdir_entry(struct bibfs_state *b,
	void *buf, fuse_fill_dir_t fill, off_t offset, struct fuse_file_info *fi,
	const char *key)
{
	const struct bib_entry *e;
	size_t i;

	/* TODO: share with file contents. add a callback to concat values */
	struct {
		const char *path;
		const char *name;
	} a[] = {
		{ "abstract.txt", "abstract" },
		{ "notes.txt",    "notes"    }
	};

	assert(b != NULL);
	assert(buf != NULL);
	assert(fill != NULL);
	assert(fi != NULL);
	assert(key != NULL);

	(void) offset;
	(void) fi;

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	for (i = 0; i < sizeof a / sizeof *a; i++) {
		struct bib_field *f;

		f = find_field(e->field, a[i].name);
		if (f == NULL) {
			continue;
		}

		fill(buf, a[i].path, NULL, 0);
	}

	{
		struct bib_field *f;
		struct bib_value *v;

		f = find_field(e->field, "file");
		if (f != NULL) {
			for (v = f->value; v != NULL; v = v->next) {
				fill(buf, v->text, NULL, 0);
			}
		}
	}

	/* TODO: index.bib etc */

	return 0;
}

