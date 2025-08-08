SRC := $(wildcard src/*.c)
OUT := ./bin/nal.exe

CC := gcc
CFLAGS := -Wall -Wextra -std=c99 -Iinclude -Iexternal
LDFLAGS := -Llib -lSDL2main -lSDL2

all: build run

build:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

run:
	$(OUT)
