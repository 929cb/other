#!/bin/env bash

# 判断当前主机是否和指定主机互通

read -p "请输入你要ping的主机IP: " IP

ping -c1 $IP > /dev/null

if [ $? -eq 0 ];then
	echo "OK"
else
	echo "ERR"
fi
