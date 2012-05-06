obj-m+= Lucid.o
Lucid-objs:= frhash.o socket_ops.o pf_kernel.o 

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules	
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean	
