#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#include <bib/debug.h>
#include <bib/lex.h>

#define WHITE " \t\v\f\n"

static enum lex_type
zone_body(struct lex_state *l, const char **p, const char **s, const char **e)
{
	assert(l != NULL);
	assert(p != NULL);
	assert(s != NULL && e != NULL);
	assert(l->b == 1);

	*s = *p;

	for (;;) {
		switch (**p) {
		case '\n':
		case '\0':
			return tok_panic;

		case '}':
			if (l->b == 1) {
				*e = *p;
				(*p)++;
				return tok_str;
			}

			l->b--;
			(*p)++;
			continue;

		case '{':
			if (l->b == UINT_MAX) {
				errno = ERANGE;
				return tok_error;
			}

			l->b++;
			(*p)++;
			continue;

		default:
			(*p)++;
			continue;
		}
	}
}

static enum lex_type
zone_main(struct lex_state *l, const char **p, const char **s, const char **e)
{
	assert(l != NULL);
	assert(p != NULL);
	assert(s != NULL && e != NULL);

	*p += strspn(*p, WHITE);

	switch (**p) {
	case '%':
		*p += strcspn(*p, "\n");
	case '\0':
		*s = *p;
		*e = *s;
		return tok_nl;

	case '{':
		if (l->b == 1) {
			(*p)++;
			return zone_body(l, p, s, e);
		}

		l->b = 1;

		*s = *p;
		*e = *s + 1;
		(*p)++;
		return **s;

	case '}':
		if (l->b != 1) {
			errno = EINVAL;
			return tok_error;
		}

		l->b = 0;
	case '=':
	case ',':
	case '@':
		*s = *p;
		*e = *s + 1;
		(*p)++;
		return **s;

	case '\"':
		(*p)++;
		*s = *p;
		*p += strcspn(*p, "\"");
		*e = *p;
		(*p) += **p == '\"';
		return tok_str;

	default:
		*s = *p;
		*p += strcspn(*p, WHITE "@%=,{}\"");
		*e = *p;
		return tok_str;
	}
}

void
lex_next(struct lex_state *l, struct lex_tok *t)
{
	assert(l != NULL);
	assert(l->f != NULL);
	assert(l->p != NULL);
	assert(t != NULL);

	if (*l->p == '\0') {
		l->buf[sizeof l->buf - 1] = 'x';
		errno = 0;

		if (!fgets(l->buf, sizeof l->buf, l->f)) {
			if (errno != 0) {
				perror("fgets");

				t->type = tok_error;

				return;
			}

			t->type = tok_eof;

			return;
		}

		if (debug & DEBUG_BUF) {
			fprintf(stderr, "[%s]\n", l->buf);
		}

		if (l->buf[sizeof l->buf - 1] == '\0' && l->buf[sizeof l->buf - 2] != '\n') {
			int c;

			fprintf(stderr, "underflow; panic\n");

			while (c = fgetc(l->f), c != EOF && c != '\n')
				;

			t->type = tok_panic;

			return;
		}

		l->p = l->buf;
	}

	t->type = zone_main(l, &l->p, &t->s, &t->e);

	if (t->type == tok_nl) {
		lex_next(l, t);
		return;
	}

	if (debug & DEBUG_LEX) {
		const char *name;

		switch (t->type) {
		case tok_eof: name = "eof "; break;
		case tok_nl:  name = "nl ";  break;
		case tok_str: name = "str "; break;
		default:      name = "";     break;
		}

		fprintf(stderr, "<%s\"%.*s\">\n",
			name, (int) (t->e - t->s), t->s);
	}

	return;
}

