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
    switch(*opcode >> 4) {
        case 0x0:
            switch(*opcode) {
                case 0x00E0: memset(display, 0, sizeof(display)); break; // CLS: Clears the Screen
                case 0x00EE: regis.PC = stack[regis.SP]; regis.SP--; break; // RTS: Return from subroutine 
                case 0x0012: printf("0x0012 hit"); break;
            }
            break;
        case 0x1: regis.PC = (*opcode & 0x0F) << 8 | *(opcode + 1); break; // JP sets PC to the address
        case 0x2: regis.SP++; stack[regis.SP] = regis.PC; regis.PC = (*opcode & 0x0F) << 8 | *(opcode + 1); break; // CALL: Pushes the current PC onto the stack and sets the PC to the address to jump
        case 0x3: if (regis.gregis[*opcode & 0x0F] == *(opcode + 1)) { regis.PC += 2; } break; // SE: Skips to the next instruction if Vx = kk
        case 0x4: if (regis.gregis[*opcode & 0x0F] != *(opcode + 1)) { regis.PC += 2; } break; // SNE: Skips to the next instruction if Vx != kk
        case 0x5: if (regis.gregis[*opcode & 0x0F] == regis.gregis[*(opcode + 1) >> 4])  { regis.PC += 2; } break; // SE: Skips to the next instruction if Vx == Vy
        case 0x6: regis.gregis[*opcode & 0x0F] = *(opcode + 1); break; // LD sets Vx to kk
        case 0x7: regis.gregis[*opcode & 0x0F] = regis.gregis[*opcode & 0x0F] + *(opcode + 1); break; // Add Vx to kk and store in Vx
        case 0x8:
            switch(*(opcode + 1) & 0x0F) {
                case 0x0: regis.gregis[*opcode & 0x0F] = regis.gregis[*(opcode + 1) >> 4]; break;
                case 0x1: regis.gregis[*opcode & 0x0F] = regis.gregis[*opcode & 0x0F] | regis.gregis[*(opcode + 1) >> 4]; break;
                case 0x2: regis.gregis[*opcode & 0x0F] = regis.gregis[*opcode & 0x0F] & regis.gregis[*(opcode + 1) >> 4]; break;
                case 0x3: regis.gregis[*opcode & 0x0F] = regis.gregis[*opcode & 0x0F] ^ regis.gregis[*(opcode + 1) >> 4]; break;
                case 0x4: regis.VF = ((int) regis.gregis[*opcode & 0x0F] + regis.gregis[*(opcode + 1) >> 4]) > 255 ? 1 : 0; regis.gregis[*opcode & 0x0F] = regis.gregis[*opcode & 0x0F] + regis.gregis[*(opcode + 1) >> 4]; break;
                case 0x5: regis.VF = (regis.gregis[*opcode & 0x0F] > regis.gregis[*(opcode + 1) >> 4]) ? 1 : 0; regis.gregis[*opcode & 0x0F] = regis.gregis[*opxode & 0x0F] - regis.gregis[*(opcode + 1) >> 4]; break;
                case 0x6: regis.VF = (regis.gregis[*opcode & 0x0F] & regis.gregis[*(opcode + 1) >> 4]); regis.gregis[*opcode & 0x0F] = regis.gregis[*opcode & 0x0F] & regis.gregis[*(opcode + 1) >> 4]; break;
                case 0x7: regis.VF = (regis.gregis[*opcode & 0x0F] < regis.gregis[*(opcode + 1) >> 4]) ? 1 : 0; regis.gregis[*opcode & 0x0F] = regis.gregis[*(opcode + 1) >> 4] - regis.gregis[*opcode & 0x0F]; break;
                case 0xE: regis.VF = (regis.gregis[*opcode & 0x0F] >> 7) & 0x1; regis.gregis[*opcode & 0x0F] = regis.gregis[*opcode & 0x0F] << 1; break; // Look up that that does :hAA:
            }
        case 0x9: if (regis.gregis[*opcode & 0x0F] != regis.gregis[*(opcode + 1) >> 4]) { pc += 2; } break;
        case 0xA: regis.I = *opcode & 0x0F << 8 | *(opcode + 1); break;
        case 0xB: regis.PC = (*opcode & 0x0F << 8 | *(opcode + 1)) + regis.gregis[0]; break;
        case 0xC: regis.gregis[*opcode & 0x0F] = 10 & *(opcode + 1); break; // TODO: Make 10 to a random byte
        // TODO: Imlement next instruction
        

        default:
            fprintf(stderr, "The opcode %04x is not implemented yet\n", *opcode);       
            exit(1);
            break;
    }
}
