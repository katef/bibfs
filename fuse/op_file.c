#define _XOPEN_SOURCE 500
#define FUSE_USE_VERSION 26

#include <sys/types.h>
#include <sys/stat.h>

#include <fuse.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <bib/bib.h>
#include <bib/find.h>

#include "internal.h"
#include "op.h"

static int
file_getattr(struct bibfs_state *b,
	struct stat *st,
	const char *key, const char *name, const char *ext)
{
	struct bib_entry *e;
	struct bib_field *f;
	struct bib_value *v;
	char s[FILENAME_MAX];
	const char *n;

	assert(b != NULL);
	assert(st != NULL);
	assert(key != NULL && name != NULL && ext != NULL);

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	f = find_field(e->field, "file");
	if (f == NULL) {
		return -ENOENT;
	}

	if (strlen(name) + 1 + strlen(ext) + 1 > sizeof s) {
		return -ENAMETOOLONG;
	}

	sprintf(s, "%s.%s", name, ext);

	for (v = f->value; v != NULL; v = v->next) {
		n = filename(v->text);

		if (0 != strcmp(s, n)) {
			continue;
		}

		st->st_size  = 0;

		/* TODO: consider relative paths */
		if (v->text[0] == '/') {
			(void) stat(v->text, st);
		}

		st->st_mode  = S_IFLNK | 0444;
		st->st_nlink = 1;

		return 0;
	}

	return -ENOENT;
}

static int
file_readlink(struct bibfs_state *b,
	char *buf, size_t bufsz,
	const char *key, const char *name, const char *ext)
{
	struct bib_entry *e;
	struct bib_field *f;
	struct bib_value *v;
	char s[FILENAME_MAX];
	const char *n;

	assert(b != NULL);
	assert(buf != NULL);
	assert(key != NULL && name != NULL && ext != NULL);

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	f = find_field(e->field, "file");
	if (f == NULL) {
		return -ENOENT;
	}

	if (strlen(name) + 1 + strlen(ext) + 1 > sizeof s) {
		return -ENAMETOOLONG;
	}

	sprintf(s, "%s.%s", name, ext);

	for (v = f->value; v != NULL; v = v->next) {
		n = filename(v->text);

		if (0 != strcmp(s, n)) {
			continue;
		}

		if (v->text[0] != '/' && b->prefix != NULL) {
			if (strlen(b->prefix) + 1 + strlen(v->text) + 1 > bufsz) {
				return -ENAMETOOLONG;
			}

			sprintf(buf, "%s/%s", b->prefix, v->text);
		} else {
			if (strlen(b->prefix) + 1 > bufsz) {
				return -ENAMETOOLONG;
			}

			strcpy(buf, v->text);
		}

		return 0;
	}

	return -ENOENT;
}

struct bibfs_op op_file = {
	file_getattr,
	NULL,
	file_readlink,
	NULL,
	NULL
};

