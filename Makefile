CC = gcc
SDL2FLAGS = $(shell pkg-config sdl2 --cflags --libs)
 
CFLAGS = -Wall

SRC=$(wildcard src/*.c)

test: $(SRC)
		gcc -o $@ $^ $(CFLAGS)

run: test
		./test.exe H:/ROMs/Chip-8/TETRIS
