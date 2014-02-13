
LD     ?= ld
AR     ?= ar
RANLIB ?= ar s

ARFLAGS  ?= cr
LDRFLAGS ?=

.for lib in ${LIB}
lib::    ${BUILD}/lib/${lib}.a
CLEAN += ${BUILD}/lib/${lib}.a
CLEAN += ${BUILD}/lib/${lib}.o
.endfor

# objects are collated to avoid passing possibly conflicting filenames to ar(1)
.for lib in ${LIB}
${BUILD}/lib/${lib}.o:
	${LD} -r -o $@ ${.ALLSRC:M*.o} ${LDRFLAGS}
.endfor

.for lib in ${LIB}
${BUILD}/lib/${lib}.a: ${BUILD}/lib/${lib}.o
	${AR} ${ARFLAGS}${ARFLAGS_${lib}} $@ ${.ALLSRC:M*.o}
	${RANLIB} $@
.endfor

