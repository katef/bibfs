.MAKEFLAGS: -r -m share/mk

# targets
all::  mkdir .WAIT dep .WAIT lib prog
dep::
gen::
test:: all
install:: all
clean::

# things to override
CC     ?= gcc
BUILD  ?= build
PREFIX ?= /usr/local

PKG += fuse

# layout
SUBDIR += include/bib
SUBDIR += include
SUBDIR += out
SUBDIR += lib
SUBDIR += bibt
SUBDIR += fuse

INCDIR += include

TEST += /usr/share/texlive/texmf-dist/bibtex/bib/base/xampl.bib

test:: ${BUILD}/bin/bibfs ${TEST}
	${BUILD}/bin/bibt -da -e json ${.ALLSRC:M*.bib}

.include <subdir.mk>
.include <pkgconf.mk>
.include <sid.mk>
.include <obj.mk>
.include <dep.mk>
.include <part.mk>
.include <ar.mk>
.include <so.mk>
.include <prog.mk>
.include <mkdir.mk>
.include <install.mk>
.include <clean.mk>

