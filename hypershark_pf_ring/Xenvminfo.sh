xm list | awk 'NR!=1 {printf"%10s %10s  ",$2,$1;cmd="echo `virsh domuuid "$1"` | cut -d'-' -f1,2,3,4,5 --output-delimiter=''";system(cmd)}'

insmod ../PF_RING-5.1.0/kernel/pf_ring.ko

insmod HyperShark.ko

./Daemon /media/STORAGE/storage/hs1234/
