
# Declaring the compiler flags
CFLAGS=-g -Wall -Wextra

MKDIR_P=mkdir -p
UNAME:=$(shell uname)

# MacOS flags
ifeq ($(UNAME), Darwin)
	NCURSES=-lncurses
	CC=clang
endif

# Linux flags
ifeq ($(UNAME), Linux)
	NCURSES=-lncurses -lpthread -lm -ldl
	CC=gcc
endif

# Windows flags
ifeq ($(UNAME), Windows_NT)
	NCURSES=-lncurses
	CC=gcc
endif

# BSD's flags
ifeq ($(UNAME), OpenBSD)
	NCURSES=-lncurses -lpthread -lm
	CC=clang
endif
ifeq ($(UNAME), FreeBSD)
	NCURSES=-lncurses -lpthread -lm
	CC=clang
endif
ifeq ($(UNAME), NetBSD)
	NCURSES=-lncurses -lpthread -lm
	CC=clang
endif

# Declaring the main directories spot
SRC=src
OBJ=obj
BINDIR=bin
OTHERDIR=./other/
OTHER=$(OTHERDIR)title.mp3
BIN=$(BINDIR)/xtetris

# Declaring the pattern that the make file has
# to follow to retrive the correct files
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
HDRS=$(wildcard $(SRC)/*.h)

# Default make command that compile everything
all: setup $(BIN)

# setup all the directories
setup: binDir objDir

# rules for compiling objects files
$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(NCURSES)

# rules for compiling c code files
$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

binDir: $(BINDIR)
objDir: $(OBJ)

# creating folders if not already created
$(BINDIR):
	$(MKDIR_P) $@

$(OBJ):
	$(MKDIR_P) $@

# make clean command for delete all the compiled files
clean:
	$(RM) -r $(BINDIR)/* $(OBJ)/*

fly:
	$(BINDIR)/./xtetris $(OTHER)
