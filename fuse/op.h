#ifndef BIBFS_FUSE_OP_H
#define BIBFS_FUSE_OP_H

struct bibfs_op {
	int
	(*getattr)(struct bibfs_state *b,
		struct stat *st,
		const char *key, const char *name);

	int
	(*readdir)(struct bibfs_state *b,
		void *buf, fuse_fill_dir_t fill, off_t offset, struct fuse_file_info *fi,
		const char *key, const char *name);

	int
	(*readlink)(struct bibfs_state *b, char *buf, size_t bufsz,
		const char *key, const char *name);

	int
	(*open)(struct bibfs_state *b,
		struct fuse_file_info *fi,
		const char *key, const char *name);

	int
	(*read)(struct bibfs_state *b,
		char *buf, size_t size, off_t offset, struct fuse_file_info *fi,
		const char *key, const char *name);
};

void
bibfs_init(struct fuse_operations *op);

#endif

