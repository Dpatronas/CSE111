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
  echo "USAGE: ./perf.sh numbers"
  exit;
fi
num=$1

speedup() {
  num=$1
  lists=$2
  cores=$3
  pct=$4

  req=`echo "10 * $pct" | bc -l`

  ./perf $num $lists $cores > radix.single &
  pid=$!

  cmax=0
  single=true
  while true
  do
    cat /proc/$pid/status > radix.threads 2>/dev/null
    status=$?
    if [ $status -ne 0 ]
    then
      break
    fi

    grep "Generate time:" radix.single >/dev/null
    if [ $? -eq 0 ]
    then
      threads=`cat radix.threads | grep Threads | awk '{print $2'}`
      cmax=$(( threads > cmax ? threads : cmax ))
      grep "Single Elapsed" radix.single >/dev/null
      if [ $? -eq 0 ]
      then
        if [ "$single" = "true" ] 
        then
          if [ $cmax -gt 2 ]
          then 
            (( cmax -= 1 ))
            echo ""
            echo -n "Speedup lists: $lists cores: $cores required: $req% "
            echo "FAIL ( too many cores on single test: $cmax )"
            return
          else
            single=false;
          fi
        fi
        echo -n .
      else
        echo -n ^
      fi
    else
      echo -n -
    fi

    sleep 0.01
  done

  echo ""
  cat radix.single

  echo -n "Speedup lists: $lists cores: $cores required: $req% "

  fail=`grep FAIL radix.single | wc -l`
  pass=`grep PASS radix.single | wc -l`

  if [[ fail -gt 0 ]]
  then
    echo "FAIL ( functional - $fail failures )"
  else
    if [[ pass -gt 0 ]]
    then
      (( cmax -= 1 ))
      if [ $cmax -le $cores ]
      then
        speedup=`grep 'Speedup:' radix.single | awk '{print $2}' | sed 's/\%//'`
        printf "achieved: %.0f%% : " $speedup

        if (( $(echo "$speedup > $req" | bc -l) ))
        then
          echo "PASS"
        else
          echo "FAIL ( insufficient speedup )"
        fi
      else
        echo "FAIL ( too many cores: $cmax, should have been no more than $cores )"
      fi
    else
      echo "FAIL ( functional - no passes )"
    fi
  fi
  echo ""
}

capped() {
  num=$1
  lists=$2
  cores=$3

  ./radix $num $lists $cores > radix.single &
  pid=$!

  cmax=0
  while true
  do
    cat /proc/$pid/status > radix.threads 2>/dev/null
    status=$?
    if [ $status -ne 0 ]
    then
      break
    fi

    grep "Generate time:" radix.single >/dev/null
    if [ $? -eq 0 ]
    then
      threads=`cat radix.threads | grep Threads | awk '{print $2'}`
      cmax=$(( threads > cmax ? threads : cmax ))
      echo -n .
    else
      echo -n -
    fi

    sleep 0.01
  done

  echo ""
  echo -n "Capped $lists lists $cores cores: "
  fail=`grep FAIL radix.single | wc -l`
  pass=`grep PASS radix.single | wc -l`

  if [[ fail -gt 0 ]]
  then
    echo "FAIL ( native )"
  else
    if [[ pass -gt 0 ]]
    then
      (( cmax -= 1 ))
      if [ $cmax -le $cores ]
      then
        echo "PASS"
      else
        echo "FAIL ( too many cores: $cmax, should have been no more than $cores )"
      fi
    else
      echo "FAIL ( functional )"
    fi
  fi
}

for pct in 15 20 25 30 35 40 45 50 
do
  speedup $num 1 10 $pct
done

for pct in 40 45 50 55 65 80 85 90
do
  speedup $num 1 20 $pct
done

for lists in 1 2 4 
do
  (( cores = lists * 3 ))
  capped $num $lists $cores
done

for lists in 1 2 4 
do
  (( cores = lists * 6 ))
  capped $num $lists $cores
done

rm radix.single radix.quad radix.threads 2>/dev/null
