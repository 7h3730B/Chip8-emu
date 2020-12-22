#ifndef _CPU_H
#define _CPU_H

#include <stdint.h>

struct regis {
    uint8_t gregis[16];
    uint16_t I;
    uint16_t VF;
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint16_t PC;
    uint8_t SP;
} regis;

#endif
