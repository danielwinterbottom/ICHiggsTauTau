#!/bin/sh
PREFIX=root://xrootd.grid.hep.ph.ic.ac.uk//store/user/agilbert/5March2014/Data_42X/

bin/Zbb --cfg=scripts/default.cfg --filelist=filelists/5March2014_Data_42X_DoubleElectron-2011A-May10ReReco-v1.dat \
  --input_prefix=$PREFIX --output_name="Data.root" --is_data=true