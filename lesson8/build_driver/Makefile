obj-m += interrupt_driver.o

all:
	make  -C /lib/modules/$(shell uname  -r)/build M=$(PWD) modules
	rm -rf *.o *.mod.* *.cmd .module* modules* Module* .*.cmd .tmp* *.mod
clean:
	rm -rf *.o *.ko *.mod.* *.cmd .module* modules* Module* .*.cmd .tmp*
	make  -C /lib/modules/$(shell uname  -r)/build M=$(PWD) clean	