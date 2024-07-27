PROJNAME=term-display

build:
	gcc src/main.c -o $(PROJNAME)

run: build
	./$(PROJNAME)
