#!/bin/bash

var1=123
var2="${var1}ab"
var3=$(pwd)

read -p 'input a-> ' a
read -p 'input b-> ' b

echo $a
echo $b


sum=$((a+b))	#求和运算
echo "sum:$sum"

case $sum in
	1)
		echo "sum=1"
		;;
	2)
		echo "sum=2"
		;;
	*)
		echo "sum!=1且sum!=2"
		;;
esac

echo $var3

echo $0
echo $1
echo $2
echo $#
echo $*

test $a -eq $b && echo 'a=b'	#检测某个条件是否成立
[ $a -eq $b ]|| echo 'a!=b'
[ -f test.c ] && echo 'yes'

if [ -f test.sh ]
then 
	echo 'Yes!'
elif test -z $sum
then
	echo '空'
else
	echo 'No!'
fi

for n in 1 2 3 4 5	#或{1..5}
do
	echo $n
done

for n in $(ls /bin/*sh)	#`ls /bin/*sh`
do
	echo $n
done
 
i=0
#[ $i -lt 10 ]
while (( i <10))
do
	echo "${i}"
	i=$((i+1))
done

function name(){
	echo 'cb'
}

name	#调用函数

exit 1

