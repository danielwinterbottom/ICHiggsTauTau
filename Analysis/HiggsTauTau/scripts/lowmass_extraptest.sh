#!/bin/sh

# This script copies a set of inputs from the official repository
# and makes duplicates which test the extrapolation of signal
# shapes.
# The script requires that the datacard respository folder:
#   UserCode/HiggsAnalysis/HiggsToTauTau/datacards/collected/
# is symlinked or copied to the current working directory.

cp -v collected/Imperial/htt_mt.inputs-sm-7TeV.root htt_mt.inputs-sm-7TeV-extraptest.root 
cp -v collected/Imperial/htt_et.inputs-sm-7TeV.root htt_et.inputs-sm-7TeV-extraptest.root 
cp -v collected/Imperial/htt_mt.inputs-sm-8TeV.root htt_mt.inputs-sm-8TeV-extraptest.root 
cp -v collected/Imperial/htt_et.inputs-sm-8TeV.root htt_et.inputs-sm-8TeV-extraptest.root 
cp -v collected/MIT/htt_em.inputs-sm-7TeV.root htt_em.inputs-sm-7TeV-extraptest.root 
cp -v collected/MIT/htt_em.inputs-sm-8TeV.root htt_em.inputs-sm-8TeV-extraptest.root
cp -v collected/MIT/htt_tt.inputs-sm-8TeV.root htt_tt.inputs-sm-8TeV-extraptest.root 

# cp -v collected/Imperial/htt_em.inputs-sm-8TeV.root htt_mt.inputs-sm-8TeV-extraptest.root 
# cp -v collected/CERN/htt_mt.inputs-sm-8TeV.root htt_mt.inputs-sm-8TeV-extraptest.root 
# cp -v collected/CERN/htt_et.inputs-sm-8TeV.root htt_et.inputs-sm-8TeV-extraptest.root 
# cp -v collected/Wisconsin/htt_mt.inputs-sm-8TeV.root htt_mt.inputs-sm-8TeV-extraptest.root 
# cp -v collected/Wisconsin/htt_et.inputs-sm-8TeV.root htt_et.inputs-sm-8TeV-extraptest.root 

#### 8 TeV em
./bin/SignalStudies \
  --input=htt_em.inputs-sm-8TeV-extraptest.root \
  --channel=emu \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110"

./bin/SignalStudies \
  --input=htt_em.inputs-sm-8TeV-extraptest.root \
  --channel=emu \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_e_8TeVDown"

./bin/SignalStudies \
  --input=htt_em.inputs-sm-8TeV-extraptest.root \
  --channel=emu \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_e_8TeVUp"

./bin/SignalStudies \
  --input=htt_em.inputs-sm-8TeV-extraptest.root \
  --channel=emu \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_e_highpt_8TeVDown"

./bin/SignalStudies \
  --input=htt_em.inputs-sm-8TeV-extraptest.root \
  --channel=emu \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_e_highpt_8TeVUp"

./bin/SignalStudies \
  --input=htt_em.inputs-sm-8TeV-extraptest.root \
  --channel=emu \
  --prod="VH" --no_rms=true --force_yield=true \
  --extrap="145=135,125,115:140=130,120,110"

./bin/SignalStudies \
  --input=htt_em.inputs-sm-8TeV-extraptest.root \
  --channel=emu \
  --prod="VH" --no_rms=true --force_yield=true  \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_e_8TeVDown"

./bin/SignalStudies \
  --input=htt_em.inputs-sm-8TeV-extraptest.root \
  --channel=emu \
  --prod="VH" --no_rms=true --force_yield=true \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_e_8TeVUp"

./bin/SignalStudies \
  --input=htt_em.inputs-sm-8TeV-extraptest.root \
  --channel=emu \
  --prod="VH" --no_rms=true --force_yield=true \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_e_highpt_8TeVDown"

./bin/SignalStudies \
  --input=htt_em.inputs-sm-8TeV-extraptest.root \
  --channel=emu \
  --prod="VH" --no_rms=true --force_yield=true \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_e_highpt_8TeVUp"

#### 8 TeV et
./bin/SignalStudies \
  --input=htt_et.inputs-sm-8TeV-extraptest.root \
  --channel=eleTau \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110"

./bin/SignalStudies \
  --input=htt_et.inputs-sm-8TeV-extraptest.root \
  --channel=eleTau \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_t_etau_8TeVDown"

./bin/SignalStudies \
  --input=htt_et.inputs-sm-8TeV-extraptest.root \
  --channel=eleTau \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_t_etau_8TeVUp"

./bin/SignalStudies \
  --input=htt_et.inputs-sm-8TeV-extraptest.root \
  --channel=eleTau \
  --prod="VH" --no_rms=true --force_yield=true \
  --extrap="145=135,125,115:140=130,120,110"

./bin/SignalStudies \
  --input=htt_et.inputs-sm-8TeV-extraptest.root \
  --channel=eleTau \
  --prod="VH" --no_rms=true --force_yield=true  \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_t_etau_8TeVDown"

./bin/SignalStudies \
  --input=htt_et.inputs-sm-8TeV-extraptest.root \
  --channel=eleTau \
  --prod="VH" --no_rms=true --force_yield=true \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_t_etau_8TeVUp"

#### 8 TeV mt
./bin/SignalStudies \
  --input=htt_mt.inputs-sm-8TeV-extraptest.root \
  --channel=muTau \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110"

./bin/SignalStudies \
  --input=htt_mt.inputs-sm-8TeV-extraptest.root \
  --channel=muTau \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_t_mutau_8TeVDown"

./bin/SignalStudies \
  --input=htt_mt.inputs-sm-8TeV-extraptest.root \
  --channel=muTau \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_t_mutau_8TeVUp"

./bin/SignalStudies \
  --input=htt_mt.inputs-sm-8TeV-extraptest.root \
  --channel=muTau \
  --prod="VH" --no_rms=true --force_yield=true \
  --extrap="145=135,125,115:140=130,120,110"

./bin/SignalStudies \
  --input=htt_mt.inputs-sm-8TeV-extraptest.root \
  --channel=muTau \
  --prod="VH" --no_rms=true --force_yield=true  \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_t_mutau_8TeVDown"

./bin/SignalStudies \
  --input=htt_mt.inputs-sm-8TeV-extraptest.root \
  --channel=muTau \
  --prod="VH" --no_rms=true --force_yield=true \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_t_mutau_8TeVUp"

#### 8 TeV tt
./bin/SignalStudies \
  --input=htt_tt.inputs-sm-8TeV-extraptest.root \
  --channel=tauTau \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110"

./bin/SignalStudies \
  --input=htt_tt.inputs-sm-8TeV-extraptest.root \
  --channel=tauTau \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_t_tautau_8TeVDown"

./bin/SignalStudies \
  --input=htt_tt.inputs-sm-8TeV-extraptest.root \
  --channel=tauTau \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_t_tautau_8TeVUp"

./bin/SignalStudies \
  --input=htt_tt.inputs-sm-8TeV-extraptest.root \
  --channel=tauTau \
  --prod="VH" --no_rms=true --force_yield=true \
  --extrap="145=135,125,115:140=130,120,110"

./bin/SignalStudies \
  --input=htt_tt.inputs-sm-8TeV-extraptest.root \
  --channel=tauTau \
  --prod="VH" --no_rms=true --force_yield=true  \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_t_tautau_8TeVDown"

./bin/SignalStudies \
  --input=htt_tt.inputs-sm-8TeV-extraptest.root \
  --channel=tauTau \
  --prod="VH" --no_rms=true --force_yield=true \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_t_tautau_8TeVUp"


#### 7 TeV em
./bin/SignalStudies \
  --input=htt_em.inputs-sm-7TeV-extraptest.root \
  --channel=emu \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110"

./bin/SignalStudies \
  --input=htt_em.inputs-sm-7TeV-extraptest.root \
  --channel=emu \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_e_7TeVDown"

./bin/SignalStudies \
  --input=htt_em.inputs-sm-7TeV-extraptest.root \
  --channel=emu \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_e_7TeVUp"

./bin/SignalStudies \
  --input=htt_em.inputs-sm-7TeV-extraptest.root \
  --channel=emu \
  --prod="VH" --no_rms=true --force_yield=true \
  --extrap="145=135,125,115:140=130,120,110"

./bin/SignalStudies \
  --input=htt_em.inputs-sm-7TeV-extraptest.root \
  --channel=emu \
  --prod="VH" --no_rms=true --force_yield=true  \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_e_7TeVDown"

./bin/SignalStudies \
  --input=htt_em.inputs-sm-7TeV-extraptest.root \
  --channel=emu \
  --prod="VH" --no_rms=true --force_yield=true \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_e_7TeVUp"


#### 7 TeV et
./bin/SignalStudies \
  --input=htt_et.inputs-sm-7TeV-extraptest.root \
  --channel=eleTau \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110"

./bin/SignalStudies \
  --input=htt_et.inputs-sm-7TeV-extraptest.root \
  --channel=eleTau \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_t_etau_7TeVDown"

./bin/SignalStudies \
  --input=htt_et.inputs-sm-7TeV-extraptest.root \
  --channel=eleTau \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_t_etau_7TeVUp"

./bin/SignalStudies \
  --input=htt_et.inputs-sm-7TeV-extraptest.root \
  --channel=eleTau \
  --prod="VH" --no_rms=true --force_yield=true \
  --extrap="145=135,125,115:140=130,120,110"

./bin/SignalStudies \
  --input=htt_et.inputs-sm-7TeV-extraptest.root \
  --channel=eleTau \
  --prod="VH" --no_rms=true --force_yield=true  \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_t_etau_7TeVDown"

./bin/SignalStudies \
  --input=htt_et.inputs-sm-7TeV-extraptest.root \
  --channel=eleTau \
  --prod="VH" --no_rms=true --force_yield=true \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_t_etau_7TeVUp"

#### 7 TeV mt
./bin/SignalStudies \
  --input=htt_mt.inputs-sm-7TeV-extraptest.root \
  --channel=muTau \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110"

./bin/SignalStudies \
  --input=htt_mt.inputs-sm-7TeV-extraptest.root \
  --channel=muTau \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_t_mutau_7TeVDown"

./bin/SignalStudies \
  --input=htt_mt.inputs-sm-7TeV-extraptest.root \
  --channel=muTau \
  --prod="ggH,qqH" \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_t_mutau_7TeVUp"

./bin/SignalStudies \
  --input=htt_mt.inputs-sm-7TeV-extraptest.root \
  --channel=muTau \
  --prod="VH" --no_rms=true --force_yield=true \
  --extrap="145=135,125,115:140=130,120,110"

./bin/SignalStudies \
  --input=htt_mt.inputs-sm-7TeV-extraptest.root \
  --channel=muTau \
  --prod="VH" --no_rms=true --force_yield=true  \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_t_mutau_7TeVDown"

./bin/SignalStudies \
  --input=htt_mt.inputs-sm-7TeV-extraptest.root \
  --channel=muTau \
  --prod="VH" --no_rms=true --force_yield=true \
  --extrap="145=135,125,115:140=130,120,110" \
  --append="_CMS_scale_t_mutau_7TeVUp"


