#ifndef BIB_H
#define BIB_H

struct bib_value {
	char *text;

	struct bib_value *next;
};

struct bib_field {
	char *name;
	struct bib_value *value;

	struct bib_field *next;
};

struct bib_entry {
	char *type;
	char *key;

	struct bib_field *field;

	struct bib_entry *next;
};

struct bib_value *
bib_new_value(char *text);

struct bib_field *
bib_new_field(char *name, struct bib_value *value);

struct bib_entry *
bib_new_entry(char *type, char *key, struct bib_field *field);

void
bib_refactor(struct bib_entry *e);

#endif

