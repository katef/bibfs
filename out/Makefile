.include "../share/mk/top.mk"

SRC += out/bibtex.c
SRC += out/json.c
SRC += out/xml.c
SRC += out/zim.c

SRC += out/outs.c
SRC += out/values.c

# explicitly not permitted by contractual obligation for fstat()
.if ${CC:T:Mclang}
CFLAGS_out/outs.c += -Wno-sign-conversion
.endif

