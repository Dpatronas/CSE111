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
  echo "USAGE: ./grade.sh numbers"
  exit;
fi

num=$1

./check.sh $num | tee check.out

echo ""
echo "#######################################################"
echo ""
echo "CSE111 Assignment 7"
echo ""
date

ccode=0
cstr="yes"
valg=0
vstr="n/a"
if [ ! -s make.out ]
then
  echo ""
  echo -n "Checking for memory errors..."
  (( ccode = 5 ))
  cstr="none"
  > valgrind.out
  flags="--track-origins=yes --leak-check=full --show-leak-kinds=all"
  valgrind $flags ./radix 32 1 1 -p 1>/dev/null 2>>valgrind.out

  valg=`grep 'ERROR SUMMARY' valgrind.out | grep -v 'ERROR SUMMARY: 0' | \
    awk 'BEGIN {sum=0} {sum += $4} END { print sum }'`

  valw=`grep 'Warning: ' valgrind.out | wc -l`

  (( valg += valw ))
  (( valg -= 2 ))
  echo ""
fi

pass=`grep PASS check.out | wc -l`
tests=56

bpct=`cat check.out | grep 'Basic:' | grep '\%' | sed 's/\%//' | awk -F "/" '{print $2}' | awk -F " " '{print $2}'`
bpct=`echo "scale=2; $bpct * 0.4" | bc -l`
apct=`cat check.out | grep 'Advanced:' | grep '\%' | sed 's/\%//' | awk -F "/" '{print $2}' | awk -F " " '{print $2}'`
apct=`echo "scale=2; $apct * 0.3" | bc -l`
spct=`cat check.out | grep 'Stretch:' | grep '\%' | sed 's/\%//' | awk -F "/" '{print $2}' | awk -F " " '{print $2}'`
spct=`echo "scale=2; $spct * 0.2" | bc -l`
epct=`cat check.out | grep 'Extreme:' | grep '\%' | sed 's/\%//' | awk -F "/" '{print $2}' | awk -F " " '{print $2}'`
epct=`echo "scale=2; $epct * 0.1" | bc -l`

echo ""
printf "%30s:           %4.1f%% of %d%%\n" "Basic" $bpct 40
printf "%30s:           %4.1f%% of %d%%\n" "Advanced" $apct 30
printf "%30s:           %4.1f%% of %d%%\n" "Stretch" $spct 20
printf "%30s:           %4.1f%% of %d%%\n" "Extremem" $epct 10

total=`echo "scale=2; ($bpct + $apct + $spct + $epct) * 0.9" | bc -l`

errors=0
if (( valg > 0 ))
then
  vstr="$valg"
fi

if (( ccode == 5 && valg == 0 ))
then
  (( errors = 5 ))
  vstr="none"
fi
printf "%30s:   %4s   %5.1f%% of  5%%\n" "Compiler Warnings" $cstr $ccode 
printf "%30s:   %4s   %5.1f%% of  5%%\n" "Additional Memory Problems" $vstr $errors

total=`echo "scale=2; $total + $ccode + $errors" | bc -l`
printf "\n%30s: %5.1f%%\n\n" "Total" $total 

rm check.out 2>/dev/null

if (( valg > 0 ))
then
  grep 'ERROR SUMMARY' valgrind.out | grep -v 'ERROR SUMMARY: 0'
  echo ""
  echo "See valgrind.out for details"
  echo ""
fi
