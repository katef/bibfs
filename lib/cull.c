/*
 * Copyright 2014-2017 Katherine Flavel
 *
 * See LICENCE for the full copyright terms.
 */

#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <bib/bib.h>
#include <bib/refactor.h>

void
bib_cull(struct bib_entry *e)
{
	struct bib_field **p, **next;

	for (p = &e->field; *p != NULL; p = next) {
		next = &(*p)->next;

		/* TODO: remove values with empty strings */

		if ((*p)->value == NULL) {
			free(*p);
			*p = *next;
			next = p;
		}
	}
}

