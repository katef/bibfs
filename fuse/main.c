#define _XOPEN_SOURCE 500
#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <unistd.h>

#include <stdio.h>
#include <errno.h>

#include <bib/bib.h>
#include <bib/debug.h>

#include "../build/lib/parser.h" /* XXX: needs a formal api */

#include "internal.h"
#include "op.h"

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
	struct bibfs_state b;

	b.path = NULL;
	b.f    = NULL;

	/* TODO: if we're being called from /etc/fstab, drop privledges to non-root */

	{
		int c;

		while (c = getopt(argc, argv, "d:f:"), c != -1) {
			switch (c) {
			case 'd':
				if (-1 == debug_flags(optarg)) {
					goto usage;
				}
				break;

			case 'f':
				b.path = optarg;
				b.f = fopen(b.path, "r");
				if (b.f == NULL) {
					perror(b.path);
					return 1;
				}
				break;

			default:
				goto usage;
			}
		}

		argc -= optind;
		argv += optind;
	}

	if (b.f == NULL) {
		goto usage;
	}

	/* TODO: move into bibfs_reload() or somesuch */
	{
		errno = 0;
		b.e =  bib_parse(b.f);
		if (b.e == NULL && errno != 0) {
			perror("bib_parse");
			/* TODO: note we continue anyway; eventually this might become readable */
		}

		if (-1 == bib_refactor(b.e)) {
			perror("bib_refactor");
			return 1;
		}

		/* TODO: convert to tree or qsort for quick lookup by key */
	}

	{
		static struct fuse_operations op;

		bibfs_init(&op);

/* XXX: are argc and argv correct? */
/* XXX: no, because argv[0] is supposed to be the filename */
/* TODO: use the "--" as argv[0] instead */

		return fuse_main(argc, argv, &op, &b);
	}

usage:

	fprintf(stderr, "usage: bibfs [-d ablc] -f file.bib [-- 'bibfs' [fuse options]]\n");

	return 1;
}

