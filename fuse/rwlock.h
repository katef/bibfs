/*
 * Copyright 2014-2017 Katherine Flavel
 *
 * See LICENCE for the full copyright terms.
 */

#ifndef BIBFS_FUSE_RWLOCK_H
#define BIBFS_FUSE_RWLOCK_H

/*
 * Non-recursive read/write locks. (Attempting to lock a locked lock is illegal)
 *
 * Recursive locks are intentionally not implemented for various reasons.
 * In particular, although convenient, they are often a symptom of poor design.
 * Here we attempt to keep to a minimal set of features to aid portability.
 */

enum rwlock_type {
	RWLOCK_READ,
	RWLOCK_WRITE
};

struct rwlock *
rwlock_create(void);

void
rwlock_destroy(struct rwlock *);

void
rwlock_lock(struct rwlock *, enum rwlock_type);

void
rwlock_unlock(struct rwlock *);

#endif

