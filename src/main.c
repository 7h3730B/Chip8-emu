#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>

#include "memory.h"
#include "cpu.h"

#define DEBUG
#ifdef DEBUG

#define LOG(...) printf(__VA_ARGS__);

#else

#define LOG(...)

#endif

void init();
void cycle();
void draw();

SDL_Event event;
SDL_Renderer *renderer;
SDL_Window *window;

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./main.o <path_to_file>");
        exit(1);
    }

    init();

    load_rom(argv[1]);

    cycle();

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64, 32, 0);

    // TODO: Implement Font Sprites into Intepreter Ram
    // Set the program counter to 0x200 where the program was loaded
    regis.PC = 0x200;

    // Set all regis to 0
    regis.SP = 0;
    regis.VF = 0;
    regis.I = 0;

    regis.delayTimer = 0;
    regis.soundTimer = 0;

    memset(regis.gregis, 0, sizeof(regis.gregis));

    memset(memory, 0, sizeof(memory));
    memset(stack, 0, sizeof(stack));
    memset(display, 0, sizeof(display));
}

void cycle() {
    // count down timers!!!
    for (int i = 0; i <= 300; i++) {
        parse(&memory[regis.PC]);
        regis.PC += 2;
        // draw();
    }
}

void draw() {

}
