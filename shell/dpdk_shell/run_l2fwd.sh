#!/bin/bash 
DPDK_PATH=/home/haizhi/work/dpdk/
#DPDK_PATH=/home/haizhi/work/dpdk_2.0/
GV_PCIE_0="0000:65:00.0"
GV_PCIE_1="0000:65:00.1"
echo 1 > /sys/bus/pci/devices/${GV_PCIE_0}/remove
echo 1 > /sys/bus/pci/devices/${GV_PCIE_1}/remove
echo 1 > /sys/bus/pci/rescan
modprobe uio
insmod ${DPDK_PATH}/x86_64-native-linuxapp-gcc/kmod/igb_uio.ko
echo "0x1957 0x8d80" > /sys/bus/pci/drivers/igb_uio/new_id
lspci -D | grep "Freescale"
ifconfig enp184s0f2 down
dpdk-devbind.py -b igb_uio 0000:b8:00.2
echo -e "\033[31m l2fwd_path=${DPDK_PATH}/examples/l2fwd/build/l2fwd \033[0m"
${DPDK_PATH}/examples/l2fwd/build/l2fwd -c 0x3 -n 4 -w ${GV_PCIE_0} -w 0000:b8:00.2 -- -p 3 -q 1 -T 1 -b 32 --no-mac-updating



