gcc -o libcapfilter.so -shared -Wl,-soname,libnative.so -I/usr/lib/jvm/java-6-openjdk/include -I/usr/lib/jvm/java-6-openjdk/include/linux capfilter.c -static -lc
cp Core_capfilter.h /usr/lib
cp libcapfilter.so /usr/lib
LD_LIBRARY_PATH=`pwd`
export LD_LIBRARY_PATH
