/*
 * Copyright 2014-2017 Katherine Flavel
 *
 * See LICENCE for the full copyright terms.
 */

#define _XOPEN_SOURCE 500

/*
 * Note that errors are considered fatal here, since they may only occur from
 * design flaws, hence sit in the same role as assertions.
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <pthread.h>

#include "rwlock.h"

struct rwlock {
	pthread_rwlock_t rwlock;
};

static void
destroyattr(pthread_rwlockattr_t *attr)
{
	int r;

	assert(attr != NULL);

	r = pthread_rwlockattr_destroy(attr);
	assert(0 == r);

	(void) r;
}

struct rwlock *
rwlock_create(void)
{
	pthread_rwlockattr_t attr;
	struct rwlock *rw;
	int r;

	rw = malloc(sizeof *rw);
	if (rw == NULL) {
		return NULL;
	}

	r = pthread_rwlockattr_init(&attr);
	if (0 != r) {
		errno = r;
		return NULL;
	}

#ifndef NDEBUG
	/*
	 * If there were an equivalent to PTHREAD_MUTEX_ERRORCHECK for
	 * pthread_rwlockattr_t, we would set it here.
	 */
#endif

	r = pthread_rwlock_init(&rw->rwlock, &attr);
	if (0 != r) {
		errno = r;
		goto error;
	}

	destroyattr(&attr);

	return rw;

error:

	destroyattr(&attr);

	return NULL;
}

void
rwlock_destroy(struct rwlock *rw)
{
	int r;

	assert(rw != NULL);

	r = pthread_rwlock_destroy(&rw->rwlock);
	assert(0 == r);

	(void) r;

	free(rw);
}

void
rwlock_lock(struct rwlock *rw, enum rwlock_type type)
{
	int r;

	assert(rw != NULL);

	switch (type) {
	case RWLOCK_READ:  r = pthread_rwlock_rdlock(&rw->rwlock); break;
	case RWLOCK_WRITE: r = pthread_rwlock_wrlock(&rw->rwlock); break;

	default:
		errno = EINVAL;
		abort();
	}

	if (0 != r) {
		errno = r;
		perror("pthread_rwlock_wrlock");
		abort();
	}
}

void
rwlock_unlock(struct rwlock *rw)
{
	int r;

	assert(rw != NULL);

	r = pthread_rwlock_unlock(&rw->rwlock);
	if (0 != r) {
		errno = r;
		perror("pthread_rwlock_unlock");
		abort();
	}
}

