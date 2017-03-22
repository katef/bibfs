.include "../share/mk/top.mk"

SRC += fuse/main.c
SRC += fuse/filename.c
SRC += fuse/reload.c
SRC += fuse/sread.c
SRC += fuse/rwlock.c

SRC += fuse/op.c
SRC += fuse/op_root.c
SRC += fuse/op_zim.c
SRC += fuse/op_entry.c
SRC += fuse/op_index.c
SRC += fuse/op_field.c
SRC += fuse/op_file.c

PROG += bibfs

.for src in ${SRC:Mfuse/main.c} ${SRC:Mfuse/op*.c}
CFLAGS.${src} += `pkg-config fuse --cflags`
DFLAGS.${src} += `pkg-config fuse --cflags`
.endfor

# <fuse/fuse_common.h> has trailing commas for enums
.if ${CC:T:Mgcc}
.for src in ${SRC:Mfuse/main.c} ${SRC:Mfuse/op*.c}
CFLAGS.${src} += -Wno-pedantic
.endfor
.endif
.if ${CC:T:Mclang}
.for src in ${SRC:Mfuse/main.c} ${SRC:Mfuse/op*.c}
CFLAGS.${src} += -Wno-c99-extensions
.endfor
.endif

# apparently fuse has all sorts of documentation errors
# XXX: I would prefer to use -isystem-prefix here, but I don't see how for <fuse.h>
.if ${CC:T:Mclang}
.for src in ${SRC:Mfuse/main.c} ${SRC:Mfuse/op*.c}
CFLAGS.${src} += -Wno-documentation
.endfor
.endif

# these are explicitly range checked
.if ${CC:T:Mgcc}
CFLAGS.fuse/op.c += -Wno-sign-compare
.endif
.if ${CC:T:Mclang}
CFLAGS.fuse/op.c += -Wno-sign-conversion
CFLAGS.fuse/op.c += -Wno-sign-compare
.endif

# op structs have their own extern declarations elsewhere
.if ${CC:T:Mclang}
.for src in ${SRC:Mfuse/op*.c}
CFLAGS.${src} += -Wno-missing-variable-declarations
.endfor
.endif

# sread(2)'s return value is explicitly range checked
# (silly that read(2) returns int)
.if ${CC:T:Mclang}
CFLAGS.fuse/sread.c += -Wno-shorten-64-to-32
.endif

# explitly range checked
# (silly that stat.st_size is an off_t)
.if ${CC:T:Mclang}
CFLAGS.fuse/op_zim.c   += -Wno-sign-conversion
CFLAGS.fuse/op_index.c += -Wno-sign-conversion
CFLAGS.fuse/op_field.c += -Wno-sign-conversion
.endif

.for lib in ${LIB:Mlibbib}
${BUILD}/bin/bibfs: ${BUILD}/lib/${lib:R}.a
.endfor

LFLAGS.bibfs  ?= `pkg-config fuse --libs`

.for src in ${SRC:Mfuse/*.c}
${BUILD}/bin/bibfs: ${BUILD}/${src:R}.o
.endfor

