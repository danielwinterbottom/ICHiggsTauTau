#!/bin/bash
#
export dirname=$1
cat $dirname/output_* > $dirname/optimizeOutput.txt
sort -n -r $dirname/optimizeOutput.txt > $dirname/optimizeOutputSorted.txt
rm $dirname/output_*
