obj-m += chartest.o hello.o

SRC := $(shell pwd)
all:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules
modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules_install
clean:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) clean 
	rm -f *.o *.ko *.mod.c *.mod *.history modules.order Module.symvers .*.cmd
	rm -rf .tmp_versionssou	
