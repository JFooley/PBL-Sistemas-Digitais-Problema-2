obj-m += kernelcjjg.o
KERNEL_DIR = /usr/src/$(shell uname -r)
MODULE_NAME = kernelcjjg
DEVICE_NAME = gpucjjg

all:
    $(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules
    insmod $(MODULE_NAME).ko

clean:
    rm -rf *.o *.ko *.mod.* *.symvers *.order *~
    rm -f /dev/$(DEVICE_NAME)