.include "../mk/top.mk"

SRC += bibt/main.c

PROG += bibt

.for src in ${SRC:Mlib/*.c} ${SRC:Mout/*.c} ${GEN:Mlib/*.c} ${SRC:Mbibt/*.c}
${BUILD}/bin/bibt: ${BUILD}/${src:R}.o
.endfor

src/main.c: build/lib/parser.h

