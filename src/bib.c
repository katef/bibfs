#include <assert.h>
#include <stdlib.h>

#include "bib.h"

struct bib_value *
bib_new_value(const char *text)
{
	struct bib_value *v;

	v = malloc(sizeof *v);
	if (v == NULL) {
		return NULL;
	}

	v->text = text;
	v->next = NULL;

	return v;
}

struct bib_field *
bib_new_field(const char *name, struct bib_value *value)
{
	struct bib_field *f;

	assert(name != NULL);

	f = malloc(sizeof *f);
	if (f == NULL) {
		return NULL;
	}

	f->name  = name;
	f->value = value;
	f->next  = NULL;

	return f;
}

struct bib_entry *
bib_new_entry(const char *type, const char *key, struct bib_field *field)
{
	struct bib_entry *e;

	assert(type != NULL);
	assert(key != NULL);

	e = malloc(sizeof *e);
	if (e == NULL) {
		return NULL;
	}

	e->type  = type;
	e->key   = key;
	e->field = field;
	e->next  = NULL;

	return e;
}

