/*
 * Automatically generated from the files:
 *	lib/parser.sid
 * and
 *	lib/parser.act
 * by:
 *	sid
 */

/* BEGINNING OF HEADER */

#line 55 "lib/parser.act"


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

#line 52 "lib/parser.c"


#ifndef ERROR_TERMINAL
#error "-s no-numeric-terminals given and ERROR_TERMINAL is not defined"
#endif

/* BEGINNING OF FUNCTION DECLARATIONS */

static void p_entries(lex_state, act_state, entry *);
static void p_entries_C_Centry(lex_state, act_state, entry *);
static void p_cats(lex_state, act_state, string *);
extern void p_bib(lex_state, act_state, entry *);
static void p_49(lex_state, act_state, string *, string *);
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
		/* BEGINNING OF INLINE: 42 */
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
#line 209 "lib/parser.act"

		assert((ZIa)->next == NULL);

		(ZIa)->next = (ZIb);
		(ZIe) = (ZIa);

		if (debug & DEBUG_ACT) {
			fprintf(stderr, "<add-entry>(a = %p, b = %p) -> (e = %p)\n",
				(void *) (ZIa), (void *) (ZIb), (void *) (ZIe));
		}
	
#line 113 "lib/parser.c"
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
		/* END OF INLINE: 42 */
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
#line 97 "lib/parser.act"

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
	
#line 180 "lib/parser.c"
			}
			/* END OF EXTRACT: word */
			break;
		default:
			goto ZL1;
		}
		ADVANCE_LEXER;
		/* BEGINNING OF ACTION: tolower */
		{
#line 141 "lib/parser.act"

		char *p;

		for (p = (ZIt); *p != '\0'; p++) {
			*p = tolower((unsigned char) *p);
		}
	
#line 198 "lib/parser.c"
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
#line 97 "lib/parser.act"

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
	
#line 230 "lib/parser.c"
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
#line 193 "lib/parser.act"

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
	
#line 274 "lib/parser.c"
		}
		/* END OF ACTION: new-entry */
		/* BEGINNING OF ACTION: free */
		{
#line 110 "lib/parser.act"

		assert((ZIk) != NULL);

		free((ZIk));
	
#line 285 "lib/parser.c"
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
		string ZI47;

		p_str_Hor_Hvar (lex_state, act_state, &ZI47);
		p_49 (lex_state, act_state, &ZI47, &ZIv);
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
		/* BEGINNING OF INLINE: 45 */
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
#line 221 "lib/parser.act"

		(ZIe) = NULL;

		if (debug & DEBUG_ACT) {
			fprintf(stderr, "<null-entry> () -> (%p)\n",
				(void *) (ZIe));
		}
	
#line 357 "lib/parser.c"
					}
					/* END OF ACTION: null-entry */
				}
				break;
			}
		}
		/* END OF INLINE: 45 */
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
	SAVE_LEXER ((ERROR_TERMINAL));
	return;
ZL0:;
	*ZOe = ZIe;
}

static void
p_49(lex_state lex_state, act_state act_state, string *ZI47, string *ZOv)
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
#line 118 "lib/parser.act"

		size_t za, zb;

		assert((*ZI47) != NULL);
		assert((ZIb) != NULL);

		za = strlen((*ZI47));
		zb = strlen((ZIb));

		(ZIv) = realloc((*ZI47), za + zb + 1);
		if ((ZIv) == NULL) {
			perror("realloc");
			goto ZL1;
		}

		strcpy((ZIv) + za, (ZIb));
	
#line 417 "lib/parser.c"
			}
			/* END OF ACTION: strcat */
		}
		break;
	default:
		{
			ZIv = *ZI47;
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
#line 79 "lib/parser.act"

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
	
#line 466 "lib/parser.c"
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
#line 97 "lib/parser.act"

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
	
#line 496 "lib/parser.c"
			}
			/* END OF EXTRACT: word */
			ADVANCE_LEXER;
			/* BEGINNING OF ACTION: lookup */
			{
#line 135 "lib/parser.act"

		/* TODO: populate variables from "String" constructs */
		(ZIv) = (ZIw);
	
#line 507 "lib/parser.c"
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
#line 97 "lib/parser.act"

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
	
#line 559 "lib/parser.c"
			}
			/* END OF EXTRACT: word */
			break;
		default:
			goto ZL1;
		}
		ADVANCE_LEXER;
		/* BEGINNING OF ACTION: tolower */
		{
#line 141 "lib/parser.act"

		char *p;

		for (p = (ZIn); *p != '\0'; p++) {
			*p = tolower((unsigned char) *p);
		}
	
#line 577 "lib/parser.c"
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
#line 149 "lib/parser.act"

		struct bib_value *v;

		assert((ZIn) != NULL);
		assert((ZIv) != NULL);

		v = bib_new_value((ZIv));
		if (v == NULL) {
			perror("bib_new_value");
			goto ZL1;
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
	
#line 619 "lib/parser.c"
		}
		/* END OF ACTION: new-field */
		/* BEGINNING OF ACTION: free */
		{
#line 110 "lib/parser.act"

		assert((ZIn) != NULL);

		free((ZIn));
	
#line 630 "lib/parser.c"
		}
		/* END OF ACTION: free */
		/* BEGINNING OF ACTION: free */
		{
#line 110 "lib/parser.act"

		assert((ZIv) != NULL);

		free((ZIv));
	
#line 641 "lib/parser.c"
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
			/* BEGINNING OF INLINE: 35 */
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
#line 172 "lib/parser.act"

		assert((ZIa)->next == NULL);

		(ZIa)->next = (ZIb);
		(ZIf) = (ZIa);

		if (debug & DEBUG_ACT) {
			fprintf(stderr, "<add-field> (a = %p, b = %p) -> (f = %p)\n",
				(void *) (ZIa), (void *) (ZIb), (void *) (ZIf));
		}
	
#line 691 "lib/parser.c"
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
			/* END OF INLINE: 35 */
		}
		break;
	default:
		{
			/* BEGINNING OF ACTION: null-field */
			{
#line 184 "lib/parser.act"

		(ZIf) = NULL;

		if (debug & DEBUG_ACT) {
			fprintf(stderr, "<null-field> () -> (f = %p)\n",
				(void *) (ZIf));
		}
	
#line 722 "lib/parser.c"
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

#line 271 "lib/parser.act"


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

#line 783 "lib/parser.c"

/* END OF FILE */
