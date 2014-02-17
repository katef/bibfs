
CC ?= gcc

.for dir in ${INCDIR}
CFLAGS += -I ${dir}
.endfor

.if ${CC:T:Mgcc}
.if defined(DEBUG)
CFLAGS += -std=c89 -pedantic
#CFLAGS += -Werror
CFLAGS += -Wall -Wextra -Wno-system-headers
CFLAGS += -ggdb
CFLAGS += -O0 # or -Og if you have it
.else
CFLAGS += -std=c89 -pedantic
CFLAGS += -O3
.endif
.endif

.if ${CC:T:Mclang}
.if defined(DEBUG)
CFLAGS += -std=c89 -pedantic
#CFLAGS += -Werror
CFLAGS += -Weverything -Wno-system-headers
CFLAGS += -Wno-padded # padding is not an error
CFLAGS += -O0
.else
CFLAGS += -ansi -pedantic
CFLAGS += -O3
.endif
.endif

.if defined(DEBUG)
CFLAGS += -g
.else
CFLAGS += -DNDEBUG
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

