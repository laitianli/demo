#! /bin/bash

inittab_file=$1
grub_conf=$2
string="S0:2345:respawn:/sbin/agetty -L 9600 ttyS0"
#echo "inittab_file=$inittab_file"
#echo "grub_conf=$grub_conf"
#echo "string=$string"
sed -i '/tty6/ a\S0:2345:respawn:/sbin/agetty -L 115200 ttyS0' $inittab_file

sed -i "s/quiet/console=ttyS0,115200/g" $grub_conf

