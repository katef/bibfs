/*
 * Copyright 2014-2017 Katherine Flavel
 *
 * See LICENCE for the full copyright terms.
 */

#define _POSIX_C_SOURCE 2

#include <unistd.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <bib/debug.h>
#include <bib/bib.h>
#include <bib/out.h>
#include <bib/free.h>
#include <bib/refactor.h>

#include "../lib/parser.h" /* XXX: needs a formal api */

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
	(FILE *, const struct bib_entry *, int)
{
	size_t i;

	struct {
		const char *name;
		void (*f)(FILE *, const struct bib_entry *, int);
	} a[] = {
		{ "bibtex", out_bibtex  },
		{ "json",   out_json    },
		{ "xml",    out_xml     },
		{ "zim",    out_zim     }
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
	void (*out)(FILE *, const struct bib_entry *, int);
	FILE *f;

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

	if (argc != 1) {
		goto usage;
	}

	f = fopen(argv[0], "r");
	if (f == NULL) {
		perror(argv[0]);
		return 1;
	}

	{
		struct bib_entry *e;

		errno = 0;
		e =  bib_parse(f);
		if (e == NULL && errno != 0) {
			perror("bib_parse");
		}

		if (-1 == bib_refactor(e)) {
			perror("bib_refactor");
			return 1;
		}

		/* TODO: convert to tree or qsort for quick lookup by key */

		if (out != NULL) {
			out(stdout, e, 1);
		}

		bib_free_entry(e);
	}

	fclose(f);

	return 0;

usage:

	fprintf(stderr, "usage: bibfs [-d ablc] [-e json|bibtex|xml|zim] <file.bib>\n");

	return 1;
}

