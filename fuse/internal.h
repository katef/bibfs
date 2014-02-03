#ifndef BIB_FUSE_INTERNAL_H
#define BIB_FUSE_INTERNAL_H

struct bibfs_state {
	const char *path;
	/* TODO: store stat struct, for mtime */
	FILE *f;
	struct bib_entry *e;
};

const char *
filename(const char *path);

#endif

