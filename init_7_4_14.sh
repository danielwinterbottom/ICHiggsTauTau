git cms-addpkg RecoMET
git clone https://github.com/rfriese/RecoMET-METPUSubtraction ../../RecoMET/METPUSubtraction/data -b 74X-13TeV-Summer15-July2015
rm -rf ../../RecoMET/METPUSubtraction/data/.git #necessary as sandbox is too large
sed 's!label = cms.string("full")!label = cms.string("full"),\n\tetaBinnedWeights=cms.bool(False)!' ../../RecoMET/METPUSubtraction/python/mvaPFMET_cff.py >  tmp
mv tmp ../../RecoMET/METPUSubtraction/python/mvaPFMET_cff.py
