gcc -o libvminfo.so -shared -Wl,-soname,libnative.so -I/usr/lib/jvm/java-6-openjdk/include -I/usr/lib/jvm/java-6-openjdk/include/linux read_vm_details.c -static -lc
cp libvminfo.so /usr/lib
cp FileAccess_VMInfo.h /usr/lib
LD_LIBRARY_PATH=`pwd`
export LD_LIBRARY_PATH
