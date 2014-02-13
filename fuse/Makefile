.include "../mk/top.mk"

SRC += fuse/main.c
SRC += fuse/filename.c
SRC += fuse/reload.c
SRC += fuse/sread.c

SRC += fuse/op.c
SRC += fuse/op_root.c
SRC += fuse/op_zim.c
SRC += fuse/op_entry.c
SRC += fuse/op_index.c
SRC += fuse/op_field.c
SRC += fuse/op_file.c

PROG += bibfs

.for src in ${SRC:Mfuse/main.c} ${SRC:Mfuse/op*.c}
CFLAGS_${src} ?= `pkg-config fuse --cflags`
.endfor

# <fuse/fuse_common.h> has trailing commas for enums
.for src in ${SRC:Mfuse/main.c} ${SRC:Mfuse/op*.c}
CFLAGS_${src} += -Wno-pedantic
.endfor

.for lib in ${LIB:Mlibbib}
${BUILD}/bin/bibfs: ${BUILD}/lib/${lib:R}.a
.endfor

LFLAGS_bibfs  ?= `pkg-config fuse --libs`

.for src in ${SRC:Mfuse/*.c}
${BUILD}/bin/bibfs: ${BUILD}/${src:R}.o
.endfor

