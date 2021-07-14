#!/bin/bash

#######################################################################
#
# Copyright (C) 2018-2021 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without 
# the express written permission of the copyright holder.
#
#######################################################################

if [ "$#" -ne 1 ]; then
  echo "USAGE: ./check.sh numbers"
  exit;
fi
num=$1

> radix.out
for lists in {1..4}
do
  for cores in $(seq 1 $lists)
  do
    ./radix $num $lists $cores | tee radix.tmp.out
    cat radix.tmp.out >> radix.out
  done
done
rm radix.tmp.out

total=30
pass=`grep PASS radix.out | wc -l`
fail=`grep FAIL radix.out | wc -l`
pct=`echo "scale=2; $pass / $total * 100.0" | bc -l`

rm radix.out
echo "" 

printf "%20s:  %3d/%3d   %5.1f%%\n\n" "Tests" $pass $total $pct
