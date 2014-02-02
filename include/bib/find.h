#ifndef BIB_FIND_H
#define BIB_FIND_H

struct bib_entry;

size_t
bib_count(const struct bib_entry *e);

struct bib_entry *
find_entry(struct bib_entry *e, const char *path, size_t n);

struct bib_field *
find_field(struct bib_field *f, const char *name, size_t n);

#endif

