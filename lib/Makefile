.include "../mk/top.mk"

GEN += lib/parser.c

SRC += lib/lex.c
SRC += lib/bib.c
SRC += lib/refactor.c
SRC += lib/find.c
SRC += lib/split.c
SRC += lib/merge.c
SRC += lib/cull.c
SRC += lib/free.c

PARSER += lib/parser.sid

LIB += libbib

.for src in ${SRC:Mlib/*.c} ${SRC:Mout/*.c} ${GEN:Mlib/*.c}
${BUILD}/lib/libbib.o: ${BUILD}/${src:R}.o
.endfor

