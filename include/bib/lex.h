/*
 * Copyright 2014-2017 Katherine Flavel
 *
 * See LICENCE for the full copyright terms.
 */

#ifndef BIB_LEX_H
#define BIB_LEX_H

enum lex_type {
	tok_error  = -1,

	tok_eof    = '\0',
	tok_nl     = '\n',
	tok_str    = '\'',
	tok_word   = 'a',
	tok_equ    = '=',
	tok_comma  = ',',
	tok_at     = '@',
	tok_cat    = '#',
	tok_obrace = '{',
	tok_cbrace = '}'
};

struct lex_tok {
	enum lex_type type;
	const char *s;
	const char *e;
};

struct lex_state {
	unsigned b;
	char *buf;
	const char *p;
	FILE *f;
};

void
lex_next(struct lex_state *l, struct lex_tok *t);

#endif

