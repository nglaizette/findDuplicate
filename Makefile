CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb
LIBS=
SRC=src/main.c src/recdir.c

findduplicate: $(SRC)
	$(CC) $(CFLAGS) -o findduplicate $(SRC) $(LIBS)