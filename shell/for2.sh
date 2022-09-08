#!/bin/env bash

# 计算1～100的奇数和

SUM=0

for i in $(seq 1 2 100)
do
	let SUM+=$i
done

echo $SUM
