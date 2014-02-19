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
#include "rwlock.h"
#include "op.h"

extern struct bibfs_op op_root;
extern struct bibfs_op op_zim;
extern struct bibfs_op op_entry;
extern struct bibfs_op op_index;
extern struct bibfs_op op_field;
extern struct bibfs_op op_file;

static struct {
	const char *fmt;
	struct bibfs_op *op;
} op[] = {
	{ "/",               &op_root  },
	{ "/notebook.zim",   &op_zim   },
	{ "/*.txt",          &op_zim   },
	{ "/*",              &op_entry },
	{ "/*/index.bib",    &op_index },
	{ "/*/*.txt",        &op_field },
	{ "/*/*.*",          &op_file  }
};

static int
split(char *s, const char *fmt, ...)
{
	const char *p;
	va_list ap;

	assert(s != NULL);

	va_start(ap, fmt);

	for (p = fmt; *p != '\0' && *s != '\0'; p++) {
		if (*p == '*') {
			* va_arg(ap, const char **) = s;
			s += strcspn(s, "/.");
			continue;
		}

		if (strspn(s, "/.")) {
			*s++ = '\0';
			continue;
		}

		if (*p != *s) {
			break;
		}

		s++;
	}

	va_end(ap);

	return *p == '\0' && *s == '\0';
}

static int
bibfs_getattr(const char *path, struct stat *st)
{
	struct bibfs_state *b = fuse_get_context()->private_data;

	size_t i;
	char s[PATH_MAX];
	const char *key, *name, *ext;

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

	key  = NULL;
	name = NULL;
	ext  = NULL;

	for (i = 0; i < sizeof op / sizeof *op; i++) {
		int r;

		strcpy(s, path);

		if (!split(s, op[i].fmt, &key, &name, &ext)) {
			continue;
		}

		rwlock_lock(b->rw, RWLOCK_READ);

		*st = b->st;

		r = op[i].op->getattr(b, st, key, name, ext);

		rwlock_unlock(b->rw);

		return r;
	}

	return -ENOENT;
}

static int
bibfs_readlink(const char *path, char *buf, size_t bufsz)
{
	struct bibfs_state *b = fuse_get_context()->private_data;

	size_t i;
	char s[PATH_MAX];
	const char *key, *name, *ext;

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

	key  = NULL;
	name = NULL;
	ext  = NULL;

	for (i = 0; i < sizeof op / sizeof *op; i++) {
		int r;

		strcpy(s, path);

		if (!split(s, op[i].fmt, &key, &name, &ext)) {
			continue;
		}

		rwlock_lock(b->rw, RWLOCK_READ);

		r = op[i].op->readlink(b, buf, bufsz, key, name, ext);

		rwlock_unlock(b->rw);

		return r;
	}

	return -ENOENT;
}

static int
bibfs_readdir(const char *path, void *buf, fuse_fill_dir_t fill,
	off_t offset, struct fuse_file_info *fi)
{
	struct bibfs_state *b = fuse_get_context()->private_data;

	size_t i;
	char s[PATH_MAX];
	const char *key, *name, *ext;

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

	key  = NULL;
	name = NULL;
	ext  = NULL;

	for (i = 0; i < sizeof op / sizeof *op; i++) {
		int r;

		strcpy(s, path);

		if (!split(s, op[i].fmt, &key, &name, &ext)) {
			continue;
		}

		if (1 == fill(buf, ".",  NULL, 0)) {
			return -ENOBUFS;
		}

		if (1 == fill(buf, "..", NULL, 0)) {
			return -ENOBUFS;
		}

		rwlock_lock(b->rw, RWLOCK_READ);

		r = op[i].op->readdir(b, buf, fill, offset, fi, key, name, ext);

		rwlock_unlock(b->rw);

		return r;
	}

	return -ENOENT;
}

static int
bibfs_open(const char *path, struct fuse_file_info *fi)
{
	struct bibfs_state *b = fuse_get_context()->private_data;

	size_t i;
	char s[PATH_MAX];
	const char *key, *name, *ext;

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

	key  = NULL;
	name = NULL;
	ext  = NULL;

	for (i = 0; i < sizeof op / sizeof *op; i++) {
		int r;

		strcpy(s, path);

		if (!split(s, op[i].fmt, &key, &name, &ext)) {
			continue;
		}

		rwlock_lock(b->rw, RWLOCK_READ);

		r = op[i].op->open(b, fi, key, name, ext);

		rwlock_unlock(b->rw);

		return r;
	}

	return -ENOENT;
}

static int
bibfs_read(const char *path, char *buf, size_t size, off_t offset,
	struct fuse_file_info *fi)
{
	struct bibfs_state *b = fuse_get_context()->private_data;

	size_t i;
	char s[PATH_MAX];
	const char *key, *name, *ext;

	assert(b != NULL);
	assert(path != NULL);
	assert(path[0] == '/');
	assert(buf != NULL);
	assert(fi != NULL);

	if (offset < 0) {
		return -EINVAL;
	}

	if (offset > SIZE_MAX) {
		return -EINVAL;
	}

	if (-1 == bibfs_reload(b)) {
		return -errno;
	}

	if (strlen(path) + 1 > sizeof s) {
		return -ENAMETOOLONG;
	}

	key  = NULL;
	name = NULL;
	ext  = NULL;

	for (i = 0; i < sizeof op / sizeof *op; i++) {
		int r;

		strcpy(s, path);

		if (!split(s, op[i].fmt, &key, &name, &ext)) {
			continue;
		}

		rwlock_lock(b->rw, RWLOCK_READ);

		r = op[i].op->read(b, buf, size, offset, fi, key, name, ext);

		rwlock_unlock(b->rw);

		return r;
	}

	return -ENOENT;
}

void
bibfs_init(struct fuse_operations *o)
{
	assert(o != NULL);

	o->getattr  = bibfs_getattr;
	o->readlink = bibfs_readlink;
	o->readdir  = bibfs_readdir;
	o->open     = bibfs_open;
	o->read     = bibfs_read;
}

