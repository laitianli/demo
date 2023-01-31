#!/bin/bash

scan_ip_mac()
{
	i=1
	max=255
	timeout=60
	while [ $i -le $max ]
	do
		ip="$subaddr.$i"
		echo -n "ip: $ip  "
		mac=$(nmap -sn $ip --host-timeout ${timeout}s |grep "MAC Address: ")
		if [ -z "$mac" ];then
			echo "   -------outline--------"
		else
			echo "-online, $mac"
		fi
		((i++))
	done
}
echo ==============================$(date)================================================================
subaddr=10.88.20
scan_ip_mac $subaddr
echo ==============================$(date)================================================================
subaddr=10.88.30
scan_ip_mac $subaddr
echo ==============================$(date)================================================================
subaddr=10.88.10
scan_ip_mac $subaddr
