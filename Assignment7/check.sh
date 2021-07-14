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

rm radix.out 2>/dev/null

tpass=0
> radix.totals.out
check() {
  flags=$1
  tag=$2

  echo ""
  echo "######## $tag #########" 
  echo ""

  for lists in 1 2 4
  do
      timeout 10 ./radix $num $lists 4 $flags | tee radix.tmp.out
      cat radix.tmp.out >> radix.out
  done
  rm radix.tmp.out

  total=14
  pass=`grep PASS radix.out | wc -l`
  (( tpass += pass ))
  pct=0;
  pct=`echo "scale=2; $pass / $total * 100.0" | bc -l`
  rm radix.out
  printf "%20s:  %3d/%d   %5.1f%%\n" $tag $pass $total $pct >> radix.totals.out
}

check ""    "Basic"
check "-r"  "Advanced"
check "-l"  "Stretch"
check "-rl" "Extreme"

echo ""
cat radix.totals.out

total=56
pct=`echo "scale=2; $tpass / $total * 100.0" | bc -l`
printf "\n%20s:           %5.1f%%\n" "Tests" $pct 

rm radix.totals.out 2>/dev/null
rm radix.out 2>/dev/null
