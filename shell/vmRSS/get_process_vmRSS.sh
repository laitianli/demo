#!/bin/bash

process_array=("upapp" "/usr/sbin/smbd" "odsNameServer")

echo "size: ${#process_array[*]} ${process_array[*]}"

main()
{
    i=0
    sz=${#process_array[*]}
    while ((i<sz))
    do
        pro_name=`ps -aux | grep " ${process_array[i]}" | grep -v "grep"`
        echo "$proc_name"
        if [ -z "${pro_name}" ];then
            echo "[Error] process \"${process_array[i]}\" is not exist!!!"
            ((i++))
            continue
        fi

        grp_pid=`ps -aux | grep " ${process_array[i]}" | grep -v "grep" | awk '{print $2}'`
        for pid in ${grp_pid} 
        do
            VmRSS=`cat /proc/${pid}/status | grep VmRSS`
            printf "%-40s %20s\n" " ${process_array[i]}" "${VmRSS}"
        done
        ((i++))
    done
}


main;


