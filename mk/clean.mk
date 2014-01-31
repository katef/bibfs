
REMOVE ?= rm -f
RMDIR  ?= rm -r

clean::
.for dir in ${CLEAN:H:O:u}
	${REMOVE} ${CLEAN:M${dir}/*}
.endfor

clean::
.for dir in ${DIR:O:u} ${BUILD}
.if exists(${dir})
	${RMDIR} ${dir}
.endif
.endfor

