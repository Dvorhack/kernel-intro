MOD = module2
KPATH :=../../linux-6.9.2/
PWD :=$(shell pwd)
obj-m = $(MOD).o

all:
	$(MAKE) -C $(KPATH) M=$(PWD) modules
	rm .*.cmd *.order *.mod* *.symvers

clean:
	$(MAKE) -C $(KPATH) M=$(PWD) clean