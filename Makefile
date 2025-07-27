CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb
LIBS=

findduplicate: src/main.c
	$(CC) $(CFLAGS) -o findduplicate src/main.c $(LIBS)