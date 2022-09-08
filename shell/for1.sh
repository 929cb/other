#!/bin/env bash

# ...

for i in {50..1..3}
do
	if [[ $i%2 -eq 0 ]];then
		echo $i
	fi
done
