#!/bin/bash

#######################################################################
#
# Copyright (C) 2018-2021 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without 
# the express written permission of the copyright holder.
#
#######################################################################

tag=$1
cmd=$2

run=0
pass=0
while read e
do
    (( run += 1 ))
    exp=`echo "$e" | awk -F ':' '{printf "%s", $1}' `
    ans=`echo "$e" | awk -F ':' '{printf "%s", $2}' | sed 's/ //g'`
    res=`$cmd "$exp" | xargs echo -n`
    nans=`echo $ans | awk '{printf("%.2f", $1)}'`
    nres=`echo $res | awk '{printf("%.2f", $1)}'`
    if [ "$nres" != "$nans" ]
    then
        echo "FAIL $exp: expected $ans, got $res"
    else
        echo "PASS $exp= $res"
        (( pass += 1 ))
    fi
done < check.dat

pct=`echo "scale=2; $pass / $run * 100.0" | bc -l`
printf "\n$tag %d/%d %.0f%%\n\n" $pass $run $pct
