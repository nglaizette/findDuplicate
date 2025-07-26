CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb
LIBS=

findduplicate: main.c
	$(CC) $(CFLAGS) -o findduplicate main.c $(LIBS)