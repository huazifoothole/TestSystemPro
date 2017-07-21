#!/bin/bash
count=$(df | grep "/dev/sd" | wc -l)

if [[ $count >2 ]];then
exit
fi

for ((i=1;i<=$count;i++))
do 
var1=$(df |grep "/dev/sd"|awk -F" " 'NR=='$i' {printf $6}') 
if [[ $var1 == "/boot" ]];then
echo $(df |grep "/dev/sd"|awk -F" " 'NR=='$i' {printf $1}'| cut -b 1-8) 
break
fi
done


