obj-m += kernelcjjg.o
KERNEL_DIR = /usr/src/$(shell uname -r)
all:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules
clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order *~