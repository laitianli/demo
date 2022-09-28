#!/bin/bash
PF_NIC=enp184s0f0
VF_NIC=enp184s0f0v0
PCIeID=0000:b8:00.0

killall -2 kni

trap 'onCtrlC' INT
function onCtrlC () {
    echo 'Ctrl+C is captured'
	vf_nic=$(dpdk-devbind.py -s | grep "${VF_NIC} ");
	if [ ! -z "${vf_nic}" ];then
		ifconfig ${VF_NIC} down || exit 1
	fi
    exit 0
}


DPDK_PATH=/home/haizhi/work/dpdk/
#echo 1 > /sys/bus/pci/devices/0000:04:00.0/remove
#echo 1 > /sys/bus/pci/rescan
if [ ! -d /sys/module/uio ];then
	modprobe uio
fi

if [ ! -d /sys/module/igb_uio ];then
	insmod ${DPDK_PATH}/x86_64-native-linuxapp-gcc/kmod/igb_uio.ko || exit 1
fi


if [ ! -d /sys/module/rte_kni ];then
	insmod ${DPDK_PATH}/x86_64-native-linuxapp-gcc/kmod/rte_kni.ko || exit 1
fi
pf_nic=$(dpdk-devbind.py -s | grep "${PF_NIC} ");
if [ ! -z "${pf_nic}" ];then
	ifconfig ${PF_NIC} down || exit 1
fi

BIND_RES=$(dpdk-devbind.py -s | grep ${PCIeID} | grep "drv=igb_uio ")
if [ -z "${BIND_RES}" ];then
	dpdk-devbind.py -b igb_uio ${PCIeID} || exit 1
fi
echo -e "\033[31m l2fwd_path=${DPDK_PATH}/examples/kni/build/kni \033[0m"
i=0
while [[ 1 ]]
do
	echo 0 > /sys/bus/pci/devices/${PCIeID}/max_vfs 
	sleep 1
	echo 1 > /sys/bus/pci/devices/${PCIeID}/max_vfs 
	echo "[Note]wait 3 second...."
	sleep 3
	${DPDK_PATH}/examples/kni/build/kni --file-prefix kni -w ${PCIeID} -l 4-6 -- -P -p 0x1 -m --config="(0,4,5,6)" &
	sleep 10
	vf_nic=$(dpdk-devbind.py -s | grep "${VF_NIC} ");
	if [ ! -z "${vf_nic}" ];then
		break;
	else
		((i++));
		killall -2 kni
		sleep 1;
	fi

	echo -e "\033[032m [Note] reset max_vfs count: $i \033[0m"
done
ifconfig ${VF_NIC} 21.21.21.3/24
ifconfig vEth0_0 31.31.31.3/24 up
echo -e "\033[032m [Note] create VF nic success! \033[0m"
while [[ 1 ]]
do
	sleep 1
done
#echo "0x1957 0x8d80" > /sys/bus/pci/drivers/igb_uio/new_id


