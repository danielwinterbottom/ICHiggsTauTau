#!/bin/bash

for dir in cards_run2ana_170124/
  do cd $dir
  echo $dir:

  for subdir1 in enu munu mumu ee nunu
    do cd $subdir1
    echo $subdir1:

    mv vbfhinv_${subdir1}_13TeV_2.3_1101.txt tmp.txt
    sed "s/\-\/\-/\-/g" tmp.txt > tmp1.txt
    sed "s/\-\//1\//g"            tmp1.txt > tmp2.txt
    sed "s/\/\-/\/1/g"                       tmp2.txt > vbfhinv_${subdir1}_13TeV_2.3_1101.txt
    rm tmp*
    cd ../
    done

  for subdir2 in taunu qcd
    do cd $subdir2
    echo $subdir2:

    mv vbfhinv_${subdir2}_13TeV_-1_1101.txt tmp.txt
    sed "s/\-\/\-/\-/g" tmp.txt > tmp1.txt
    sed "s/\-\//1\//g"            tmp1.txt > tmp2.txt
    sed "s/\/\-/\/1/g"                       tmp2.txt > vbfhinv_${subdir2}_13TeV_-1_1101.txt
    rm tmp*
    cd ../
    done

  done
