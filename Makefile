SRC := $(wildcard src/*.c)
FILE := examples/window.c

CC := gcc
CFLAGS := -Wall -Wextra -std=c99 -Iinclude -Iexternal
LDFLAGS := -lgdi32 -luser32 -lwinmm

all: build

example:
	$(CC) $(CFLAGS) $(SRC) $(FILE) -o bin/$(FILE:.c=.exe) $(LDFLAGS) ; \
	./bin/$(FILE:.c=.exe)

build:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)
