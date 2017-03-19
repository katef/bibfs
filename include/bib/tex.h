/*
 * Copyright 2014-2017 Katherine Flavel
 *
 * See LICENCE for the full copyright terms.
 */

#ifndef BIB_TEX_H
#define BIB_TEX_H

char *
tex_escape(char *dst, const char *src, int normalisecase);

char *
tex_delim(const char *s, const char *delim);

#endif

