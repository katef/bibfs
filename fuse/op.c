#define _XOPEN_SOURCE 500
#define FUSE_USE_VERSION 26

#include <sys/types.h>
#include <sys/stat.h>

#include <fuse.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <stdio.h>
#include <errno.h>

#include "internal.h"
#include "op.h"

static size_t
split(char *s, size_t count, ...)
{
	size_t n, z;
	va_list ap;

	assert(s != NULL);

	va_start(ap, count);

	for (n = 0; *s != '\0'; n++) {
		*s++ = '\0';

		z = strcspn(s, "/");
		if (z == 0) {
			break;
		}

		if (n < count) {
			* va_arg(ap, const char **) = s;
		}

		s += z;
	}

	return n;
}

static int
bibfs_getattr(const char *path, struct stat *st)
{
	struct bibfs_state *b = fuse_get_context()->private_data;

	char s[PATH_MAX];
	const char *key, *name;

	assert(b != NULL);
	assert(path != NULL);
	assert(path[0] == '/');
	assert(st != NULL);

	if (-1 == bibfs_reload(b)) {
		return -errno;
	}

	if (strlen(path) > sizeof s) {
		return -ENAMETOOLONG;
	}

	strcpy(s, path);

	*st = b->st;

	switch (split(s, 2, &key, &name)) {
	case 0: return op_getattr_root (b, st);
	case 1: return op_getattr_entry(b, st, key);
	case 2: return op_getattr_field(b, st, key, name);

	default:
		return -ENOENT;
	}
}

static int
bibfs_readlink(const char *path, char *buf, size_t bufsz)
{
	struct bibfs_state *b = fuse_get_context()->private_data;

	char s[PATH_MAX];
	const char *key, *name;

	assert(b != NULL);
	assert(path != NULL);
	assert(path[0] == '/');
	assert(buf != NULL);

	if (-1 == bibfs_reload(b)) {
		return -errno;
	}

	if (strlen(path) + 1 > sizeof s) {
		return -ENAMETOOLONG;
	}

	strcpy(s, path);

	switch (split(s, 2, &key, &name)) {
	case 0: return -EINVAL;
	case 1: return -EINVAL;
	case 2: return op_readlink_field(b, buf, bufsz, key, name);

	default:
		return -ENOENT;
	}
}

static int
bibfs_readdir(const char *path, void *buf, fuse_fill_dir_t fill,
	off_t offset, struct fuse_file_info *fi)
{
	struct bibfs_state *b = fuse_get_context()->private_data;

	char s[PATH_MAX];
	const char *key;

	assert(b != NULL);
	assert(path != NULL);
	assert(path[0] == '/');
	assert(buf != NULL);
	assert(fi != NULL);

	if (-1 == bibfs_reload(b)) {
		return -errno;
	}

	if (strlen(path) > sizeof s) {
		return -ENAMETOOLONG;
	}

	strcpy(s, path);

	if (1 == fill(buf, ".",  NULL, 0)) {
		return -ENOBUFS;
	}

	if (1 == fill(buf, "..", NULL, 0)) {
		return -ENOBUFS;
	}

	switch (split(s, 1, &key)) {
	case 0: return op_readdir_root (b, buf, fill, offset, fi);
	case 1: return op_readdir_entry(b, buf, fill, offset, fi, key);

	default:
		return -ENOENT;
	}
}

static int
bibfs_open(const char *path, struct fuse_file_info *fi)
{
	struct bibfs_state *b = fuse_get_context()->private_data;

	char s[PATH_MAX];
	const char *key, *name;

	assert(b != NULL);
	assert(path != NULL);
	assert(path[0] == '/');
	assert(fi != NULL);

	if (-1 == bibfs_reload(b)) {
		return -errno;
	}

	if (strlen(path) + 1 > sizeof s) {
		return -ENAMETOOLONG;
	}

	strcpy(s, path);

	switch (split(s, 2, &key, &name)) {
	case 0: return -EINVAL;
	case 1: return -EINVAL;
	case 2: return op_open_field(b, fi, key, name);

	default:
		return -ENOENT;
	}
}

static int
bibfs_read(const char *path, char *buf, size_t size, off_t offset,
	struct fuse_file_info *fi)
{
	struct bibfs_state *b = fuse_get_context()->private_data;

	char s[PATH_MAX];
	const char *key, *name;

	assert(b != NULL);
	assert(path != NULL);
	assert(path[0] == '/');
	assert(buf != NULL);
	assert(offset <= size);
	assert(fi != NULL);

	if (-1 == bibfs_reload(b)) {
		return -errno;
	}

	if (strlen(path) + 1 > sizeof s) {
		return -ENAMETOOLONG;
	}

	strcpy(s, path);

	switch (split(s, 2, &key, &name)) {
	case 0: return -EINVAL;
	case 1: return -EINVAL;
	case 2: return op_read_field(b, buf, size, offset, fi, key, name);

	default:
		return -ENOENT;
	}
}

void
bibfs_init(struct fuse_operations *op)
{
	assert(op != NULL);

	op->getattr  = bibfs_getattr;
	op->readlink = bibfs_readlink;
	op->readdir  = bibfs_readdir;
	op->open     = bibfs_open;
	op->read     = bibfs_read;
}

