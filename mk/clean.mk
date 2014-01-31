
REMOVE ?= rm -f
RMDIR  ?= rm -r

clean::
.for dir in ${DIR}
	${REMOVE} ${CLEAN:M${dir}/*}
.endfor

clean::
.for dir in ${DIR} ${BUILD}
.if exists(${dir})
	${RMDIR} ${dir}
.endif
.endfor

