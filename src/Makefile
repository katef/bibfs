SRC += src/main.c

PROG += bibfs

src/main.c: build/lib/parser.h

.for src in ${SRC} ${GEN}
${BUILD}/bin/bibfs: ${BUILD}/${src:R}.o
.endfor

#.DEFAULT:
#	cd .. && ${MAKE} ${.TARGETS}

