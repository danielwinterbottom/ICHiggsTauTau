cd shapes
echo "Comparisons made as of git commit:"
echo "https://github.com/roger-wolf/HiggsAnalysis-HiggsToTauTau-auxiliaries/commits/"`git rev-parse HEAD`
#echo "Git commits in the past 48 hours:"
#git log --since="48 hours ago"
cd ../
#echo "******** e-mu 8TeV MSSM"
#./scripts/compare_mssm_datacards.sh emu 2 htt_em.inputs-mssm-8TeV-0.root shapes/Imperial/htt_em.inputs-mssm-8TeV-0.root mycheck_em_mssm_8TeV 1 New Old
#echo "******** mu-tau 8TeV MSSM"
#./scripts/compare_mssm_datacards.sh muTau 0 htt_mt.inputs-mssm-8TeV-0.root shapes/Imperial/htt_mt.inputs-mssm-8TeV-0.root mycheck_mt_mssm_8TeV 1 New Old
#echo "******** e-tau 8TeV MSSM"
#./scripts/compare_mssm_datacards.sh eleTau 0 htt_et.inputs-mssm-8TeV-0.root shapes/Imperial/htt_et.inputs-mssm-8TeV-0.root mycheck_et_mssm_8TeV 1 New Old

#echo "******** e-mu 7TeV MSSM"
#./scripts/compare_mssm_datacards.sh emu 2 htt_em.inputs-mssm-7TeV-0.root shapes/Imperial/htt_em.inputs-mssm-7TeV-0.root mycheck_em_mssm_7TeV 1 New Old
#echo "******** mu-tau 7TeV MSSM"
#./scripts/compare_mssm_datacards.sh muTau 0 htt_mt.inputs-mssm-7TeV-0.root shapes/Imperial/htt_mt.inputs-mssm-7TeV-0.root mycheck_mt_mssm_7TeV 1 New Old 
#echo "******** e-tau 7TeV MSSM"
#./scripts/compare_mssm_datacards.sh eleTau 0 htt_et.inputs-mssm-7TeV-0.root shapes/Imperial/htt_et.inputs-mssm-7TeV-0.root mycheck_et_mssm_7TeV 1 New Old

echo "******** e-mu 8TeV SM"
./scripts/compare_new_em.sh emu 2 htt_em.inputs-sm-8TeV.root shapes/Imperial/htt_em.inputs-sm-8TeV.root mycheck_em_sm_8TeV 1 New Old
echo "******** mu-tau 8TeV SM"
./scripts/compare_new_et_mt_2012.sh muTau 0 htt_mt.inputs-sm-8TeV.root shapes/Imperial/htt_mt.inputs-sm-8TeV.root mycheck_mt_sm_8TeV 1 New Old 
echo "******** e-tau 8TeV SM"
./scripts/compare_new_et_mt_2012.sh eleTau 0 htt_et.inputs-sm-8TeV.root shapes/Imperial/htt_et.inputs-sm-8TeV.root mycheck_et_sm_8TeV 1 New Old

echo "******** e-mu 7TeV SM"
./scripts/compare_new_em.sh emu 2 htt_em.inputs-sm-7TeV.root shapes/Imperial/htt_em.inputs-sm-7TeV.root mycheck_em_sm_7TeV 1 New Old
echo "******** mu-tau 7TeV SM"
./scripts/compare_new_et_mt_2012.sh muTau 0 htt_mt.inputs-sm-7TeV.root shapes/Imperial/htt_mt.inputs-sm-7TeV.root mycheck_mt_sm_7TeV 1 New Old
echo "******** e-tau 7TeV SM"
./scripts/compare_new_et_mt_2012.sh eleTau 0 htt_et.inputs-sm-7TeV.root shapes/Imperial/htt_et.inputs-sm-7TeV.root mycheck_et_sm_7TeV 1 New Old
