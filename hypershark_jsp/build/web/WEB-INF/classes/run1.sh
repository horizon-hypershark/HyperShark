gcc -o libpacketlib.so -shared -Wl,-soname,libnative.so -I/usr/lib/jvm/java-6-openjdk/include -I/usr/lib/jvm/java-6-openjdk/include/linux func.c ret_query.c bitmap.c filelist.c -static -lc
cp FileAccess_GetPacket.h /usr/lib
cp libpacketlib.so /usr/lib
LD_LIBRARY_PATH=`pwd`
export LD_LIBRARY_PATH
