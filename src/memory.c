#include "memory.h"

void load_rom(char *file) {
    // TODO: DO :>)
    FILE *fptr;
    fptr = fopen(file, "r");

    if (fptr == NULL) {
        fprintf(stderr, "THIS FILE DOES NOT EXIST!");
        exit(1);
    }

    // The program is mapped into ram at address 0x200 and the max size is the Max Ram Size - 0x200
    fread(&memory[0x200], 1, 0xFFF - 0x200, fptr); 

    fclose(fptr);
}
