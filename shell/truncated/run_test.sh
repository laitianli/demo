#!/bin/bash
val=0
while [[ 1 ]];
do
    echo "val=$val"
#    $((val+1))
    val=$(expr $val + 1)
    sleep 1
done

