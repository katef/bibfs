#include <assert.h>
#include <stdio.h>

#include <bib/bib.h>
#include <bib/out.h>

/* TODO: escaping and whatnot */

static void
out_value(FILE *f, const struct bib_value *v)
{
	const struct bib_value *p;

	assert(f != NULL);

	for (p = v; p != NULL; p = p->next) {
		fprintf(f, "\"%s\"%s ", p->text, p->next ? "," : "");
	}
}

static void
out_field(FILE *f, const struct bib_field *q)
{
	const struct bib_field *p;

	assert(f != NULL);

	for (p = q; p != NULL; p = p->next) {
		fprintf(f, "      {\n");
		fprintf(f, "        name:   \"%s\",\n", p->name);
		fprintf(f, "        values: [ ");
		out_value(f, p->value);
		fprintf(f, "]\n");
		fprintf(f, "      }%s\n", p->next ? "," : "");
	}
}

void
out_json(FILE *f, const struct bib_entry *e)
{
	const struct bib_entry *p;

	assert(f != NULL);

	fprintf(f, "[\n");

	for (p = e; p != NULL; p = p->next) {
		fprintf(f, "  {\n");
		fprintf(f, "    type:   \"%s\",\n", p->type);
		fprintf(f, "    key:    \"%s\",\n", p->key);
		fprintf(f, "    fields: [\n");
		out_field(f, p->field);
		fprintf(f, "    ]\n");
		fprintf(f, "  }%s\n", p->next ? ",\n" : "");
	}

	fprintf(f, "]\n");
}

