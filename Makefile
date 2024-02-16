GCC=gcc
GCC_WINDOWS=x86_64-w64-mingw32-gcc
COMMON_FLAGS=-g -Wall
SHARED_FLAGS=-fPIC
SDL_FLAGS=`sdl2-config --cflags --libs`
LD_FLAGS=-shared
INCLUDE_SDL=-I./src/chip8/SDL2/include
LIB_SDL=-L./src/chip8/SDL2/lib -lSDL2
PTHREAD_FLAGS=-pthread

SRC=src
CHIP8DIR=$(SRC)/chip8
CDP1802DIR=$(SRC)/cdp1802
BIN=bin

PARENTDIR=@mkdir -p $(@D)

.PHONY: all clean

all: linux windows

linux: $(BIN)/emulator $(BIN)/disassembler $(BIN)/cdp1802 $(BIN)/libretro.so

windows: $(BIN)/libretro.dll

$(BIN)/emulator: $(CHIP8DIR)/emulator.c $(CHIP8DIR)/chip8.c $(CHIP8DIR)/stack.c $(CHIP8DIR)/display.c
	$(PARENTDIR)
	$(GCC) $(COMMON_FLAGS) $(PTHREAD_FLAGS) $^ -o $@ $(SDL_FLAGS)

$(BIN)/disassembler: $(CHIP8DIR)/disassembler.c $(CHIP8DIR)/chip8.c $(CHIP8DIR)/stack.c
	$(PARENTDIR)
	$(GCC) $(COMMON_FLAGS) $^ -o $@

$(BIN)/cdp1802: $(CDP1802DIR)/cdp1802.c $(CDP1802DIR)/instructions.c
	$(PARENTDIR)
	$(GCC) $(COMMON_FLAGS) $^ -o $@

$(BIN)/libretro.so: $(BIN)/libretro.o $(BIN)/chip8.o $(BIN)/stack.o
	$(PARENTDIR)
	$(GCC) $(COMMON_FLAGS) -o $@ $^ $(LD_FLAGS)
	rm -rf $^

$(BIN)/libretro.o: $(CHIP8DIR)/libretro.c
	$(PARENTDIR)
	$(GCC) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $@

$(BIN)/chip8.o: $(CHIP8DIR)/chip8.c
	$(PARENTDIR)
	$(GCC) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $@

$(BIN)/stack.o: $(CHIP8DIR)/stack.c
	$(PARENTDIR)
	$(GCC) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $@

$(BIN)/emulator.exe: $(CHIP8DIR)/emulator.c $(CHIP8DIR)/chip8.c $(CHIP8DIR)/stack.c $(CHIP8DIR)/display.c
	$(PARENTDIR)
	$(GCC_WINDOWS) $(COMMON_FLAGS) $(INCLUDE_SDL) $^ -o $@ $(LIB_SDL)

$(BIN)/disassembler.exe: $(CHIP8DIR)/disassembler.c $(CHIP8DIR)/chip8.c $(CHIP8DIR)/stack.c
	$(PARENTDIR)
	$(GCC_WINDOWS) $(COMMON_FLAGS) $^ -o $@

$(BIN)/cdp1802.exe: $(CDP1802DIR)/cdp1802.c $(CDP1802DIR)/instructions.c
	$(PARENTDIR)
	$(GCC_WINDOWS) $(COMMON_FLAGS) $^ -o $@

$(BIN)/libretro.dll: $(BIN)/libretro_windows.o $(BIN)/chip8_windows.o $(BIN)/stack_windows.o
	$(GCC_WINDOWS) $(COMMON_FLAGS) -o $@ $^ $(LD_FLAGS)
	rm -rf $^

$(BIN)/libretro_windows.o: $(CHIP8DIR)/libretro.c
	$(PARENTDIR)
	$(GCC_WINDOWS) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $@

$(BIN)/chip8_windows.o: $(CHIP8DIR)/chip8.c
	$(PARENTDIR)
	$(GCC_WINDOWS) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $@

$(BIN)/stack_windows.o: $(CHIP8DIR)/stack.c
	$(PARENTDIR)
	$(GCC_WINDOWS) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $@

clean:
	rm -f $(BIN)/*
