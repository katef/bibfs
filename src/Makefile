.include "../mk/top.mk"

SRC += src/main.c

PROG += bibfs

.for src in ${SRC} ${GEN}
${BUILD}/bin/bibfs: ${BUILD}/${src:R}.o
.endfor

src/main.c: build/lib/parser.h

