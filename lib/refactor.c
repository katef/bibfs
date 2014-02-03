#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <bib/bib.h>
#include <bib/refactor.h>

static int
refactor_entry(struct bib_entry *e)
{
	struct bib_field *p;

	for (p = e->field; p != NULL; p = p->next) {
		/* TODO: and probably others */
		if (0 == strcmp(p->name, "file")) {
			if (-1 == bib_split(p, ";")) {
				return -1;
			}
		}

		bib_merge(p);
	}

	/* TODO: join values which shouldn't be multiple fields */

	return 0;
}

int
bib_refactor(struct bib_entry *e)
{
	struct bib_entry *p;

	for (p = e; p != NULL; p = p->next) {
		/* TODO: unescape e */

		if (-1 == refactor_entry(p)) {
			return -1;
		}

		bib_cull(p);
	}

	return 0;
}

