#include <assert.h>
#include <string.h>
#include <ctype.h>

#include <bib/bib.h>
#include <bib/tex.h>

#define WHITE " \n\t\r\v\f"

static const struct {
	const char *from;
	const char *to;
} a[] = {
	{ "---", "\xE2\x80\x94" },
	{ "--",  "\xE2\x80\x93" },
	{ "``",  "\xE2\x80\x9C" },
	{ "''",  "\xE2\x80\x9D" },
	{ "`",   "\xE2\x80\x98" },
	{ "'",   "\xE2\x80\x99" },
	{ "~",   "\xC2\xA0"     },

	{ "\\thinspace",      "\xE2\x80\x89" },
	{ "\\slash",     "\\" "\xE2\x80\x8B" }, /* \ zws */
	{ "\\nobreak",        "\xE2\x81\xA0" }, /* wj */
	{ "\\break",     "\n"                },

	{ "\\S",    "\xC2\xA7"     },
	{ "\\P",    "\xC2\xB6"     },
	{ "\\dag",  "\xE2\x80\xA0" },
	{ "\\ddag", "\xE2\x80\xA1" },

	{ "\\eth",  "\xC3\xB0" },
	{ "\\Dbar", "\xC4\x90" }, { "\\dbar", "\xC4\x91" },
	{ "\\ss",   "\xC3\x9F" },
	{ "\\AA",   "\xC3\x85" }, { "\\aa",   "\xC3\xA5" },
	{ "\\AE",   "\xC3\x86" }, { "\\ae",   "\xC3\xA6" },
	{ "\\OE",   "\xC5\x92" }, { "\\oe",   "\xC5\x93" },
	{ "\\O",    "\xC3\x98" }, { "\\o",    "\xC3\xB8" },
	                          { "\\i",    "\xC4\xB2" },
	{ "\\L",    "\xC5\x81" }, { "\\l",    "\xC5\x82" },

	{ "\\-",  "\xC2\xAD"    },
	{ "\\]",  ""            },
	{ "\\/",  ""            },

	{ "{\\#}", "#" },
	{ "{\\&}", "&" },
	{ "{~}",   "~" },

/* TODO: flag for {"} vs \"
	{ "\\\\", "\\" },
	{ "\\'",  "'"  },
	{ "\\\"", "\"" },
*/
	{ "\\%",  "%"  },
	{ "\\^",  "^"  },
	{ "\\_",  "_"  },
	{ "\\$",  "$"  },
	{ "\\{",  "{"  },
	{ "\\}",  "}"  },

/* TODO:
\cprime
\cdprime

TODO: need to skip whitespace after \xyz words and then have "\\ " for control space.
"^^@" NUL
"^^A" ctrl-A
\u a  "short mark"
\d n  "dot under"
\v c  č hacek
\H o  "long umlaut"
\t ii "tie-after"
\b o  "bar under"
\c c  ç "cedilla"
*/

/* TODO:
\`e è
\'e é
\^o ô
\"o ö
\~n ñ
\=a ā "long mark"
*/

	/* TODO: also \^{a} style */
	{   "\\`A", "\xC3\x80" }, {   "\\`a", "\xC3\xA0" },
	{   "\\'A", "\xC3\x81" }, {   "\\'a", "\xC3\xA1" },
	{   "\\^A", "\xC3\x82" }, {   "\\^a", "\xC3\xA2" },
	{   "\\~A", "\xC3\x83" }, {   "\\~a", "\xC3\xA3" },
	{  "\\\"A", "\xC3\x84" }, {  "\\\"a", "\xC3\xA4" },
	{   "\\_A", "\xC4\x80" }, {   "\\_a", "\xC4\x81" },
	{   "\\`E", "\xC3\x88" }, {   "\\`e", "\xC3\xA8" },
	{   "\\'E", "\xC3\x89" }, {   "\\'e", "\xC3\xA9" },
	{   "\\^E", "\xC3\x8A" }, {   "\\^e", "\xC3\xAA" },
	{  "\\\"E", "\xC3\x8B" }, {  "\\\"e", "\xC3\xAB" },
	{   "\\_E", "\xC4\x92" }, {   "\\_e", "\xC4\x93" },
	{   "\\.E", "\xC4\x96" }, {   "\\.e", "\xC4\x97" },
	{ "\\`\\I", "\xC3\x8C" }, { "\\`\\i", "\xC3\xAC" },
	{ "\\'\\I", "\xC3\x8D" }, { "\\'\\i", "\xC3\xAD" },
	{ "\\^\\I", "\xC3\x8E" }, { "\\^\\i", "\xC3\xAE" },
	{"\\\"\\I", "\xC3\x8F" }, {"\\\"\\i", "\xC3\xAF" },
	{ "\\~\\I", "\xC4\xA8" }, { "\\~\\i", "\xC4\xA9" },
	{ "\\_\\I", "\xC4\xAA" }, { "\\_\\i", "\xC4\xAB" },
	{ "\\.\\I", "\xC4\xB0" },
	{   "\\`O", "\xC3\xB2" }, {   "\\`o", "\xC3\xB2" },
	{   "\\'O", "\xC3\xB3" }, {   "\\'o", "\xC3\xB3" },
	{   "\\^O", "\xC3\xB4" }, {   "\\^o", "\xC3\xB4" },
	{   "\\~O", "\xC3\xB5" }, {   "\\~o", "\xC3\xB5" },
	{  "\\\"O", "\xC3\xB6" }, {  "\\\"o", "\xC3\xB6" },
	{   "\\_O", "\xC3\x8C" }, {   "\\_o", "\xC5\x8D" },
	{   "\\`U", "\xC3\x99" }, {   "\\`u", "\xC3\xB9" },
	{   "\\'U", "\xC3\x9A" }, {   "\\'u", "\xC3\xBA" },
	{   "\\^U", "\xC3\x9B" }, {   "\\^u", "\xC3\xBB" },
	{   "\\~U", "\xC5\xA8" }, {   "\\~u", "\xC5\xA9" },
	{  "\\\"U", "\xC3\x9C" }, {  "\\\"u", "\xC3\xBC" },
	{   "\\_U", "\xC5\xAA" }, {   "\\_u", "\xC5\xAB" },
	{"\\_\\AE", "\xC7\xA2" }, {"\\_\\ae", "\xC7\xA3" }

	{   "\\^C", "\xC4\x88" }, {   "\\^c", "\xC4\x89" },
	{   "\\.C", "\xC4\x8A" }, {   "\\.c", "\xC4\x8B" },
	{   "\\^G", "\xC4\x9C" }, {   "\\^g", "\xC4\x9D" },
	{   "\\.G", "\xC4\xA0" }, {   "\\.g", "\xC4\xA1" },
	{   "\\^H", "\xC4\xA4" }, {   "\\^h", "\xC4\xA5" },
	{ "\\^\\J", "\xC4\xB4" }, { "\\^\\j", "\xC4\xB5" },
	{   "\\.S", "\xC5\x9C" }, {   "\\.s", "\xC5\x9D" },
	{   "\\.W", "\xC5\xB4" }, {   "\\.w", "\xC5\xB5" },
	{   "\\.Y", "\xC5\xB6" }, {   "\\.y", "\xC5\xB7" },
	{  "\\\"Y", "\xC5\xB8" },
	{   "\\.Z", "\xC5\xBB" }, {   "\\.z", "\xC5\xBC" },
};

static int
xisterm(char c)
{
	return !!strchr(":.!", c);
}

char *
tex_escape(char *dst, const char *src, int normalisecase)
{
	const char *p;
	char *q;
	int depth;
	int maths;
	size_t i;

	assert(src != NULL);
	assert(dst != NULL);

	depth = 0;
	maths = 0;

	p = src;
	q = dst;

	while (*p != '\0') {
		if (isspace((unsigned char) *p)) {
			p += strspn(p, WHITE);
			if (q != dst && *p != '\0') {
				*q++ = ' ';
			}
			continue;
		}

		if (*p == '{') { depth++; p++; continue; }
		if (*p == '}') { depth--; p++; continue; }

		if (*p == '$') {
			maths = !maths;
			*q++ = maths ? '\x02' : '\x03';
			p++;
			continue;
		}

		for (i = 0; i < sizeof a / sizeof *a; i++) {
			if (0 == strncmp(p, a[i].from, strlen(a[i].from))) {
				strcpy(q, a[i].to);
				p += strlen(a[i].from);
				q += strlen(a[i].to);
				break;
			}
		}

		if (i < sizeof a / sizeof *a) {
			continue;
		}

		if (normalisecase && !maths) {
			if (q == dst) {
				*q = toupper(*p);
			} else if (q >= dst + 2 && xisterm(q[-2]) && q[-1] == ' ') {
				*q = toupper(*p);
			} else if (depth == 0) {
				*q = tolower(*p);
			} else {
				*q = *p;
			}
		} else {
			*q = *p;
		}

		p++;
		q++;
	}

	*q = '\0';

	return dst;
}

char *
tex_delim(const char *s, const char *delim)
{
	const char *p;
	int depth;
	int maths;
	size_t i;

	assert(s != NULL);

	if (delim == NULL) {
		return NULL;
	}

	depth = 0;
	maths = 0;

	p = s;

	while (*p != '\0') {
		if (*p == '{') { depth++; p++; continue; }
		if (*p == '}') { depth--; p++; continue; }

		if (*p == '$') {
			maths = !maths;
			p++;
			continue;
		}

		for (i = 0; i < sizeof a / sizeof *a; i++) {
			if (0 == strncmp(p, a[i].from, strlen(a[i].from))) {
				p += strlen(a[i].from);
				break;
			}
		}

		if (i < sizeof a / sizeof *a) {
			continue;
		}

		if (depth == 0 && maths == 0) {
			if (0 == strncmp(p, delim, strlen(delim))) {
				return (char *) p;
			}
		}

		p++;
	}

	return NULL;
}

