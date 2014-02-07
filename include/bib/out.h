#ifndef BIB_OUT_H
#define BIB_OUT_H

struct bib_entry;

void out_bibtex(FILE *f, const struct bib_entry *e, int all);
void out_zimtex(FILE *f, const struct bib_entry *e, int all);
void out_json(FILE *f, const struct bib_entry *e, int all);
void out_zim(FILE *f, const struct bib_entry *e, int all);

char *
bib_outs(const struct bib_entry *e,
	void (*out)(FILE *f, const struct bib_entry *, int));

void
out_bibtex_entry(FILE *f, const struct bib_entry *e,
	int (*filter)(const char *));

const char *
lookup_delim(const char *name);

void
out_values(FILE *f, const struct bib_value *v,
	void (*puts)(FILE *, const char *),
	const char *delim);

#endif

