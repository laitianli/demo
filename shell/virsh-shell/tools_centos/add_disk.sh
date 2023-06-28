#!/bin/bash


FULL_NAME=$1

if [ -z "${FULL_NAME}" ];then
	echo -e "\033[31m [Error] Please input user full name!!! \033[0m"
	echo -e "\033[31m eg: $0 laishiming 50G\033[0m"
	exit 0
fi

IMAGE_SIZE=$2

if [ -z "${IMAGE_SIZE}" ];then
	echo -e "\033[31m [Error] Please input disk size!!! \033[0m"
	echo -e "\033[31m eg: $0 laishiming 50G\033[0m"
	exit 0
fi

VM_NAME=centos7.0-${FULL_NAME}
is_exist=$(virsh list --all | grep ${VM_NAME})
if [ -z "${is_exist}" ];then
	echo -e "\033[31m [Error] VM ${VM_NAME} does not exist, Please input user right name!!! \033[0m"
	exit 0
fi

QCOW2_FILE=
for file in $(virsh domblklist ${VM_NAME} | grep qcow2 | awk '{print $2}')
do
	QCOW2_FILE=${file}
	echo "QCOW2_FILE=${QCOW2_FILE}"
done
if [ -z "${QCOW2_FILE}" ];then
	 echo -e "\033[31m [Error] qcow2 is null\033[0m"
	exit 0;
fi
QCOW2_DIR=$(dirname "${QCOW2_FILE}")
#echo "qcow2 dir: ${QCOW2_DIR}"

TARGET=
for vd in $(virsh domblklist ${VM_NAME} | grep qcow2 | awk '{print $1}')
do
	TARGET=${vd}
#	echo "TARGET=${TARGET}"
done
ch=${TARGET:2}
var=$(printf %d "'${ch}")
var=$(($var+1))
ch_var=$(printf \\$(printf %o ${var}))
#TARGET=${TARGET/%${ch}/${ch_var}}
TARGET="vd${ch_var}"

#echo "var=${var} ch_var=${ch_var}, TARGET=${TARGET}"
#exit 0;
QCOW2_UUID=$(cat /proc/sys/kernel/random/uuid)
EXT_QCOW2_FILE=${QCOW2_DIR}/${VM_NAME}_${QCOW2_UUID}_${IMAGE_SIZE}.qcow2

echo "extern qcow2 file: ${EXT_QCOW2_FILE}"

qemu-img create -f qcow2 ${EXT_QCOW2_FILE} ${IMAGE_SIZE} || (echo "[Error] qemu-image create failed"; rm -rf ${EXT_QCOW2_FILE}; exit -1)

virsh attach-disk ${VM_NAME} --source ${EXT_QCOW2_FILE} --target ${TARGET} --subdriver qcow2 || (echo "[Error] virsh attach-disk failed"; rm -rf ${EXT_QCOW2_FILE}; exit -1)
