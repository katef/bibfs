.include "../mk/top.mk"

SRC += bibt/main.c

PROG += bibt

.for lib in ${LIB:Mlibbib}
${BUILD}/bin/bibt: ${BUILD}/lib/${lib:R}.a
.endfor

.for src in ${SRC:Mbibt/*.c}
${BUILD}/bin/bibt: ${BUILD}/${src:R}.o
.endfor

bibt/main.c: build/lib/parser.h

