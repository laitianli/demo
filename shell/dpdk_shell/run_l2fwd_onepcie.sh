#!/bin/bash 
DPDK_PATH=/home/haizhi/work/dpdk/
#echo 1 > /sys/bus/pci/rescan
modprobe uio
insmod ${DPDK_PATH}/x86_64-native-linuxapp-gcc/kmod/igb_uio.ko
lspci -D | grep "Freescale"
ifconfig enp184s0f2 down
ifconfig enp184s0f0 down

PCIeID=0000:b8:00.0
dpdk-devbind.py -b igb_uio ${PCIeID}

echo -e "\033[31m l2fwd_path=${DPDK_PATH}/examples/l2fwd/build/l2fwd \033[0m"
${DPDK_PATH}/examples/l2fwd/build/l2fwd -c 0x3 -n 4 -w 0000:b8:00.0 -- -p 1 -q 1 -T 1000 -b 32 --no-mac-updating




