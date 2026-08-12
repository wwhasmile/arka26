CC ?= cc

ASSEMBLY_NAME=arka26

CFLAGS := -std=c99 -Wall -Wextra -Wpedantic -Werror
LDFLAGS := -L../has/lib -lhas
INCLUDE_FLAGS := -Isrc -I../has/src

DEFINES :=

OUTPUT_NAME :=

UNAME_S :=
ifneq ($(OS),Windows_NT)
	UNAME_S = $(shell uname -s)
endif
ifeq ($(OS),Windows_NT)
	HOST := win32
	VERSION := $(shell git describe --tags --always --dirty 2>NUL || echo local)
	rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
	DIR := $(subst /,\,$(CURDIR))
	SRC_FILES = $(call rwildcard,src,*.c)
	DIRECTORIES = \src $(subst $(DIR),,$(shell dir src /S /AD /B | findstr /i src))
	OBJ_FILES = $(SRC_FILES:%=obj/%.o)
else ifeq ($(UNAME_S),Linux)
	HOST := linux
	VERSION := $(shell git describe --tags --always --dirty 2>/dev/null || echo local)
	SRC_FILES = $(shell find src -name "*.c")
	DIRECTORIES = $(shell find src -type d)
	OBJ_FILES = $(SRC_FILES:%=obj/%.o)
else
	$(error Unsupported platform in this makefile.)
endif

PLATFORM := $(HOST)

.PHONY: build-debug build-release flags-debug flags-release
.PHONY: scaffold scaffold-win32 scaffold-linux
.PHONY: link link-win32 link-linux
.PHONY: clean clean-win32 clean-linux

build-release: scaffold flags-release compile link
build-debug: scaffold flags-debug compile link

flags-release: DEFINES += -DARKA26_RELEASE -DVERSION=\"$(VERSION)\"
flags-release: CFLAGS += -MD -O3
flags-release: LDFLAGS +=
flags-release: link compile

flags-debug: DEFINES += -DARKA26_DEBUG -DVERSION=\"$(VERSION)-debug\"
flags-debug: CFLAGS += -MD -O0 -g3 -fno-omit-frame-pointer
flags-debug: LDFLAGS += -g3
flags-debug: link compile

.NOTPARALLEL: scaffold
scaffold: scaffold-$(PLATFORM)

scaffold-win32:
	-@mkdir $(addprefix obj\\,$(DIRECTORIES)) 2>nul
	@if not exist bin mkdir bin

scaffold-linux:
	@mkdir -p bin
	@mkdir -p $(addprefix obj/,$(DIRECTORIES))

clean: clean-$(PLATFORM)

clean-win32:
	@echo Cleaning
	if exist bin del /s /q bin\*.*
	if exist obj del /s /q obj\*.*

clean-linux:
	@echo Cleaning
	@rm -rf bin/*
	@rm -rf obj/*

link: link-$(PLATFORM)

link-win32: CFLAGS += -Xlinker /INCREMENTAL
link-win32: DEFINES += -D_CRT_SECURE_NO_WARNINGS -DUNICODE
link-win32: LDFLAGS +=
link-win32: OUTPUT_NAME = bin\$(ASSEMBLY_NAME).exe

link-linux: CFLAGS +=
link-linux: DEFINES +=
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
	@$(CC) $< -c -o $@ $(CFLAGS) $(DEFINES) $(INCLUDE_FLAGS)
-include $(OBJ_FILES:.o=.d)
