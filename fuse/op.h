#ifndef BIBFS_FUSE_OP_H
#define BIBFS_FUSE_OP_H

void
bibfs_init(struct fuse_operations *op);

int
op_getattr_root (struct bibfs_state *b,
	struct stat *st);

int
op_getattr_entry(struct bibfs_state *b,
	struct stat *st,
	const char *key);

int
op_getattr_field(struct bibfs_state *b,
	struct stat *st,
	const char *key, const char *name);

int
op_readdir_root(struct bibfs_state *b,
	void *buf, fuse_fill_dir_t fill, off_t offset, struct fuse_file_info *fi);

int
op_readdir_entry(struct bibfs_state *b,
	void *buf, fuse_fill_dir_t fill, off_t offset, struct fuse_file_info *fi,
	const char *key);

int
op_readlink_field(struct bibfs_state *b, char *buf, size_t bufsz,
	const char *key, const char *name);

#endif

