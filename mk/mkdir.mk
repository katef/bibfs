
MKDIR ?= mkdir

# XXX: no :O:u for openbsd make
mkdir:: ${BUILD} ${DIR:O:u}

.for dir in ${BUILD} ${DIR:O:u}
${dir}:
	${MKDIR} -p ${dir}
.endfor

