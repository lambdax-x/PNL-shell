CC=gcc
CFLAGS=-std=gnu11 -I$(MDIR)/include
LDFLAGS=-static

SRC=src
OBJ=obj
BIN=bin

.PHONY: mkdirs clean distclean

all: mkdirs $(BIN)/tests

mkdirs:
	- mkdir $(OBJ) $(BIN)

$(BIN)/%: $(OBJ)/%.o
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -vf -- $(OBJ)/*.o

distclean: clean
	rm -rvf -- $(OBJ) $(BIN)
