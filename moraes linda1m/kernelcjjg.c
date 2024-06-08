#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <asm/io.h>
#include "address_map_arm.h"

#define DEVICE_NAME "gpucjjg"

static int major;
static unsigned long long device_buffer;
static int device_open = 0;

// Ponteiros essenciais
volatile int *pointer_dataA;
volatile int *pointer_dataB;
volatile int *pointer_START;
void *pointer_bridge;

void printBits(unsigned long long num) {
    // Determina o tamanho do inteiro em bits
    int size = 64;

    // Loop através de cada bit do número
    int i;
    for (i = size - 1; i >= 0; i--) {
        // Verifica se o bit na posição i é 1 ou 0
        if (num & (1 << i))
            printk(KERN_INFO "1");

        else
            printk(KERN_INFO "0");


        // Adiciona espaçamento para melhor visualização
        if (i % 8 == 0)
            printk(KERN_INFO " ");

    }
    printf("\n");
}

void createNode(const char *device_name, int major) {
    char comando[100];
    // Formatar o comando mknod com os parâmetros fornecidos
    sprintf(comando, "mknod /dev/%s c %d 0", device_name, major)
    
    // Executar o comando mknod
    int status = system(comando);
    
    if (status == 0) {
        printf("No de dispositivo %s criado com sucesso.\n", device_name);
    } else {
        printf("Erro ao criar o dispositivo %s.\n", device_name);
    }
}

void removeNode(const char *device_name) {
    char comando[100];
    // Construir o comando rm manualmente
    sprintf(comando, "rm /dev/%s", device_name);

    // Executar o comando rm
    int status = system(comando);

    if (status == 0) {
        printf("No %s removido com sucesso.\n", device_name);
    } else {
        printf("Erro ao remover o no %s.\n", device_name);
    }
}

static int dev_open(struct inode *inodep, struct file *filep) {
    if (device_open) {
        return -EBUSY;
    }
    device_open++;
    try_module_get(THIS_MODULE);
    printk(KERN_INFO "Dispositivo aberto com sucesso!\n");
    return 0;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    device_open--;
    module_put(THIS_MODULE);
    printk(KERN_INFO "Dispositivo fechado com sucesso!\n");
    return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {

    // Copiar os dados do buffer do usuário para o buffer do dispositivo
    if (copy_from_user(device_buffer, buffer, len) != 0) {
        printk(KERN_ERR "Falha ao copiar dados do buffer de usuário para o buffer do dispositivo\n");
        return -EFAULT;
    }    
   	
    printk(KERN_INFO "\n");
    printk(KERN_INFO "\n");

    // Recuper a instrucao
    printk(KERN_INFO "VALOR DA INSTRUCAO:\n");
    printBits(device_buffer);

    pointer_dataA = (volatile int *)(pointer_bridge + DATA_A);
    pointer_dataB = (volatile int *)(pointer_bridge + DATA_B);
    pointer_START = (volatile int *)(pointer_bridge + START);

    unsigned long long dataA = 4;
    unsigned long long dataB = 0;

    switch (device_buffer & 0b1111)
    {
    case 0b0000:
        dataA = device_buffer & ((1 << 9) - 1);
        dataB = device_buffer >> 9;
        break;

    case 0b0001:
        dataA = device_buffer & ((1 << 19) - 1);
        dataB = device_buffer >> 19;
        break;

    case 0b0010:
        dataA = device_buffer & ((1 << 17) - 1);
        dataB = device_buffer >> 17;
        break;
        
    case 0b0011:
        dataA = device_buffer & ((1 << 9) - 1);
        dataB = device_buffer >> 9;
        break;

    default:
        printk(KERN_ERR "kernelcjjg: Instrução não reoconhecida");
        break;
    }
    
    printk(KERN_INFO "dataBus[0]: %d\n", dataA);
    printk(KERN_INFO "dataBus[1]: %d\n", dataB);

    // Insere no barramento
    *pointer_dataA = dataA;
    *pointer_dataB = dataB;
    
    printk(KERN_INFO "*pointer_dataA: %lu\n", *pointer_dataA);
    printk(KERN_INFO "*pointer_dataB: %lu\n", *pointer_dataB);

    // Autoriza a inserção nas FIFOs
    *pointer_START = 1;
    *pointer_START = 0; 

    // Limpa o buffer do dispositivo
    memset(buffer, 0, sizeof(buffer));

    // Debug
    printk(KERN_INFO "Dados escritos no dispositivo!\n");
    return len;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    // Verificar se o tamanho do buffer de saída é o esperado
    if (len != sizeof(device_buffer)) {
        printk(KERN_ERR "Tamanho do buffer de saída incorreto\n");
        return -EINVAL;
    }

    // Copiar os dados do buffer do dispositivo para o buffer do usuário
    if (copy_to_user(buffer, device_buffer, len) != 0) {
        printk(KERN_ERR "Falha ao copiar dados do buffer do dispositivo para o buffer do usuário\n");
        return -EFAULT;
    }

    printk(KERN_INFO "Dados lidos do dispositivo!\n");
    return len;
}

static struct file_operations fops = {
    .open = dev_open,
    .release = dev_release,
    .write = dev_write,
    .read = dev_read,
};

static int __init driver_gpu_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "Falha ao registrar o dispositivo com o número major %d\n", major);
        return major;
    }
    printk(KERN_INFO "Dispositivo registrado com sucesso.\n");
    
    // Mapear o endereço físico para um endereço virtual
    pointer_bridge = ioremap(LW_BRIDGE_BASE, LW_BRIDGE_SPAN);

    printk(KERN_INFO "'mknod /dev/%s c %d 0' executado.\n");
    
    createNode(DEVICE_NAME, major);

    return 0;
}

static void __exit driver_gpu_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Driver removido com sucesso!\n");
    removeNode(DEVICE_NAME);
}

module_init(driver_gpu_init);
module_exit(driver_gpu_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Seu Nome");
MODULE_DESCRIPTION("Um driver simples de caractere");
MODULE_VERSION("0.1");
