CFLAGS=	-ansi -pedantic -Wall -W -Werror
SID_CFLAGS= -s no-numeric-terminals -s no-terminals

OBJS += lex.o
OBJS += main.o
OBJS += parser.o
OBJS += bib.o
OBJS += refactor.o

OBJS += out/bibtex.o
OBJS += out/json.o

bibfs: $(OBJS)
	$(CC) -o $@ $>

parser.c parser.h: parser.sid parser.act
	sid $(SID_CFLAGS) $> parser.c parser.h || { rm -f parser.c parser.h; false; }

parser.h: parser.act
parser.c: parser.act

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS)

