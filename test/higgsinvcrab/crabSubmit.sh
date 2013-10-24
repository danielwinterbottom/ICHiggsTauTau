#!/bin/sh
if (( $# != 2 ))
    then
    echo "Usage: ./crabSubmit.sh <command:crab/multicrab> <cfg file>"
else
    $1 -cfg $2  -create -submit all
#crab -resubmit xx -GRID.se_black_list T2_US_Wisconsin
fi
