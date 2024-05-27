#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/semaphore.h>
#include "instructions.h"

struct fake_device
{
    char data[100];
    struct semaphore sem;
} virtual_device;

struct cdev *mcdev;
int major_number;
int ret;

dev_t dev_num;

#define DEVICE_NAME "GPU_CJJG"

int device_open(struct inode *inode, struct file *filp)
{
    if (down_interruptible(&virtual_device.sem) != 0)
    {
        printk(KERN_ALERT "GPU_CJJG: Não foi possível tarvar o device durante a abertura");
        return -1;
    }

    printk(KERN_INFO "GPU_CJJG: device aberto");
    return 0;
}

ssize_t device_read(struct file *filp, char *bufStoreData, size_t bufCount, loff_t *curOffset) {
    printk(KERN_INFO "main: lendo do device");
    ret = copy_to_user(bufStoreData, virtual_device.data, bufCount);
    return ret;
}

ssize_t device_write(struct file *filp, char *bufSourceData, size_t bufCount, loff_t *curOffset)
{
    printk(KERN_INFO "GPU_CJJG: Intrucao recebida");
    ret = copy_to_user(virtual_device.data, bufSourceData, bufCount);

    ////////////////////////////////////////////// Aqui é novo
    volatile int instruction;
    instruction = (int)virtual_device.data;

    int mask = 0b00001111; //Mascara para separar o opcode
    int opcode = vi & mask; //Opcode
    int dataBus[2];

    switch (opcode)
    {
    case 0b0000:
        datasBus = instruction_WBR(instruction); //Funcao no arquivo instructions.c que retorna o dataA e dataB dessa instrução em um vetor
        break;
    
    case 0b0001:
        /* INTRUÇÃO WSM */
        break;

    case 0b0010:
        /* INTRUÇÃO WBM */
        break;

    case 0b0011:
        /* INTRUÇÃO DP: */
        break;
    default:

        printk(KERN_INFO "GPU_CJJG: opcode incorreto");
        break;
    }

    /* ESSA PARTE PEGA OS VALORES SETADOS NAS PARTES E MANDA PRA GPU*/
    // Ponteiros essenciais
    void *pointer_bridge;

    // Mapear o endereço físico para um endereço virtual
    pointer_bridge = ioremap(LW_BRIDGE_BASE, LW_BRIDGE_SPAN);

    pointer_dataA = (volatile int *)(pointer_bridge + DATA_A);
    pointer_dataB = (volatile int *)(pointer_bridge + DATA_B);
    pointer_START = (volatile int *)(pointer_bridge + START);

    *pointer_START = 0;
    *pointer_dataA = dataBus[0]; //Valor de dataA retornado pela função em outro arquivo
    *pointer_dataB = dataBus[1]; //Valor de dataB retornado pela função em outro arquivo
    *pointer_START = 1;

    return ret;
}

int device_close(struct inode *inode, struct file *filp) {
    up(&virtual_device.sem);
    printk(KERN_INFO "main: device fechado");
    return 0;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_close,
    .write = device_write,
    .read = device_read};

static int driver_entry(void) {
    // Passo 1: Usando alocação dinamica para assign nosso device
    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret < 0)
    {
        printk(KERN_ALERT "main: Falha ao criar o major number");
        return ret;
    }
    major_number = MAJOR(dev_num);
    printk(KERN_INFO "main: major number is %d", major_number);
    printk(KERN_INFO "main: \tuse \"mknod%s c %d 0\" for device file", DEVICE_NAME, major_number);

    // Passo 2:
    mcdev = cdev_alloc();
    mcdev->ops = &fops;

    ret = cdev_add(mcdev, dev_num, 1);
    if (ret < 0)
    {
        printk(KERN_ALERT "main: Falha ao criar o major number");
        return ret;
    }

    return 0;
}

static void driver_exit(void) {
    cdev_del(mcdev);

    unregister_chrdev_region(dev_num, 1);
    printk(KERN_ALERT "main: modulo descarregado");
}

module_init(driver_entry);
module_exit(driver_exit);