
.for dir in ${SUBDIR}
.include "${dir}/Makefile"
.endfor

.for dir in ${SUBDIR}
DIR += ${BUILD}/${dir}
.endfor

