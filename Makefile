CC = gcc
SDL2FLAGS = $(shell pkg-config sdl2 --cflags --libs)
LIBS = -LC:\libsdl\lib -lmingw32 -lSDL2main -lSDL2 -lopengl32
CFLAGS = -Wall -pedantic-errors -IC:\libsdl\include

SRC=$(wildcard src/*.c)

test: $(SRC)
		$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

run: test
	./test.exe H:/ROMs/Chip-8/TETRIS
