#include <assert.h>
#include <stdio.h>

#include <bib/bib.h>
#include <bib/out.h>

/* TODO: escaping and whatnot */
static void
out_str(FILE *f, const char *s)
{
	const char *p;

	putc('\"', f);

	for (p = s; *p != '\0'; p++) {
		switch (*p) {
		case '\n': fputs("\\n", f); continue;
		case '\r': fputs("\\r", f); continue;
		case '\t': fputs("\\t", f); continue;
		case '\v': fputs("\\v", f); continue;
		case '\f': fputs("\\f", f); continue;

		default:
			putc(*p, f);
		}
	}

	putc('\"', f);
}

static void
out_field(FILE *f, const struct bib_field *q)
{
	const struct bib_field *p;

	assert(f != NULL);

	for (p = q; p != NULL; p = p->next) {
		fprintf(f, "      {\n");
		fprintf(f, "        name:   ");
		out_str(f, p->name);
		fprintf(f, ",\n");

		fprintf(f, "        values: [ ");
		out_values(f, p->value, out_str, ", ");
		fprintf(f, "]\n");
		fprintf(f, "      }%s\n", p->next ? "," : "");
	}
}

static void
out_entry(FILE *f, const struct bib_entry *e)
{
	assert(e != NULL);
	assert(e->type != NULL);
	assert(e->key != NULL);

	fprintf(f, "  {\n");
	fprintf(f, "    type:   \"%s\",\n", e->type);
	fprintf(f, "    key:    \"%s\",\n", e->key);
	fprintf(f, "    fields: [\n");
	out_field(f, e->field);
	fprintf(f, "    ]\n");
	fprintf(f, "  }");
}

void
out_json(FILE *f, const struct bib_entry *e, int all)
{
	const struct bib_entry *p;

	assert(f != NULL);

	if (!all) {
		out_entry(f, e);
		fprintf(f, "\n");
		return;
	}

	fprintf(f, "[\n");

	for (p = e; p != NULL; p = p->next) {
		out_entry(f, e);
		fprintf(f, "%s\n", p->next ? ",\n" : "");
	}

	fprintf(f, "]\n");
}

