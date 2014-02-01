.MAKEFLAGS: -r -m mk

# targets
all::  mkdir .WAIT prog
test:: all
clean::

# things to override
BUILD ?= build

SUBDIR += lib
SUBDIR += out
SUBDIR += bibt

TEST += /usr/share/texlive/texmf-dist/bibtex/bib/base/xampl.bib

test:: ${BUILD}/bin/bibfs ${TEST}
	${BUILD}/bin/bibfs -da -e json < ${.ALLSRC:M*.bib}

.include <mk/subdir.mk>
.include <mk/obj.mk>
.include <mk/sid.mk>
.include <mk/prog.mk>
.include <mk/mkdir.mk>
.include <mk/clean.mk>

