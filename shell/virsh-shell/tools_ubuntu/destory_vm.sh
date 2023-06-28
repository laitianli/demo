#!/bin/bash

FULL_NAME=$1
IMG_PATH=/var/lib/libvirt/qcow2_image_ubuntu/
VM_NAME=Ubuntu18.04-${FULL_NAME}

if [ -z "${FULL_NAME}" ];then
	echo -e "\033[31m [Error] Please input user full name!!! \033[0m"
	echo -e "\033[31m eg: $0 laishiming \033[0m"
	exit 0
fi

#check vm exist
result=`virsh list --all | grep ${VM_NAME}`
if [ -z "${result}" ];then
	echo -e "\033[31m [Error] VM ${VM_NAME} doesn't exist!!!\033[0m"
	exit -1;
fi

VM_STATUS=`virsh list --all | grep "${VM_NAME}" | awk '{print $3}'`
echo "[Shell note] shutdown ${VM_NAME} ..."
if [ "${VM_STATUS}" == "running" ];then
	virsh shutdown ${VM_NAME}
fi

echo "[Shell note] begin delete ${VM_NAME} ..."
if [ -f "${IMG_PATH}/${VM_NAME}.qcow2" ];then
	virsh undefine ${VM_NAME}
	rm -rf ${IMG_PATH}/${VM_NAME}.qcow2
fi


