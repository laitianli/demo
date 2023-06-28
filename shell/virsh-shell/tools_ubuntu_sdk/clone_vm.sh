#!/bin/bash


FULL_NAME=$1

if [ -z "${FULL_NAME}" ];then
	echo -e "\033[31m [Error] Please input user full name!!! \033[0m"
	echo -e "\033[31m eg: $0 laishiming \033[0m"
	exit 0
fi

BASE_VM_NAME=Ubuntu18.04-SDK-basic
IMAGE_PATH=/var/lib/libvirt/qcow2_image_ubuntu_sdk/
VM_NAME=Ubuntu18.04-SDK-${FULL_NAME}

if [ ! -d "${IMAGE_PATH}" ];then
	mkdir -p ${IMAGE_PATH}
fi

#check vm exist
result=`virsh list --all | grep ${VM_NAME}`
if [ ! -z "${result}" ];then
	echo -e "\033[31m [Error] VM ${VM_NAME} has exist!!!\033[0m"
	exit -1;
fi

#clone vm
virt-clone --original ${BASE_VM_NAME}  --name  ${VM_NAME}  --file ${IMAGE_PATH}/${VM_NAME}.qcow2


virsh  autostart ${VM_NAME}
