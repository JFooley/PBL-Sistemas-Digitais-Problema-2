#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/interrupt.h>
#include <linux/io.h>
#include <asm/io.h>

#include "address_map_arm.h"
#include <linux/types.h>

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
volatile uint64_t *pointer_dataA;
volatile uint64_t *pointer_dataB;
volatile uint64_t *pointer_START;

static int __init set_background(unsigned int R, unsigned int G, unsigned int B)
{

    // Mapear o endereço físico para um endereço virtual
    pointer_bridge = ioremap(LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
    if (!pointer_bridge)
    {
        printk(KERN_ERR "Erro ao mapear o endereço 0x%lx\n", (unsigned long)LW_BRIDGE_SPAN);
        return -ENOMEM;
    }

    pointer_dataA = (volatile uint64_t)(pointer_bridge + DATA_A);
    pointer_dataB = (volatile uint64_t)(pointer_bridge + DATA_B);
    pointer_START = (volatile uint64_t)(pointer_bridge + START);

    // Adiciona o campo R
    partB |= ((uint64_t)R & 0b111) << 0;
    // Adiciona o campo G
    partB |= ((uint64_t)G & 0b111) << 3;
    // Adiciona o campo B
    partB |= ((uint64_t)B & 0b111) << 6;

    *pointer_dataA = 0;
    *pointer_dataB = partB;
    *pointer_START = 1;

    // Desmapear o endereço
    // iounmap(pointer_bridge);

    return 0;
}

static void __exit exit_background(void)
{
    *pointer_START = 0;
    iounmap(pointer_bridge);
}

// Aqui vai ser adicionado todas as outras funções, é preciso ver como faz pra chamar elas separadamente
MODULE_LICENSE("GPL");
MODULE_AUTHOR("João Gabriel, Gabriel Moraes, Caleo Silva e João Pedro");
MODULE_DESCRIPTION("Modulo kernel de escrita da placa de video");

module_init(set_background(0, 0, 7));
module_exit(set_background(0, 0, 7));