SRC := $(wildcard src/*.c)
OUT := ./bin/nel.exe

CC := gcc
CFLAGS := -Wall -Wextra -std=c99 -Iinclude -Iexeternal
LDFLAGS := -Llib -lSDL2main -lSDL2

all: build run

build:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

run:
	$(OUT)
