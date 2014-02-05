#ifndef BIBFS_FUSE_FIELD_H
#define BIBFS_FUSE_FIELD_H

/* TODO: add a callback to concat values */
struct field {
	const char *path;
	const char *name;
};

static const struct field fields[] = {
	{ "abstract.txt", "abstract" },
	{ "notes.txt",    "notes"    }
};

#endif

