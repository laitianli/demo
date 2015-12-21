#!/bin/bash
insmod `pwd`/module/cp.ko

mknod /dev/cp-info c 199 0

sleep 1

`pwd`/app/cp-app

