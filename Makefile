SRC := $(wildcard src/*.c)
OUT := ./bin/nal.exe

CC := gcc
CFLAGS := -Wall -Wextra -std=c99 -Iinclude -Iexternal
LDFLAGS := -lgdi32 -luser32 -lwinmm

all: build run

build:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

run:
	$(OUT)
