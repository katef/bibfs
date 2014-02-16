
CC ?= gcc

.for dir in ${INCDIR}
CFLAGS += -I ${dir}
.endfor

.if ${CC:T:Mgcc}
CFLAGS += -ansi -pedantic
.if !defined(NDEBUG)
CFLAGS += -Wall -W -Werror
CFLAGS += -ggdb
CFLAGS += -O0 # or -Og if you have it
.else
CFLAGS += -O3
.endif
.endif

.if !defined(NDEBUG)
CFLAGS += -g
.endif

.for src in ${SRC} ${GEN}

CLEAN += ${BUILD}/${src:R}.o

${BUILD}/${src:R}.o:
	${CC} -o $@ ${CFLAGS} ${CFLAGS_${src}} -c ${.ALLSRC:M*.c}

.endfor

.for src in ${SRC}
${BUILD}/${src:R}.o: ${src}
.endfor

.for src in ${GEN}
${BUILD}/${src:R}.o: ${BUILD}/${src}
.endfor

