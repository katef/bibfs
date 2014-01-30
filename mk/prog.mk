CC  ?= gcc
SID ?= sid

BUILD ?= build

OBJS += lib/lex.o
OBJS += lib/bib.o
OBJS += lib/refactor.o
OBJS += lib/parser.o

OBJS += out/bibtex.o
OBJS += out/json.o

OBJS += src/main.o

.if ${CC:T:Mgcc}
CFLAGS += -ansi -pedantic
#CFLAGS += -Wall -W -Werror
CFLAGS += -O2
.endif

CFLAGS += -I include

SID_CFLAGS += -s no-numeric-terminals -s no-terminals

.for obj in ${OBJS}
${BUILD}/bibfs: ${BUILD}/${obj}
.endfor

${BUILD}/bibfs:
	mkdir -p ${BUILD}/${@:H}
	${CC} -o ${BUILD}/$@ $>

${BUILD}/lib/parser.c ${BUILD}/lib/parser.h: lib/parser.sid lib/parser.act
	${SID} ${SID_CFLAGS} $> ${BUILD}/parser.c ${BUILD}/parser.h \
		|| { rm -f ${BUILD}/parser.c ${BUILD}/parser.h; false; }

${BUILD}/lib/parser.h: lib/parser.sid lib/parser.act
${BUILD}/lib/parser.c: lib/parser.sid lib/parser.act

${BUILD}/parser.o: ${BUILD}/parser.c ${BUILD}/parser.h

.for obj in ${OBJS}
${BUILD}/${obj}: ${obj:R}.c
	mkdir -p ${BUILD}/${@:H}
	${CC} -o ${BUILD}/$@ ${CFLAGS} -c $<
.endfor

all:: ${BUILD}/bibfs

clean::
	rm -f ${OBJS}
	rm -r ${BUILD}

test:: bibfs
	./bibfs -da -e json < /usr/share/texlive/texmf-dist/bibtex/bib/base/xampl.bib

