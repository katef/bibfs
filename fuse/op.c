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
#include <limits.h>
#include <stdio.h>
#include <errno.h>

#include "internal.h"
#include "op.h"

static size_t
tokparts(char *s, const char *a[], size_t count)
{
	size_t n;

	assert(s != NULL);
	assert(a != NULL);

	n = 0;

	for (;;) {
		if (n < count) {
			a[n] = s;
		}

		s += strcspn(s, "/");
		if (*s == '\0') {
			break;
		}

		*s = '\0';
		s++;
		n++;
	}

	return n;
}

static int
bibfs_getattr(const char *path, struct stat *st)
{
	struct bibfs_state *b = fuse_get_context()->private_data;
	static const struct stat st_default;

	char s[PATH_MAX];
	const char *a[3];

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

	*st = st_default;

	switch (tokparts(s, a, sizeof a / sizeof *a)) {
	case 0: return op_getattr_root (b, st);
	case 1: return op_getattr_entry(b, st, a[1]);
	case 2: return op_getattr_field(b, st, a[1], a[2]);

	default:
		return -ENOENT;
	}
}

static int
bibfs_readlink(const char *path, char *buf, size_t bufsz)
{
	struct bibfs_state *b = fuse_get_context()->private_data;

	char s[PATH_MAX];
	const char *a[3];

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

	switch (tokparts(s, a, sizeof a / sizeof *a)) {
	case 0: return -EINVAL;
	case 1: return -EINVAL;
	case 2: return op_readlink_field(b, buf, bufsz, a[1], a[2]);

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
	const char *a[2];

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

	switch (tokparts(s, a, sizeof a / sizeof *a)) {
	case 0: return op_readdir_root (b, buf, fill, offset, fi);
	case 1: return op_readdir_entry(b, buf, fill, offset, fi, a[1]);

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
}

