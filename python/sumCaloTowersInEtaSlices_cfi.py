import FWCore.ParameterSet.Config as cms

etaBinsForResidualCorr = [
    # NOTE: binning needs to match exactly the binning used for data/MC residual CaloJet energy corrections
    -5.191, -2.964, -2.853, -2.5, -2.411, -2.322, -1.93, -1.479, -1.305, -1.131, -0.957, -0.783, -0.522, -0.261, 0.,
    0.261, 0.522, 0.783, 0.957, 1.131, 1.305, 1.479, 1.93, 2.322, 2.411, 2.5, 2.853, 2.964, 5.191
]
numEtaBinsForResidualCorr = len(etaBinsForResidualCorr) - 1

#--------------------------------------------------------------------------------
# NOTE: this part of the config need to be run during PAT-tuple production
#
#       Uou need to add a
#         'keep *_sumCaloTowersInEtaSlicesNoHF_*_*'
#       statememt when defining the event content.
#
sumCaloTowersInEtaSlicesNoHF = cms.EDProducer("CaloTowerMETcorrInputProducer",
    src = cms.InputTag('towerMaker'),
    residualCorrLabel = cms.string(""),
    residualCorrEtaMax = cms.double(9.9),
    residualCorrOffset = cms.double(0.),
    globalThreshold = cms.double(0.3), # NOTE: this value need to match met.globalThreshold, defined in RecoMET/METProducers/python/CaloMET_cfi.py
    noHF = cms.bool(True)
)
etaBinniningConfigs = []
for idxEtaBinForResidualCorr in range(numEtaBinsForResidualCorr):
    binEdgeLow = etaBinsForResidualCorr[idxEtaBinForResidualCorr]
    binEdgeHigh = etaBinsForResidualCorr[idxEtaBinForResidualCorr + 1]
    binLabel = "eta%1.3ftoeta%1.3f" % (binEdgeLow, binEdgeHigh)
    binLabel = binLabel.replace(".", "p")
    binLabel = binLabel.replace("+", "P")
    binLabel = binLabel.replace("-", "M")
    binSelection = "eta >= %f & eta < %f" % (binEdgeLow, binEdgeHigh)
    etaBinniningConfig = cms.PSet(
        binLabel = cms.string(binLabel),
        binSelection = cms.string(binSelection)
    )
    etaBinniningConfigs.append(etaBinniningConfig)
sumCaloTowersInEtaSlicesNoHF.binning = cms.VPSet(etaBinniningConfigs)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# NOTE: this part of the config need to be run during Tree production,
#       using the 'sumCaloTowersInEtaSlicesNoHF' collection stored in the PAT-tuple as input
#
etaBinniningConfigs2 = []
metNoHFresidualCorrected = cms.EDProducer("CaloMEtFromEtaSliceSumsProducer",
    srcMEt = cms.InputTag('metNoHF'),
    srcCaloTowerEtaSliceSums = cms.InputTag('sumCaloTowersInEtaSlicesNoHF'),                                      
    residualCorrLabel = cms.string("ak5CaloResidual"), # apply data/MC residual correction
    ##residualCorrLabel = cms.string(""), # do not apply data/MC residual correction                                    
    residualCorrEtaMax = cms.double(9.9),
    extraCorrFactor = cms.double(1.0),                                       
    isMC = cms.bool(True),
    verbosity = cms.int32(0)                                          
)                                          
for idxEtaBinForResidualCorr in range(numEtaBinsForResidualCorr):
    binEdgeLow = etaBinsForResidualCorr[idxEtaBinForResidualCorr]
    binEdgeHigh = etaBinsForResidualCorr[idxEtaBinForResidualCorr + 1]
    binLabel = "eta%1.3ftoeta%1.3f" % (binEdgeLow, binEdgeHigh)
    binLabel = binLabel.replace(".", "p")
    binLabel = binLabel.replace("+", "P")
    binLabel = binLabel.replace("-", "M")
    binCenter = 0.5*(binEdgeLow + binEdgeHigh)
    etaBinniningConfig2 = cms.PSet(
        binLabel = cms.string(binLabel),
        binCenter = cms.double(binCenter)
    )
    etaBinniningConfigs2.append(etaBinniningConfig2)
metNoHFresidualCorrected.binning = cms.VPSet(etaBinniningConfigs2)
#--------------------------------------------------------------------------------                                
