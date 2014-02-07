#include <assert.h>
#include <string.h>
#include <stdio.h>

#include <bib/bib.h>
#include <bib/find.h>
#include <bib/out.h>

static const char *
filename(const char *path)
{
	const char *p;

	assert(path != NULL);

	p = strrchr(path, '/');
	if (p == NULL) {
		return path;
	}

	return p + 1;
}

static int
filter(const char *s)
{
	size_t i;

	const char *a[] = {
		"abstract",
		"notes",
		"file",
		"tags",
		"keywords"
	};

	for (i = 0; i < sizeof a / sizeof *a; i++) {
		if (0 == strcmp(s, a[i])) {
			return 1;
		}
	}

	return 0;
}

static void
out_str(FILE *f, const char *s)
{
	fputs(s, f);
}

static void
out_tag(FILE *f, const char *s)
{
	const char *p;

	putc('@', f);

	for (p = s; *p != '\0'; p++) {
		switch (*p) {
		case ' ': putc('_', f); continue;

		default:
			putc(*p, f);
		}
	}
}

static void
out_keyword(FILE *f, const char *s)
{
	fprintf(f, "//%s//", s);
}

static void
out_file(FILE *f, const char *s)
{
	fprintf(f, " * [[%s|%s]]\n", s, filename(s));
}

/* TODO: escaping and whatnot */

static void
out_field(FILE *f, const struct bib_field *q)
{
	const struct bib_field *p;

	assert(f != NULL);

	for (p = q; p != NULL; p = p->next) {
		if (filter(p->name)) {
			continue;
		}

		fprintf(f, "**%-15s**\t", p->name);
		out_values(f, p->value, out_str, ",");
		fprintf(f, "\n");
	}
}

static void
out_entry(FILE *f, const struct bib_entry *e)
{
	const char *title;
	const struct bib_field *p;

	assert(e != NULL);
	assert(e->key != NULL);
	assert(e->type != NULL);

	p = find_field(e->field, "title");
	if (p != NULL && p->value != NULL) {
		/* TODO: concat values */
		title = p->value->text;
	} else {
		title = e->key;
	}

	fprintf(f, "===== %s =====\n", title);
	fprintf(f, "\n");

	p = find_field(e->field, "file");
	if (p != NULL) {
		out_values(f, p->value, out_file, "");
		fprintf(f, "\n");
	}

	fprintf(f, "**%-15s**\t''%s''\n",  "key",  e->key);
	fprintf(f, "**%-15s**\t@%s\n", "type", e->type);

	p = find_field(e->field, "tags");
	if (p != NULL) {
		fprintf(f, "**%-15s**\t",  "tags");
		out_values(f, p->value, out_tag, " ");
		fprintf(f, "\n");
	}

	p = find_field(e->field, "keywords");
	if (p != NULL) {
		fprintf(f, "**%-15s**\t",  "keywords");
		out_values(f, p->value, out_keyword, ", ");
		fprintf(f, "\n");
	}

	out_field(f, e->field);
	fprintf(f, "\n");

	p = find_field(e->field, "notes");
	if (p != NULL && p->value != NULL) {
		/* TODO: concat values */
		fprintf(f, "== %s ==\n", "Notes");
		fprintf(f, "  %s\n", p->value->text);
		fprintf(f, "\n");
	}

	p = find_field(e->field, "abstract");
	if (p != NULL && p->value != NULL) {
		/* TODO: concat values */
		fprintf(f, "== %s ==\n", "Abstract");
		fprintf(f, "  //%s//\n", p->value->text);
		fprintf(f, "\n");
	}

	fprintf(f, "== %s ==\n", "BibTeX");
	fprintf(f, "'''\n");
	out_bibtex_entry(f, e, filter);
	fprintf(f, "'''\n");
	fprintf(f, "\n");
}

void
out_zim(FILE *f, const struct bib_entry *e, int all)
{
	const struct bib_entry *p;

	assert(f != NULL);

	fprintf(f, "Content-Type: text/x-zim-wiki\n");
	fprintf(f, "Wiki-Format: zim 0.4\n");
	fprintf(f, "Creation-Date: 1981-08-21T23:15:00+00:00\n"); /* TODO: from b->st */
	fprintf(f, "\n");

	if (!all) {
		out_entry(f, e);
		return;
	}

	for (p = e; p != NULL; p = p->next) {
		out_entry(f, e);
		fprintf(f, "\n");
	}
}

