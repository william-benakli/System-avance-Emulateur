CC_LINUX=gcc
CC_WINDOWS=x86_64-w64-mingw32-gcc
CC_ANDROID=android/android-ndk-r26c/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android21-clang
COMMON_FLAGS=-g -Wall
SHARED_FLAGS=-fPIC
SDL_FLAGS=`sdl2-config --cflags --libs`
LD_FLAGS=-shared
PTHREAD_FLAGS=-pthread

# directories
SRC=src
CHIP8_SRC_DIR=$(SRC)/chip8
CDP1802_SRC_DIR=$(SRC)/cdp1802
BIN=bin
OBJ=$(BIN)/obj

CORE_NAME=chip8_core

PARENTDIR=@mkdir -p $(@D)

.PHONY: all clean linux windows android

all: linux windows android

linux: $(BIN)/$(CORE_NAME)_linux.zip $(BIN)/emulator $(BIN)/disassembler $(BIN)/cdp1802

windows:$(BIN)/$(CORE_NAME)_windows.zip

android: $(BIN)/$(CORE_NAME)_android.zip

.INTERMEDIATE: $(BIN)/libretro.o $(BIN)/chip8.o $(BIN)/stack.o $(BIN)/libretro_windows.o $(BIN)/chip8_windows.o $(BIN)/stack_windows.o $(BIN)/libretro_android.o $(BIN)/chip8_android.o $(BIN)/stack_android.o

$(BIN)/emulator: $(CHIP8_SRC_DIR)/emulator.c $(CHIP8_SRC_DIR)/chip8.c $(CHIP8_SRC_DIR)/stack.c $(CHIP8_SRC_DIR)/display.c
	$(PARENTDIR)
	$(CC_LINUX) $(COMMON_FLAGS) $(PTHREAD_FLAGS) $^ -o $@ $(SDL_FLAGS)

$(BIN)/disassembler: $(CHIP8_SRC_DIR)/disassembler.c $(CHIP8_SRC_DIR)/chip8.c $(CHIP8_SRC_DIR)/stack.c
	$(PARENTDIR)
	$(CC_LINUX) $(COMMON_FLAGS) $^ -o $@

$(BIN)/cdp1802: $(CDP1802_SRC_DIR)/cdp1802.c $(CDP1802_SRC_DIR)/instructions.c
	$(PARENTDIR)
	$(CC_LINUX) $(COMMON_FLAGS) $^ -o $@

$(BIN)/$(CORE_NAME)_linux.zip: $(BIN)/$(CORE_NAME)_linux.so $(CORE_NAME)_libretro.info
	$(PARENTDIR)
	mkdir -p $(basename $@)/cores/ $(basename $@)/info/
	cp $< $(basename $@)/cores/$(CORE_NAME)_libretro.so
	cp $(CORE_NAME)_libretro.info $(basename $@)/info/$(CORE_NAME)_libretro.info
	cd $(basename $@) ; zip -r ../../$@ . *
	rm -rf $(basename $@)

$(BIN)/$(CORE_NAME)_linux.so: $(OBJ)/libretro.o $(OBJ)/chip8.o $(OBJ)/stack.o
	$(PARENTDIR)
	$(CC_LINUX) $(COMMON_FLAGS) -o $@ $^ $(LD_FLAGS)

$(OBJ)/libretro.o: $(CHIP8_SRC_DIR)/libretro.c
	$(PARENTDIR)
	$(CC_LINUX) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $@

$(OBJ)/chip8.o: $(CHIP8_SRC_DIR)/chip8.c
	$(PARENTDIR)
	$(CC_LINUX) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $@

$(OBJ)/stack.o: $(CHIP8_SRC_DIR)/stack.c
	$(PARENTDIR)
	$(CC_LINUX) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $@

$(BIN)/disassembler.exe: $(CHIP8_SRC_DIR)/disassembler.c $(CHIP8_SRC_DIR)/chip8.c $(CHIP8_SRC_DIR)/stack.c
	$(PARENTDIR)
	$(CC_WINDOWS) $(COMMON_FLAGS) $^ -o $@

$(BIN)/cdp1802.exe: $(CDP1802_SRC_DIR)/cdp1802.c $(CDP1802_SRC_DIR)/instructions.c
	$(PARENTDIR)
	$(CC_WINDOWS) $(COMMON_FLAGS) $^ -o $@

$(BIN)/$(CORE_NAME)_windows.zip: $(BIN)/$(CORE_NAME)_windows.dll $(CORE_NAME)_libretro.info
	$(PARENTDIR)
	mkdir -p $(basename $@)/cores/ $(basename $@)/info/
	cp $< $(basename $@)/cores/$(CORE_NAME)_libretro.dll
	cp $(CORE_NAME)_libretro.info $(basename $@)/info/$(CORE_NAME)_libretro.info
	cd $(basename $@) ; zip -r ../../$@ . *
	rm -rf $(basename $@)

$(BIN)/chip8_core_windows.dll: $(OBJ)/libretro_windows.o $(OBJ)/chip8_windows.o $(OBJ)/stack_windows.o
	$(CC_WINDOWS) $(COMMON_FLAGS) -o $@ $^ $(LD_FLAGS)

$(OBJ)/libretro_windows.o: $(CHIP8_SRC_DIR)/libretro.c
	$(PARENTDIR)
	$(CC_WINDOWS) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $@

$(OBJ)/chip8_windows.o: $(CHIP8_SRC_DIR)/chip8.c
	$(PARENTDIR)
	$(CC_WINDOWS) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $@

$(OBJ)/stack_windows.o: $(CHIP8_SRC_DIR)/stack.c
	$(PARENTDIR)
	$(CC_WINDOWS) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $@

$(BIN)/$(CORE_NAME)_android.zip: $(BIN)/$(CORE_NAME)_android.so $(CORE_NAME)_libretro.info
	$(PARENTDIR)
	mkdir -p $(basename $@)/cores/ $(basename $@)/info/
	cp $< $(basename $@)/cores/$(CORE_NAME)_libretro.so
	cp $(CORE_NAME)_libretro.info $(basename $@)/info/$(CORE_NAME)_libretro.info
	cd $(basename $@) ; zip -r ../../$@ . *
	rm -rf $(basename $@)

$(BIN)/chip8_core_android.so: $(OBJ)/libretro_android.o $(OBJ)/chip8_android.o $(OBJ)/stack_android.o
	$(CC_ANDROID) $(COMMON_FLAGS) -o $@ $^ $(LD_FLAGS)

$(OBJ)/libretro_android.o: $(CHIP8_SRC_DIR)/libretro.c
	$(PARENTDIR)
	$(CC_ANDROID) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $@

$(OBJ)/chip8_android.o: $(CHIP8_SRC_DIR)/chip8.c
	$(PARENTDIR)
	$(CC_ANDROID) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $@

$(OBJ)/stack_android.o: $(CHIP8_SRC_DIR)/stack.c
	$(PARENTDIR)
	$(CC_ANDROID) $(COMMON_FLAGS) $(SHARED_FLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BIN)

clean:
	rm -rf $(BIN)/*
