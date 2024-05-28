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
    unsigned int word = 0;

    word |= ((unsigned int)R & 0b111) << 9;
    word |= ((unsigned int)G & 0b111) << 12;
    word |= ((unsigned int)B & 0b111) << 15;

    return word;
}

static int WBR_S(unsigned int reg, unsigned int offset, unsigned int X, unsigned int Y, unsigned int onScreen) {
    unsigned int word = 0;

    word |= ((unsigned int)WBR_OPCODE & OPCODE_MASK) << 0;
    word |= ((unsigned int)reg & REG_MASK) << 4;

    word |= ((unsigned int)offset & OFFSET_MASK) << 9;
    word |= ((unsigned int)Y & 0b111111111) << 18;
    word |= ((unsigned int)X & 0b111111111) << 28;
    word |= ((unsigned int)onScreen & 0b111111111) << 38;

    return word;
}

static int WSM(unsigned int mem_address, unsigned int R, unsigned int G, unsigned int B) {
    unsigned int word = 0;

    word |= ((unsigned int)WSM_OPCODE & OPCODE_MASK) << 0;
    word |= ((unsigned int)mem_address & SMEN_OFFSET_MASK) << 4;

    word |= ((unsigned int)R & 0b111) << 18;
    word |= ((unsigned int)G & 0b111) << 21;
    word |= ((unsigned int)B & 0b111) << 24; 

    return word;
}

static int WBM(unsigned int mem_address, unsigned int R, unsigned int G, unsigned int B) {
    unsigned int word = 0;

    word |= ((unsigned int)WSM_OPCODE & OPCODE_MASK) << 0;
    word |= ((unsigned int)mem_address & SMEN_OFFSET_MASK) << 4;

    word |= ((unsigned int)R & 0b111) << 18;
    word |= ((unsigned int)G & 0b111) << 21;
    word |= ((unsigned int)B & 0b111) << 24;  

    return word;
}

static int DP(unsigned int address, unsigned int ref_point_X, unsigned int ref_point_Y, unsigned int size, unsigned int R, unsigned int G, unsigned int B, unsigned int shape) {
    unsigned int word = 0;

    word |= ((unsigned int)DP_OPCODE & OPCODE_MASK) << 0;
    word |= ((unsigned int)address & DPMEN_OFFSET_MASK) << 4;

    word |= ((unsigned int)ref_point_X & REF_POINT_MASK) << 8;
    word |= ((unsigned int)ref_point_Y & REF_POINT_MASK) << 17;
    word |= ((unsigned int)size & 0b1111) << 26;
    word |= ((unsigned int)R & 0b111) << 30;
    word |= ((unsigned int)G & 0b111) << 33;
    word |= ((unsigned int)B & 0b111) << 36;
    word |= ((unsigned int)shape & 0b1) << 39;

    return word;
}