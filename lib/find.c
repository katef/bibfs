#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <bib/bib.h>
#include <bib/find.h>

size_t
bib_count(const struct bib_entry *e)
{
	size_t n;

	for (n = 0; e != NULL; e = e->next) {
		n++;
	}

	return n;
}

struct bib_entry *
find_entry(struct bib_entry *e, const char *path)
{
	assert(path != NULL);

	for ( ; e != NULL; e = e->next) {
		if (0 == strcmp(path, e->key)) {
			return e;
		}
	}

	return NULL;
}

struct bib_field *
find_field(struct bib_field *f, const char *name)
{
	assert(name != NULL);

	for ( ; f != NULL; f = f->next) {
		if (0 == strcmp(name, f->name)) {
			return f;
		}
	}

	return NULL;
}

