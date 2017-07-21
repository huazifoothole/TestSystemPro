#!/bin/bash
count=$(df | grep "/dev/sd" | wc -l)

if [[ $count < 3 ]];then
exit
fi

disk1=$1
disk2=$2

echo $disk1
echo $disk2


for ((i=1;i<=$count;i++))
do 
disk3=$(df |grep "/dev/sd"|awk -F" " 'NR=='$i' {printf $1}'| cut -b 1-8) 
if [[ $disk3 != $disk1 && $disk3 != $disk2 ]];then
echo $disk3
break
fi
done


