cd shapes
echo "Comparisons made as of git commit:"
echo "https://github.com/roger-wolf/HiggsAnalysis-HiggsToTauTau-auxiliaries/commits/"`git rev-parse HEAD`
#echo "Git commits in the past 48 hours:"
#git log --since="48 hours ago"
cd ../
#echo "******** tau-tau 8TeV MSSM"
#./scripts/compare_mssm_datacards.sh tauTau 0 shapes/MIT/htt_tt.inputs-mssm-8TeV-0.root shapes/CERN/htt_tt.inputs-mssm-8TeV-0.root mit_cern_tt_mssm_8TeV 1 MIT CERN
#echo "******** e-mu 8TeV MSSM"
#./scripts/compare_mssm_datacards.sh emu 2 shapes/MIT/htt_em.inputs-mssm-8TeV-0.root shapes/Imperial/htt_em.inputs-mssm-8TeV-0.root mit_ic_em_mssm_8TeV 1 MIT Imperial
#echo "******** mu-tau 8TeV MSSM"
#./scripts/compare_mssm_datacards.sh muTau 0 shapes/Wisconsin/htt_mt.inputs-mssm-8TeV-0.root shapes/Imperial/htt_mt.inputs-mssm-8TeV-0.root wisconsin_ic_mt_mssm_8TeV 1 Wisconsin Imperial
#./scripts/compare_mssm_datacards.sh muTau 0 shapes/Wisconsin/htt_mt.inputs-mssm-8TeV-0.root shapes/MIT/htt_mt.inputs-mssm-8TeV-0.root wisconsin_mit_mt_mssm_8TeV 1 Wisconsin MIT
#./scripts/compare_mssm_datacards.sh muTau 0 shapes/MIT/htt_mt.inputs-mssm-8TeV-0.root shapes/Imperial/htt_mt.inputs-mssm-8TeV-0.root mit_ic_mt_mssm_8TeV 1 MIT Imperial
#echo "******** e-tau 8TeV MSSM"
#./scripts/compare_mssm_datacards.sh eleTau 0 shapes/Wisconsin/htt_et.inputs-mssm-8TeV-0.root shapes/Imperial/htt_et.inputs-mssm-8TeV-0.root wisconsin_ic_et_mssm_8TeV 1 Wisconsin Imperial
#./scripts/compare_mssm_datacards.sh eleTau 0 shapes/Wisconsin/htt_et.inputs-mssm-8TeV-0.root shapes/MIT/htt_et.inputs-mssm-8TeV-0.root wisconsin_mit_et_mssm_8TeV 1 Wisconsin MIT
#./scripts/compare_mssm_datacards.sh eleTau 0 shapes/MIT/htt_et.inputs-mssm-8TeV-0.root shapes/Imperial/htt_et.inputs-mssm-8TeV-0.root mit_ic_et_mssm_8TeV 1 MIT Imperial
#
#echo "******** e-mu 7TeV MSSM"
#./scripts/compare_mssm_datacards.sh emu 2 shapes/MIT/htt_em.inputs-mssm-7TeV-0.root shapes/Imperial/htt_em.inputs-mssm-7TeV-0.root mit_ic_em_mssm_7TeV 1 MIT Imperial
#echo "******** mu-tau 7TeV MSSM"
#./scripts/compare_mssm_datacards.sh muTau 0 shapes/Wisconsin/htt_mt.inputs-mssm-7TeV-0.root shapes/Imperial/htt_mt.inputs-mssm-7TeV-0.root wisconsin_ic_mt_mssm_7TeV 1 Wisconsin Imperial
#echo "******** e-tau 7TeV MSSM"
#./scripts/compare_mssm_datacards.sh eleTau 0 shapes/Wisconsin/htt_et.inputs-mssm-7TeV-0.root shapes/Imperial/htt_et.inputs-mssm-7TeV-0.root wisconsin_ic_et_mssm_7TeV 1 Wisconsin Imperial
#
echo "******** e-mu 8TeV SM"
./scripts/compare_new_em.sh emu 2 shapes/MIT/htt_em.inputs-sm-8TeV.root shapes/Imperial/htt_em.inputs-sm-8TeV.root mit_ic_em_sm_8TeV 1 MIT Imperial
echo "******** mu-tau 8TeV SM"
./scripts/compare_new_et_mt_2012.sh muTau 0 shapes/Wisconsin/htt_mt.inputs-sm-8TeV.root shapes/Imperial/htt_mt.inputs-sm-8TeV.root wisconsin_ic_mt_sm_8TeV 1 Wisconsin Imperial
echo "******** e-tau 8TeV SM"
./scripts/compare_new_et_mt_2012.sh eleTau 0 shapes/Wisconsin/htt_et.inputs-sm-8TeV.root shapes/Imperial/htt_et.inputs-sm-8TeV.root wisconsin_ic_et_sm_8TeV 1 Wisconsin Imperial

echo "******** e-mu 7TeV SM"
./scripts/compare_new_em.sh emu 2 shapes/MIT/htt_em.inputs-sm-7TeV.root shapes/Imperial/htt_em.inputs-sm-7TeV.root mit_ic_em_sm_7TeV 1 MIT Imperial
echo "******** mu-tau 7TeV SM"
./scripts/compare_new_et_mt_2012.sh muTau 0 shapes/Wisconsin/htt_mt.inputs-sm-7TeV.root shapes/Imperial/htt_mt.inputs-sm-7TeV.root wisconsin_ic_mt_sm_7TeV 1 Wisconsin Imperial
echo "******** e-tau 7TeV SM"
./scripts/compare_new_et_mt_2012.sh eleTau 0 shapes/Wisconsin/htt_et.inputs-sm-7TeV.root shapes/Imperial/htt_et.inputs-sm-7TeV.root wisconsin_ic_et_sm_7TeV 1 Wisconsin Imperial

#echo "******** mu-tau-soft 8TeV SM"
#./scripts/compare_mtmet_2012.sh muTau_soft 0 shapes/LLR/htt_mt.inputs-sm-8TeV-soft.root shapes/Imperial/htt_mt.inputs-sm-8TeV-soft.root llr_ic_mt_sm_8TeV 1 LLR Imperial
