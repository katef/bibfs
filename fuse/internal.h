#ifndef BIB_FUSE_INTERNAL_H
#define BIB_FUSE_INTERNAL_H

struct bibfs_state {
	const char *path;
	struct stat st;
	FILE *f;
	struct bib_entry *e;
	int zim;
};

const char *
filename(const char *path);

int
bibfs_reload(struct bibfs_state *b);

#endif

