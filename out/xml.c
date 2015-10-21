#include <assert.h>
#include <stdio.h>

#include <bib/bib.h>
#include <bib/out.h>

/* TODO: escaping and whatnot */
static void
out_str(FILE *f, const char *s)
{
	const char *p;

	fputs("      <value>", f);

	for (p = s; *p != '\0'; p++) {
		switch (*p) {
		case '\a': fputs("&#07;", f); continue;
		case '\b': fputs("&#08;", f); continue;
		case '\t': fputs("&#09;", f); continue;
		case '\n': fputs("&#10;", f); continue;
		case '\v': fputs("&#11;", f); continue;
		case '\f': fputs("&#12;", f); continue;
		case '\r': fputs("&#13;", f); continue;

		case '&':  fputs("&amp;", f); continue;
		case '\"': fputs("&#34;", f); continue;
		case '\'': fputs("&#39;", f); continue;

		default:
			putc(*p, f);
		}
	}

	fputs("</value>\n", f);
}

static void
out_field(FILE *f, const struct bib_field *q)
{
	const struct bib_field *p;

	assert(f != NULL);

	for (p = q; p != NULL; p = p->next) {
		fprintf(f, "    <field name='%s'>\n", p->name);
		out_values(f, p->value, out_str, ", ");
		fprintf(f, "    </field>\n");
	}
}

static void
out_entry(FILE *f, const struct bib_entry *e)
{
	assert(e != NULL);
	assert(e->type != NULL);
	assert(e->key != NULL);

	fprintf(f, "  <entry type='%s' key='%s'>\n", e->type, e->key);
	out_field(f, e->field);
	fprintf(f, "  </entry>\n");
}

void
out_xml(FILE *f, const struct bib_entry *e, int all)
{
	const struct bib_entry *p;

	assert(f != NULL);

	fprintf(f, "<bibt xmlns='http://xml.elide.org/bibt'>\n");

	if (!all) {
		out_entry(f, e);
		return;
	}

	for (p = e; p != NULL; p = p->next) {
		out_entry(f, p);
	}

	fprintf(f, "</bibt>\n");
}

