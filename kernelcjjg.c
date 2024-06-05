#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <asm/io.h>
#include "address_map_arm.h"
#include "instructions.c"

#define DEVICE_NAME "gpucjjg"

static int major;
static unsigned char device_buffer[8];
static int device_open = 0;

// Endereços físicos
#define DATA_A 0x80 // Registrador com opcode e endereçamentos
#define DATA_B 0x70 // Registradores de dados
#define START 0xc0  // WRREG

// Ponteiros essenciais
volatile int *pointer_dataA;
volatile int *pointer_dataB;
volatile int *pointer_START;
void *pointer_bridge;


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

int charArrayToInt(const char *array) {
    uint64_t result = 0;

    // Itera sobre os chars do array
    int i;
    for (i = 0; i < 8; i++) {
        // Adiciona 8 bits do char atual ao resultado
        result |= ((uint64_t)array[i] << (i * 8));
    }

    return result;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {

    *pointer_dataA = 0;
    *pointer_dataB = 0;
    printk(KERN_INFO "Data A reset: %d\n", *pointer_dataA);
    printk(KERN_INFO "Data B reset: %d\n", *pointer_dataB);
    // Copiar os dados do buffer do usuário para o buffer do dispositivo
    if (copy_from_user(device_buffer, buffer, len) != 0) {
        printk(KERN_ERR "Falha ao copiar dados do buffer de usuário para o buffer do dispositivo\n");
        return -EFAULT;
    }

    // Debug
    int i;
    printk(KERN_INFO "Valores em data:\n");
    for (i = 0; i < 8; i++) {
        printk(KERN_INFO "%u ", device_buffer[i]);
    }
    printk(KERN_INFO "\n");
    printk(KERN_INFO "\n");

    // Recuper a instrucao
    unsigned int word = charArrayToInt(device_buffer);

    // Mapear o endereço físico para um endereço virtual
    pointer_bridge = ioremap(LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
    pointer_dataA = (volatile int *)(pointer_bridge + DATA_A);
    pointer_dataB = (volatile int *)(pointer_bridge + DATA_B);
    pointer_START = (volatile int *)(pointer_bridge + START);

    unsigned int *dataBus[2];
    *dataBus = WBR_BG(word);

    // Insere no barramento
    *pointer_dataA = dataBus[1];
    *pointer_dataB = dataBus[0];

    *pointer_START = 0; 
    *pointer_START = 1;
    *pointer_START = 0; 

    // Limpa o buffer do dispositivo
    memset(device_buffer, 0, sizeof(device_buffer));
    memset(buffer, 0, sizeof(buffer));

    // Debug
    printk(KERN_INFO "Data A: %d\n", *pointer_dataA);
    printk(KERN_INFO "Data B: %d\n", *pointer_dataB);
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
    printk(KERN_INFO "Dispositivo registrado com sucesso com o número major %d. Para interagir com o driver, crie um arquivo de dispositivo com 'mknod /dev/%s c %d 0'.\n", major, DEVICE_NAME, major);
    return 0;
}

static void __exit driver_gpu_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Driver removido com sucesso!\n");
}

module_init(driver_gpu_init);
module_exit(driver_gpu_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Seu Nome");
MODULE_DESCRIPTION("Um driver simples de caractere");
MODULE_VERSION("0.1");
