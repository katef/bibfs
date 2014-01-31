
clean::
.for clean in ${CLEAN}
. if exists(${clean})
	rm ${clean}
. endif
.endfor

clean::
.for dir in ${DIR:O:u} ${BUILD}
. if exists(${dir})
	rm -r ${dir}
. endif
.endfor

