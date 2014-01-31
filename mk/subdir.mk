
.for subdir in ${SUBDIR}
.include "${subdir}/Makefile"
.endfor

.for subdir in ${SUBDIR}
DIR += ${BUILD}/${subdir}
.endfor

