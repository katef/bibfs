/*
 * Automatically generated from the files:
 *	lib/parser.sid
 * and
 *	lib/parser.act
 * by:
 *	sid
 */

/* BEGINNING OF HEADER */

#line 56 "lib/parser.act"


	#define _POSIX_SOURCE

	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/mman.h>

	#include <assert.h>
	#include <string.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <ctype.h>

	#include <bib/debug.h>
	#include <bib/lex.h>
	#include <bib/bib.h>
	#include <bib/tex.h>

	typedef char *             string;
	typedef struct bib_field * field;
	typedef struct bib_entry * entry;
	typedef struct act_state * act_state;
	typedef struct lex_state * lex_state;

	struct act_state {
		struct lex_tok t;
		enum lex_type type, save;
	};

	#define CURRENT_TERMINAL act_state->type
	#define ERROR_TERMINAL   tok_error
	#define ADVANCE_LEXER    lex_next(lex_state, &act_state->t); \
	                         act_state->type = act_state->t.type;
	#define SAVE_LEXER(tok)  act_state->save = tok;
	#define RESTORE_LEXER    act_state->type = act_state->save;

	struct bib_entry *bib_parse(FILE *f);

#line 53 "lib/parser.c"


#ifndef ERROR_TERMINAL
#error "-s no-numeric-terminals given and ERROR_TERMINAL is not defined"
#endif

/* BEGINNING OF FUNCTION DECLARATIONS */

static void p_entries(lex_state, act_state, entry *);
static void p_entries_C_Centry(lex_state, act_state, entry *);
static void p_cats(lex_state, act_state, string *);
extern void p_bib(lex_state, act_state, entry *);
static void p_50(lex_state, act_state, string *, string *);
static void p_str_Hor_Hvar(lex_state, act_state, string *);
static void p_fields_C_Cfield(lex_state, act_state, field *);
static void p_fields(lex_state, act_state, field *);

/* BEGINNING OF STATIC VARIABLES */


/* BEGINNING OF FUNCTION DEFINITIONS */

static void
p_entries(lex_state lex_state, act_state act_state, entry *ZOe)
{
	entry ZIe;

	if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		return;
	}
	{
		entry ZIa;

		p_entries_C_Centry (lex_state, act_state, &ZIa);
		/* BEGINNING OF INLINE: 43 */
		{
			switch (CURRENT_TERMINAL) {
			case (tok_at):
				{
					entry ZIb;

					p_entries (lex_state, act_state, &ZIb);
					if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
						RESTORE_LEXER;
						goto ZL1;
					}
					/* BEGINNING OF ACTION: add-entry */
					{
#line 239 "lib/parser.act"

		assert((ZIa)->next == NULL);

		(ZIa)->next = (ZIb);
		(ZIe) = (ZIa);

		if (debug & DEBUG_ACT) {
			fprintf(stderr, "<add-entry>(a = %p, b = %p) -> (e = %p)\n",
				(void *) (ZIa), (void *) (ZIb), (void *) (ZIe));
		}
	
#line 114 "lib/parser.c"
					}
					/* END OF ACTION: add-entry */
				}
				break;
			default:
				{
					ZIe = ZIa;
				}
				break;
			case (ERROR_TERMINAL):
				RESTORE_LEXER;
				goto ZL1;
			}
		}
		/* END OF INLINE: 43 */
	}
	goto ZL0;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
ZL0:;
	*ZOe = ZIe;
}

static void
p_entries_C_Centry(lex_state lex_state, act_state act_state, entry *ZOe)
{
	entry ZIe;

	if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		return;
	}
	{
		string ZIt;
		string ZIk;
		field ZIf;

		switch (CURRENT_TERMINAL) {
		case (tok_at):
			break;
		default:
			goto ZL1;
		}
		ADVANCE_LEXER;
		switch (CURRENT_TERMINAL) {
		case (tok_word):
			/* BEGINNING OF EXTRACT: word */
			{
#line 98 "lib/parser.act"

		size_t n;

		assert(act_state->t.s != NULL);
		assert(act_state->t.e >= act_state->t.s);

		n = act_state->t.e - act_state->t.s;

		ZIt = malloc(n + 1);
		if (ZIt == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE); /* XXX: not really what i want to do */
		}

		memcpy(ZIt, act_state->t.s, n);
		ZIt[n] = '\0';
	
#line 181 "lib/parser.c"
			}
			/* END OF EXTRACT: word */
			break;
		default:
			goto ZL1;
		}
		ADVANCE_LEXER;
		/* BEGINNING OF ACTION: tolower */
		{
#line 142 "lib/parser.act"

		char *p;

		for (p = (ZIt); *p != '\0'; p++) {
			*p = tolower((unsigned char) *p);
		}
	
#line 199 "lib/parser.c"
		}
		/* END OF ACTION: tolower */
		switch (CURRENT_TERMINAL) {
		case (tok_obrace):
			break;
		default:
			goto ZL1;
		}
		ADVANCE_LEXER;
		switch (CURRENT_TERMINAL) {
		case (tok_word):
			/* BEGINNING OF EXTRACT: word */
			{
#line 98 "lib/parser.act"

		size_t n;

		assert(act_state->t.s != NULL);
		assert(act_state->t.e >= act_state->t.s);

		n = act_state->t.e - act_state->t.s;

		ZIk = malloc(n + 1);
		if (ZIk == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE); /* XXX: not really what i want to do */
		}

		memcpy(ZIk, act_state->t.s, n);
		ZIk[n] = '\0';
	
#line 231 "lib/parser.c"
			}
			/* END OF EXTRACT: word */
			break;
		default:
			goto ZL1;
		}
		ADVANCE_LEXER;
		switch (CURRENT_TERMINAL) {
		case (tok_comma):
			break;
		default:
			goto ZL1;
		}
		ADVANCE_LEXER;
		p_fields (lex_state, act_state, &ZIf);
		switch (CURRENT_TERMINAL) {
		case (tok_cbrace):
			break;
		case (ERROR_TERMINAL):
			RESTORE_LEXER;
			goto ZL1;
		default:
			goto ZL1;
		}
		ADVANCE_LEXER;
		/* BEGINNING OF ACTION: new-entry */
		{
#line 223 "lib/parser.act"

		assert((ZIt) != NULL);
		assert((ZIk) != NULL);

		(ZIe) = bib_new_entry((ZIt), (ZIk), (ZIf));
		if ((ZIe) == NULL) {
			perror("bib_new_entry");
			goto ZL1;
		}

		if (debug & DEBUG_ACT) {
			fprintf(stderr, "<new-entry>(t = \"%s\", k = \"%s\", f = %p) -> (e = %p)\n",
				(ZIt), (ZIk), (void *) (ZIf), (void *) (ZIe));
		}
	
#line 275 "lib/parser.c"
		}
		/* END OF ACTION: new-entry */
		/* BEGINNING OF ACTION: free */
		{
#line 111 "lib/parser.act"

		assert((ZIk) != NULL);

		free((ZIk));
	
#line 286 "lib/parser.c"
		}
		/* END OF ACTION: free */
	}
	goto ZL0;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
ZL0:;
	*ZOe = ZIe;
}

static void
p_cats(lex_state lex_state, act_state act_state, string *ZOv)
{
	string ZIv;

	if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		return;
	}
	{
		string ZI48;

		p_str_Hor_Hvar (lex_state, act_state, &ZI48);
		p_50 (lex_state, act_state, &ZI48, &ZIv);
		if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
			RESTORE_LEXER;
			goto ZL1;
		}
	}
	goto ZL0;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
ZL0:;
	*ZOv = ZIv;
}

void
p_bib(lex_state lex_state, act_state act_state, entry *ZOe)
{
	entry ZIe;

	if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		return;
	}
	{
		/* BEGINNING OF INLINE: 46 */
		{
			switch (CURRENT_TERMINAL) {
			case (tok_at):
				{
					p_entries (lex_state, act_state, &ZIe);
					if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
						RESTORE_LEXER;
						goto ZL1;
					}
				}
				break;
			default:
				{
					/* BEGINNING OF ACTION: null-entry */
					{
#line 251 "lib/parser.act"

		(ZIe) = NULL;

		if (debug & DEBUG_ACT) {
			fprintf(stderr, "<null-entry> () -> (%p)\n",
				(void *) (ZIe));
		}
	
#line 358 "lib/parser.c"
					}
					/* END OF ACTION: null-entry */
				}
				break;
			}
		}
		/* END OF INLINE: 46 */
		switch (CURRENT_TERMINAL) {
		case (tok_eof):
			break;
		default:
			goto ZL1;
		}
		ADVANCE_LEXER;
	}
	goto ZL0;
ZL1:;
	{
		/* BEGINNING OF ACTION: err-syntax */
		{
#line 261 "lib/parser.act"

		fprintf(stderr, "syntax error\n");
	
#line 383 "lib/parser.c"
		}
		/* END OF ACTION: err-syntax */
		/* BEGINNING OF ACTION: null-entry */
		{
#line 251 "lib/parser.act"

		(ZIe) = NULL;

		if (debug & DEBUG_ACT) {
			fprintf(stderr, "<null-entry> () -> (%p)\n",
				(void *) (ZIe));
		}
	
#line 397 "lib/parser.c"
		}
		/* END OF ACTION: null-entry */
	}
ZL0:;
	*ZOe = ZIe;
}

static void
p_50(lex_state lex_state, act_state act_state, string *ZI48, string *ZOv)
{
	string ZIv;

	switch (CURRENT_TERMINAL) {
	case (tok_cat):
		{
			string ZIb;

			ADVANCE_LEXER;
			p_cats (lex_state, act_state, &ZIb);
			if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
				RESTORE_LEXER;
				goto ZL1;
			}
			/* BEGINNING OF ACTION: strcat */
			{
#line 119 "lib/parser.act"

		size_t za, zb;

		assert((*ZI48) != NULL);
		assert((ZIb) != NULL);

		za = strlen((*ZI48));
		zb = strlen((ZIb));

		(ZIv) = realloc((*ZI48), za + zb + 1);
		if ((ZIv) == NULL) {
			perror("realloc");
			goto ZL1;
		}

		strcpy((ZIv) + za, (ZIb));
	
#line 441 "lib/parser.c"
			}
			/* END OF ACTION: strcat */
		}
		break;
	default:
		{
			ZIv = *ZI48;
		}
		break;
	case (ERROR_TERMINAL):
		return;
	}
	goto ZL0;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
ZL0:;
	*ZOv = ZIv;
}

static void
p_str_Hor_Hvar(lex_state lex_state, act_state act_state, string *ZOv)
{
	string ZIv;

	switch (CURRENT_TERMINAL) {
	case (tok_str):
		{
			/* BEGINNING OF EXTRACT: str */
			{
#line 80 "lib/parser.act"

		size_t n;

		assert(act_state->t.s != NULL);
		assert(act_state->t.e >= act_state->t.s);

		n = act_state->t.e - act_state->t.s;

		ZIv = malloc(n + 1);
		if (ZIv == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE); /* XXX: not really what i want to do */
		}

		memcpy(ZIv, act_state->t.s, n);
		ZIv[n] = '\0';
	
#line 490 "lib/parser.c"
			}
			/* END OF EXTRACT: str */
			ADVANCE_LEXER;
		}
		break;
	case (tok_word):
		{
			string ZIw;

			/* BEGINNING OF EXTRACT: word */
			{
#line 98 "lib/parser.act"

		size_t n;

		assert(act_state->t.s != NULL);
		assert(act_state->t.e >= act_state->t.s);

		n = act_state->t.e - act_state->t.s;

		ZIw = malloc(n + 1);
		if (ZIw == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE); /* XXX: not really what i want to do */
		}

		memcpy(ZIw, act_state->t.s, n);
		ZIw[n] = '\0';
	
#line 520 "lib/parser.c"
			}
			/* END OF EXTRACT: word */
			ADVANCE_LEXER;
			/* BEGINNING OF ACTION: lookup */
			{
#line 136 "lib/parser.act"

		/* TODO: populate variables from "String" constructs */
		(ZIv) = (ZIw);
	
#line 531 "lib/parser.c"
			}
			/* END OF ACTION: lookup */
		}
		break;
	case (ERROR_TERMINAL):
		return;
	default:
		goto ZL1;
	}
	goto ZL0;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
ZL0:;
	*ZOv = ZIv;
}

static void
p_fields_C_Cfield(lex_state lex_state, act_state act_state, field *ZOf)
{
	field ZIf;

	if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
		return;
	}
	{
		string ZIn;
		string ZIv;

		switch (CURRENT_TERMINAL) {
		case (tok_word):
			/* BEGINNING OF EXTRACT: word */
			{
#line 98 "lib/parser.act"

		size_t n;

		assert(act_state->t.s != NULL);
		assert(act_state->t.e >= act_state->t.s);

		n = act_state->t.e - act_state->t.s;

		ZIn = malloc(n + 1);
		if (ZIn == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE); /* XXX: not really what i want to do */
		}

		memcpy(ZIn, act_state->t.s, n);
		ZIn[n] = '\0';
	
#line 583 "lib/parser.c"
			}
			/* END OF EXTRACT: word */
			break;
		default:
			goto ZL1;
		}
		ADVANCE_LEXER;
		/* BEGINNING OF ACTION: tolower */
		{
#line 142 "lib/parser.act"

		char *p;

		for (p = (ZIn); *p != '\0'; p++) {
			*p = tolower((unsigned char) *p);
		}
	
#line 601 "lib/parser.c"
		}
		/* END OF ACTION: tolower */
		switch (CURRENT_TERMINAL) {
		case (tok_equ):
			break;
		default:
			goto ZL1;
		}
		ADVANCE_LEXER;
		p_cats (lex_state, act_state, &ZIv);
		if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
			RESTORE_LEXER;
			goto ZL1;
		}
		/* BEGINNING OF ACTION: new-field */
		{
#line 152 "lib/parser.act"

		struct bib_value *v;
		int normalisecase;
		const char *delim;

		assert((ZIn) != NULL);
		assert((ZIv) != NULL);

		/* TODO: keep in an array of structs, lookup per field name */
		normalisecase = (0 == strcmp((ZIn), "title"));
		delim         = (0 == strcmp((ZIn), "author") ? " and " : NULL); /* XXX */
/*
file ";"
tags ","
isbn ","
author " and "
*/

		{
			char *s, *d;
			struct bib_value **tail;

			tail = &v;

			for (s = (ZIv); s != NULL; s = d) {
				d = tex_delim(s, delim);
				if (d != NULL) {
					*d = '\0';
					d += strlen(delim);
				}

				*tail = bib_new_value(s, normalisecase);
				if (*tail == NULL) {
					perror("bib_new_value");
					goto ZL1;
				}

				tail = &(*tail)->next;
			}
		}

		(ZIf) = bib_new_field((ZIn), v);
		if ((ZIf) == NULL) {
			/* TODO: bib_free_value(v); */
			perror("bib_new_field");
			goto ZL1;
		}

		if (debug & DEBUG_ACT) {
			fprintf(stderr, "<new-field> (n = \"%s\", v = \"%s\" -> (v = %p, f = %p)\n",
				(ZIn), (ZIv), (void *) v, (void *) (ZIf));
		}
	
#line 672 "lib/parser.c"
		}
		/* END OF ACTION: new-field */
		/* BEGINNING OF ACTION: free */
		{
#line 111 "lib/parser.act"

		assert((ZIn) != NULL);

		free((ZIn));
	
#line 683 "lib/parser.c"
		}
		/* END OF ACTION: free */
		/* BEGINNING OF ACTION: free */
		{
#line 111 "lib/parser.act"

		assert((ZIv) != NULL);

		free((ZIv));
	
#line 694 "lib/parser.c"
		}
		/* END OF ACTION: free */
	}
	goto ZL0;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
ZL0:;
	*ZOf = ZIf;
}

static void
p_fields(lex_state lex_state, act_state act_state, field *ZOf)
{
	field ZIf;

	switch (CURRENT_TERMINAL) {
	case (tok_word):
		{
			field ZIa;

			p_fields_C_Cfield (lex_state, act_state, &ZIa);
			/* BEGINNING OF INLINE: 36 */
			{
				switch (CURRENT_TERMINAL) {
				case (tok_comma):
					{
						field ZIb;

						ADVANCE_LEXER;
						p_fields (lex_state, act_state, &ZIb);
						if ((CURRENT_TERMINAL) == (ERROR_TERMINAL)) {
							RESTORE_LEXER;
							goto ZL1;
						}
						/* BEGINNING OF ACTION: add-field */
						{
#line 202 "lib/parser.act"

		assert((ZIa)->next == NULL);

		(ZIa)->next = (ZIb);
		(ZIf) = (ZIa);

		if (debug & DEBUG_ACT) {
			fprintf(stderr, "<add-field> (a = %p, b = %p) -> (f = %p)\n",
				(void *) (ZIa), (void *) (ZIb), (void *) (ZIf));
		}
	
#line 744 "lib/parser.c"
						}
						/* END OF ACTION: add-field */
					}
					break;
				default:
					{
						ZIf = ZIa;
					}
					break;
				case (ERROR_TERMINAL):
					RESTORE_LEXER;
					goto ZL1;
				}
			}
			/* END OF INLINE: 36 */
		}
		break;
	default:
		{
			/* BEGINNING OF ACTION: null-field */
			{
#line 214 "lib/parser.act"

		(ZIf) = NULL;

		if (debug & DEBUG_ACT) {
			fprintf(stderr, "<null-field> () -> (f = %p)\n",
				(void *) (ZIf));
		}
	
#line 775 "lib/parser.c"
			}
			/* END OF ACTION: null-field */
		}
		break;
	case (ERROR_TERMINAL):
		return;
	}
	goto ZL0;
ZL1:;
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
ZL0:;
	*ZOf = ZIf;
}

/* BEGINNING OF TRAILER */

#line 305 "lib/parser.act"


	struct bib_entry *bib_parse(FILE *f) {
		struct lex_state l, *lex_state = &l;
		struct act_state a, *act_state = &a;
		struct bib_entry *e;
		struct stat st;
		void *p;

		assert(f != NULL);

		if (-1 == fstat(fileno(f), &st)) {
			return NULL;
		}

		p = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fileno(f), 0U);
		if (p == MAP_FAILED) {
			return NULL;
		}

		l.buf = malloc(st.st_size + 1);
		if (l.buf == NULL) {
			(void) munmap(p, st.st_size);
			return NULL;
		}

		memcpy(l.buf, p, st.st_size);
		l.buf[st.st_size] = '\0';

		l.b = 0;
		l.f = f;
		l.p = l.buf;

		ADVANCE_LEXER;	/* XXX: what if the first token is unrecognised? */
		p_bib(lex_state, act_state, &e);

		(void) munmap(p, st.st_size);
		free(l.buf);

		return e;
	}

#line 836 "lib/parser.c"

/* END OF FILE */
