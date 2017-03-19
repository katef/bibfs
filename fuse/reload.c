/*
 * Copyright 2014-2017 Katherine Flavel
 *
 * See LICENCE for the full copyright terms.
 */

#define _POSIX_SOURCE

#include <sys/types.h>
#include <sys/stat.h>

#include <assert.h>
#include <stdio.h>
#include <errno.h>

#include <bib/bib.h>
#include <bib/free.h>
#include <bib/refactor.h>

#include "../lib/parser.h" /* XXX: needs a formal api */

#include "internal.h"
#include "rwlock.h"

int
bibfs_reload(struct bibfs_state *b)
{
	struct stat st;

	assert(b != NULL);
	assert(b->path != NULL);

	rwlock_lock(b->rw, RWLOCK_WRITE);

	if (-1 == stat(b->path, &st)) {
		rwlock_unlock(b->rw);
		return -1;
	}

	rwlock_unlock(b->rw);

	if (b->f != NULL) {
		rwlock_lock(b->rw, RWLOCK_READ);

		if (st.st_mtime == b->st.st_mtime) {
			goto done;
		}

		rwlock_unlock(b->rw);
	}

	rwlock_lock(b->rw, RWLOCK_WRITE);

	if (b->f != NULL) {
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

done:

	rwlock_unlock(b->rw);

	return 0;

error:

	perror(b->path);

	bib_free_entry(b->e);
	b->e = NULL;

	rwlock_unlock(b->rw);

	return -1;
}

