#!/bin/env bash

# first_shell

STR="hello world!"

echo ${STR:2:7}
echo "$STR任务已于$(date +'%F %T')完成..."
