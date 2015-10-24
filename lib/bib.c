#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <bib/bib.h>
#include <bib/tex.h>

/*
 * Maximum leeway for expanding out TeX escapes to utf8 sequences.
 * The worst case here is one byte in TeX to several bytes utf8.
 */
#define UTF8 3

struct bib_value *
bib_new_value(char *text, int normalisecase)
{
	struct bib_value *v, *w;

	v = malloc(sizeof *v + strlen(text) * UTF8 + 1);
	if (v == NULL) {
		return NULL;
	}

	v->text = tex_escape((char *) v + sizeof *v, text, normalisecase);
	v->next = NULL;

	w = realloc(v, sizeof *v + strlen(v->text) + 1);
	if (w == NULL) {
		return v;
	}

	return w;
}

struct bib_field *
bib_new_field(char *name, struct bib_value *value)
{
	struct bib_field *f;

	assert(name != NULL);

	f = malloc(sizeof *f + strlen(name) + 1);
	if (f == NULL) {
		return NULL;
	}

	f->name  = strcpy((char *) f + sizeof *f, name);
	f->value = value;
	f->next  = NULL;

	return f;
}

struct bib_entry *
bib_new_entry(char *type, char *key, struct bib_field *field)
{
	struct bib_entry *e;

	assert(type != NULL);
	assert(key != NULL);

	e = malloc(sizeof *e + strlen(key) + 1);
	if (e == NULL) {
		return NULL;
	}

	e->type  = type;
	e->key   = strcpy((char *) e + sizeof *e, key);
	e->field = field;
	e->zim   = NULL;
	e->bib   = NULL;
	e->next  = NULL;

	return e;
}

