#include <string.h>

#include "memory.h"
#include "cpu.h"

#define DEBUG
#ifdef DEBUG

#define LOG(...) printf(__VA_ARGS__);

#else

#define LOG(...)

#endif

void init();

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./main.o <path_to_file>");
        exit(1);
    }

    LOG("MAINN......\n");
    init();
    load_rom(argv[1]);

    return 0;
}

void init() {
    LOG("INITING...\n");

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
