# Compiler/linker options
CC = g++
LDFLAGS = -l argp
SRC = src/hexdiff.cpp
EXE = hexdiff

# Phony
.PHONY: build clean

# Make
build:
	$(CC) $(LDFLAGS) $(SRC) -o $(EXE)
	@echo Build successful!

clean:
	rm -Rf $(OBJ_DIR) $(EXE)
