
REMOVE ?= rm -f
RMDIR  ?= rm -r

.for dir in ${DIR}
clean::
	${REMOVE} ${CLEAN:M${dir}/*:M*.mk}
	${REMOVE} ${CLEAN:M${dir}/*:N*.mk}
.endfor

.for dir in ${DIR} ${BUILD}
.if exists(${dir})
clean::
	${RMDIR} ${dir}
.endif
.endfor

