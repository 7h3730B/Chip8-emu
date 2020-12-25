#include "cpu.h"

void parse(uint8_t *opcode) {
    // An Instruction is 2 Bytes (16 Bit long)
    // Almost all Instruction can be differenced by the first 4 bit
    // The Instruction is structured like this:
    // 0xF          0xF             0xF             0xF
    // code         x               y               1-7, E
    //              |--------------nnn--------------|
    //                              |-------kk------|
    // x = *opcode & 0x0F
    // y = *(opcode + 1) >> 4 
    // kk = *(opcode + 1)
    // *(opcode + 1) & 0x0F gets the last 4 bit
    // code = *(opcode + 1) >> 4
    // nnn = (*opcode & 0x0F) << 8 | *(opcode + 1)

    printf("%04x: %02x %02x\n", regis.PC, *opcode, *(opcode + 1));
    // Maybe use a nibble, but seems to much trouble for this
    uint8_t x = *opcode & 0x0F;
    uint8_t y = *(opcode + 1) >> 4;
    uint8_t kk = *(opcode + 1);
    uint8_t nnn = (*opcode & 0x0F) << 8 | *(opcode + 1);

    switch(*opcode >> 4) {
        case 0x0:
            switch(*opcode) {
                case 0x00E0: memset(display, 0, sizeof(display)); break; // CLS: Clears the Screen
                case 0x00EE: regis.PC = stack[regis.SP]; regis.SP--; break; // RTS: Return from subroutine 
                case 0x0012: printf("0x0012 hit"); break;
            }
            break;
        case 0x1: regis.PC = nnn; break; // JP sets PC to the address
        case 0x2: regis.SP++; stack[regis.SP] = regis.PC; regis.PC = nnn; break; // CALL: Pushes the current PC onto the stack and sets the PC to the address to jump
        case 0x3: if (regis.gregis[x] == kk) { regis.PC += 2; } break; // SE: Skips to the next instruction if Vx = kk
        case 0x4: if (regis.gregis[x] != kk) { regis.PC += 2; } break; // SNE: Skips to the next instruction if Vx != kk
        case 0x5: if (regis.gregis[x] == regis.gregis[y])  { regis.PC += 2; } break; // SE: Skips to the next instruction if Vx == Vy
        case 0x6: regis.gregis[x] = kk; break; // LD sets Vx to kk
        case 0x7: regis.gregis[x] += kk; break; // Add Vx to kk and store in Vx
        case 0x8:
            switch(*(opcode + 1) & 0x0F) {
                case 0x0: regis.gregis[x] = regis.gregis[y]; break;
                case 0x1: regis.gregis[x] |= regis.gregis[y]; break;
                case 0x2: regis.gregis[x] &= regis.gregis[y]; break;
                case 0x3: regis.gregis[x] ^= regis.gregis[y]; break;
                case 0x4: regis.VF = ((int) regis.gregis[x] + regis.gregis[y]) > 255 ? 1 : 0; regis.gregis[x] += regis.gregis[y]; break;
                case 0x5: regis.VF = (regis.gregis[x] > regis.gregis[y]) ? 1 : 0; regis.gregis[x] -= regis.gregis[y]; break;
                case 0x6: regis.VF = (regis.gregis[x] & regis.gregis[y]); regis.gregis[x] &= regis.gregis[y]; break;
                case 0x7: regis.VF = (regis.gregis[x] < regis.gregis[y]) ? 1 : 0; regis.gregis[y] -= regis.gregis[x]; break;
                case 0xE: regis.VF = (regis.gregis[x] >> 7) & 0x1; regis.gregis[x] = regis.gregis[x] << 1; break; // Look up that that does :hAA:
            }
        case 0x9: if (regis.gregis[x] != regis.gregis[y]) { regis.PC += 2; } break;
        case 0xA: regis.I = nnn; break;
        case 0xB: regis.PC = nnn + regis.gregis[0]; break;
        case 0xC: regis.gregis[x] = 10 & kk; break; // TODO: Make 10 to a random byte
        case 0xD: break; // TODO: Implement
        case 0xE:
            switch(kk) {
                case 0x9E: break; // TODO: Implement Keyboard press
                case 0xA1: break; // TODO: Implement Keyboard press
            }
        case 0xF:
            switch(kk) {
                case 0x07: regis.gregis[x] = regis.delayTimer; break;
                case 0x0A: break; // TODO: Implement hAA
                case 0x15: regis.delayTimer = regis.gregis[x]; break;
                case 0x18: regis.soundTimer = regis.gregis[x]; break;
                case 0x1E: regis.I += regis.gregis[x]; break;
                case 0x29: printf("%016x", x); break;
                case 0x33: memory[regis.I] = (regis.gregis[x] % 1000) / 100; memory[regis.I + 1] = (regis.gregis[x] % 100) / 10; memory[regis.I + 2] = (regis.gregis[x] % 10); break; // TODO: Implement BCD Representatin
                case 0x55: for (int i = 0; i <= x; i++) { memory[regis.I + i] = regis.gregis[i]; } break; // Here could be a exploit or sth
                case 0x65: for (int i = 0; i <= x; i++) { regis.gregis[i] = memory[regis.I + i]; } break;
            } 

        default:
            fprintf(stderr, "The opcode %04x is not implemented yet\n", *opcode);       
            exit(1);
            break;
    }
}
