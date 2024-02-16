GCC=gcc
COMMON_FLAGS=-g -Wall
SHARED_FLAGS=-fPIC
SDL_FLAGS=`sdl2-config --cflags --libs`
LD_FLAGS=-shared 

SRC=src
CHIP8DIR=$(SRC)/chip8
CDP1802DIR=$(SRC)/cdp1802
BIN=bin

PARENTDIR=@mkdir -p $(@D)

.PHONY: all clean

all: $(BIN)/emulator $(BIN)/disassembler $(BIN)/cdp1802 $(BIN)/libretro.dll

$(BIN)/cdp1802: $(CDP1802DIR)/cdp1802.c $(CDP1802DIR)/instructions.c
	$(PARENTDIR)
	$(GCC) $(COMMON_FLAGS) $^ -o $@

$(BIN)/emulator: $(CHIP8DIR)/emulator.c $(CHIP8DIR)/chip8.c $(CHIP8DIR)/stack.c $(CHIP8DIR)/display.c
	$(PARENTDIR)
	$(GCC) $(COMMON_FLAGS) $^ -o $@ $(SDL_FLAGS)

$(BIN)/disassembler: $(CHIP8DIR)/disassembler.c $(CHIP8DIR)/chip8.c $(CHIP8DIR)/stack.c
	$(PARENTDIR)
	$(GCC) $(COMMON_FLAGS) $^ -o $@

$(BIN)/libretro.dll: $(BIN)/libretro.o $(BIN)/chip8.o $(BIN)/stack.o
	$(PARENTDIR)
	$(GCC) $(COMMON_FLAGS) -o $@ $^ $(LD_FLAGS)

$(BIN)/libretro.o: $(CHIP8DIR)/libretro.c
	$(PARENTDIR)
	$(GCC) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $(BIN)/libretro.o

$(BIN)/chip8.o: $(CHIP8DIR)/chip8.c
	$(PARENTDIR)
	$(GCC) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $(BIN)/chip8.o

$(BIN)/stack.o: $(CHIP8DIR)/stack.c
	$(PARENTDIR)
	$(GCC) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $(BIN)/stack.o

clean:
	rm -f $(BIN)/*
