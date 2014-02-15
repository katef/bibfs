.MAKEFLAGS: -r -m mk

# targets
all::  mkdir .WAIT dep .WAIT lib prog
test:: all
install:: all
clean::

# things to override
BUILD ?= build

# layout
SUBDIR += include/bib
SUBDIR += out
SUBDIR += lib
SUBDIR += bibt
SUBDIR += fuse

INCDIR += include

TEST += /usr/share/texlive/texmf-dist/bibtex/bib/base/xampl.bib

test:: ${BUILD}/bin/bibfs ${TEST}
	${BUILD}/bin/bibfs -da -e json < ${.ALLSRC:M*.bib}

.include <mk/subdir.mk>
.include <mk/sid.mk>
.include <mk/dep.mk>
.include <mk/obj.mk>
.include <mk/lib.mk>
.include <mk/prog.mk>
.include <mk/mkdir.mk>
.include <mk/install.mk>
.include <mk/clean.mk>

