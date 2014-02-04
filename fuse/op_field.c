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

int
op_getattr_field(struct bibfs_state *b, struct stat *st,
	const char *key, const char *name)
{
	struct bib_entry *e;
	struct bib_field *f;
	size_t i;

	/* TODO: share with file contents. add a callback to concat values */
	struct {
		const char *path;
		const char *name;
	} a[] = {
		{ "abstract.txt", "abstract" },
		{ "notes.txt",    "notes"    }
	};

	assert(b != NULL);
	assert(st != NULL);
	assert(key != NULL);
	assert(name != NULL);

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	for (i = 0; i < sizeof a / sizeof *a; i++) {
		if (0 != strcmp(name, a[i].path)) {
			continue;
		}

		f = find_field(e->field, a[i].name);
		if (e == NULL) {
			return -ENOENT;
		}

		if (f->value == NULL || f->value->next != NULL) {
			return -EBADFD;
		}

		st->st_mode  = S_IFREG | 0444;
		st->st_nlink = 1;
		st->st_size  = strlen(f->value->text); /* XXX: strlen() of all f->value comma seperated */

		return 0;
	}

	f = find_field(e->field, "file");
	if (f != NULL) {
		struct bib_value *v;
		const char *n;

		for (v = f->value; v != NULL; v = v->next) {
			n = filename(v->text);

			if (0 != strcmp(name, n)) {
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
	}

	if (0 == strcmp(name, "index.bib")) {
		st->st_mode  = S_IFREG | 0444;
		st->st_nlink = 1;
		st->st_size  = 57; /* TODO: size of formatted entry */

		return 0;
	}

	return -ENOENT;
}

int
op_readlink_field(struct bibfs_state *b, char *buf, size_t bufsz,
	const char *key, const char *name)
{
	struct bib_entry *e;
	struct bib_field *f;
	struct bib_value *v;

	assert(b != NULL);
	assert(buf != NULL);
	assert(key != NULL);
	assert(name != NULL);

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	f = find_field(e->field, "file");
	if (f == NULL) {
		return -ENOENT;
	}

	for (v = f->value; v != NULL; v = v->next) {
		if (0 != strcmp(name, filename(v->text))) {
			continue;
		}

		if (strlen(v->text) + 1 > bufsz) {
			return -ENAMETOOLONG;
		}

		/* TODO: cli option to prefix a directory for non-absolute paths */
		strcpy(buf, v->text);

		return 0;
	}

	return -ENOENT;
}

int
op_open_field(struct bibfs_state *b, struct fuse_file_info *fi,
	const char *key, const char *name)
{
	struct bib_entry *e;
	struct bib_field *f;
	struct bib_value *v;
	size_t i;

	/* TODO: share with file contents. add a callback to concat values */
	struct {
		const char *path;
		const char *name;
	} a[] = {
		{ "abstract.txt", "abstract" },
		{ "notes.txt",    "notes"    }
	};

	assert(b != NULL);
	assert(fi != NULL);
	assert(key != NULL);
	assert(name != NULL);

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	for (i = 0; i < sizeof a / sizeof *a; i++) {
		if (0 != strcmp(a[i].path, name)) {
			continue;
		}

		f = find_field(e->field, a[i].name);
		if (f == NULL) {
			continue;
		}

		goto done;
	}

	if (0 == strcmp(name, "index.bib")) {
		goto done;
	}

	return -ENOENT;

done:

	if ((fi->flags & 03) != O_RDONLY) {
		return -EACCES;
	}

	return 0;
}

int
op_read_field(struct bibfs_state *b, char *buf, size_t size, off_t offset, struct fuse_file_info *fi,
	const char *key, const char *name)
{
	struct bib_entry *e;
	struct bib_field *f;
	const char *s;
	size_t i;
	size_t l;
	int n;

	/* TODO: share with file contents. add a callback to concat values */
	struct {
		const char *path;
		const char *name;
	} a[] = {
		{ "abstract.txt", "abstract" },
		{ "notes.txt",    "notes"    }
	};

	assert(b != NULL);
	assert(fi != NULL);
	assert(buf != NULL);
	assert(offset <= size);
	assert(key != NULL);
	assert(name != NULL);

	(void) fi;

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	for (i = 0; i < sizeof a / sizeof *a; i++) {
		if (0 != strcmp(a[i].path, name)) {
			continue;
		}

		f = find_field(e->field, a[i].name);
		if (f == NULL) {
			continue;
		}

		/* TODO: concat all values and append a newline */
		s = f->value->text;

		goto done;
	}

	if (0 == strcmp(name, "index.bib")) {
		/* TODO: use out() to write a single entry to memory */

		s = "@TODO {TODO\n\ttodo = {TODO}\n}\n";

		goto done;
	}

	return -ENOENT;

done:

	l = strlen(s);
	if (offset >= l) {
		return 0;
	}

	if (offset + size > l) {
		n = l - offset;
	}

	memcpy(buf, s + offset, n);

	return n;
}

