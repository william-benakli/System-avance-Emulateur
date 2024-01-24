GCC=gcc -Wall
SRC=src
BIN=bin

.PHONY: all clean

all: $(BIN)/main

$(BIN)/main: $(SRC)/main.c $(SRC)/chip8.c $(SRC)/instructions.c $(SRC)/stack.c
	$(GCC) $(SRC)/stack.c $(SRC)/instructions.c $(SRC)/chip8.c $(SRC)/main.c -o $(BIN)/main

clean:
	rm -f $(BIN)/*