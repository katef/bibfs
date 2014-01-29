#define _POSIX_C_SOURCE 2

#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>

#include "debug.h"
#include "parser.h"

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

int
main(int argc, char *argv[])
{
	{
		int c;

		while (c = getopt(argc, argv, "d:"), c != -1) {
			switch (c) {
			case 'd':
				if (-1 == debug_flags(optarg)) {
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

		/* TODO: dump e */
	}

	return 0;

usage:

	fprintf(stderr, "usage: bibfs [-d ablc]\n");

	return 1;
}

