/*
 * Automatically generated from the files:
 *	lib/parser.sid
 * and
 *	lib/parser.act
 * by:
 *	sid
 */

/* BEGINNING OF HEADER */

#line 67 "lib/parser.act"


	#include <stdio.h>

	struct bib_entry;

	typedef struct lex_state * lex_state;
	typedef struct act_state * act_state;
	typedef struct bib_entry * entry;

	struct bib_entry *bib_parse(FILE *f);

#line 26 "lib/parser.h"

/* BEGINNING OF FUNCTION DECLARATIONS */

extern void p_bib(lex_state, act_state, entry *);
/* BEGINNING OF TRAILER */

#line 277 "lib/parser.act"


#line 36 "lib/parser.h"

/* END OF FILE */
