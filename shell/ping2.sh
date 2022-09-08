#!/bin/env bash

# 将局域网内的IP按能不能ping通来分类存放

IP=192.168.1
LOG_PATH=/home/cb/repository/other/shell/log

for i in $(seq 10)
do
{
	ping -c1 $IP.$i &>/dev/null
	if test $? -eq 0;then
		echo "$IP.$i is OK" >> $LOG_PATH
	else
		echo "$IP.$i is down" >>$LOG_PATH
	fi
}&	# 开启并发（后台）
done

wait	# 等待执行完毕
echo "OK."
