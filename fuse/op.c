#define _POSIX_C_SOURCE 200809L
#define FUSE_USE_VERSION 26

#include <sys/types.h>
#include <sys/stat.h>

#include <fuse.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <bib/bib.h>
#include <bib/find.h>

#include "internal.h"

static int
bibfs_getattr(const char *path, struct stat *st)
{
	struct bibfs_state *b = fuse_get_context()->private_data;
	static const struct stat st_default;
	const struct bib_entry *e;

	assert(path != NULL);
	assert(st != NULL);
	assert(b != NULL);
	assert(b->f != NULL); /* XXX: open if neccessary */

	*st = st_default;

	if (0 == strcmp(path, "/")) {
		st->st_mode  = S_IFDIR | 0755;
		st->st_nlink = bib_count(b->e) + 1;

		/* TODO: time etc from b->st */

		return 0;
	}

	if (path[0] != '/') {
		return -ENOENT;
	}

	path += strspn(path, "/");

	e = find_entry(b->e, path, strcspn(path, "/"));
	if (e == NULL) {
		return -ENOENT;
	}

	path += strcspn(path, "/");
	path += strspn (path, "/");

	/* "/key" */
	if (path[0] == '\0') {
		st->st_mode  = S_IFDIR | 0755;
		st->st_nlink = 57; /* TODO */

		return 0;
	}

	/* "/key/abstract.txt" */
	{
		size_t i;

		/* TODO: share with file contents. add a callback to concat values */
		struct {
			const char *path;
			const char *field;
		} a[] = {
			{ "abstract.txt", "abstract" },
			{ "notes.txt",    "notes"    }
		};

		for (i = 0; i < sizeof a / sizeof *a; i++) {
			const struct bib_field *f;

			if (0 != strcmp(path, a[i].path)) {
				continue;
			}

			f = find_field(e->field, a[i].field, strlen(a[i].field));
			if (f == NULL) {
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
	}

	return -ENOENT;
}

static int
bibfs_readdir(const char *path, void *buf, fuse_fill_dir_t fill,
	off_t offset, struct fuse_file_info *fi)
{
	struct bibfs_state *b = fuse_get_context()->private_data;

	assert(path != NULL);
	assert(buf != NULL);
	assert(fi != NULL);

	(void) offset;
	(void) fi;

	if (0 == strcmp(path, "/")) {
		const struct bib_entry *e;

		fill(buf, ".",  NULL, 0);
		fill(buf, "..", NULL, 0);

		for (e = b->e; e != NULL; e = e->next) {
			fill(buf, e->key, NULL, 0);
		}

		return 0;
	}

	return -ENOENT;
}

void
bibfs_init(struct fuse_operations *op)
{
	assert(op != NULL);

	op->getattr = bibfs_getattr;
	op->readdir = bibfs_readdir;
}

