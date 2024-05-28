#include "instructions.h"
#include "address_map_arm.h"
#include <asm/io.h>

// Instrução
#define WBR_OPCODE 0b0000
#define WSM_OPCODE 0b0001
#define WBM_OPCODE 0b0010
#define DP_OPCODE  0b0011

// Endereços físicos
#define DATA_A 0x80 // Registrador com opcode e endereçamentos
#define DATA_B 0x70 // Registradores de dados
#define START 0xc0  // WRREG

// Máscaras para extrair os diferentes campos
#define OPCODE_MASK 0b1111                  // 4 bits
#define REG_MASK 0b11111                    // 5 bits
#define OFFSET_MASK 0b111111111             // 9 bits
#define SMEN_OFFSET_MASK 0b11111111111111   // 14 bits
#define BMEN_OFFSET_MASK 0b111111111111     // 12 bits
#define DPMEN_OFFSET_MASK 0b1111            // 4 bits
#define REF_POINT_MASK 0b111111111          // 9 bits

static int WBR_BG(unsigned int R, unsigned int G, unsigned int B) {
    unsigned int dataBus[2];
    
    unsigned int dataB = (R & 0b111) << 0;
    dataB |= (G & 0b111) << 3;
    dataB |= (B & 0b111) << 6;

    unsigned int dataA = 0;

    dataBus[0] = dataA;
    dataBus[1] = dataB;

    return dataBus;
}

static int WBR_S(unsigned int reg, unsigned int offset, unsigned int X, unsigned int Y, unsigned int onScreen) {
    unsigned int dataBus[2];

    unsigned int dataA |= ((unsigned int)WBR_OPCODE & OPCODE_MASK) << 0;
    dataA |= ((unsigned int)reg & REG_MASK) << 4;

    unsigned int dataB |= ((unsigned int)offset & OFFSET_MASK) << 0;
    dataB |= ((unsigned int)Y & 0b111111111) << 9;
    dataB |= ((unsigned int)X & 0b111111111) << 19;
    dataB |= ((unsigned int)onScreen & 0b111111111) << 29;

    dataBus[0] = dataA;
    dataBus[1] = dataB;

    return dataBus;
}

static int WSM(unsigned int mem_address, unsigned int R, unsigned int G, unsigned int B) {
    unsigned int dataBus[2];

    unsigned int dataA |= ((unsigned int)WSM_OPCODE & OPCODE_MASK) << 0;
    dataA |= ((unsigned int)mem_address & SMEN_OFFSET_MASK) << 4;

    unsigned int dataB = ((unsigned int)R & 0b111) << 0;
    dataB |= ((unsigned int)G & 0b111) << 3;
    dataB |= ((unsigned int)B & 0b111) << 6; 

    dataBus[0] = dataA;
    dataBus[1] = dataB;

    return dataBus;
}

static int WBM(unsigned int mem_address, unsigned int R, unsigned int G, unsigned int B) {
    unsigned int dataBus[2];

    unsigned int dataA |= ((unsigned int)WBM_OPCODE & OPCODE_MASK) << 0;
    dataA |= ((unsigned int)mem_address & BMEN_OFFSET_MASK) << 4;

    unsigned int dataB = ((unsigned int)R & 0b111) << 0;
    dataB |= ((unsigned int)G & 0b111) << 3;
    dataB |= ((unsigned int)B & 0b111) << 6; 

    dataBus[0] = dataA;
    dataBus[1] = dataB;

    return dataBus;
}

static int DP(unsigned int address, unsigned int ref_point_X, unsigned int ref_point_Y, unsigned int size, unsigned int R, unsigned int G, unsigned int B, unsigned int shape) {
    unsigned int dataBus[2];

    unsigned int dataA |= ((unsigned int)DP_OPCODE & OPCODE_MASK) << 0;
    dataA |= ((unsigned int)address & DPMEN_OFFSET_MASK) << 4;

    unsigned int dataB |= ((unsigned int)ref_point_X & REF_POINT_MASK) << 0;
    dataB |= ((unsigned int)ref_point_Y & REF_POINT_MASK) << 9;
    dataB |= ((unsigned int)size & 0b1111) << 13;
    dataB |= ((unsigned int)R & 0b111) << 22;
    dataB |= ((unsigned int)G & 0b111) << 25;
    dataB |= ((unsigned int)B & 0b111) << 28;
    dataB |= ((unsigned int)shape & 0b1) << 31;

    dataBus[0] = dataA;
    dataBus[1] = dataB;

    return dataBus;
}