# -*- coding: utf-8 -*-
import FWCore.ParameterSet.Config as cms
import os

def customise(process):
 
  process._Process__name="EmbeddedINPUT"

  try:
	  outputModule = process.output
  except:
    pass
  try:
	  outputModule = getattr(process,str(getattr(process,list(process.endpaths)[-1])))
  except:
    pass

  outputModule.outputCommands.extend(
    cms.untracked.vstring("keep *_WmunuCandidatesGe0IsoMuons_*_*",
                          "keep *_WmunuCandidatesGe1IsoMuons_*_*",
                          "keep *_goodMuons*_*_*",))
                          #"keep *_WmunuCandidatesGe2IsoMuons_*_*"))

  process.load('Configuration.StandardSequences.GeometryDB_cff')
  process.load('Configuration.StandardSequences.MagneticField_38T_cff')
  process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
  process.load("UserCode/ICHiggsTauTau/WmunuStandalonSelection_cff")

  # Add mumu selection to schedule
  process.WmunuSkimPath = cms.Path(process.WmunuSelectionSequence)
  process.schedule.insert(0, process.WmunuSkimPath)

  # Only write out events which have at least one muon
  outputModule.SelectEvents = cms.untracked.PSet(
    SelectEvents = cms.vstring('WmunuSkimPath'))

  return(process)
