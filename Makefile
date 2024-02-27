GCC=gcc
GCC_WINDOWS=x86_64-w64-mingw32-gcc
GCC_ANDROID=android/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android21-clang
COMMON_FLAGS=-g -Wall
SHARED_FLAGS=-fPIC
SDL_FLAGS=`sdl2-config --cflags --libs`
LD_FLAGS=-shared
INCLUDE_SDL=-I./src/chip8/SDL2/include
PTHREAD_FLAGS=-pthread

SRC=src
CHIP8DIR=$(SRC)/chip8
CDP1802DIR=$(SRC)/cdp1802
BIN=bin

LIBRETRO_CORE_NAME=
LINUX_LIBRETRO_CORE_NAME=
WINDOWS_LIBRETRO_CORE_NAME=

PARENTDIR=@mkdir -p $(@D)

.PHONY: all clean linux windows android

all: linux windows android

linux: $(BIN)/chip8_core_linux.so $(BIN)/emulator $(BIN)/disassembler $(BIN)/cdp1802

windows: $(BIN)/chip8_core_windows.dll

android: $(BIN)/chip8_core_android.so

$(BIN)/emulator: $(CHIP8DIR)/emulator.c $(CHIP8DIR)/chip8.c $(CHIP8DIR)/stack.c $(CHIP8DIR)/display.c
	$(PARENTDIR)
	$(GCC) $(COMMON_FLAGS) $(PTHREAD_FLAGS) $^ -o $@ $(SDL_FLAGS)

$(BIN)/disassembler: $(CHIP8DIR)/disassembler.c $(CHIP8DIR)/chip8.c $(CHIP8DIR)/stack.c
	$(PARENTDIR)
	$(GCC) $(COMMON_FLAGS) $^ -o $@

$(BIN)/cdp1802: $(CDP1802DIR)/cdp1802.c $(CDP1802DIR)/instructions.c
	$(PARENTDIR)
	$(GCC) $(COMMON_FLAGS) $^ -o $@

$(BIN)/chip8_core_linux.so: $(BIN)/libretro.o $(BIN)/chip8.o $(BIN)/stack.o
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

$(BIN)/disassembler.exe: $(CHIP8DIR)/disassembler.c $(CHIP8DIR)/chip8.c $(CHIP8DIR)/stack.c
	$(PARENTDIR)
	$(GCC_WINDOWS) $(COMMON_FLAGS) $^ -o $@

$(BIN)/cdp1802.exe: $(CDP1802DIR)/cdp1802.c $(CDP1802DIR)/instructions.c
	$(PARENTDIR)
	$(GCC_WINDOWS) $(COMMON_FLAGS) $^ -o $@

$(BIN)/chip8_core_windows.dll: $(BIN)/libretro_windows.o $(BIN)/chip8_windows.o $(BIN)/stack_windows.o
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

$(BIN)/chip8_core_android.so: $(BIN)/libretro_android.o $(BIN)/chip8_android.o $(BIN)/stack_android.o
	$(GCC_ANDROID) $(COMMON_FLAGS) -o $@ $^ $(LD_FLAGS)
	rm -rf $^

$(BIN)/libretro_android.o: $(CHIP8DIR)/libretro.c
	$(PARENTDIR)
	$(GCC_ANDROID) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $@

$(BIN)/chip8_android.o: $(CHIP8DIR)/chip8.c
	$(PARENTDIR)
	$(GCC_ANDROID) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $@

$(BIN)/stack_android.o: $(CHIP8DIR)/stack.c
	$(PARENTDIR)
	$(GCC_ANDROID) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BIN)

clean:
	rm -f $(BIN)/*
