/*! \mainpage


Objects
=========================



Electrons
-------------------------

Definition: ic::Electron

CMSSW Producer: ICElectronProducer

The ic::Electron class can be used with either particle-flow or gsf electrons.  
The producer is configured like this:

		process.icElectronProducer = cms.EDProducer('ICElectronProducer',
		    input = cms.InputTag("gsfElectrons"),
		    branchName = cms.string("electrons"),
		    minPt = cms.double(9.5),
		    maxEta = cms.double(2.6),
		    vertexCollection = cms.InputTag("goodOfflinePrimaryVertices"),
		    pfIsoPostfix = cms.string("PFIso"),
		    isPF = cms.bool(False)
		)

The `vertexCollection` is used to to calculate impact parameters of the electron 
track with respect to the first vertex in the given collection.  In this example
a pre-filtered vertex collection is used. The `pfIsoPostifx` is a string that helps
the module locate the correct particle-flow isolation values in the event.  If
these isolation values are not found in the event they will be silently ignored by
the module. See the section on particle-flow isolation for further details.
The `isPF` flag determines the type of the input, PF if `True` and 
gsfElectron if `False`.  Note that currently the use of PF Electrons is not widespread
in CMS, and this module will do nothing if the flag is set to `True`.  It is intended
that support will be added when required.

Muons
-------------------------

Definition: ic::Muon

CMSSW Producer: ICMuonProducer

The ic::Muon class can be used with either particle-flow or standard reco muons.  
The producer is configured like this:

		process.icMuonProducer = cms.EDProducer('ICMuonProducer',
		    input = cms.InputTag("pfAllMuons"),
		    branchName = cms.string("electrons"),
		    minPt = cms.double(3.0),
		    maxEta = cms.double(2.6),
		    vertexCollection = cms.InputTag("goodOfflinePrimaryVertices"),
		    pfIsoPostfix = cms.string("PFIso"),
		    isPF = cms.bool(True)
		)

The `vertexCollection` is used to to calculate impact parameters of the muon 
track with respect to the first vertex in the given collection.  In this example
a pre-filtered vertex collection is used. The `pfIsoPostifx` is a string that helps
the module locate the correct particle-flow isolation values in the event.  If
these isolation values are not found in the event they will be silently ignored by
the module. See the section on particle-flow isolation for further details.
The `isPF` flag determines the type of the input, PF if `True` and 
gsfElectron if `False`. Important:  For a PF input the module expects that all
PFCandidates will be PF Muons




*/