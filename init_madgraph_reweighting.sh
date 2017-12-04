# script checks out latest branch of maggraph reweighting tool by Aachen group. required files are copied to the IC directroy and the source code directroy paths are modified accordingly

cwd=$(pwd)
cd $CMSSW_BASE/src
git clone https://github.com/CMSAachen3B/MadGraphReweighting.git CMSAachen3B/MadGraphReweighting
git pull origin master
cd $cwd
cd $CMSSW_BASE/src/UserCode/ICHiggsTauTau

cp -R $CMSSW_BASE/src/CMSAachen3B/MadGraphReweighting/data .
cp -R $CMSSW_BASE/src/CMSAachen3B/MadGraphReweighting/interface .
cp -R $CMSSW_BASE/src/CMSAachen3B/MadGraphReweighting/src .
cp -R $CMSSW_BASE/src/CMSAachen3B/MadGraphReweighting/python .

while read a ; do echo ${a//CMSAachen3B/UserCode} ; done < ./src/MadGraphTools.cc > ./src/MadGraphTools.cc.t ; mv ./src/MadGraphTools.cc{.t,}

while read a ; do echo ${a//MadGraphReweighting/ICHiggsTauTau} ; done < ./src/MadGraphTools.cc > ./src/MadGraphTools.cc.t ; mv ./src/MadGraphTools.cc{.t,}

scramv1 b -j8
cd $cwd
