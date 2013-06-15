#LABEL="Inclusive"
LABEL="20 < p_{T}^{#tau} < 30"
FILENAME="pt_20-30_pt_tt_50"
/Users/Andrew/Code/scripts/cmssw_env_107.sh
eval `scramv1 runtime -sh`
./bin/EmbeddedStudyPlots --var="m_vis(50,0,250)" --cat="pt_2>20. && pt_2<30. && pt_tt>50." --datacard="muTau_inclusive"
mv datacard_muTau_inclusive.root output/TauID_2012/htt_mt.input_8TeV.root

cd /Users/Andrew/Code/CMSSW_6_1_1/src/
/Users/Andrew/Code/scripts/cmssw_env_108.sh
eval `scramv1 runtime -sh`
create-datacard.py -i TauID_2012/htt_mt.input_8TeV.root -u setups_tauid/unc-sm-8TeV-00.vals -c setups_tauid/cgs-sm-8TeV-00.conf -d setups_tauid/unc-sm-8TeV-00.conf -o TauID_2012/htt_mt_0_8TeV.txt 125
text2workspace.py TauID_2012/htt_mt_0_8TeV.txt -o tmp.root -m 125
combine -M MaxLikelihoodFit tmp.root -v 2 --out=TauID_2012/fitoutput/ --robustFit=1 --stepSize=0.5  --minimizerStrategy=0 --minimizerTolerance=0.1 --preFitValue=0.1  --X-rtd FITTER_DYN_STEP  --cminFallbackAlgo="Minuit;0.001"
python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py -A -a -f text TauID_2012/fitoutput/mlfit.root > TauID_2012/fitoutput/mlfit.txt

cd -
/Users/Andrew/Code/scripts/cmssw_env_107.sh
eval `scramv1 runtime -sh`
./bin/PostFitPlot --channel=mt --selection="$LABEL:0" --eras="8TeV" --title_left="CMS Preliminary, #sqrt{s} = 8 TeV, L = 19.4 fb^{-1}" --datacard_path=output/TauID_2012/ --root_file_path=output/TauID_2012/ --pulls_file=output/TauID_2012/fitoutput/mlfit.txt --signal_mass=125 --postfit=false
./bin/PostFitPlot --channel=mt --selection="$LABEL:0" --eras="8TeV" --title_left="CMS Preliminary, #sqrt{s} = 8 TeV, L = 19.4 fb^{-1}" --datacard_path=output/TauID_2012/ --root_file_path=output/TauID_2012/ --pulls_file=output/TauID_2012/fitoutput/mlfit.txt --signal_mass=125 --postfit=true
mv prefit_mt_0.pdf prefit_mt_0_$FILENAME.pdf
mv postfit_mt_0.pdf postfit_mt_0_$FILENAME.pdf
open prefit_mt_0_$FILENAME.pdf postfit_mt_0_$FILENAME.pdf
grep eff_t output/TauID_2012/fitoutput/mlfit.txt
echo $FILENAME >> tau_results.txt
grep eff_t output/TauID_2012/fitoutput/mlfit.txt >> tau_results.txt
cat output/TauID_2012/fitoutput/mlfit.txt
