import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
process = cms.Process("MAIN")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")


process.MessageLogger.cerr.FwkReport.reportEvery = 1

process.source = cms.Source(
  "PoolSource",
  fileNames = cms.untracked.vstring(
    #'file:/Volumes/Storage/samples/TauPlusX-2012C-PromptReco-v1-Sample.root'
    'root://eoscms//eos/cms/store/user/agilbert/samples/VBF_HToTauTau_M-125-53X.root'
  )
)

process.GlobalTag.globaltag = cms.string('START53_V10::All')
# process.GlobalTag.globaltag = cms.string('GR_P_V41_AN1::All')

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5) )

# Load the tau sequences
process.load("RecoTauTag.Configuration.RecoPFTauTag_cff")

# Load the MET sequences
process.load("JetMETCorrections.Type1MET.pfMETCorrections_cff")
## Switch for data!
## process.pfJetMETcorr.jetCorrLabel = cms.string("ak5PFL1FastL2L3Residual")

# Load example producer
process.tauMETExample = cms.EDProducer('TauMETExample')


process.p = cms.Path(
  process.recoTauClassicHPSSequence
  +process.producePFMETCorrections
  +process.tauMETExample
  )


