# -*- coding: utf-8 -*-
import FWCore.ParameterSet.Config as cms

  
from Configuration.Generator.PythiaUESettings_cfi import *

TauolaNoPolar = cms.PSet(
    UseTauolaPolarization = cms.bool(False)
)
TauolaPolar = cms.PSet(
   UseTauolaPolarization = cms.bool(True)
)


from TauAnalysis.MCEmbeddingTools.MCParticleReplacer_cfi import *
newSource.algorithm = "ZTauTau"
newSource.ZTauTau.TauolaOptions.InputCards.mdtau = cms.int32(0)
newSource.ZTauTau.minVisibleTransverseMomentum = cms.untracked.string("")
newSource.ZTauTau.transformationMode = cms.untracked.int32(3)

source = cms.Source("PoolSource",
        skipEvents = cms.untracked.uint32(0),
        fileNames = cms.untracked.vstring(
    'file:/afs/cern.ch/work/a/amagnan/CMSSW_5_3_11_patch6/src/UserCode/ICHiggsTauTau/python/testFile_run2012A.root'
    )
)

filterEmptyEv = cms.EDFilter("EmptyEventsFilter",
    minEvents = cms.untracked.int32(1),
    target =  cms.untracked.int32(1),
    src = cms.untracked.InputTag("generator")
                             )

adaptedMuonsFromWmunu = cms.EDProducer("PATMuonMETAdapter",
   patMuCands = cms.untracked.InputTag("goodMuonsPFIso"),
   patMet = cms.untracked.InputTag("pfMet"),
   outputCol = cms.untracked.InputTag("adaptedMuonsFromWmunu","wMusExtracted")                   
)

# Removes input muon from tracks and PF candidate collections
removedInputMuons = cms.EDProducer('ZmumuPFEmbedder',
    tracks = cms.InputTag("generalTracks"),
    selectedMuons = cms.InputTag("goodMuonsPFIso"),
    keepMuonTrack = cms.bool(False),
    useCombinedCandidate = cms.untracked.bool(False),
)

generator = newSource.clone()
generator.src = cms.InputTag("adaptedMuonsFromWmunu","wMusExtracted")

ProductionFilterSequence = cms.Sequence(adaptedMuonsFromWmunu*removedInputMuons*generator*filterEmptyEv)
#ProductionFilterSequence = cms.Sequence(removedInputMuons*generator*filterEmptyEv)
