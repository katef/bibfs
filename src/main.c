#define _POSIX_C_SOURCE 2

#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <bib/bib.h>
#include <bib/debug.h>
#include <bib/out.h>

#include "../build/lib/parser.h" /* XXX: needs a formal api */

unsigned debug;

static int
debug_flags(const char *s)
{
	for ( ; *s != '\0'; s++) {
		switch (*s) {
		case 'a': debug = ~0U;        break;
		case 'b': debug |= DEBUG_BUF; break;
		case 'l': debug |= DEBUG_LEX; break;
		case 'c': debug |= DEBUG_ACT; break;

		default:
			fprintf(stderr, "-d: unrecognised flag '%c'\n", *s);
			return -1;
		}
	}

	return 0;
}

static void
	(*out_lookup(const char *s))
	(FILE *, const struct bib_entry *)
{
	size_t i;

	struct {
		const char *name;
		void (*f)(FILE *, const struct bib_entry *);
	} a[] = {
		{ "bibtex", out_bibtex },
		{ "json",   out_json   }
	};

	for (i = 0; i < sizeof a / sizeof *a; i++) {
		if (0 == strcmp(a[i].name, s)) {
			return a[i].f;
		}
	}

	return NULL;
}

int
main(int argc, char *argv[])
{
	void (*out)(FILE *, const struct bib_entry *);

	out = NULL;

	{
		int c;

		while (c = getopt(argc, argv, "d:e:"), c != -1) {
			switch (c) {
			case 'd':
				if (-1 == debug_flags(optarg)) {
					goto usage;
				}
				break;

			case 'e':
				out = out_lookup(optarg);
				if (out == NULL) {
					goto usage;
				}
				break;

			default:
				goto usage;
			}
		}

		argc -= optind;
		argv += optind;
	}

	if (argc != 0) {
		goto usage;
	}

	{
		struct bib_entry *e;

		errno = 0;
		e =  bib_parse(stdin);
		if (e == NULL && errno != 0) {
			perror("bib_parse");
		}

		bib_refactor(e);
		/* TODO: convert to tree or qsort for quick lookup by key */

		if (out != NULL) {
			out(stdout, e);
		}
	}

	return 0;

usage:

	fprintf(stderr, "usage: bibfs [-d ablc] [-e json|bibtex]\n");

	return 1;
}

