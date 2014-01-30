#include <assert.h>
#include <stdio.h>

#include <bib/bib.h>
#include <bib/out.h>

/* TODO: escaping and whatnot */

static void
out_value(FILE *f, const char *name, const struct bib_value *v)
{
	const struct bib_value *p;

	assert(f != NULL);

	for (p = v; p != NULL; p = p->next) {
		fprintf(f, "  %s = {%s},\n", name, p->text);
	}
}

static void
out_field(FILE *f, const struct bib_field *q)
{
	const struct bib_field *p;

	assert(f != NULL);

	for (p = q; p != NULL; p = p->next) {
		out_value(f, p->name, p->value);
	}
}

void
out_bibtex(FILE *f, const struct bib_entry *e)
{
	const struct bib_entry *p;

	assert(f != NULL);

	for (p = e; p != NULL; p = p->next) {
		fprintf(f, "@%s{%s,\n", p->type, p->key);
		out_field(f, p->field);
		fprintf(f, "}\n");
		fprintf(f, "\n");
	}
}

