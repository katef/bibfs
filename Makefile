.MAKEFLAGS: -m mk

all::  mkdir .WAIT prog
test:: mkdir .WAIT prog

BUILD ?= build

GEN += lib/parser.c

SRC += lib/lex.c
SRC += lib/bib.c
SRC += lib/refactor.c

SRC += out/bibtex.c
SRC += out/json.c

SRC += src/main.c

PARSER += lib/parser.sid

.for src in ${SRC} ${GEN}
${BUILD}/bin/bibfs: ${BUILD}/${src:R}.o
.endfor

PROG += bibfs

TEST += /usr/share/texlive/texmf-dist/bibtex/bib/base/xampl.bib

test:: ${BUILD}/bin/bibfs ${TEST}
	${BUILD}/bin/bibfs -da -e json < ${.ALLSRC:M*.bib}

.include <mk/obj.mk>
.include <mk/sid.mk>
.include <mk/prog.mk>
.include <mk/mkdir.mk>
.include <mk/clean.mk>

src/main.c: build/lib/parser.h

