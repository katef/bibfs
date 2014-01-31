
DIR += ${BUILD}/bin

.for prog in ${PROG}
prog::   ${BUILD}/bin/${prog}
CLEAN += ${BUILD}/bin/${prog}
.endfor

.for prog in ${PROG}
${BUILD}/bin/${prog}:
	${CC} -o $@ ${CFLAGS} ${.ALLSRC:M*.o}
.endfor

