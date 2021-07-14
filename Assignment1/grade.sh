#!/bin/bash

#######################################################################
#
# Copyright (C) 2018-2021 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without 
# the express written permission of the copyright holder.
#
#######################################################################

total=0;

check()
{
    TAG=$1
    NAME=$2
    MARKS=$3

    printf "%20s:         " $NAME

    marks=0;
    grep $TAG check.out > /dev/null
    if [ $? -eq 0 ]
    then
        set `grep $TAG check.out`
        pct=$2
        if [[ $pct != 0* ]]
        then
            marks=`echo "scale=2; ($pct)*$MARKS" | bc -l`
        fi
    fi
    total=`echo "scale=2; $total + $marks" | bc -l`
    printf "%5.1f%% of %2s%%\n" $marks $MARKS
}

printf "%s\n\n%s\n\n" "CMPS111 Assignment 1" "Reverse Polish Notation Calculator"

check 'RpncC: '    'C'       15
check 'RpncSH: '   'Bash'    15
check 'RpncPY: '   'Python3' 15
check 'RpncAWK: '  'AWK'     15
check 'RpncPerl: ' 'Perl'    15
check 'RpncJS: '   'Node.js' 15

echo ""

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
  valgrind $flags ./rpnc '15 7 1 1 + - / 3 * 2 1 1 + + - : 5' 1>/dev/null 2>>valgrind.out

  valg=`grep 'ERROR SUMMARY' valgrind.out | grep -v 'ERROR SUMMARY: 0' | \
    awk 'BEGIN {sum=0} {sum += $4} END { print sum }'`

  valw=`grep 'Warning: ' valgrind.out | wc -l`

  (( valg += valw ))
fi
printf "%20s:   %4s  %5.1f%% of  5%%\n" "Compiler Warnings" $cstr $ccode 

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
printf "%20s:   %4s  %5.1f%% of  5%%\n" "Memory Problems" $vstr $errors

total=`echo "scale=2; $total + $ccode +$errors" | bc -l`
printf "\n%20s: %5.1f%%\n\n" "Total" $total 
