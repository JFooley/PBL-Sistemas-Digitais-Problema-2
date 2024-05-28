#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "DRIVERMORAES"
#define BUFFER_SIZE_BITS 8 // 8 bytes * 8 bits por byte TOTAL DE 64 BITS

static int major;
static unsigned int device_buffer[BUFFER_SIZE_BITS / (sizeof(unsigned int) * 8)];
static int device_open = 0;

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

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    // Convertendo o tamanho do buffer de bits para bytes
    size_t buffer_size_bytes = BUFFER_SIZE_BITS / 8;

    // Verificando se o tamanho solicitado não ultrapassa o tamanho do buffer
    if (len > buffer_size_bytes) {
        len = buffer_size_bytes;
    }

    // Copiando dados do buffer de bits para o buffer do usuário
    if (copy_to_user(buffer, device_buffer, len)) {
        return -EFAULT;
    }

    //Iterando sobre o buffer e printando os dados recebidos
    for (int i = 0; i < len; i++) {
        printk(KERN_INFO "Byte[%d] = 0x%X\n", i, device_buffer[i]);
    }
    
    printk(KERN_INFO "Dados lidos do dispositivo!\n");
    return len;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    // Convertendo o tamanho do buffer de bits para bytes
    size_t buffer_size_bytes = BUFFER_SIZE_BITS / 8;

    // Verificando se o tamanho solicitado não ultrapassa o tamanho do buffer
    if (len > buffer_size_bytes) {
        len = buffer_size_bytes;
    }

    // Copiando dados do buffer do usuário para o buffer de bits
    if (copy_from_user(device_buffer, buffer, len)) {
        return -EFAULT;
    }

    printk(KERN_INFO "Dados escritos no dispositivo!\n");
    return len;
}

static struct file_operations fops = {
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write,
};

static int __init moraes_driver_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "Falha ao registrar o dispositivo com o número major %d\n", major);
        return major;
    }
    printk(KERN_INFO "Dispositivo registrado com sucesso com o número major %d. Para interagir com o driver, crie um arquivo de dispositivo com 'mknod /dev/%s c %d 0'.\n", major, DEVICE_NAME, major);
    return 0;
}

static void __exit moraes_driver_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Driver removido com sucesso!\n");
}

module_init(moraes_driver_init);
module_exit(moraes_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Seu Nome");
MODULE_DESCRIPTION("Um driver simples de caractere");
MODULE_VERSION("0.1");
