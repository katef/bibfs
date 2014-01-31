
# XXX: no :O:u for openbsd make
.for dir in ${BUILD} ${DIR:O:u}
mkdir:: ${dir}
.endfor

.for dir in ${BUILD} ${DIR:O:u}
${dir}:
	mkdir -p $@
.endfor

