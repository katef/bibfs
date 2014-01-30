#ifndef BIB_OUT_H
#define BIB_OUT_H

struct bib_entry;

void out_bibtex(FILE *f, const struct bib_entry *e);
void out_json(FILE *f, const struct bib_entry *e);

#endif

