/*
 * Copyright 2014-2017 Katherine Flavel
 *
 * See LICENCE for the full copyright terms.
 */

#include <assert.h>
#include <string.h>

#include <bib/bib.h>
#include <bib/refactor.h>

static void
move_value(struct bib_value **dst, struct bib_value **src)
{
	assert(dst != NULL);
	assert(src != NULL);

	while (*dst != NULL) {
		dst = &(*dst)->next;
	}

	*dst = *src;
	*src = NULL;
}

void
bib_merge(struct bib_field *p)
{
	struct bib_field *q;

	for (q = p->next; q != NULL; q = q->next) {
		assert(p->name != NULL);
		assert(q->name != NULL);

		if (0 == strcmp(p->name, q->name)) {
			move_value(&p->value, &q->value);
		}
	}
}

