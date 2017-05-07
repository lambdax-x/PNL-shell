CC = gcc

CFLAGS =	-std=gnu11						\
		-Wall -Werror						\
		-Iinclude						\
		-I$(COMMON)
ifeq ($(DEBUG), 1)
    CFLAGS += -g -DDEBUG
endif

LDFLAGS = -lreadline

SRC = src
OBJ = obj
BIN = bin

objs := shell.o utils/parse.o cmd/builtins.o cmd/parse.o cmd/exec.o

cmds := hello sleep list fg kill wait meminfo modinfo exec lsmod
cmd-objs := $(addprefix cmd/handlers/,$(addsuffix .o, $(cmds)))

.PHONY: mkdirs clean distclean

all: mkdirs $(BIN)/shell

mkdirs:
	- mkdir -p $(OBJ)/utils $(BIN)
	- mkdir -p $(OBJ)/cmd/handlers $(BIN)

$(BIN)/shell: $(addprefix $(OBJ)/,$(objs) $(cmd-objs))
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	- find $(OBJ) -regex ".*\.o$$" -delete

distclean: clean
	rm -rvf -- $(OBJ) $(BIN)
