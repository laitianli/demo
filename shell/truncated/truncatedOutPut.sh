#!/usr/bin/bash
#usage: ls -al | sh ./truncatedOutPut.sh /tmp/out

maxLines=500000
maxFiles=50
file=$1
line=0
fileNum=1
beginTime=`date +%Y%m%d.%H%M%S`
> $file
rm -rf /tmp/fullTruncatedOutPut
while true; do
  while [[ -f /tmp/fullTruncatedOutPut ]]
  do
    fileNum=0
    sleep 1
  done
  read -t 3 output
  if [ -z "$output" ]; then
    sleep 5
    continue
  fi
  #line_num=`wc -l $file | awk '{print $1}'`
  #if [[ $line_num -ge $maxLines ]];then
  if [[ $line -ge $maxLines ]];then
    line=0
    timeBuffer=`date +%Y%m%d.%H%M%S`
    mv $file $file"."$beginTime"-"$timeBuffer".log"
    beginTime=$timeBuffer
    > $file
    if [[ $fileNum -ne $maxFiles ]];then
      fileNum=`expr $fileNum + 1`
    else
      rm -rf $file
      > /tmp/fullTruncatedOutPut
      continue
    fi
  fi
  echo "`date "+%Y/%m/%d %Z %H:%M:%S.%6N"` $output" >> $file
  line=`expr $line + 1`
  
  lostStr=`echo $output | grep "sudden change of timing index detected"`
  recoveryStr=`echo $output | grep "BBU timing recovery state finished at global sf"`
  
  if [ -n $lostStr ];then
    echo "" > /tmp/phyLostTiming
  elif [ -n $recoveryStr ];then
    echo `date +%s` > /tmp/phyLostTiming
  fi

done

