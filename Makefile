# Project
PROJECT_NAME       ?= rpong
PROJECT_SOURCE     ?= main.c
RAYLIB_PATH        ?= /workspaces/raylib
EMSDK_PATH         ?= /workspaces/emsdk

# Compiler
CC                 := emcc
CFLAGS             := -Os -Wall -I$(RAYLIB_PATH)/src -L$(RAYLIB_PATH)/src -s USE_GLFW=3 --shell-file $(RAYLIB_PATH)/src/minshell.html -DPLATFORM_WEB -s ASYNCIFY

# Build target
all: $(RAYLIB_PATH)/src/libraylib.a
	$(CC) -o $(PROJECT_NAME).html $(PROJECT_SOURCE) $(RAYLIB_PATH)/src/libraylib.a $(CFLAGS)

$(RAYLIB_PATH)/src/libraylib.a:
	cd $(RAYLIB_PATH)/src && make PLATFORM=PLATFORM_WEB -j$(nproc)

clean:
	rm -f $(PROJECT_NAME).html $(PROJECT_NAME).js $(PROJECT_NAME).wasm $(PROJECT_NAME).data
	cd $(RAYLIB_PATH)/src && make clean