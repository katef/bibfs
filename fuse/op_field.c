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

		/* TODO: etc */
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

			st->st_mode  = S_IFLNK | 0444;
			st->st_nlink = 1;
			st->st_size  = strlen(n); /* XXX: size of destination? */

			return 0;
		}
	}

	/* TODO: time etc from b->st otherwise*/

	return -ENOENT;
}

