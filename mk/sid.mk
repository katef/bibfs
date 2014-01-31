
SID ?= sid

SID_CFLAGS += -s no-numeric-terminals -s no-terminals

.for parser in ${PARSER}
${BUILD}/${parser:R}.c ${BUILD}/${parser:R}.h: ${parser} ${parser:R}.act
	${SID} ${SID_CFLAGS} $> ${BUILD}/${parser:R}.c ${BUILD}/${parser:R}.h \
		|| { rm -f ${BUILD}/${parser:R}.c ${BUILD}/${parser}.h; false; }
.endfor

.for parser in ${PARSER}
${BUILD}/${parser:R}.h: ${parser} ${parser:R}.act
${BUILD}/${parser:R}.c: ${parser} ${parser:R}.act
.endfor

