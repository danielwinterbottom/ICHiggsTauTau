#!/bin/bash
#crab -status -c $1 | tee tmp.txt

# njobs=`grep "Total Jobs" tmp.txt | awk '{print $2}'`
# echo Task $1 has $njobs jobs
# if [ "$njobs" -le "500" ]
#     then
#     crab -submit -c $1
# else
#     rem=`expr $njobs % 500`
#     mult=`expr $njobs / 500`
#     if [ "$mult" -gt "0" ]
# 	then
# 	subrangestart="$(($mult * 500))"
# 	subrangeend=`expr $subrangestart + $rem`
# 	subrangestart=`expr $subrangestart + 1`
# 	subrange=`echo "$subrangestart-$subrangeend"`
# 	echo $subrange
# 	crab -submit $subrange -c $1
#     fi

#     counter=0
#     while [ $counter -lt $mult ]
#       do
#       subrangestart="$(($counter * 500))"
#       subrangestart=`expr $subrangestart + 1`
#       counter=`expr $counter + 1`
#       subrangeend="$(($counter * 500))"
#       subrange=`echo "$subrangestart-$subrangeend"`
#       echo $subrange
#       crab -submit $subrange -c $1
#     done
# fi

for dir in $1;do echo $dir; crab -status -c $dir;crab -get -c $dir;crab -resubmit bad -c $dir; done