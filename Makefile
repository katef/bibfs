CC  ?= gcc
SID ?= sid

BUILD ?= build

#.MAKEFLAGS: -m mk

.if ${CC:T:Mgcc}
CFLAGS += -ansi -pedantic
#CFLAGS += -Wall -W -Werror
CFLAGS += -O2
.endif

CFLAGS += -I include

SID_CFLAGS += -s no-numeric-terminals -s no-terminals

TEST += /usr/share/texlive/texmf-dist/bibtex/bib/base/xampl.bib

GEN += lib/parser.c

SRC += lib/lex.c
SRC += lib/bib.c
SRC += lib/refactor.c

SRC += out/bibtex.c
SRC += out/json.c

SRC += src/main.c

all:: ${BUILD}/bin/bibfs

.for src in ${SRC} ${GEN}
CLEAN += ${BUILD}/${src:R}.o
.endfor

clean::
.for clean in ${CLEAN} ${BUILD}/bin/bibfs
. if exists(${clean})
	rm ${clean}
. endif
.endfor
.for dir in ${DIR:O:u} ${BUILD}/bin ${BUILD}
. if exists(${dir})
	rm -r ${dir}
. endif
.endfor

.for src in ${SRC} ${GEN}
DIR += ${BUILD}/${src:H}
.endfor

# XXX: no :O:u for openbsd make
.for dir in ${BUILD} ${BUILD}/bin ${DIR:O:u}
${dir}:
	mkdir -p $@
.endfor

.for src in ${SRC}
${BUILD}/${src:R}.o: ${@:H} ${src}
.endfor

.for src in ${GEN}
${BUILD}/${src:R}.o: ${@:H} ${BUILD}/${src}
.endfor

.for src in ${SRC} ${GEN}
${BUILD}/${src:R}.o:
	${CC} -o $@ ${CFLAGS} -c ${.ALLSRC:M*.c}
.endfor

.for src in ${SRC} ${GEN}
${BUILD}/bin/bibfs: ${@:H} ${BUILD}/${src:R}.o
.endfor

${BUILD}/bin/bibfs: ${@:H}
	${CC} -o $@ ${CFLAGS} ${.ALLSRC:M*.o}

${BUILD}/lib/parser.c ${BUILD}/lib/parser.h: lib/parser.sid lib/parser.act
	${SID} ${SID_CFLAGS} $> ${BUILD}/lib/parser.c ${BUILD}/lib/parser.h \
		|| { rm -f ${BUILD}/parser.c ${BUILD}/parser.h; false; }

src/main.c: build/lib/parser.h

${BUILD}/lib/parser.h: lib/parser.sid lib/parser.act
${BUILD}/lib/parser.c: lib/parser.sid lib/parser.act

test:: ${BUILD}/bin/bibfs ${TEST}
	${BUILD}/bin/bibfs -da -e json < ${.ALLSRC:M*.bib}

