#ifndef OUT_H
#define OUT_H

void out_bibtex(FILE *f, const struct bib_entry *e);
void out_json(FILE *f, const struct bib_entry *e);

#endif

