export PATH=$HOME/.local/bin:/afs/cern.ch/cms/lumi/brilconda-1.0.3/bin:$PATH (bash)
export PATH=$HOME/.local/bin:/afs/cern.ch/cms/lumi/brilconda-1.0.3/bin:$PATH
pip install --install-option="--prefix=$HOME/.local" brilws
pip show brilws
#pip install --upgrade pip
brilcalc lumi -i 151030/MET/crab_SinglePhoton-2015D-05Oct-v1/results/lumiSummary.json -u /pb --normtag /afs/cern.ch/user/c/cmsbril/public/normtag_json/OfflineNormtagV1.json
