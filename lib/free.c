#include <assert.h>
#include <stdlib.h>

#include <bib/bib.h>
#include <bib/free.h>

void
bib_free_value(struct bib_value *v)
{
	struct bib_value *next;

	for ( ; v != NULL; v = next) {
		next = v->next;

		free(v);
	}
}

void
bib_free_field(struct bib_field *f)
{
	struct bib_field *next;

	for ( ; f != NULL; f = next) {
		next = f->next;

		bib_free_value(f->value);

		free(f);
	}
}

void
bib_free_entry(struct bib_entry *e)
{
	struct bib_entry *next;

	for ( ; e != NULL; e = next) {
		next = e->next;

		assert(e->type != NULL);

		bib_free_field(e->field);
		free(e->type);

		if (e->zim != NULL) {
			free(e->zim);
		}

		free(e);
	}
}

