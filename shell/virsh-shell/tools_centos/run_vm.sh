#!/bin/bash

FULL_NAME=$1
VM_NAME=centos7.0-${FULL_NAME}
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
if [ "${VM_STATUS}" == "running" ];then
	echo "[Shell note] VM ${VM_NAME} is running!"
else
	echo "[Shell note] start ${VM_NAME} ..."
	virsh start ${VM_NAME} 
fi
