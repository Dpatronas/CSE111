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

total=0
> radix.out
for lists in {1..4}
do
  ./radix $num $lists 12 | tee radix.tmp.out
  cat radix.tmp.out >> radix.out
done
rm radix.tmp.out

pass=`grep PASS radix.out | wc -l`
fail=`grep FAIL radix.out | wc -l`
(( total = pass + fail))
pct=0;
if (( total > 0 ))
then
    pct=`echo "scale=2; $pass / $total * 100.0" | bc -l`
fi
rm radix.out
echo "" 

printf "%20s:  %3d/%3d   %5.1f%%\n\n" "Tests" $pass $total $pct
