obj-m+= HyperShark.o
HyperShark-objs:= frhash.o afcpature1jan.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
