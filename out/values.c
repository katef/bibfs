#include <assert.h>
#include <stdio.h>

#include <bib/bib.h>
#include <bib/out.h>

const char *
lookup_delim(const char *name)
{
	size_t i;

	const struct {
		const char *name;
		const char *delim;
	} a[] = {
		{ "author", "\n\tand " },
		{ "year",   ", "       },
		{ "file",   ";"        },
		{ "title",  "\n\t"     },
		{ "doi",    "\n\t"     },
		{ "url",    " "        }
	};

	for (i = 0; i < sizeof a / sizeof *a; i++) {
		if (0 == strcmp(a[i].name, name)) {
			return a[i].delim;
		}
	}

	return ";\n\t";
}

void
out_values(FILE *f, const struct bib_value *v,
	void (*puts)(FILE *, const char *),
	const char *delim)
{
	const struct bib_value *p;

	assert(f != NULL);
	assert(puts != NULL);
	assert(delim != NULL);

	for (p = v; p != NULL; p = p->next) {
		puts(f, p->text);

		if (p->next != NULL) {
			fprintf(f, "%s", delim);
		}
	}
}

