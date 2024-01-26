GCC=gcc -g -Wall
SRC=src
BIN=bin
SDL_FLAGS=`sdl2-config --cflags --libs`

.PHONY: all clean

all: $(BIN)/emulator $(BIN)/disassembler

$(BIN)/emulator: $(SRC)/emulator.c $(SRC)/chip8.c $(SRC)/stack.c $(SRC)/util.c $(SRC)/display.c
	mkdir -p $(@D)
	$(GCC) $^ -o $@ $(SDL_FLAGS)

$(BIN)/disassembler: $(SRC)/disassembler.c $(SRC)/chip8.c $(SRC)/stack.c $(SRC)/util.c
	mkdir -p $(@D)
	$(GCC) $^ -o $@

clean:
	rm -f $(BIN)/*
