
.for prog in ${PROG}
all:: ${BUILD}/bin/${prog}
.endfor

.for prog in ${PROG}
${BUILD}/bin/${prog}: ${@:H}
	${CC} -o $@ ${CFLAGS} ${.ALLSRC:M*.o}
.endfor

.for prog in ${PROG}
CLEAN += ${BUILD}/bin/${prog}
.endfor

DIR += ${BUILD}/bin

