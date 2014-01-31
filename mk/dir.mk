
# XXX: no :O:u for openbsd make
.for dir in ${BUILD} ${DIR:O:u}
${dir}:
	mkdir -p $@
.endfor

