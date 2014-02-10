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

static int
root_getattr(struct bibfs_state *b,
	struct stat *st,
	const char *key, const char *name, const char *ext)
{
	assert(b != NULL);
	assert(st != NULL);
	assert(key == NULL && name == NULL && ext == NULL);

	(void) key;
	(void) name;
	(void) ext;

	st->st_mode  = S_IFDIR | 0755;
	st->st_nlink = bib_count(b->e) + 2;

	return 0;
}

static int
root_readdir(struct bibfs_state *b,
	void *buf, fuse_fill_dir_t fill, off_t offset, struct fuse_file_info *fi,
	const char *key, const char *name, const char *ext)
{
	const struct bib_entry *e;

	assert(b != NULL);
	assert(buf != NULL);
	assert(fill != NULL);
	assert(fi != NULL);
	assert(key == NULL && name == NULL && ext == NULL);

	(void) offset;
	(void) fi;
	(void) key;
	(void) name;
	(void) ext;

	for (e = b->e; e != NULL; e = e->next) {
		if (1 == fill(buf, e->key, NULL, 0)) {
			return -ENOBUFS;
		}

		if (b->zim) {
			char s[FILENAME_MAX];

			if (strlen(e->key) + 4 + 1 > sizeof s) {
				return -ENAMETOOLONG;
			}

			sprintf(s, "%s.txt", e->key);

			if (1 == fill(buf, s, NULL, 0)) {
				return -ENOBUFS;
			}
		}
	}

	if (b->zim) {
		if (1 == fill(buf, "notebook.zim", NULL, 0)) {
			return -ENOBUFS;
		}
	}

	return 0;
}

struct bibfs_op op_root = {
	root_getattr,
	root_readdir,
	NULL,
	NULL,
	NULL
};

