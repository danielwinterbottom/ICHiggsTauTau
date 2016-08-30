#!/bin/bash
#
export dirname=$1
export test=$(ls $dirname/ | grep optimizeOutput.txt)

if [ "$test" != "optimizeOutput.txt" ]; then
  cat $dirname/output_* > $dirname/optimizeOutput.txt
  sort -n -r $dirname/optimizeOutput.txt > $dirname/optimizeOutputSorted.txt
  #rm $dirname/output_*
fi

#cat $dirname/output_* > $dirname/optimizeOutput.txt
#sort -n -r $dirname/optimizeOutput.txt > $dirname/optimizeOutputSorted.txt
#rm $dirname/output_*

#export olddir=$(pwd)
#cd $dirname
#
#cat $(ls | grep output_ | grep _1.txt | grep _medium_) | cut -d" " -f1-4 > optimizeOutput_medium_vbf.txt
#cat $(ls | grep output_ | grep _1.txt | grep _tight_ ) | cut -d" " -f1-4 > optimizeOutput_tight_vbf.txt
#cat $(ls | grep output_ | grep _1.txt | grep _vtight_) | cut -d" " -f1-4 > optimizeOutput_vtight_vbf.txt
#                                   
#cat $(ls | grep output_ | grep _2.txt | grep _medium_) | cut -d" " -f1-4 > optimizeOutput_medium_1jet.txt
#cat $(ls | grep output_ | grep _2.txt | grep _tight_ ) | cut -d" " -f1-4 > optimizeOutput_tight_1jet.txt
#cat $(ls | grep output_ | grep _2.txt | grep _vtight_) | cut -d" " -f1-4 > optimizeOutput_vtight_1jet.txt
#                                   
#cat $(ls | grep output_ | grep _3.txt | grep _medium_) | cut -d" " -f1-4 > optimizeOutput_medium_0jet.txt
#cat $(ls | grep output_ | grep _3.txt | grep _tight_ ) | cut -d" " -f1-4 > optimizeOutput_tight_0jet.txt
#cat $(ls | grep output_ | grep _3.txt | grep _vtight_) | cut -d" " -f1-4 > optimizeOutput_vtight_0jet.txt
#
#cd $olddir
