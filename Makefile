PROJNAME=term-display
SRC=src
BIN=bin
CC=gcc
CFLAGS=-Wall -Werror
LD=

build: $(BIN)
	$(CC) $(CFLAGS) src/main.c -o $(BIN)/$(PROJNAME) $(LD)

$(BIN):
	mkdir $(BIN)

run: build
	./$(BIN)/$(PROJNAME)

clean:
	rm -rf $(BIN)
