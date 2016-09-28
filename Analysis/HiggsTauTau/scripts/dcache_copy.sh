#!/bin/bash

export path1=$1
export path2=$2
export dir=$3
cd $dir

source /vols/grid/cms/setup.sh
eval `scramv1 runtime -sh`

lcg-cp $path1 $path2
