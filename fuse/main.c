#define _XOPEN_SOURCE 500
#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <unistd.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <bib/debug.h>

#include "internal.h"
#include "op.h"

unsigned debug;

enum {
	key_debug
};

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
			fprintf(stderr, "-o bibfs_debug: unrecognised flag '%c'\n", *s);
			return -1;
		}
	}

	return 0;
}

static int
opt_proc(void *data, const char *arg, int key, struct fuse_args *outargs)
{
	struct bibfs_state *b = data;

	(void) outargs;

	switch (key) {
	case FUSE_OPT_KEY_NONOPT:
		if (b->path == NULL) {
			b->path = arg;
			return 0;
		}

		/* keep for fuse_main */
		return 1;

	case key_debug:
		if (0 != strncmp(arg, "bibfs_debug=", strlen("bibfs_debug="))) {
			return -1;
		}

		return debug_flags(arg + strlen("bibfs_debug="));

	default:
		return 1;
	}
}

int
main(int argc, char *argv[])
{
	static struct fuse_operations op;
	struct fuse_args args;
	struct bibfs_state b;

	struct fuse_opt opts[] = {
		{ "bibfs_debug=", -1U, key_debug },
		{ "bibfs_prefix=%s", offsetof(struct bibfs_state, prefix), -1 },
		{ "bibfs_zim=true",  offsetof(struct bibfs_state, zim),     1 },
		{ "bibfs_zim=false", offsetof(struct bibfs_state, zim),     0 },
		FUSE_OPT_END
	};

	b.prefix = NULL;
	b.path   = NULL;
	b.f      = NULL;
	b.zim    = 0;

	args.argc      = argc;
	args.argv      = argv;
	args.allocated = 0;

	/*
	 * I really dislike this. The FUSE options parsing system (typical of things
	 * from the Linux ecosystem) is unneccessarily overfeatured, complex, and
	 * bewildering both as a user and as a developer. Other than stupid tricks
	 * involving offsetof() and the like, I particularly dislike scattering options
	 * information throughout this file, rather than dealing with it all in main().
	 * It is also bewildering as to why an options parser would ever need to
	 * allocate memory.
	 *
	 * I was much happier with my origional getopt parsing here, and passing the
	 * remainder after "--" to FUSE. However, I've switched to using fuse_opt_parse()
	 * here instead, for consistency with other FUSE programs.
	 */
	if (-1 == fuse_opt_parse(&args, &b, opts, opt_proc)) {
		goto usage;
	}

	/* TODO: if we're being called from /etc/fstab, drop privledges to non-root */

	if (b.path == NULL) {
		goto usage;
	}

	bibfs_init(&op);

	return fuse_main(args.argc, args.argv, &op, &b);

usage:

	fprintf(stderr,
		"usage: bibfs [fuse options] [-o bibfs_debug={ablc}] [-o bibfs_zim={true|false}] "
		"file.bib mountpoint\n");

	return 1;
}

