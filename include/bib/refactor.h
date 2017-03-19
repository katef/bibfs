/*
 * Copyright 2014-2017 Katherine Flavel
 *
 * See LICENCE for the full copyright terms.
 */

#ifndef BIB_REFACTOR_H
#define BIB_REFACTOR_H

int
bib_refactor(struct bib_entry *e);

void
bib_merge(struct bib_field *p);

void
bib_cull(struct bib_entry *e);

#endif

