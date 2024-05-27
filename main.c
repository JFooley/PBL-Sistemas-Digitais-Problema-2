#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#include "address_map_arm.h"

#include <linux/moduleparam.h>



// Instrução
#define WBR 0b0000
#define X0 0b00000

// Máscaras para extrair os diferentes campos
#define OPCODE_MASK 0b1111 // 4 bits
#define REG_MASK 0b11111   // 5 bits

// Endereços físicos
#define DATA_A 0x80 // Registrador com opcode e endereçamentos
#define DATA_B 0x70 // Registradores de dados
#define START 0xc0  // WRREG

// Ponteiros essenciais
void __iomem *pointer_bridge;
volatile int *pointer_dataA;
volatile int *pointer_dataB;
volatile int *pointer_START;


// Criando os params:
int arrayRGB[3] = {0, 0, 0};

// Registrando:
module_param_array(arrayRGB, int, NULL, S_IRUSR | S_IWUSR);



static int set_background(void)
{

    // Mapear o endereço físico para um endereço virtual
    pointer_bridge = ioremap(LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
    if (!pointer_bridge)
    {
        printk(KERN_ERR "Erro ao mapear o endereço 0x%lx\n", (unsigned long)LW_BRIDGE_SPAN);
        return -ENOMEM;
    }

    pointer_dataA = (volatile int)(pointer_bridge + DATA_A);
    pointer_dataB = (volatile int)(pointer_bridge + DATA_B);
    pointer_START = (volatile int)(pointer_bridge + START);

    // Adiciona o campo R
    unsigned int partB = (arrayRGB[0] & 0b111) << 0;
    // Adiciona o campo G
    partB |= ((int)arrayRGB[1] & 0b111) << 3;
    // Adiciona o campo B
    partB |= ((int)arrayRGB[2] & 0b111) << 6;

    *pointer_dataA = 0;
    *pointer_dataB = partB;
    *pointer_START = 1;

    return 0;
}

static void exit_background(void)
{
    *pointer_START = 0;
    iounmap(pointer_bridge);
}

module_init(set_background);
module_exit(exit_background);