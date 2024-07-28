PROJNAME=cli-morpion
SRC=src
BIN=bin
CC=gcc
CFLAGS=-Wall -Werror
LD=

build: $(BIN) $(BIN)/main.o
	$(CC) $(BIN)/*.o -o $(BIN)/$(PROJNAME) $(LD)

$(BIN)/main.o: $(SRC)/main.* $(BIN)/utils.o
	$(CC) -c $(CFLAGS) src/main.c -o $(BIN)/main.o

$(BIN)/utils.o: $(SRC)/utils.*
	$(CC) -c $(CFLAGS) src/utils.c -o $(BIN)/utils.o

$(BIN):
	mkdir $(BIN)

run: build
	./$(BIN)/$(PROJNAME)

clean:
	rm -rf $(BIN)
