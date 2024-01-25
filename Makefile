GCC=gcc -g -Wall
SRC=src
BIN=bin

.PHONY: all clean

all: $(BIN)/emulator $(BIN)/disassembler

$(BIN)/emulator: $(SRC)/emulator.c $(SRC)/chip8.c $(SRC)/stack.c $(SRC)/util.c
	mkdir -p $(@D)
	$(GCC) $^ -o $@

$(BIN)/disassembler: $(SRC)/disassembler.c $(SRC)/chip8.c $(SRC)/stack.c $(SRC)/util.c
	mkdir -p $(@D)
	$(GCC) $^ -o $@

clean:
	rm -f $(BIN)/*
