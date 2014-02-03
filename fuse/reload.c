#define _POSIX_SOURCE

#include <sys/types.h>
#include <sys/stat.h>

#include <assert.h>
#include <stdio.h>
#include <errno.h>

#include <bib/bib.h>
#include <bib/free.h>
#include <bib/refactor.h>

#include "../build/lib/parser.h" /* XXX: needs a formal api */

#include "internal.h"

int
bibfs_reload(struct bibfs_state *b)
{
	struct stat st;

	assert(b != NULL);
	assert(b->path != NULL);

	if (-1 == stat(b->path, &st)) {
		return -1;
	}

	if (b->f != NULL) {
		if (st.st_mtime == b->st.st_mtime) {
			return 0;
		}

		(void) fclose(b->f);
	}

	b->f = fopen(b->path, "r");
	if (b->f == NULL) {
		goto error;
	}

	if (-1 == fstat(fileno(b->f), &b->st)) {
		goto error;
	}

	errno = 0;
	b->e =  bib_parse(b->f);
	if (b->e == NULL && errno != 0) {
		perror("bib_parse");
		/* TODO: note we continue anyway; eventually this might become readable */
		/* TODO: depending on errno, try re-fopen()ing b->f and parse again (just once) */
	}

	if (-1 == bib_refactor(b->e)) {
		goto error;
	}

	/* TODO: convert to tree or qsort for quick lookup by key */

	return 0;

error:

	perror(b->path);

	bib_free_entry(b->e);
	b->e = NULL;

	return -1;
}

