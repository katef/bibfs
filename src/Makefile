CFLAGS=	-ansi -pedantic -Wall -W -Werror

OBJS += lex.o
OBJS += main.o

bibfs: $(OBJS)
	$(CC) -o $@ $>

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS)

