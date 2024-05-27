#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/semaphore.h>

struct fake_device
{
    char data[100];
    struct semaphore sem;
} virtual_device;

struct cdev *mcdev;
int major_number;
int ret;

dev_t dev_num;

#define DEVICE_NAME "main"

int device_open(struct inode *inode, struct file *filp)
{
    if (down_interruptible(&virtual_device.sem) != 0)
    {
        printk(KERN_ALERT "main: Não foi possível tarvar o device durante a abertura");
        return -1;
    }

    printk(KERN_INFO "main: device aberto");
    return 0;
}

ssize_t device_read(struct file *filp, char *bufStoreData, size_t bufCount, loff_t *curOffset)
{
    printk(KERN_INFO "main: lendo do device");
    ret = copy_to_user(bufStoreData, virtual_device.data, bufCount);
    return ret;
}

ssize_t device_write(struct file *filp, char *bufSourceData, size_t bufCount, loff_t *curOffset)
{
    printk(KERN_INFO "main: escrevendo no device");
    ret = copy_to_user(virtual_device.data, bufSourceData, bufCount);
    return ret;


    // AS INSTRUÇÕES DEVEM ESTAR AQUI DENTRO
}

int device_close(struct inode *inode, struct file *filp)
{
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

static int driver_entry(void)
{
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

static void driver_exit(void)
{
    cdev_del(mcdev);

    unregister_chrdev_region(dev_num, 1);
    printk(KERN_ALERT "main: modulo descarregado");
}

module_init(driver_entry);
module_exit(driver_exit);