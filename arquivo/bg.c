#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include "address_map_arm.h"

// Máscaras para extrair os diferentes campos
#define OPCODE_MASK 0b1111 // 4 bits
#define REG_MASK 0b11111   // 5 bits

// Endereços físicos
#define DATA_A 0x80 // Registrador com opcode e endereçamentos
#define DATA_B 0x70 // Registradores de dados
#define START 0xc0  // WRREG

// Ponteiros essenciais
volatile int *pointer_dataA;
volatile int *pointer_dataB;
volatile int *pointer_START;
void *pointer_bridge;

// Parâmetros
static unsigned int R = 0;
static unsigned int G = 0;
static unsigned int B = 0b111;

module_param(R, uint, 0644);
MODULE_PARM_DESC(R, "Valor de R (0 a 7)");
module_param(G, uint, 0644);
MODULE_PARM_DESC(G, "Valor de G (0 a 7)");
module_param(B, uint, 0644);
MODULE_PARM_DESC(B, "Valor de B (0 a 7)");

static int __init set_background_params(void)
{
    // Mapear o endereço físico para um endereço virtual
    pointer_bridge = ioremap(LW_BRIDGE_BASE, LW_BRIDGE_SPAN);

    pointer_dataA = (volatile int *)(pointer_bridge + DATA_A);
    pointer_dataB = (volatile int *)(pointer_bridge + DATA_B);
    pointer_START = (volatile int *)(pointer_bridge + START);

    // Adiciona o campo R
    unsigned int partB = (R & 0b111) << 0;
    // Adiciona o campo G
    partB |= (G & 0b111) << 3;
    // Adiciona o campo B
    partB |= (B & 0b111) << 6;

    *pointer_START = 0;

    *pointer_dataA = 0;
    *pointer_dataB = partB;

    *pointer_START = 1;

    return 0;
}

static void __exit exit_kernel(void)
{
    *pointer_START = 0;
    iounmap(pointer_bridge);
}

module_init(set_background_params);
module_exit(exit_kernel);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Seu Nome");
MODULE_DESCRIPTION("Módulo do kernel para configurar cores de fundo.");