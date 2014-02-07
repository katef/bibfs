#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <bib/bib.h>

struct bib_value *
bib_new_value(char *text)
{
	struct bib_value *v;

	v = malloc(sizeof *v + strlen(text) + 1);
	if (v == NULL) {
		return NULL;
	}

	v->text = strcpy((char *) v + sizeof *v, text);
	v->next = NULL;

	return v;
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

