CC ?= cc
AR ?= ar
EMCC ?= emcc
EMAR ?= emar

ASSEMBLY_NAME=has

CFLAGS := -std=c99 -Wall -Wextra -Wpedantic -Werror
INCLUDE_FLAGS := -Isrc

DEFINES :=

OUTPUT_NAME :=

UNAME_S :=
ifneq ($(OS),Windows_NT)
	UNAME_S = $(shell uname -s)
endif
ifeq ($(OS),Windows_NT)
	HOST := win32
	rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
	DIR := $(subst /,\,$(CURDIR))
	SRC_FILES = $(call rwildcard,src,*.c)
	DIRECTORIES = \src $(subst $(DIR),,$(shell dir src /S /AD /B | findstr /i src))
	OBJ_FILES = $(SRC_FILES:%=obj/%.o)
else ifeq ($(UNAME_S),Linux)
	HOST := linux
	SRC_FILES = $(shell find src -name "*.c")
	DIRECTORIES = $(shell find src -type d)
	OBJ_FILES = $(SRC_FILES:%=obj/%.o)
else
	$(error Unsupported platform in this makefile.)
endif

PLATFORM ?= $(HOST)
ifeq ($(PLATFORM),emscripten)
	CC = $(EMCC)
	EMAR = $(EMAR)
endif

.PHONY: build-debug build-release flags-debug flags-release
.PHONY: scaffold scaffold-win32 scaffold-linux
.PHONY: link link-win32 link-linux link-emscripten
.PHONY: clean clean-win32 clean-linux

build-release: scaffold flags-release compile link
build-debug: scaffold flags-debug compile link

flags-release: DEFINES += -DGFXC_RELEASE
flags-release: CFLAGS += -flto -MD -O3
flags-release: link compile

flags-debug: DEFINES += -DGFXC_DEBUG
flags-debug: CFLAGS += -MD -O0 -g3 -fno-omit-frame-pointer
flags-debug: link compile

.NOTPARALLEL: scaffold
scaffold: scaffold-$(HOST)

scaffold-win32:
	-@mkdir $(addprefix obj\\,$(DIRECTORIES)) 2>nul
	@if not exist bin mkdir bin

scaffold-linux:
	@mkdir -p lib
	@mkdir -p $(addprefix obj/,$(DIRECTORIES))

clean: clean-$(HOST)

clean-win32:
	@echo Cleaning
	if exist lib del /s /q bin\*.*
	if exist obj del /s /q obj\*.*

clean-linux:
	@echo Cleaning
	@rm -rf lib/*
	@rm -rf obj/*

link: link-$(PLATFORM)

link-win32: CFLAGS += -Xlinker /INCREMENTAL
link-win32: DEFINES += -D_CRT_SECURE_NO_WARNINGS -DUNICODE
link-win32: OUTPUT_NAME = lib\$(ASSEMBLY_NAME).lib

link-linux: CFLAGS +=
link-linux: DEFINES +=
link-linux: OUTPUT_NAME = lib/lib$(ASSEMBLY_NAME).a

link-emscripten: CFLAGS +=
link-emscripten: DEFINES +=
link-emscripten: OUTPUT_NAME = lib/lib$(ASSEMBLY_NAME).a

link-win32 link-linux link-emscripten: link-common

link-common: $(OBJ_FILES)
	@$(AR) rcs $(OUTPUT_NAME) $(OBJ_FILES)

.PHONY: compile
compile:
-include $(OBJ_FILES:.o=.d)

obj/%.c.o: %.c
	@echo $<...
	@$(CC) $< -c -o $@ $(CFLAGS) $(DEFINES) $(INCLUDE_FLAGS)
-include $(OBJ_FILES:.o=.d)
