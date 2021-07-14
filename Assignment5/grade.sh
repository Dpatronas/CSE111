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
./perf.sh  $num | tee perf.out

echo ""
echo "CSE111 Assignment 5"
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
  valgrind $flags ./radix 1024 4 2 1>/dev/null 2>>valgrind.out

  valg=`grep 'ERROR SUMMARY' valgrind.out | grep -v 'ERROR SUMMARY: 0' | \
    awk 'BEGIN {sum=0} {sum += $4} END { print sum }'`

  valw=`grep 'Warning: ' valgrind.out | wc -l`

  (( valg += valw ))
  echo ""
fi

pass=`grep PASS check.out | wc -l`
fail=`grep FAIL check.out | wc -l`
(( tests = pass + fail ))

pct=`cat check.out | grep 'Tests' | grep '\%' | sed 's/\%//' | awk -F "/" '{print $2}' | awk -F " " '{print $2}'`
total=`echo "scale=2; $pct * 0.5" | bc -l`
printf "\n%30s:  %2d/%2d    %4.1f%% of %d%%\n" "Functional" $pass $tests $total 50

spass=`grep 'Speedup ' perf.out | grep PASS | wc -l`
sfail=`grep 'Speedup ' perf.out | grep FAIL | wc -l`
cpass=`grep Capped perf.out | grep PASS | wc -l`
cfail=`grep Capped perf.out | grep FAIL | wc -l`
(( pass = spass + cpass ))
(( fail = sfail + cfail ))
(( tests = pass + fail ))

ptotal=0
if [ $tests -ne 0 ]
then
    ptotal=`echo "scale=2; ($pass / $tests ) * 40.0" | bc -l`
fi
printf "%30s:  %2d/%2d    %4.1f%% of %d%%\n" "Non-Functional" $pass $tests $ptotal 40

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
printf "%30s:   %4s   %5.1f%% of  5%%\n" "Memory Problems" $vstr $errors

total=`echo "scale=2; $total + $ptotal + $ccode + $errors" | bc -l`
printf "\n%30s: %5.1f%%\n\n" "Total" $total 

rm perf.out check.out 2>/dev/null

if (( valg > 0 ))
then
  grep 'ERROR SUMMARY' valgrind.out | grep -v 'ERROR SUMMARY: 0'
  echo ""
  echo "See valgrind.out for details"
  echo ""
fi
