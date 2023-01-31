#!/bin/bash


cpuid=1

for pid in `pidof stress`
do
    cpuid=$(expr $cpuid + 1)
    echo "pid=$pid, cpuid=$cpuid"
    taskset -cp $cpuid $pid
done
