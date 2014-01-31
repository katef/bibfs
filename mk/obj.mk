
CC ?= gcc

CFLAGS += -I include

.if ${CC:T:Mgcc}
CFLAGS += -ansi -pedantic
#CFLAGS += -Wall -W -Werror
CFLAGS += -O2
.endif

.for src in ${SRC} ${GEN}
CLEAN += ${BUILD}/${src:R}.o
DIR   += ${BUILD}/${src:H}
.endfor

.for src in ${SRC}
${BUILD}/${src:R}.o: ${src}
.endfor

.for src in ${GEN}
${BUILD}/${src:R}.o: ${BUILD}/${src}
.endfor

.for src in ${SRC} ${GEN}
${BUILD}/${src:R}.o:
	${CC} -o $@ ${CFLAGS} -c ${.ALLSRC:M*.c}
.endfor

