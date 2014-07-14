Definition: ic::Electron

CMSSW Producer: ICElectronProducer

The ic::Electron class can be used with either particle-flow or gsf electrons.
The producer is configured like this:

~~~~~~~~~~~~~py
process.icElectronProducer = cms.EDProducer('ICElectronProducer',
    input = cms.InputTag("gsfElectrons"),
    branchName = cms.string("electrons"),
    minPt = cms.double(9.5),
    maxEta = cms.double(2.6),
    vertexCollection = cms.InputTag("goodOfflinePrimaryVertices"),
    pfIsoPostfix = cms.string("PFIso"),
    isPF = cms.bool(False)
)
~~~~~~~~~~~~~
 The `vertexCollection` is used to to calculate impact parameters of the electron track with respect to the first vertex in the given collection.  In this example a pre-filtered vertex collection is used. The `pfIsoPostifx` is a string that helps the module locate the correct particle-flow isolation values in the event.  If these isolation values are not found in the event they will be silently ignored by the module. See the section on particle-flow isolation for further details. The `isPF` flag determines the type of the input, PF if `True` and gsfElectron if `False`.  Note that currently the use of PF Electrons is not widespread in CMS, and this module will do nothing if the flag is set to `True`.  It is intended that support will be added when required.

Default information:

Name          | Getter        | CMSSW Equivalent
------------- | ------------- | -------------
Four-momentum | ic::Candidate::vector() | reco::Candidate::p4()
HCAL tower energy sum (\f$\Delta\mathrm{R} = 0.3\f$)  | ic::Electron::dr03_hcal_tower_sum_et() | reco::GsfElectron::dr03HcalTowerSumEt()
ECAL rechit energy sum (\f$\Delta\mathrm{R} = 0.3\f$)  | ic::Electron::dr03_ecal_rechit_sum_et() | reco::GsfElectron::dr03EcalRecHitSumEt()
Track \f$p_{\mathrm T}\f$ sum (\f$\Delta\mathrm{R} = 0.3\f$)  | ic::Electron::dr03_tk_sum_pt() | reco::GsfElectron::dr03TkSumPt()

\subsection photons Photons

Definition: ic::Photon

CMSSW Producer: ICPhotonProducer

The producer currently support the `reco::Photon` class as input only. Example usage:

~~~~~~~~~~~~~py
process.icPhotonProducer = cms.EDProducer("ICPhotonProducer",
  input = cms.InputTag("photons"),
  branchName = cms.string("photons"),
  minPt = cms.double(5.0),
  maxEta = cms.double(3.0)
)
~~~~~~~~~~~~~

Details on the cut-based photon ID recommendation can be found here: https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonID2012.
For technical information about the PF photon isolation see https://twiki.cern.ch/twiki/bin/view/CMS/EgammaPFBasedIsolation#Example_for_photons.
A rho-based pileup subtraction is used for the isolation sum.  The rho value can be retrieved from the ic::EventInfo class.


\subsection muons Muons

Definition: ic::Muon

CMSSW Producer: ICMuonProducer

The ic::Muon class can be used with either particle-flow or standard reco muons.
The producer is configured like this:

~~~~~~~~~~~~~py
		process.icMuonProducer = cms.EDProducer('ICMuonProducer',
		    input = cms.InputTag("pfAllMuons"),
		    branchName = cms.string("electrons"),
		    minPt = cms.double(3.0),
		    maxEta = cms.double(2.6),
		    vertexCollection = cms.InputTag("goodOfflinePrimaryVertices"),
		    pfIsoPostfix = cms.string("PFIso"),
		    isPF = cms.bool(True)
		)
~~~~~~~~~~~~~

The `vertexCollection` is used to to calculate impact parameters of the muon 
track with respect to the first vertex in the given collection.  In this example
a pre-filtered vertex collection is used. The `pfIsoPostifx` is a string that helps
the module locate the correct particle-flow isolation values in the event.  If
these isolation values are not found in the event they will be silently ignored by
the module. See the section on particle-flow isolation for further details.
The `isPF` flag determines the type of the input, PF if `True` and 
gsfElectron if `False`. Important:  For a PF input the module expects that all
PFCandidates will be PF Muons
