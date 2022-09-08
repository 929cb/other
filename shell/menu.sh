#!/bin/env bash

# 菜单维护
clear
function menu()
{
	cat <<-EOF
		h	显示命令帮助
		f	显示磁盘分区
		d	显示磁盘挂载
		m	显示内存使用
		u	显示系统负载
		q	退出程序
	EOF
}

menu

while true
do
read -p "请选择需要操作的内容(help h): " action
	clear
case $action in
	h|help)
		menu
		;;
	f)
		sudo fdisk -l
		;;
	d)
		df -h
		;;
	m)
		free -m
		;;
	u)
		uptime
		;;
	q)
		exit
		;;
	*)
		echo "请输入正确的指令！"
		;;
esac
done
