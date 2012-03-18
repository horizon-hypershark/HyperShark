gcc -o libgraph.so -shared -Wl,-soname,libnative.so -I/usr/lib/jvm/java-6-openjdk/include -I/usr/lib/jvm/java-6-openjdk/include/linux top_src_ip.c -static -lc
cp libgraph.so /usr/lib
cp FileAccess_graph.h /usr/lib
LD_LIBRARY_PATH=`pwd`
export LD_LIBRARY_PATH
