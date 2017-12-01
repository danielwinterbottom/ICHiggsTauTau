cwd=$(pwd)
cd $CMSSW_BASE/src
pwd
git clone https://github.com/CMSAachen3B/MadGraphReweighting.git CMSAachen3B/MadGraphReweighting
scram b -j 8
cd $cwd
