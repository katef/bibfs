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
find_entry(struct bib_entry *e, const char *path, size_t n)
{
	assert(path != NULL);

	for ( ; e != NULL; e = e->next) {
		if (n != strlen(e->key)) {
			continue;
		}

		if (0 != memcmp(path, e->key, n)) {
			continue;
		}

		return e;
	}

	return NULL;
}

struct bib_field *
find_field(struct bib_field *f, const char *name, size_t n)
{
	assert(name != NULL);

	for ( ; f != NULL; f = f->next) {
		if (n != strlen(f->name)) {
			continue;
		}

		if (0 != memcmp(name, f->name, n)) {
			continue;
		}

		return f;
	}

	return NULL;
}

