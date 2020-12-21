#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t memory[0xFFF]; // 4096
uint16_t stack[0x10]; // 16
uint8_t display[32][64];

void load_rom(char *file);

#endif
