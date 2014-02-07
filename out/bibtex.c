#include <assert.h>
#include <stdio.h>

#include <bib/bib.h>
#include <bib/out.h>

/* TODO: escaping and whatnot */
static void
out_str(FILE *f, const char *s)
{
	const char *p;

	for (p = s; *p != '\0'; p++) {
		switch (*p) {
		case '\\': fputs("\\\\", f); continue;

		case '{': fputs("\\{", f); continue;
		case '}': fputs("\\}", f); continue;
		case '&': fputs("\\&", f); continue;
		case '$': fputs("\\$", f); continue;

		case '\n': fputs(" ", f); continue;
		case '\r': fputs(" ", f); continue;
		case '\t': fputs(" ", f); continue;
		case '\v': fputs(" ", f); continue;
		case '\f': fputs(" ", f); continue;

		default:
			putc(*p, f);
		}
	}
}

static void
out_field(FILE *f, const struct bib_field *q,
	int (*filter)(const char *))
{
	const struct bib_field *p;

	assert(f != NULL);

	for (p = q; p != NULL; p = p->next) {
		const char *delim;

		if (filter != NULL && filter(p->name)) {
			continue;
		}

		delim = lookup_delim(p->name);

		fprintf(f, "  %-10s = {", p->name);

		out_values(f, p->value, out_str, delim);

		fprintf(f, "},\n");
	}
}

void
out_bibtex_entry(FILE *f, const struct bib_entry *e,
	int (*filter)(const char *))
{
	assert(e != NULL);
	assert(e->type != NULL);
	assert(e->key != NULL);

	fprintf(f, "@%s{%s,\n", e->type, e->key);
	out_field(f, e->field, filter);
	fprintf(f, "}\n");
	fprintf(f, "\n");
}

void
out_bibtex(FILE *f, const struct bib_entry *e, int all)
{
	const struct bib_entry *p;

	assert(f != NULL);

	if (!all) {
		out_bibtex_entry(f, e, NULL);
		return;
	}

	for (p = e; p != NULL; p = p->next) {
		out_bibtex_entry(f, e, NULL);
	}
}

