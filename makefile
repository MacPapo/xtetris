# Declaring the compiler preferences
CC=clang

# Declaring the compiler flags
CFLAGS=-g -Wall -Wextra

NCURSES=-lncurses
MKDIR_P=mkdir -p

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

# Declaring the test directories spot
TEST=tests

# Declaring the pattern that the make file has
# to follow to retrive the correct test files
TESTS=$(wildcard $(TEST)/*.c)
TESTBINS=$(patsubst $(TEST)/*.c, $(TEST)/bin/%.o, $(TESTS))

# Default make command that compile everything
all: setup $(BIN)

# setup all the directories
setup: binDir objDir

# make release command for production only
release: CFLAGS=-Wall -Wextra -O2 -DNDEBUG
release: clean
release: $(BIN)

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

$(TEST)/bin:
	$(MKDIR_P) $@

# rules for compiling test files
$(TEST)/bin/%: $(TEST)/%.c
	$(CC) $(CFLAGS) $< $(OBJS) -o $@ -lncurses

# make test command for test only
test: $(TEST)/bin $(TESTBINS)
	for test in $(TESTBINS) ; do ./$$test ; done

# make clean command for delete all the compiled files
clean:
	$(RM) -r $(BINDIR)/* $(OBJ)/*

fly:
	$(BINDIR)/./xtetris $(OTHER)
