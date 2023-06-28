#!/bin/bash

arr_vm_name=`virsh list --all | grep "centos7.0-" | awk '{print $2}'`
arr_vm_status=`virsh list --all | grep "centos7.0-" | awk '{print $3}'`
echo "arr_vm_name=${arr_vm_name}"
echo "arr[1]=${#arr_vm_name[0]}"
if [ -z "${arr_vm_name}" ];then
	echo -e "\033[31m [Error] VM ${VM_NAME} doesn't exist!!!\033[0m"
	exit -1;
fi


