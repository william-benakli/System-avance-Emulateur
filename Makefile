GCC=gcc -g -Wall
SRC=src
BIN=bin

.PHONY: all clean

all: $(BIN)/main

$(BIN)/main: $(SRC)/main.c $(SRC)/chip8.c $(SRC)/instructions.c $(SRC)/stack.c $(SRC)/util.c
	mkdir -p $(@D)
	$(GCC) $(SRC)/util.c $(SRC)/stack.c $(SRC)/instructions.c $(SRC)/chip8.c $(SRC)/main.c -o $(BIN)/main

clean:
	rm -f $(BIN)/*
