import FWCore.ParameterSet.Config as cms

from RecoMET.METPUSubtraction.MVAMETConfiguration_cff import runMVAMET

def configureMVAMET(process, jetCollectionPF, isData):
    runMVAMET(process, jetCollectionPF=jetCollectionPF)
    process.MVAMET.srcLeptons = cms.VInputTag("slimmedMuons", "slimmedElectrons", "slimmedTaus")
    process.MVAMET.requireOS = cms.bool(False)

    process.icMvaMetSequence = cms.Sequence(
        process.tauDecayProducts+
        process.egmGsfElectronIDs+
        process.electronMVAValueMapProducer+
        process.electronRegressionValueMapProducer+
        process.photonIDValueMapProducer+
        process.photonMVAValueMapProducer+
        process.slimmedElectronsTight+
        process.slimmedMuonsTight+
        process.slimmedTausLoose+
        process.slimmedTausLooseCleaned+
        process.selectedUpdatedPatJetsUpdatedJECCleaned+
        process.pfNeutrals+
        process.neutralInJets+
        process.pfChargedPV+
        process.pfChs+
        process.pfChargedPU+
        process.pfMETCands+
        process.pfTrackMETCands+
        process.pfNoPUMETCands+
        process.pfPUMETCands+
        process.pfChargedPUMETCands+
        process.pfNeutralPUMETCands+
        process.pfNeutralPVMETCands+
        process.pfNeutralUnclusteredMETCands+
        process.pfPUCorrectedMETCands+
        process.ak4PFCHSL1FastjetCorrector+
        process.ak4PFCHSL2RelativeCorrector+
        process.ak4PFCHSL3AbsoluteCorrector+
        process.ak4PFCHSL1FastL2L3Corrector+
        process.ak4PFCHSResidualCorrector+
        process.ak4PFCHSL1FastL2L3ResidualCorrector+
        process.ak4JetsForpfMET+
        process.ak4JetsForpfTrackMET+
        process.ak4JetsForpfPUMET+
        process.ak4JetsForpfChargedPUMET+
        process.ak4JetsForpfNeutralPUMET+
        process.ak4JetsForpfNeutralPVMET+
        process.ak4JetsForpfNeutralUnclusteredMET+
        process.ak4JetsForpfPUCorrectedMET+
        process.ak4JetsForpfNoPUMET+
        process.corrpfMET+
        process.corrpfTrackMET+
        process.corrpfPUMET+
        process.corrpfChargedPUMET+
        process.corrpfNeutralPVMET+
        process.corrpfNeutralUnclusteredMET+
        process.corrpfNeutralPUMET+
        process.corrpfPUCorrectedMET+
        process.corrpfNoPUMET+
        process.pfMET+
        process.pfMETT1+
        process.patpfMET+
        process.pfTrackMET+
        process.patpfTrackMET+
        process.pfTrackMETT1+
        process.patpfTrackMETT1+
        process.pfPUCorrectedMET+
        process.pfPUMET+
        process.pfChargedPUMET+
        process.pfNeutralPUMET+
        process.patpfPUMET+
        process.patpfChargedPUMET+
        process.patpfNeutralPUMET+
        process.pfNeutralPVMET+
        process.pfNeutralUnclusteredMET+
        process.patpfNeutralPVMET+
        process.patpfNeutralUnclusteredMET+
        process.patpfPUCorrectedMET+
        process.pfPUMETT1+
        process.patpfPUMETT1+
        process.pfChargedPUMETT1+
        process.pfNeutralPUMETT1+
        process.patpfChargedPUMETT1+
        process.patpfNeutralPUMETT1+
        process.pfPUCorrectedMETT1+
        process.patpfPUCorrectedMETT1+
        process.pfNoPUMET+
        process.patpfNoPUMET+
        process.pfNoPUMETT1+
        process.patpfNoPUMETT1+
        process.patpfMETT1+
        process.allDecayProducts+
        process.tauPFMET+
        process.tauMET+
        process.tausSignificance+
        process.MVAMET
    )

    if not isData:
        process.icMvaMetSequence.remove(process.ak4PFCHSResidualCorrector)
        process.icMvaMetSequence.remove(process.ak4PFCHSL1FastL2L3ResidualCorrector)
