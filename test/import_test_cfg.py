import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.MessageLogger.cerr.FwkReport.reportEvery = 50
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring('file:/Volumes/HDD/DYJetsToLL.root')
)
process.GlobalTag.globaltag = cms.string('START53_V22::All')

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )

import UserCode.ICHiggsTauTau.default_producers_cfi as defaults

process.icElectronProducer = defaults.icElectronProducer.clone(

)

process.icEventProducer = defaults.icEventProducer.clone()

process.p = cms.Path(
  process.icElectronProducer+
  process.icEventProducer
)

#print process.dumpPython()