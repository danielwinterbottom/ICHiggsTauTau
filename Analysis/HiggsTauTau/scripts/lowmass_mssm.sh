#!/bin/sh

# Testing low signal mass extrapolation using MSSM signal
# sample and horzizontal morphing. This test can be run
# on an 8 TeV mu-tau datacard input only.
#
# The script first copies such an input from the repository,
# then performs the standard extrapolation for the 90 GeV
# mass point using the SignalStudies tool.  A second tool,
# InjectMSSM is used to replace the 90 GeV signal shapes
# with those from the MSSM ggH 90 GeV job out.  Finally,
# the SignalStudies tool is used to extrapolate the yield
# of the intermediate mass points, but takes the shapes
# from the pre-prepared output of the horizontal morphing
# tool.
#
# The script requires that the datacard respository folder:
#   UserCode/HiggsAnalysis/HiggsToTauTau/datacards/collected/
# is symlinked or copied to the current working directory.
cp -v collected/Imperial/htt_mt.inputs-sm-8TeV.root htt_mt.inputs-sm-8TeV-lowmssm.root 


./bin/SignalStudies \
--input=htt_mt.inputs-sm-8TeV-lowmssm.root \
--channel=muTau \
--prod="ggH,qqH" \
--extrap="110=90"

./bin/SignalStudies \
--input=htt_mt.inputs-sm-8TeV-lowmssm.root \
--channel=muTau \
--prod="ggH,qqH" \
--extrap="110=90" \
--append="_CMS_scale_t_mutau_8TeVDown"

./bin/SignalStudies \
--input=htt_mt.inputs-sm-8TeV-lowmssm.root \
--channel=muTau \
--prod="ggH,qqH" \
--extrap="110=90" \
--append="_CMS_scale_t_mutau_8TeVUp"

./bin/SignalStudies \
--input=htt_mt.inputs-sm-8TeV-lowmssm.root \
--channel=muTau \
--prod="VH" --no_rms=true \
--extrap="110=90"

./bin/SignalStudies \
--input=htt_mt.inputs-sm-8TeV-lowmssm.root \
--channel=muTau \
--prod="VH" --no_rms=true  \
--extrap="110=90" \
--append="_CMS_scale_t_mutau_8TeVDown"

./bin/SignalStudies \
--input=htt_mt.inputs-sm-8TeV-lowmssm.root \
--channel=muTau \
--prod="VH" --no_rms=true \
--extrap="110=90" \
--append="_CMS_scale_t_mutau_8TeVUp"

./bin/InjectMSSM \
--input=htt_mt.inputs-sm-8TeV-lowmssm.root \
--mssm_file=SUSYGluGluToHToTauTau_M-90_mt_2012.root \
--channel=muTau \
--prod=ggH

./bin/InjectMSSM \
--input=htt_mt.inputs-sm-8TeV-lowmssm.root \
--mssm_file=SUSYGluGluToHToTauTau_M-90_mt_2012_TDown.root \
--channel=muTau \
--prod=ggH \
--append="_CMS_scale_t_mutau_8TeVDown"

./bin/InjectMSSM \
--input=htt_mt.inputs-sm-8TeV-lowmssm.root \
--mssm_file=SUSYGluGluToHToTauTau_M-90_mt_2012_TUp.root \
--channel=muTau \
--prod=ggH \
--append="_CMS_scale_t_mutau_8TeVUp"

./bin/InjectMSSM \
--input=htt_mt.inputs-sm-8TeV-lowmssm.root \
--mssm_file=SUSYGluGluToHToTauTau_M-90_mt_2012.root \
--channel=muTau \
--prod=qqH

./bin/InjectMSSM \
--input=htt_mt.inputs-sm-8TeV-lowmssm.root \
--mssm_file=SUSYGluGluToHToTauTau_M-90_mt_2012_TDown.root \
--channel=muTau \
--prod=qqH \
--append="_CMS_scale_t_mutau_8TeVDown"

./bin/InjectMSSM \
--input=htt_mt.inputs-sm-8TeV-lowmssm.root \
--mssm_file=SUSYGluGluToHToTauTau_M-90_mt_2012_TUp.root \
--channel=muTau \
--prod=qqH \
--append="_CMS_scale_t_mutau_8TeVUp"

cp -v htt_mt.inputs-sm-8TeV-lowmssm.root htt_mt.inputs-sm-8TeV-lowmssmpatched.root 

./bin/SignalStudies \
--input=htt_mt.inputs-sm-8TeV-lowmssmpatched.root \
--channel=muTau \
--prod="ggH,qqH" \
--extrap="110=105,100,95" \
--patch_shape=horizontalMorph_forAndrew_all.root

./bin/SignalStudies \
--input=htt_mt.inputs-sm-8TeV-lowmssmpatched.root \
--channel=muTau \
--prod="ggH,qqH" \
--extrap="110=105,100,95" \
--append="_CMS_scale_t_mutau_8TeVDown" \
--patch_shape=horizontalMorph_forAndrew_all.root


./bin/SignalStudies \
--input=htt_mt.inputs-sm-8TeV-lowmssmpatched.root \
--channel=muTau \
--prod="ggH,qqH" \
--extrap="110=105,100,95" \
--append="_CMS_scale_t_mutau_8TeVUp" \
--patch_shape=horizontalMorph_forAndrew_all.root


./bin/SignalStudies \
--input=htt_mt.inputs-sm-8TeV-lowmssmpatched.root \
--channel=muTau \
--prod="VH" --no_rms=true \
--extrap="110=105,100,95" \
--patch_shape=horizontalMorph_forAndrew_all.root


./bin/SignalStudies \
--input=htt_mt.inputs-sm-8TeV-lowmssmpatched.root \
--channel=muTau \
--prod="VH" --no_rms=true  \
--extrap="110=105,100,95" \
--append="_CMS_scale_t_mutau_8TeVDown" \
--patch_shape=horizontalMorph_forAndrew_all.root


./bin/SignalStudies \
--input=htt_mt.inputs-sm-8TeV-lowmssmpatched.root \
--channel=muTau \
--prod="VH" --no_rms=true \
--extrap="110=105,100,95" \
--append="_CMS_scale_t_mutau_8TeVUp" \
--patch_shape=horizontalMorph_forAndrew_all.root





