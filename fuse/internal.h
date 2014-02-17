#ifndef BIB_FUSE_INTERNAL_H
#define BIB_FUSE_INTERNAL_H

struct bibfs_state {
	const char *prefix;
	const char *path;
	struct stat st;
	FILE *f;
	struct bib_entry *e;
	int zim;
};

int
bibfs_reload(struct bibfs_state *b);

#endif

