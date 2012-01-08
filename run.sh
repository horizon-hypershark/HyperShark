cd PF_RING-5.1.0/kernel/
make 
make install
insmod pf_ring.ko
cp Module.symvers ../../pf_kernel/
cd ../../pf_kernel/
make
insmod pf_kernel.ko
