/*
 * Copyright 2014-2017 Katherine Flavel
 *
 * See LICENCE for the full copyright terms.
 */

#ifndef BIB_FREE_H
#define BIB_FREE_H

struct bib_entry;

void
bib_free_entry(struct bib_entry *e);

void
bib_free_field(struct bib_field *f);

void
bib_free_value(struct bib_value *v);

#endif

