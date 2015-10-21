#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <bib/bib.h>
#include <bib/refactor.h>

#define WHITE " \n\t\r\v\f"

static void
stolower(char *s)
{
	assert(s != NULL);

	while (*s != '\0') {
		*s = tolower((unsigned char) *s);
		s++;
	}
}

static void
normalise_space(char *s)
{
	char *p;

	assert(s != NULL);

	for (p = s; *p != '\0'; s++) {
		if (isspace((unsigned char) *p)) {
			p += strspn(p, WHITE);
			*s = ' ';
			continue;
		}

		*s = *p;
		p++;
	}

	*s = '\0';
}

static void
normalise_file(const struct bib_field *f)
{
	struct bib_value *v;

	assert(f != NULL);

	for (v = f->value; v != NULL; v = v->next) {
		/* ":/path/to/file.pdf:PDF" -> "/path/to/file.pdf" */
		if (v->text[0] == ':') {
			v->text++;
			v->text[strcspn(v->text, ":")] = '\0';
		}
	}
}

static void
normalise_keyname(char *s)
{
	assert(s != NULL);

	/* XXX: this is a workaround for op.c splitting by '.' */
	while (*s != '\0') {
		if (*s == '.') {
			*s = '_';
		}
		s++;
	}
}

static int
refactor_entry(struct bib_entry *e)
{
	struct bib_field *p;

	for (p = e->field; p != NULL; p = p->next) {
		stolower(p->name);
		normalise_space(p->name);

		if (0 == strcmp(p->name, "title")) {
			struct bib_value *v;

			for (v = p->value; v != NULL; v = v->next) {
				normalise_space(v->text);
			}
		}

		if (0 == strcmp(p->name, "file")) {
			if (-1 == bib_split(p, ";")) {
				return -1;
			}
		}

		if (0 == strcmp(p->name, "tags")) {
			struct bib_value *v;

			if (-1 == bib_split(p, ",")) {
				return -1;
			}

			for (v = p->value; v != NULL; v = v->next) {
				normalise_space(v->text);
			}
		}

		if (0 == strcmp(p->name, "author")) {
			struct bib_value *v;

			for (v = p->value; v != NULL; v = v->next) {
				normalise_space(v->text);
			}

			if (-1 == bib_split(p, " and ")) {
				return -1;
			}

			if (-1 == bib_split(p, ";")) {
				return -1;
			}

			for (v = p->value; v != NULL; v = v->next) {
				normalise_space(v->text);
			}
		}

		bib_merge(p);

		if (0 == strcmp(p->name, "file")) {
			normalise_file(p);
		}
	}

	/* TODO: join values which shouldn't be multiple fields */

	return 0;
}

int
bib_refactor(struct bib_entry *e)
{
	struct bib_entry *p;

	for (p = e; p != NULL; p = p->next) {
		/* TODO: unescape e */

		normalise_keyname(p->key);

		if (-1 == refactor_entry(p)) {
			return -1;
		}

		bib_cull(p);
	}

	return 0;
}

