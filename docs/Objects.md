Physics Objects {#physics-objects}
==================================

[TOC]

This page documents:

  - The physics object classes available in the framework
  - The CMSSW modules used to produce them
  - Example python configuration for each module
  - The object properties that are not filled by default in each module
  - Recipes for non-standard objects and reconstruction

Use the contents panel on the right to navigate between objects. Each section provides links to the documentation for the object class in question as well as the producer module. To check the list of properties an object class provides, follow the link to the object page. From this page the documentation for each `set_[property]` method will contain a link to the source code of the producer module where this method is called.

An example python configuration for each producer module is also given below. All these snippets are extracted from the file: `python/default_producers_cfi.py`.

The basic configuration for each module is usually the same: the `input` option takes an `edm::InputTag` to the input object collection, and the `branch` option takes a string that will label the branch in the output TTree. Other options typically control the addition of extra object properties that are not filled by default in the module. Reasons for this can vary, but are typically due to one of the following:

  - **The interface to access the property changes between CMSSW releases, or is not present at all in some subset of releases** \n The code to access this property can't be included in the producer, as it would fail to compile in some releases. The solution is often a separate producer for just this property, stored in an independent git repository that can be checked-out as a separate CMSSW package. This repository can then have separate branches for different versions of CMSSW. This separate producer will typically add an `edm::ValueMap<T>` to the event, where `T` is the type of the property in question. An option is then provided in the producer to read load this `ValueMap` and extract the values. There are several examples of this workaround below. These separate producers follow a module naming convention of `IC[Object][Property]Calcuator`, a CMSSW package convention of `ICAnalysis/[Property]Calculator` and git repository convention of `ICAnalysis-[Property]Calculator`.
  - **A new method to calculate some property is introduced in a later CMSSW release, but the possibility to use the old method should be maintained.** \n If both the new and the old method are supported in all CMSSW versions then one possibility is to just add an option to the main producer to switch between methods. Or, if the new method is only available in later releases, a better way is to create a separate producer in a new package as in the previous example. The old method should also be moved to a separate producer. However this may remain within this package as long as it still compiles in all the supported releases.
  - **The property requires several other inputs from the edm::Event** \n In this case to allow maximum flexibility the other inputs must also be specified in the module configuration, and the calculation should remain optional to allow for the case where these inputs are not available in some dataset to be processed.

\note **Producer input collections**: The majority of the CMSSW producer modules listed below use `edm::View` objects to access the input collection. This provides a certain amount of flexibility in the actual type of the input collection. An `edm::View<T>`, where `T` is some object class, can access an `std::vector<T>`, an `edm::RefVector<T>`, or the corresponding collections for some other type `U` which derives from the type `T`.

\warning **Reading ValueMaps**: Because of the way references are stored in `edm::ValueMap` objects these should not be produced using and `edm::RefVector<T>` collection as input, but rather the original `std::vector<T>` collection of concrete objects. For example, even if you intend to ntuple-ise a filtered `edm::RefVector<GsfElectron>` collection, you should still produce the ID and isolation `ValueMap` objects using the original `std::vector<GsfElectron>` collection.

Candidate {#objs-candidate}
===========================
 - **Definition**: ic::Candidate
 - **CMSSW Producer**: ICCandidateProducer
 - **Producer Input**: Any collection compatible with an `edm::View<reco::Candidate>`
 - **Producer Output**: `std::vector<ic::Candidate>`

**Example usage**
@snippet python/default_producers_cfi.py Candidate

PFCandidate {#objs-pf-candidate}
===========================
 - **Definition**: ic::PFCandidate
 - **CMSSW Producers**: ICPFProducer, ICPFFromPackedProducer
 - **Producer Input**: Either a collection compatible with an `edm::View<reco::PFCandidate>` or an `edm::View<pat::PackedCandidate>` (the latter only available in CMSSW_7_X_Y series releases)
 - **Producer Output**: `std::vector<ic::PFCandidate>`

**Example usage**
@snippet python/default_producers_cfi.py PFCandidate

CompositeCandidate {#objs-composite}
=========
 - **Definition**: ic::CompositeCandidate

An ic::CompositeCandidate aggregates multiple ic::Candidate (or derived) objects, summing the charge and four-momenta, while maintaining pointers to the original input objects. Due to its transient nature a CMSSW producer is not provided.

Electron {#objs-electron}
=========================
 - **Definition**: ic::Electron
 - **CMSSW Producer**: ICElectronProducer
 - **Producer Input**: Any collection compatible with an `edm::View<reco::GsfElectron>`
 - **Producer Output**: `std::vector<ic::Electron>`

The producer takes as input any collection where the objects are, or derive from, `reco::GsfElectron` objects. This includes `pat::Electron` collections. There are several ic::Electron properties which are not calculated by default in the producer:

Properties    | Notes
:------------ | :---------- |
ic::Electron::r9 | A `ValueMap` for this variable can be calculated by the external producer [ICAnalysis-ElectronR9Calculator](https://github.com/ajgilbert/ICAnalysis-ElectronR9Calculator). The option `inputR9` should be set to this `ValueMap` and the `includeR9` option should be set to `True`.
ic::Electron::hcal_sum | The extra inputs required to calculate this variable are not available in the AOD for all CMSSW release series. The method by which the variable is calculated is also expected to change in the `CMSSW_7_X_Y` series. A `ValueMap` for this variable can be produced by the ICElectronHcalDepthCalculator module within this package. The option `inputHcalSum` should be set to this `ValueMap` and the option `includeHcalSum` option set to `true`.
ic::Electron::has_matched_conversion | In anticipation of the method to calculate this flag changing in future releases the calculation has moved to a separate producer, ICElectronConversionCalculator, within this package. This calculates a `ValueMap` which should be specified by the option `inputConversionMatches` when the `includeConversionMatches` option is set to `True`.
ic::Electron::dxy_vertex <br> ic::Electron::dz_vertex | These must be calculated with respect to a user-specified reco::Vertex. The first vertex in the collection specified by the `inputVertices` option will be used when `includeVertexIP` is set to `True`.
ic::Electron::dxy_beamspot | This must be calculated with respect to a user-specified `reco::BeamSpot`. The beamspot set by the `inputBeamspot` option will be used when the option `includeBeamspotIP` is set to `True`.
ic::Electron::dr03_pfiso_charged_all <br> ic::Electron::dr03_pfiso_charged <br> ic::Electron::dr03_pfiso_neutral <br> ic::Electron::dr03_pfiso_gamma <br> ic::Electron::dr03_pfiso_pu <br> ic::Electron::dr04_pfiso_charged_all <br> ic::Electron::dr04_pfiso_charged <br> ic::Electron::dr04_pfiso_neutral <br> ic::Electron::dr04_pfiso_gamma <br> ic::Electron::dr04_pfiso_pu | These should be calculated on-the-fly in a CMSSW job. Individual analyses may modify the isolation definition, for example changing veto cones. See the section \ref obs-pf-iso below for software recipes.  Two sets of isolation values may be stored, nominally for isolation cones of sizes \f$\Delta R = 0.3\f$ and \f$\Delta R = 0.4\f$. These are enabled via the options `includePFIso03` and `includePFIso04` respectively. The `InputTag` for each isolation sum `ValueMap` must be specified in the `pfIso03` and/or `pfIso04` PSets as relevant.

**Example usage**
\snippet python/default_producers_cfi.py Electron

Muon {#objs-muon}
=================
 - **Definition**: ic::Muon
 - **CMSSW Producer**: ICMuonProducer
 - **Producer Input**: Any collection compatible with an `edm::View<reco::Muon>` or an `edm::View<reco::PFCandidate>`, but see below for details
 - **Producer Output**: `std::vector<ic::Muon>`

The producer operates in one of two modes, controlled by the option `isPF`. When set to `False`, the input collection should contain `reco::Muon` (or derived) objects. This includes `pat::Muon` collections. When set to `True`, the input should be a collection of `reco::PFCandidate` objects that are identified as PF muons. There are several ic::Muon properties which are not calculated by default in the producer:

Properties    | Notes
:------------ | :---------- |
ic::Muon::dxy_vertex <br> ic::Muon::dz_vertex | These must be calculated with respect to a user-specified reco::Vertex. The first vertex in the collection specified by the `inputVertices` option will be used when `includeVertexIP` is set to `True`.
ic::Muon::dxy_beamspot | This must be calculated with respect to a user-specified `reco::BeamSpot`. The beamspot set by the `inputBeamspot` option will be used when the option `includeBeamspotIP` is set to `True`.
ic::Muon::dr03_pfiso_charged_all <br> ic::Muon::dr03_pfiso_charged <br> ic::Muon::dr03_pfiso_neutral <br> ic::Muon::dr03_pfiso_gamma <br> ic::Muon::dr03_pfiso_pu <br> ic::Muon::dr04_pfiso_charged_all <br> ic::Muon::dr04_pfiso_charged <br> ic::Muon::dr04_pfiso_neutral <br> ic::Muon::dr04_pfiso_gamma <br> ic::Muon::dr04_pfiso_pu | These should be calculated on-the-fly in a CMSSW job. Individual analyses may modify the isolation definition, for example changing veto cones. See the section \ref obs-pf-iso below for software recipes.  Two sets of isolation values may be stored, nominally for isolation cones of sizes \f$\Delta R = 0.3\f$ and \f$\Delta R = 0.4\f$. These are enabled via the options `includePFIso03` and `includePFIso04` respectively. The `InputTag` for each isolation sum `ValueMap` must be specified in the `pfIso03` and/or `pfIso04` PSets as relevant.

**Example usage**
\snippet python/default_producers_cfi.py Muon

Photon {#objs-photon}
=====================
 - **Definition**: ic::Photon
 - **CMSSW Producer**: ICPhotonProducer
 - **Producer Input**: Any collection compatible with an `edm::View<reco::Photon>`
 - **Producer Output**: `std::vector<ic::Photon>`

The producer takes as input a collection of `reco::Photon` objects. The following ic::Photon properties are not calculated by default:

Properties    | Notes
:------------ | :---------- |
ic::Photon::pass_electron_veto | This flag is calculated by a separate producer in this package: ICPhotonElectronVetoCalculator
ic::Photon::had_tower_over_em | This is calculated by an external producer available in the [ICAnalysis-PhotonHadTowerOverEmCalculator](https://github.com/ajgilbert/ICAnalysis-PhotonHadTowerOverEmCalculator) repository.
ic::Photon::dr03_pfiso_charged <br> ic::Photon::dr03_pfiso_neutral <br> ic::Photon::dr03_pfiso_gamma | As for Electron and Muon production the isolation values will be extracted from the specified `ValueMap` inputs.

**Example usage**
\snippet python/default_producers_cfi.py Photon

Tau {#objs-tau}
===============
 - **Definition**: ic::Tau
 - **CMSSW Producer**: ICTauProducer
 - **Producer Input**: Any collection compatible with an `edm::View<reco::PFTau>` or an `edm::View<pat::Tau>`, but see below
 - **Producer Output**: `std::vector<ic::Tau>`
 - **Producer Requests**: Can request the `reco::Track` objects from the charged PF constituents. The request will be produced with the label `requestedTracks`.

The producer is a template class on the type of the input objects. Two specific template instances are defined as CMSSW modules: one for `reco::PFTau` input (ICPFTauProducer) and the other for `pat::Tau` input (ICPFTauFromPatProducer). The main difference between the two is that the user must specify the list of discriminators to extract from the `edm::Event` in the `reco::PFTau` case,  whereas in the `pat::Tau` case these are extracted directly from the input object.

The following ic::Tau properties are not calculated by default:

Properties    | Notes
:------------ | :---------- |
ic::Tau::lead_dxy_vertex <br> ic::Tau::lead_dz_vertex | These must be calculated with respect to a user-specified reco::Vertex. The first vertex in the collection specified by the `inputVertices` option will be used when `includeVertexIP` is set to `True`.

**Example usage**
\snippet python/default_producers_cfi.py Tau

Missing Tranvserse Energy {#objs-met}
=====================================
 - **Definition**: ic::Met
 - **CMSSW Producers**: ICMetProducer, ICSingleMetProducer
 - **Producer Input**: Any collection compatible with an `edm::View<reco::MET>`
 - **Producer Output**: `std::vector<ic::Met>` in the ICMetProducer, and `ic::Met` in the ICSingleMetProducer

Two producers are provided, both of which take a vector of `reco::MET` (or derived) objects as input. The ICMetProducer will produce a vector of `ic::Met` objects, whereas the ICSingleMetProducer will produce a single `ic::Met` object. The ICMetProducer can optionally read a `vector<std::size_t>` from the edm::Event that should be aligned with the input `vector<reco::MET>` and will be used to set the ic::Met::id values in the output collection. The main use for this feature is in conjunction with the [MVA MET](\ref objs-mva-met) producer where `MET` objects must be identified by combined hashes of the lepton inputs.

**Example usage**
\snippet python/default_producers_cfi.py Met

\snippet python/default_producers_cfi.py SingleMet

Jet {#objs-jet}
===============
 - **Definition**: ic::Jet, ic::CaloJet, ic::JPTJet, ic::PFJet
 - **CMSSW Producers**: ICJetProducer (see below for template instantiations)
 - **Producer Input**: Any collection compatible with an `edm::View<reco::Jet>` or `edm::View<pat::Jet>` (see below for details)
 - **Producer Output**: `std::vector<ic::Jet>`, `std::vector<ic::CaloJet>`, `std::vector<ic::JPTJet>`, `std::vector<ic::PFJet>`
 - **Producer Requests**:
   + Can request the `reco::Track` objects for the charged constituents of PFJet or JPTJet inputs. The request will be produced with the label `requestedTracks`.
   + Can request the `reco::SecondaryVertexTagInfo` objects associated to the jets, which will be produced with the label `requestedSVInfo`.

The ic::JetProducer is a template class that is specialised for both the input and output jet types. While making the code more complex, this avoids a large amount of duplication that would result for have a normal producer for each possible input-output pairing. The possible CMSSW producers are defined [here](\ref JetProducers).

Each choice of input and output type brings with it the need for a specific set of options. This is addressed through two helper structs, the JetSrcHelper which is templated only on the input type, and the JetDestHelper which is templated only on the output type. Please see the documentation pages of these structs for further details.

An important distinction to make between using a reco::Jet and a pat::Jet as input is the treatment of the jet energy corrections. This module assumes a reco::Jet collection as input is uncorrected. The JetSrcHelper in this case provides extra options to specify the list of corrections to apply and an optional cut string that may be applied after the input jet is corrected and before a corresponding ic::Jet is produced.

The following properties are not calculated by default:
Properties    | Notes
:------------ | :---------- |
ic::Jet::parton_flavour | *For reco::Jet input producers only*: The MC truth jet flavour (as a PDG ID). The method by which this is calculated is expected to change for 2015 data analyses. In anticipation, the calculation has moved to a separate producer: ICJetFlavourCalculator, within this package. This calculates a `ValueMap` which should be specified by the option `inputJetFlavour` when the `includeJetFlavour` option is set to `True`.
ic::PFJet::pu_id_mva_value | *For ic::PFJet output producers only*: See the section [below](\ref objs-mva-met) for details on producing a pileup jet MVA `ValueMap`. This should be specified by the option `inputPileupID` when the `icludePileupID` option is set to `True`.
ic::PFJet::beta <br> ic::PFJet::beta_max <br> ic::PFJet::linear_radial_moment <br> ic::PFJet::charged_multiplicity_nopu | *For ic::PFJet output producers only:* These variables are only calculated when `includeTrackBasedVars` is set to `True` and both the `inputTracks` and `inputVertices` collections are defined.
ic::JPTJet::beta <br> ic::JPTJet::beta_max <br> ic::JPTJet::track_pt_total | *For ic::JPTJet output producers only:* These variables are only calculated when `includeTrackBasedVars` is set to `True` and both the `inputTracks` and `inputVertices` collections are defined.
ic::CaloJet::fHPD <br> ic::CaloJet::fRBX <br> ic::CaloJet::id_loose <br> ic::CaloJet::id_tight | *For ic::CaloJet and ic::JPTJet output producers only:* Requires an external `reco::JetID` map. Set `inputJetID` to the label of this map and set `includeJetID` to `True`.

**Example usage**
@snippet python/default_producers_cfi.py Jet

Vertex {#objs-vertex}
=====================
 - **Definition**: ic::Vertex
 - **CMSSW Producers**: ICVertexProducer
 - **Producer Input**: Any collection compatible with an `edm::View<reco::Vertex>`
 - **Producer Output**: `std::vector<ic::Vertex>`
 - **Producer Requests**: Can request the `reco::Track` objects used in the vertex fit. The request will be produced with the label `requestedTracks`.

The producer can be configured to store only the first vertex in the input collection by setting the value of the `firstVertexOnly` option to `True`. The ic::Track::id values of the tracks used in each vertex fit can also be stored (along with the fit weighting), subject to a minimum threshold set by the option `trackPtThreshold`.

**Example usage**
@snippet python/default_producers_cfi.py Vertex

Secondary Vertex {#objs-sec-vertex}
===================================
 - **Definition**: ic::SecondaryVertex
 - **CMSSW Producers**: ICSecondaryVertexProducer
 - **Producer Input**: Any collection compatible with an `edm::View<reco::SecondaryVertexTagInfo>`
 - **Producer Output**: `std::vector<ic::SecondaryVertex>`
 - **Producer Requests**: Can request the `reco::Track` objects used in the vertex fit. The request will be produced with the label `requestedTracks`.

Note that each reco::SecondaryVertexTagInfo input (one per jet) can contain multiple secondary vertices, each of which will be added to the output collection. The ic::Track::id values of the tracks used in each vertex fit can also be stored (along with the fit weighting), subject to a minimum threshold set by the option `trackPtThreshold`.

**Example usage**
@snippet python/default_producers_cfi.py SecondaryVertex

Track {#objs-track}
===================
 - **Definitions**: ic::Track, ic::LightTrack
 - **CMSSW Producers**: ICTrackProducer, ICLightTrackProducer
 - **Producer Input**: Any collection compatible with an `edm::View<reco::Track>`
 - **Producer Output**: `std::vector<ic::Track>` or `std::vector<ic::LightTrack>`

Two track classes are provided. The `ic::Track` is the standard class and the `ic::LightTrack` provides a limited subset of the `ic::Track` variables for situations where these are sufficient for analysis and ntuple space is at a premium.

**Example usage**
@snippet python/default_producers_cfi.py Track
@snippet python/default_producers_cfi.py LightTrack

GenParticle {#objs-genparticle}
=========================
 - **Definition**: ic::GenParticle
 - **CMSSW Producers**: ICGenParticleProducer
 - **Producer Input**: Any collection compatible with an `edm::View<reco::GenParticle>`
 - **Producer Output**: `std::vector<ic::GenParticle>`

The producer can optionally store the relations between particles in decay chains using the `includeMothers` and `includeDaughters` flags. When enabled, these options tell the producer to save the corresponding vectors of ic::GenParticle::index values. Note that this index is different from the ic::GenParticle::id value, and is equal to the vector index position of the `reco::GenParticle` in the input collection.

**Example usage**
@snippet python/default_producers_cfi.py GenParticle

GenJet {#objs-genjet}
=====================
 - **Definition**: ic::GenJet
 - **CMSSW Producers**: ICGenJetProducer
 - **Producer Input**: Any collection compatible with an `edm::View<reco::GenJet>`
 - **Producer Output**: `std::vector<ic::GenJet>`
 - **Producer Requests**: Can request the `reco::GenParticle` objects clustered into each jet. The request will be produced with the label `requestedGenParticles`.

Note that due to technical limitations, if the `requestGenParticles` option is set to `True`, the original `reco::GenParticle` collection from which the jets were clustered must also be specified in the option `inputGenParticles`.

**Example usage**
@snippet python/default_producers_cfi.py GenJet

PileupInfo {#objs-pu-info}
==========================
 - **Definition**: ic::PileupInfo
 - **CMSSW Producers**: ICPileupInfoProducer
 - **Producer Input**: Any collection compatible with an `edm::View<PileupSummaryInfo>`
 - **Producer Output**: `std::vector<ic::PileupInfo>`

Each object `PileupSummaryInfo` object in the input collection provides the pileup information for a single bunch crossing. The bunch crossing number, ic::PileupInfo::bunch_crossing can be used to identify the bunch crossing relative to the hard scatter interaction (which has a bunch crossing of zero).

**Example usage**
@snippet python/default_producers_cfi.py PileupInfo

SuperCluster {#objs-supercluster}
=================================
 - **Definition**: ic::SuperCluster
 - **CMSSW Producers**: ICSuperClusterProducer
 - **Producer Input**: Two collections (EB and EE) compatible with an `edm::View<reco::SuperCluster>`
 - **Producer Output**: `std::vector<ic::SuperCluster>`

The `ic::SuperCluster` class does not inherit from ic::Candidate but is instead specified by the supercluster position and direction with respect to the detector origin. The producer requires two collections as input as these are stored separately for barrel and endcap superclusters in the `edm::Event`.

**Example usage**
@snippet python/default_producers_cfi.py SuperCluster

TriggerPath {#objs-trig-path}
=============================
 - **Definition**: ic::TriggerPath
 - **CMSSW Producers**: ICTriggerPathProducer
 - **Producer Input**: A `pat::TriggerEvent` object
 - **Producer Output**: `std::vector<ic::TriggerPath>`

The file `test/validation_cfg.py` contains examples for creating the `pat::TriggerEvent` in various CMSSW releases. The producer can be configured to only store the paths that fired in each event via the option `includeAcceptedOnly`. There is also an option `splitVersion` (off by default) to strip the integer version number from each trigger path name, and store this is a separate variable: ic::TriggerPath::version. If this is enabled it may also be useful to set `saveStrings` to `False`. The ic::TriggerPath::name will not be written, the version-less name will still be hashed and stored in the ic::TriggerPath::id variables, saving space overall.

**Example usage**
@snippet python/default_producers_cfi.py TriggerPath

TriggerObject {#objs-trig-obj}
=============================
 - **Definition**: ic::TriggerObject
 - **CMSSW Producers**: ICTriggerObjectProducer
 - **Producer Input**: A `pat::TriggerEvent` object
 - **Producer Output**: `std::vector<ic::TriggerObject>`

The producer will store a collection of trigger objects used in the specified `hltPath`. Each object stores a vector of the (hashed) labels of the filter modules that accepted the object in the path.

**Example usage**
@snippet python/default_producers_cfi.py TriggerObject

EventInfo {#objs-event-info}
============================
 - **Definition**: ic::EventInfo
 - **CMSSW Producers**: ICEventInfoProducer
 - **Producer Input**: No mandatory input
 - **Producer Output**: `ic::EventInfo`

The core event variables, such as event, lumi and run, are stored automatically. Other variables of interest may be added via the producer options. The options `includeJetRho` and `includeLeptonRho` can be used to store the values of the median energy densities used for the pileup jet energy correction and the lepton pileup isolation respectively. The appropriate inputs should be specified by the options `inputJetRho` and `inputLeptonRho`. The number of vertices in a particular collection (`inputVertices`) can also be stored (`includeVertexCount`), which is a space-saving alternative to storing an entire `ic::Vertex` collection. The options `filters`, `weights` and `genFilterWeights` are used to specify corresponding lists of input `bool`, `double` and `GenFilterInfo` values that should be added to the `ic::EventInfo` using the labels provided in the `PSet`.

**Example usage**
@snippet python/default_producers_cfi.py EventInfo

Non-standard Objects {#obs-non-standard}
========================================

MVA MET / Pileup jet ID {#objs-mva-met}
---------------------------------------
Supported releases:
  - `CMSSW_4_2_8_patch7`
  - `CMSSW_5_3_X`
  - `CMSSW_7_0_X` *pileup jet ID only*

Links:
  - https://twiki.cern.ch/twiki/bin/viewauth/CMS/MVAMet
  - https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupJetID
  - https://github.com/ajgilbert/ICAnalysis-MVAMETPairProducer

The MVA MET is not available by default in any CMSSW release, and pileup jet ID is only included in the `CMSSW_7_0_X` series. A recipe is given below for obtaining the versions that were used in the legacy HiggsTauTau analysis, which was based on `CMSSW_5_3_7`. Newer recipes for the MVA MET have also been provided and are currently in use by some groups. See the twiki page above for details. Notably, newer MVA trainings have been provided that do not require the addition of recoil corrections, but these have not yet been tested in this framework.

The following recipe can be used to set-up the MVA MET and pileup ID in a different `CMSSW_5_3_X` release. It works by extracting the relevant git commits from a prepared branch based on the `CMSSSW_5_3_7` release, and applying these commits to a branch created from a different release.

~~~~~~~~~~~~~{.sh}
# Assume we want to set up the MVA MET in some 5_3_X release other than 5_3_7
# If you haven't already - create the project area
scramv1 project CMSSW_5_3_X; cd CMSSW_5_3_X/src
# This will initialise git and create the local branch from-CMSSW_5_3_X
# We'll assume that this is the branch you want to add the MVA MET to.
git-cms-addpkg FWCore/Version
# Add the ic-cmssw fork of the cmssw repo and fetch its branches
git remote add -f ic-cmssw git@github.com:ajgilbert/cmssw.git
# Create a temporary branch from the 5_3_7 branch containing the MVA MET
git checkout --no-track -b rebase-attempt ic-cmssw/MVAMET-PileupJetID-from-CMSSW_5_3_7
# This is the command that will do all the work, attempting to rebase the commits
# that add the MVA MET onto your 5_3_X release
git rebase --onto from-CMSSW_5_3_X CMSSW_5_3_7 rebase-attempt
# If there are no merge issues this should complete fine. If you do hit
# issues the rebase will pause, and you will have to fix any merge issues
# by hand, then instruct git to continue the rebase. If in doubt, git status
# should tell you what you need to do.
# Once the rebase is finished, switch back to our target branch
git checkout from-CMSSW_5_3_X
# This should now be a trivial fast-forward merge
git merge rebase-attempt
# All done - we can delete the temporary branch
git branch -d rebase-attempt
# Now update the sparse-checkout file so the modified packages will be visible
git-cms-sparse-checkout CMSSW_X_Y_Z HEAD
# This updates the actual working directory with the new packages
git read-tree -u --reset HEAD
# Optional: if you want to use the lepton-pair input version of the MVA MET, checkout this
# standalone package. Different branches exist for `5_3_X` and `4_2_X` releases. See the
# README on the project page for more information:
# https://github.com/ajgilbert/ICAnalysis-MVAMETPairProducer
git clone git@github.com:ajgilbert/ICAnalysis-MVAMETPairProducer.git ICAnalysis/MVAMETPairProducer
~~~~~~~~~~~~~

\todo Document example python configs for the MVA MET and pileup jet ID


Tau Reconstruction and Discrimination {#obs-tau-reco}
-----------------------------------------------------
Supported releases:
  - `CMSSW_4_2_8_patch7`
  - `CMSSW_5_3_X`
  - `CMSSW_7_0_X`

Links:
  - https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuidePFTauID

Over time many improvements to the tau reconstruction and identification have been developed by the tau POG. The twiki link above is a good reference to the different versions of the tau ID and how to add these to a release using git. The reconstruction and ID used in the legacy HiggsTauTau analysis corresponds to the section *Legacy Tau ID (Run I)*. This is now available by default in releases starting from `CMSSW_5_3_12`. For older releases the recipe provided on the twiki might cause issues with the merging. It may be safer to identify just the commits that change the `RecoTauTag` package and `git cherry-pick` them one-by-one, e.g.

~~~~~~~~~~~~~{.sh}
# Get a summary of which commits changed the RecoTauTag package between
# the current HEAD and the recommended branch
git log --oneline HEAD..cms-tau-pog/CMSSW_5_3_X -- RecoTauTag
# Cherry-pick each commit - shouldn't result in any conflicts
git cherry-pick ....
git cherry-pick ....
git checkout HEAD
~~~~~~~~~~~~~

PF Isolation {#obs-pf-iso}
--------------------------
- `CMSSW_4_2_8_patch7`
- `CMSSW_5_3_X`
- `CMSSW_7_0_X`

The calculation of PF iso-deposits for electrons changed during the `CMSSW_5_3_X` series. In the legacy HiggsTauTau analysis this was updated using a CVS tag on top of the `CMSSW_5_3_7` release. See this git commit for details of the changes:

  - https://github.com/ajgilbert/cmssw/commit/b8bd99295f462f2c049d94d8660fe46fd4f7e5f8

This updated version was included after some later `CMSSW_5_3_X` release, so check first if you need to apply it. The change from `CandIsolatorFromDeposits` to `PFCandIsolatorFromDeposits` modules for the iso-deposit calculation should be a good indicator.

\todo Document example python configs for PF isolation

MVA Electron ID {#obs-mva-elec-id}
----------------------------------
Supported releases:
  - `CMSSW_4_2_8_patch7`
  - `CMSSW_5_3_X`
  - `CMSSW_7_0_X`

The code for the electron ID MVA is contained in the `EgammaAnalysis/ElectronTools` package. In older `CMSSW_5_3_X` series releases this package was not included. For the legacy HiggsTauTau analysis this code was added to the `CMSSW_5_3_7` release using a CVS recipe, but is now most easily obtained by a `git cherry-pick` of this commit:

  - https://github.com/ajgilbert/cmssw/commit/50c18f9a40c3175f0d8d0e3d9b098b561bf067b0

\todo Document example python configs for MVA electron ID

