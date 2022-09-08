#!/bin/env bash

input_fun()
{
	input_var=""
	output_var=$1

	while [ -z $input_var ]
	do
		read -p "$output_var" input_var
	done	

	echo $input_var
}

name=$(input_fun 请输入你的姓名：)
sex=$(input_fun 请输入你的性别：)
age=$(input_fun 请输入你的年龄：)

case $sex in
	man)
		if [ $age -gt 18 -a $age -le 35 ];then
			echo "中年大叔你油腻了吗"
		elif [ $age -gt 35 ];then
			echo "保温杯里炮枸杞"
		else
			echo "年轻有为"
		fi
esac
