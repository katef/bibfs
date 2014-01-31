
clean::
.for dir in ${CLEAN:H:O:u}
	rm -f ${CLEAN:M${dir}/*}
.endfor

clean::
.for dir in ${DIR:O:u} ${BUILD}
. if exists(${dir})
	rm -r ${dir}
. endif
.endfor

