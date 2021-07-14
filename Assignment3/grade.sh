#!/bin/bash

#######################################################################
#
# Copyright (C) 2020-2021 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without 
# the express written permission of the copyright holder.
#
#######################################################################

echo ""
echo "CSE111 Assignment $1"
echo ""
date
echo ""
make -s bounds | tee grade.out
echo ""

tests=`cat grade.out | sed 's/(//' | grep ' tests' | head -1 | awk -F 'tests' '{print $1}' | awk '{print $NF}'`
fails=`cat grade.out | sed 's/(//' | grep ' failures' | head -1 | awk -F 'failures' '{print $1}' | awk '{print $NF}'`
(( passes = tests - fails ))

if [ $tests -gt 0 ]; then
  pct=`echo "scale=2; ( $passes / $tests ) * 100.0" | bc -l`
else
  pct=0;
fi
total=`echo "scale=2; $pct * 0.5" | bc -l`
total=`echo "scale=2; $pct * 0.5" | bc -l`
printf "%30s:  %5s     %5.1f%% of 50%%\n" "Tests" $passes"/"$tests $total 

lines=`cat grade.out | grep ' lines' |  awk '{print $2}'`
functions=`cat grade.out | grep ' functions' |  awk '{print $2}'`
rm grade.out

ccov=0
cstr="n/a"
if [ $pct = "100.00" ]; then
  cstr="<100%"
  if [ $lines = "100.0%" ]; then
    if [ $functions = "100.0%" ]; then
      ccov=40
      cstr="100%"
    fi
  fi
fi
printf "%30s:  %5s     %5.1f%% of 40%%\n" "Code Coverage" $cstr $ccov

ccode=0
cstr="yes"
valg=0
vstr="n/a"
if [ ! -s make.out ]
then
  (( ccode = 5 ))
  cstr="none"
  > valgrind.out
  flags="--track-origins=yes --leak-check=full --show-leak-kinds=all"
  valgrind $flags ./bounds 1>/dev/null 2>>valgrind.out

  valg=`grep 'ERROR SUMMARY' valgrind.out | grep -v 'ERROR SUMMARY: 0' | \
    awk 'BEGIN {sum=0} {sum += $4} END { print sum }'`

  valw=`grep 'Warning: ' valgrind.out | wc -l`

  (( valg += valw ))
fi
printf "%30s:   %4s     %5.1f%% of  5%%\n" "Compiler Warnings" $cstr $ccode 

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
printf "%30s:   %4s     %5.1f%% of  5%%\n" "Memory Problems" $vstr $errors

total=`echo "scale=2; $total + $ccode + $ccov + $errors" | bc -l`
printf "\n%30s: %5.1f%%\n\n" "Total" $total 

if (( valg > 0 ))
then
  grep 'ERROR SUMMARY' valgrind.out | grep -v 'ERROR SUMMARY: 0'
  echo ""
  echo "See valgrind.out for details"
  echo ""
fi
