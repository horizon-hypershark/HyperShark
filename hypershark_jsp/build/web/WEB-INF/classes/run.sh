gcc -o libstatus.so -shared -Wl,-soname,libnative.so -I/usr/lib/jvm/java-6-openjdk/include -I/usr/lib/jvm/java-6-openjdk/include/linux changeStatus.c -static -lc
cp libstatus.so /usr/lib
cp FileAccess_ChangeStatus.h /usr/lib
LD_LIBRARY_PATH=`pwd`
export LD_LIBRARY_PATH
