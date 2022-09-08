#!/bin/env bash

# 随机产生以139开头的电话号码1000个

PHONENUM_PATH=/home/cb/repository/other/shell/phonenum
PHONENUM_COUNT=1000
PHONENUM_HEAD=139
i=1

while [ $i -le $PHONENUM_COUNT ]
do
	let i++
	echo "$PHONENUM_HEAD$[$RANDOM % 10]$[$RANDOM % 10]$[$RANDOM % 10]$[$RANDOM % 10]$[$RANDOM % 10]$[$RANDOM % 10]$[$RANDOM % 10]$[$RANDOM % 10]" >>$PHONENUM_PATH
done
