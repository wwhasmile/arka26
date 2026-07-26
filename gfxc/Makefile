CC ?= cc

ASSEMBLY_NAME=gfxc

CFLAGS := -std=c99 -Wall -Wextra -Wpedantic -Werror
LDFLAGS :=
INCLUDE_FLAGS := -Isrc

DEFINES :=

OUTPUT_NAME :=

UNAME_S :=
ifneq ($(OS),Windows_NT)
	UNAME_S = $(shell uname -s)
endif
ifeq ($(OS),Windows_NT)
	PLATFORM := win32

	rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
	DIR := $(subst /,\,$(CURDIR))
	SRC_FILES = $(call rwildcard,src,*.c)
	DIRECTORIES = \src $(subst $(DIR),,$(shell dir src /S /AD /B | findstr /i src))
	OBJ_FILES = $(SRC_FILES:%=obj/%.o)
else ifeq ($(UNAME_S),Linux)
	PLATFORM := linux
	SRC_FILES = $(shell find src -name "*.c")
	DIRECTORIES = $(shell find src -type d)
	OBJ_FILES = $(SRC_FILES:%=obj/%.o)
else
	$(error Unsupported platform in this makefile.)
endif

.PHONY: build-debug build-release flags-debug flags-release
.PHONY: scaffold scaffold-win32 scaffold-linux
.PHONY: link link-win32 link-linux

build-release: scaffold flags-release compile link
build-debug: scaffold flags-debug compile link

flags-release: DEFINES += -DGFXC_RELEASE
flags-release: CFLAGS += -MD -O3
flags-release: LDFLAGS +=
flags-release: link compile

flags-debug: DEFINES += -DGFXC_DEBUG
flags-debug: CFLAGS += -MD -O0 -g3 -fno-omit-frame-pointer
flags-debug: LDFLAGS += -g3 -fsanitize=address -fsanitize=undefined -fsannitize=thread
flags-release: link compile

.NOTPARALLEL: scaffold
scaffold: scaffold-$(PLATFORM)

scaffold-win32:
	-@mkdir $(addprefix obj\\,$(DIRECTORIES)) 2>nul
	@if not exist bin mkdir bin

scaffold-linux:
	@mkdir -p bin
	@mkdir -p $(addprefix obj/,$(DIRECTORIES))

link: link-$(PLATFORM)

link-win32: CFLAGS += -Xlinker /INCREMENTAL
link-win32: DEFINES += -D_CRT_SECURE_NO_WARNINGS -DUNICODE
link-win32: OUTPUT_NAME = bin\$(ASSEMBLY_NAME).exe

link-linux: CFLAGS +=
link-linux: LDFLAGS +=
link-linux: OUTPUT_NAME = bin/$(ASSEMBLY_NAME)

link-win32 link-linux: link-common

link-common: $(OBJ_FILES)
	@$(CC) $(OBJ_FILES) -o $(OUTPUT_NAME) $(LDFLAGS)

.PHONY: compile
compile:
-include $(OBJ_FILES:.o=.d)

obj/%.c.o: %.c
	@echo $<...
	@$(CC) $< -c -o $@ $(DEFINES) $(INCLUDE_FLAGS)
-include $(OBJ_FILES:.o=.d)

.PHONY: all
all: build-debug
