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

static void
out_entry(FILE *f, const struct bib_entry *e)
{
	assert(e != NULL);
	assert(e->type != NULL);
	assert(e->key != NULL);

	fprintf(f, "@%s{%s,\n", e->type, e->key);
	out_field(f, e->field);
	fprintf(f, "}\n");
	fprintf(f, "\n");
}

void
out_bibtex(FILE *f, const struct bib_entry *e, int all)
{
	const struct bib_entry *p;

	assert(f != NULL);

	if (!all) {
		out_entry(f, e);
		return;
	}

	for (p = e; p != NULL; p = p->next) {
		out_entry(f, e);
	}
}

