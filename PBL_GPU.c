#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/types.h>

// Instrução
#define WBR 0b0000
#define X0 0b00000

// Máscaras para extrair os diferentes campos
#define OPCODE_MASK 0b1111 // 4 bits
#define REG_MASK 0b11111   // 5 bits

// Endereços físicos
#define DATA_A 0x80
#define DATA_B 0x70
#define START 0xc0 // WRREG
#define HEAVY_BRIDGE 0xfc000000
#define HEAVY_BRIDGE_SIZE 0x00005000

int set_background(unsigned int R, unsigned int G, unsigned int B)
{   
    // Ponteiros essenciais
    volatile uint64_t *pointer_dataA;
    volatile uint64_t *pointer_dataB;
    volatile uint64_t *pointer_START;
    void __iomem *pointer_bridge;

    // Mapear o endereço físico para um endereço virtual
    pointer_bridge = ioremap(HEAVY_BRIDGE, HEAVY_BRIDGE_SIZE);
    if (!pointer_bridge) {
        printk(KERN_ERR "Erro ao mapear o endereço 0x%lx\n", (unsigned long)HEAVY_BRIDGE);
        return -ENOMEM;
    }

    pointer_dataA = (volatile uint64_t)(pointer_bridge + DATA_A);
    pointer_dataB = (volatile uint64_t)(pointer_bridge + DATA_B);
    pointer_START = (volatile uint64_t)(pointer_bridge + START);

    uint64_t partA = 0;
    uint64_t partB = 0;

    // Adiciona o campo opcode
    partA |= ((uint64_t)WBR & OPCODE_MASK) << 0;

    // Adiciona o campo registrador
    partA |= ((uint64_t)X0 & REG_MASK) << 4;

    // Adiciona o campo R
    partB |= ((uint64_t)R & 0b111) << 0; 

    // Adiciona o campo G
    partB |= ((uint64_t)G & 0b111) << 3; 

    // Adiciona o campo B
    partB |= ((uint64_t)B & 0b111) << 6; 

    // Escrever nos endereços mapeados
    writeq(partA, pointer_dataA);
    writeq(partB, pointer_dataB);
    writeq(1, pointer_START); // Autoriza a leitura
    writeq(0, pointer_START); // Desautoriza logo em seguida (Isso precisa ser sincronizado com o clock da placa?)

    // Desmapear o endereço
    iounmap(pointer_bridge);

    return 0;
}

// Aqui vai ser adicionado todas as outras funções, é preciso ver como faz pra chamar elas separadamente

MODULE_LICENSE("GPL");
MODULE_AUTHOR("João Gabriel, Gabriel Moraes, Caleo Silva e João Pedro");
MODULE_DESCRIPTION("Modulo kernel de escrita da placa de video");