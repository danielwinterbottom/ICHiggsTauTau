#!/bin/sh



cp -v collected/Imperial/htt_mt.inputs-sm-7TeV.root htt_mt.inputs-sm-7TeV-ztt-up-1p05.root 
cp -v collected/Imperial/htt_et.inputs-sm-7TeV.root htt_et.inputs-sm-7TeV-ztt-up-1p05.root 
cp -v collected/Imperial/htt_mt.inputs-sm-8TeV-moriond-andrew.root htt_mt.inputs-sm-8TeV-ztt-up-1p05.root 
cp -v collected/Imperial/htt_et.inputs-sm-8TeV-moriond-andrew.root htt_et.inputs-sm-8TeV-ztt-up-1p05.root 

# cp -v collected/CERN/htt_et.inputs-sm-8TeV.root htt_et.inputs-sm-8TeV-ztt-up-1p05.root 
# cp -v collected/LLR/htt_mt.inputs-sm-8TeV.root htt_mt.inputs-sm-8TeV-ztt-up-1p05.root 

#### 8 TeV et
./bin/ScaleUp \
  --input=htt_et.inputs-sm-8TeV-ztt-up-1p05.root \
  --channel=eleTau

./bin/ScaleUp \
  --input=htt_et.inputs-sm-8TeV-ztt-up-1p05.root \
  --channel=eleTau \
  --append="_CMS_scale_t_etau_8TeVDown"

./bin/ScaleUp \
  --input=htt_et.inputs-sm-8TeV-ztt-up-1p05.root \
  --channel=eleTau \
  --append="_CMS_scale_t_etau_8TeVUp"

#### 8 TeV mt
./bin/ScaleUp \
  --input=htt_mt.inputs-sm-8TeV-ztt-up-1p05.root \
  --channel=muTau

./bin/ScaleUp \
  --input=htt_mt.inputs-sm-8TeV-ztt-up-1p05.root \
  --channel=muTau \
  --append="_CMS_scale_t_mutau_8TeVDown"

./bin/ScaleUp \
  --input=htt_mt.inputs-sm-8TeV-ztt-up-1p05.root \
  --channel=muTau \
  --append="_CMS_scale_t_mutau_8TeVUp"

#### 8 TeV et
./bin/ScaleUp \
  --input=htt_et.inputs-sm-7TeV-ztt-up-1p05.root \
  --channel=eleTau

./bin/ScaleUp \
  --input=htt_et.inputs-sm-7TeV-ztt-up-1p05.root \
  --channel=eleTau \
  --append="_CMS_scale_t_etau_7TeVDown"

./bin/ScaleUp \
  --input=htt_et.inputs-sm-7TeV-ztt-up-1p05.root \
  --channel=eleTau \
  --append="_CMS_scale_t_etau_7TeVUp"

#### 8 TeV mt
./bin/ScaleUp \
  --input=htt_mt.inputs-sm-7TeV-ztt-up-1p05.root \
  --channel=muTau

./bin/ScaleUp \
  --input=htt_mt.inputs-sm-7TeV-ztt-up-1p05.root \
  --channel=muTau \
  --append="_CMS_scale_t_mutau_7TeVDown"

./bin/ScaleUp \
  --input=htt_mt.inputs-sm-7TeV-ztt-up-1p05.root \
  --channel=muTau \
  --append="_CMS_scale_t_mutau_7TeVUp"



