#define _POSIX_C_SOURCE 2

#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>

#include "debug.h"
#include "lex.h"

unsigned debug;

static int
debug_flags(const char *s)
{
	for ( ; *s != '\0'; s++) {
		switch (*s) {
		case 'a': debug = ~0U;          break;
		case 'b': debug |= DEBUG_BUF;   break;
		case 'l': debug |= DEBUG_LEX;   break;

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
	struct lex_state l;

	l.b = 0;
	l.f = stdin;
	l.p = l.buf;

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
		struct lex_tok t;

		do {
			lex_next(&l, &t);

			assert(t.type != tok_nl);

			if (t.type == tok_error) {
				goto error;
			}

			if (t.type == tok_panic) {
				errno = ENOMEM;
				goto error;
			}
		} while (t.type != tok_eof);
	}

	return 0;

error:

	return 1;

usage:

	fprintf(stderr, "usage: bibfs [-d abl]\n");

	return 1;
}

