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

const char *
filename(const char *path);

int
sread(const char *s,
	char *buf, size_t size, off_t offset);

int
bibfs_reload(struct bibfs_state *b);

#endif

