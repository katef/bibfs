.include "../mk/top.mk"

SRC += fuse/main.c
SRC += fuse/op.c

PROG += bibfs

.for src in ${SRC:Mfuse/*.c}
CFLAGS_${src} ?= `pkg-config fuse --cflags`
CFLAGS_${src} += -Wno-pedantic # <fuse/fuse_common.h> has trailing commas for enums
.endfor

LFLAGS_bibfs  ?= `pkg-config fuse --libs`

# XXX: I don't want to use C99 just for FUSE's sake
#CFLAGS_fuse/op.c += -std=c99

.for src in ${SRC:Mlib/*.c} ${SRC:Mout/*.c} ${GEN:Mlib/*.c} ${SRC:Mfuse/*.c}
${BUILD}/bin/bibfs: ${BUILD}/${src:R}.o
.endfor

src/main.c: build/lib/parser.h

