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
#include "field.h"
#include "op.h"

static int
field_getattr(struct bibfs_state *b,
	struct stat *st,
	const char *key, const char *name, const char *ext)
{
	struct bib_entry *e;
	struct bib_field *f;
	size_t i;

	assert(b != NULL);
	assert(st != NULL);
	assert(key != NULL && name != NULL && ext == NULL);

	(void) ext;

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	for (i = 0; i < sizeof fields / sizeof *fields; i++) {
		if (0 != strcmp(fields[i].name, name)) {
			continue;
		}

		f = find_field(e->field, fields[i].name);
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

	return -ENOENT;
}

static int
field_open(struct bibfs_state *b,
	struct fuse_file_info *fi,
	const char *key, const char *name, const char *ext)
{
	struct bib_entry *e;
	struct bib_field *f;
	size_t i;

	assert(b != NULL);
	assert(fi != NULL);
	assert(key != NULL && name != NULL && ext == NULL);

	(void) ext;

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	for (i = 0; i < sizeof fields / sizeof *fields; i++) {
		if (0 != strcmp(fields[i].name, name)) {
			continue;
		}

		f = find_field(e->field, fields[i].name);
		if (f == NULL) {
			continue;
		}

		if ((fi->flags & 03) != O_RDONLY) {
			return -EACCES;
		}

		return 0;
	}

	return -ENOENT;
}

static int
field_read(struct bibfs_state *b,
	char *buf, size_t size, off_t offset, struct fuse_file_info *fi,
	const char *key, const char *name, const char *ext)
{
	struct bib_entry *e;
	struct bib_field *f;
	const char *s;
	size_t i;

	assert(b != NULL);
	assert(fi != NULL);
	assert(buf != NULL);
	assert(offset <= size);
	assert(key != NULL && name != NULL && ext == NULL);

	(void) fi;
	(void) ext;

	e = find_entry(b->e, key);
	if (e == NULL) {
		return -ENOENT;
	}

	for (i = 0; i < sizeof fields / sizeof *fields; i++) {
		if (0 != strcmp(fields[i].name, name)) {
			continue;
		}

		f = find_field(e->field, fields[i].name);
		if (f == NULL) {
			continue;
		}

		/* TODO: concat all values and append a newline */
		/* TODO: cache ala .bib and .zim */
		s = f->value->text;

		return sread(s, buf, size, offset);
	}

	return -ENOENT;
}

struct bibfs_op op_field = {
	field_getattr,
	NULL,
	NULL,
	field_open,
	field_read
};

