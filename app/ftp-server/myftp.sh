#!/bin/bash
if [ $# -ne 1 ]
then
	echo "promte:$0--[start]--[stop]--[restart]"
elif [ $1 = "start" ]
then
	./myftp
	echo "start-up [myftp] ........................[success] "
elif [ $1 = "stop" ]
then
	kill -3 `pidof myftp`
	echo "close [myftp] ............................[success] "
elif [ $1 = "restart" ]
then 
	kill -3 `pidof myftp`
	echo "close [myftp] ............................[success] "
	sleep 1
	./myftp
	echo "start-up [myftp] ........................[success] "
fi

exit 0
