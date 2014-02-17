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

# apparently a default: clause is not enough to satisfy -Weverything
.if ${CC:T:Mclang}
CFLAGS_lib/lex.c += -Wno-switch-enum
.endif

# int to char conversion for tolower()
.if ${CC:T:Mclang}
CFLAGS_lib/refactor.c += -Wno-conversion
.endif

# t.e - t.s where e > s
.if ${CC:T:Mclang}
CFLAGS_lib/parser.c += -Wno-sign-conversion
.endif

# SID generates fall-throughs which we cannot control
.if ${CC:T:Mclang}
CFLAGS_lib/parser.c += -Wno-switch-enum
.endif

.for src in ${SRC:Mlib/*.c} ${SRC:Mout/*.c} ${GEN:Mlib/*.c}
${BUILD}/lib/libbib.o: ${BUILD}/${src:R}.o
.endfor

