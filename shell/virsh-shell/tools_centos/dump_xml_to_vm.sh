#!/bin/bash

name=$1

if [ -z "$name" ];then
	echo "username is null, please input"
	exit -1;
fi

xml_dir=/etc/libvirt/qemu/
image_dir=/mnt/vm-image/qcow2_image
basic_dir=/var/lib/libvirt/images
qcow2_image=${image_dir}/centos7.0-${name}.qcow2
basic_qcow2_path=${basic_dir}/centos7.0-basic.qcow2

if [ ! -f "${qcow2_image}" ];then
	echo "file: ${qcow2_image} is not exist!!!"
	exit -1;
fi

xml_basic_file=${xml_dir}/centos7.0-basic.xml
if [ ! -f "${xml_basic_file}" ];then
	echo "file: ${xml_basic_file} is not exist !!!"
	exit -1;
fi

xml_file=${xml_dir}/centos7.0-${name}.xml
if [ ! -f "xml_file" ];then
	cp ${xml_dir}/centos7.0-basic.xml ${xml_dir}/centos7.0-${name}.xml
fi

new_name=centos7.0-${name}
sed -i "s/<name>centos7.0-basic</<name>$new_name</" ${xml_file}

NEW_UUID=`cat /proc/sys/kernel/random/uuid`
echo "NEW_UUID=$NEW_UUID"
sed -i "s/<uuid>.*<\/uuid>/<uuid>$NEW_UUID<\/uuid>/" ${xml_file}

echo "basic_qcow2_path=$basic_qcow2_path, qcow2_image=${qcow2_image}"
sed -i "s@$basic_qcow2_path@$qcow2_image@" ${xml_file}
#sed -e "s/\/var\/lib\/libvirt\/images\/centos7.0-basic.qcow2/\/mnt\/vm-image\/qcow2_image/centos7.0-$name.qcow2/" ${xml_file}

echo "create xml file: ${xml_file} success"


virsh define ${xml_file}
virsh autostart centos7.0-${name}
echo "please change mac...."
virsh edit centos7.0-${name}
