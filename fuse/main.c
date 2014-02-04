#define _XOPEN_SOURCE 500
#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <unistd.h>

#include <stdio.h>
#include <errno.h>

#include <bib/debug.h>

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
	b.zim  = 0;

	/* TODO: if we're being called from /etc/fstab, drop privledges to non-root */

	{
		int c;

		while (c = getopt(argc, argv, "d:f:z"), c != -1) {
			switch (c) {
			case 'd':
				if (-1 == debug_flags(optarg)) {
					goto usage;
				}
				break;

			case 'f':
				b.path = optarg;
				break;

			case 'z':
				b.zim = 1;
				break;

			default:
				goto usage;
			}
		}

		argc -= optind;
		argv += optind;
	}

	if (b.path == NULL) {
		goto usage;
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

	fprintf(stderr, "usage: bibfs [-d ablc] [-z] -f file.bib [-- 'bibfs' [fuse options]]\n");

	return 1;
}

