#define _XOPEN_SOURCE 500
#define FUSE_USE_VERSION 26

#include <sys/types.h>
#include <sys/stat.h>

#include <fuse.h>

#include <assert.h>
#include <stdio.h>

#include <bib/bib.h>
#include <bib/find.h>

#include "internal.h"
#include "op.h"

int
op_getattr_root(struct bibfs_state *b, struct stat *st)
{
	assert(b != NULL);
	assert(st != NULL);

	st->st_mode  = S_IFDIR | 0755;
	st->st_nlink = bib_count(b->e) + 2;

	return 0;
}

int
op_readdir_root(struct bibfs_state *b,
	void *buf, fuse_fill_dir_t fill, off_t offset, struct fuse_file_info *fi)
{
	const struct bib_entry *e;

	assert(b != NULL);
	assert(buf != NULL);
	assert(fill != NULL);
	assert(fi != NULL);

	(void) offset;
	(void) fi;

	for (e = b->e; e != NULL; e = e->next) {
		fill(buf, e->key, NULL, 0);
	}

	return 0;
}

