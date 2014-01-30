#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <bib/bib.h>

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

static void
refactor_entry(struct bib_entry *e)
{
	struct bib_field *p, *q;

	for (p = e->field; p != NULL; p = p->next) {
		for (q = p->next; q != NULL; q = q->next) {
			assert(p->name != NULL);
			assert(q->name != NULL);

			if (0 == strcmp(p->name, q->name)) {
				move_value(&p->value, &q->value);
			}
		}
	}
}

static void
cull_entry(struct bib_entry *e)
{
	struct bib_field **p, **next;

	for (p = &e->field; *p != NULL; p = next) {
		next = &(*p)->next;

		if ((*p)->value == NULL) {
			free((*p)->name);
			free(*p);
			*p = *next;
			next = p;
		}
	}
}

void
bib_refactor(struct bib_entry *e)
{
	struct bib_entry *p;

	for (p = e; p != NULL; p = p->next) {
		/* TODO: unescape e */
		/* TODO: expand out "list" fields: tag="", file="" etc */
		refactor_entry(p);
		/* TODO: remove duplicates */
		cull_entry(p);
	}
}

