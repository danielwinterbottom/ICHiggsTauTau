cd ../../Modules
make -j4
cd ../Analysis
make -j4
cd HiggsTauTau

# ../bin/DatacardCompare --inputs htt_mt.inputs-sm-8TeV-ICHEP-RESULT.root \
# --labels ICHEP-RESULT  --category muTau_X --output_file=dc_sync/ichep-result_inclusive_mt-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs htt_mt.inputs-sm-8TeV-ICHEP-RESULT.root \
# --labels ICHEP-RESULT  --category muTau_vbf --output_file=dc_sync/ichep-result_vbf_mt-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs htt_mt.inputs-sm-8TeV-ICHEP-RESULT.root \
# --labels ICHEP-RESULT --category muTau_boost_high --output_file=dc_sync/compare_boost_high_mt-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs htt_mt.inputs-sm-8TeV-ICHEP-RESULT.root \
# --labels ICHEP-RESULT --category muTau_boost_low --output_file=dc_sync/compare_boost_low_mt-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs htt_mt.inputs-sm-8TeV-ICHEP-RESULT.root \
# --labels ICHEP-RESULT --category muTau_0jet_high --output_file=dc_sync/compare_0jet_high_mt-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs htt_mt.inputs-sm-8TeV-ICHEP-RESULT.root \
# --labels ICHEP-RESULT --category muTau_0jet_low --output_file=dc_sync/compare_0jet_low_mt-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs Andrew_htt_mt.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root dc_sync/MIT-htt_mt.inputs-sm-8TeV.root dc_sync/CERN-htt_mt.inputs-sm-8TeV.root dc_sync/LLR-htt_mt.inputs-sm-8TeV.root \
# --labels ICHEP-RESULT --category muTau_btag_low --output_file=dc_sync/compare_btag_low_mt-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs Andrew_htt_et.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_et.inputs-sm-8TeV.root dc_sync/MIT-htt_et.inputs-sm-8TeV.root \
# --labels Imperial Wisconsin MIT --category eleTau_vbf --output_file=dc_sync/compare_vbf_et-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs Andrew_htt_et.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_et.inputs-sm-8TeV.root dc_sync/MIT-htt_et.inputs-sm-8TeV.root \
# --labels Imperial Wisconsin MIT --category eleTau_boost_high --output_file=dc_sync/compare_boost_high_et-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs Andrew_htt_et.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_et.inputs-sm-8TeV.root dc_sync/MIT-htt_et.inputs-sm-8TeV.root \
# --labels Imperial Wisconsin MIT --category eleTau_boost_low --output_file=dc_sync/compare_boost_low_et-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs Andrew_htt_et.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_et.inputs-sm-8TeV.root dc_sync/MIT-htt_et.inputs-sm-8TeV.root \
# --labels Imperial Wisconsin MIT --category eleTau_0jet_high --output_file=dc_sync/compare_0jet_high_et-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs Andrew_htt_et.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_et.inputs-sm-8TeV.root dc_sync/MIT-htt_et.inputs-sm-8TeV.root \
# --labels Imperial Wisconsin MIT --category eleTau_0jet_low --output_file=dc_sync/compare_0jet_low_et-sm-8TeV.pdf  


../bin/DatacardCompare --inputs htt_mt.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root  \
--labels Imperial Wisconsin  --category muTau_vbf --output_file=dc_sync/compare_vbf_mt-sm-8TeV.pdf

../bin/DatacardCompare --inputs htt_mt.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root  \
--labels Imperial Wisconsin  --category muTau_boost_high --output_file=dc_sync/compare_boost_high_mt-sm-8TeV.pdf

../bin/DatacardCompare --inputs htt_mt.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root  \
--labels Imperial Wisconsin  --category muTau_boost_low --output_file=dc_sync/compare_boost_low_mt-sm-8TeV.pdf

../bin/DatacardCompare --inputs htt_mt.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root  \
--labels Imperial Wisconsin  --category muTau_0jet_high --output_file=dc_sync/compare_0jet_high_mt-sm-8TeV.pdf

../bin/DatacardCompare --inputs htt_mt.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root  \
--labels Imperial Wisconsin  --category muTau_0jet_low --output_file=dc_sync/compare_0jet_low_mt-sm-8TeV.pdf  




# ../bin/DatacardCompare --inputs Andrew_htt_mt.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root dc_sync/MIT-htt_mt.inputs-sm-8TeV.root dc_sync/CERN-htt_mt.inputs-sm-8TeV.root dc_sync/LLR-htt_mt.inputs-sm-8TeV.root \
# --labels Imperial Wisconsin MIT CERN LLR/Saclay --category muTau_vbf --output_file=dc_sync/compare_vbf_mt-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs Andrew_htt_mt.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root dc_sync/MIT-htt_mt.inputs-sm-8TeV.root dc_sync/CERN-htt_mt.inputs-sm-8TeV.root dc_sync/LLR-htt_mt.inputs-sm-8TeV.root \
# --labels Imperial Wisconsin MIT CERN LLR/Saclay --category muTau_boost_high --output_file=dc_sync/compare_boost_high_mt-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs Andrew_htt_mt.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root dc_sync/MIT-htt_mt.inputs-sm-8TeV.root dc_sync/CERN-htt_mt.inputs-sm-8TeV.root dc_sync/LLR-htt_mt.inputs-sm-8TeV.root \
# --labels Imperial Wisconsin MIT CERN LLR/Saclay --category muTau_boost_low --output_file=dc_sync/compare_boost_low_mt-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs Andrew_htt_mt.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root dc_sync/MIT-htt_mt.inputs-sm-8TeV.root dc_sync/CERN-htt_mt.inputs-sm-8TeV.root dc_sync/LLR-htt_mt.inputs-sm-8TeV.root \
# --labels Imperial Wisconsin MIT CERN LLR/Saclay --category muTau_0jet_high --output_file=dc_sync/compare_0jet_high_mt-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs Andrew_htt_mt.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root dc_sync/MIT-htt_mt.inputs-sm-8TeV.root dc_sync/CERN-htt_mt.inputs-sm-8TeV.root dc_sync/LLR-htt_mt.inputs-sm-8TeV.root \
# --labels Imperial Wisconsin MIT CERN LLR/Saclay --category muTau_0jet_low --output_file=dc_sync/compare_0jet_low_mt-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs Andrew_htt_mt.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root dc_sync/MIT-htt_mt.inputs-sm-8TeV.root dc_sync/CERN-htt_mt.inputs-sm-8TeV.root dc_sync/LLR-htt_mt.inputs-sm-8TeV.root \
# --labels Imperial Wisconsin MIT CERN LLR/Saclay --category muTau_btag_low --output_file=dc_sync/compare_btag_low_mt-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs Andrew_htt_et.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_et.inputs-sm-8TeV.root dc_sync/MIT-htt_et.inputs-sm-8TeV.root \
# --labels Imperial Wisconsin MIT --category eleTau_vbf --output_file=dc_sync/compare_vbf_et-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs Andrew_htt_et.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_et.inputs-sm-8TeV.root dc_sync/MIT-htt_et.inputs-sm-8TeV.root \
# --labels Imperial Wisconsin MIT --category eleTau_boost_high --output_file=dc_sync/compare_boost_high_et-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs Andrew_htt_et.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_et.inputs-sm-8TeV.root dc_sync/MIT-htt_et.inputs-sm-8TeV.root \
# --labels Imperial Wisconsin MIT --category eleTau_boost_low --output_file=dc_sync/compare_boost_low_et-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs Andrew_htt_et.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_et.inputs-sm-8TeV.root dc_sync/MIT-htt_et.inputs-sm-8TeV.root \
# --labels Imperial Wisconsin MIT --category eleTau_0jet_high --output_file=dc_sync/compare_0jet_high_et-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs Andrew_htt_et.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_et.inputs-sm-8TeV.root dc_sync/MIT-htt_et.inputs-sm-8TeV.root \
# --labels Imperial Wisconsin MIT --category eleTau_0jet_low --output_file=dc_sync/compare_0jet_low_et-sm-8TeV.pdf  





#  ../bin/DatacardCompare --inputs htt_mt.inputs-sm-8TeV.root  \
#  --labels Yields --category muTau_inclusive --output_file=dc_sync/compare_inclusive_mt-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs htt_mt.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root dc_sync/CERN-htt_mt.inputs-sm-8TeV.root  \
# --labels Imperial Wisconsin CERN  --category muTau_vbf --output_file=dc_sync/ic_wisc_vbf_mt-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs htt_mt.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root dc_sync/CERN-htt_mt.inputs-sm-8TeV.root  \
# --labels Imperial Wisconsin CERN --category muTau_boost_high --output_file=dc_sync/ic_wisc_boost_high_mt-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs htt_mt.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root dc_sync/CERN-htt_mt.inputs-sm-8TeV.root  \
# --labels Imperial Wisconsin CERN --category muTau_boost_low --output_file=dc_sync/ic_wisc_boost_low_mt-sm-8TeV.pdf      

# ../bin/DatacardCompare --inputs htt_mt.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root dc_sync/CERN-htt_mt.inputs-sm-8TeV.root  \
# --labels Imperial Wisconsin CERN --category muTau_0jet_high --output_file=dc_sync/ic_wisc_0jet_high_mt-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs htt_mt.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_mt.inputs-sm-8TeV.root dc_sync/CERN-htt_mt.inputs-sm-8TeV.root  \
# --labels Imperial Wisconsin CERN --category muTau_0jet_low --output_file=dc_sync/ic_wisc_0jet_low_mt-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs htt_mt.inputs-sm-8TeV.root  \
# --labels Yields --category muTau_btag_high --output_file=dc_sync/compare_btag_high_mt-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs htt_mt.inputs-sm-8TeV.root  \
# --labels Yields --category muTau_btag_low --output_file=dc_sync/compare_btag_low_mt-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs htt_et.inputs-sm-8TeV.root  \
# --labels Yields --category eleTau_inclusive --output_file=dc_sync/compare_inclusive_et-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs htt_et.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_et.inputs-sm-8TeV.root  \
# --labels Imperial Wisconsin   --category eleTau_vbf --output_file=dc_sync/ic_wisc_vbf_et-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs htt_et.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_et.inputs-sm-8TeV.root  \
# --labels Imperial Wisconsin  --category eleTau_boost_high --output_file=dc_sync/ic_wisc_boost_high_et-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs htt_et.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_et.inputs-sm-8TeV.root  \
# --labels Imperial Wisconsin  --category eleTau_boost_low --output_file=dc_sync/ic_wisc_boost_low_et-sm-8TeV.pdf      

# ../bin/DatacardCompare --inputs htt_et.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_et.inputs-sm-8TeV.root  \
# --labels Imperial Wisconsin  --category eleTau_0jet_high --output_file=dc_sync/ic_wisc_0jet_high_et-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs htt_et.inputs-sm-8TeV.root dc_sync/Wisconsin-htt_et.inputs-sm-8TeV.root  \
# --labels Imperial Wisconsin  --category eleTau_0jet_low --output_file=dc_sync/ic_wisc_0jet_low_et-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs htt_et.inputs-sm-8TeV.root  \
# --labels Yields --category eleTau_btag_high --output_file=dc_sync/compare_btag_high_et-sm-8TeV.pdf

# ../bin/DatacardCompare --inputs htt_et.inputs-sm-8TeV.root  \
# --labels Yields --category eleTau_btag_low --output_file=dc_sync/compare_btag_low_et-sm-8TeV.pdf


# ../bin/DatacardCompare --mode=2 --inputs htt_em.inputs-sm-8TeV-mvis.root dc_sync/htt_em.inputs-sm-8TeV.root  \
# --labels Imperial MIT   --category emu_vbf --output_file=dc_sync/ic_mit_vbf_em-sm-8TeV.pdf

# ../bin/DatacardCompare --mode=2 --inputs htt_em.inputs-sm-8TeV-mvis.root dc_sync/htt_em.inputs-sm-8TeV.root  \
# --labels Imperial MIT  --category emu_boost_high --output_file=dc_sync/ic_mit_boost_high_em-sm-8TeV.pdf

# ../bin/DatacardCompare --mode=2 --inputs htt_em.inputs-sm-8TeV-mvis.root dc_sync/htt_em.inputs-sm-8TeV.root  \
# --labels Imperial MIT  --category emu_boost_low --output_file=dc_sync/ic_mit_boost_low_em-sm-8TeV.pdf      

# ../bin/DatacardCompare --mode=2 --inputs htt_em.inputs-sm-8TeV-mvis.root dc_sync/htt_em.inputs-sm-8TeV.root  \
# --labels Imperial MIT  --category emu_0jet_high --output_file=dc_sync/ic_mit_0jet_high_em-sm-8TeV.pdf

# ../bin/DatacardCompare --mode=2 --inputs htt_em.inputs-sm-8TeV-mvis.root dc_sync/htt_em.inputs-sm-8TeV.root  \
# --labels Imperial MIT  --category emu_0jet_low --output_file=dc_sync/ic_mit_0jet_low_em-sm-8TeV.pdf
  


 

