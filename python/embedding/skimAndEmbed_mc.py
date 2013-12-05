# Auto generated configuration file
# using: 
# Revision: 1.381.2.28 
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: UserCode/ICHiggsTauTau/PFEmbeddingSource_Wemb_cff.py --customise=UserCode/ICHiggsTauTau/pf_01_customizeAll.py -s GEN,SIM,DIGI,L1,DIGI2RAW,HLT:7E33v3,RAW2DIGI,RECO --no_exec --conditions auto:startup_7E33v3 --fileout=embedded.root --python_filename=skimAndEmbed_mc.py -n -1
import FWCore.ParameterSet.Config as cms

process = cms.Process('HLT')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.GeometrySimDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic8TeVCollision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('HLTrigger.Configuration.HLT_7E33v3_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    skipEvents = cms.untracked.uint32(0),
    fileNames = cms.untracked.vstring('file:/afs/cern.ch/work/a/amagnan/CMSSW_5_3_11_patch6/src/UserCode/ICHiggsTauTau/python/testFile_W2j.root')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.381.2.28 $'),
    annotation = cms.untracked.string('UserCode/ICHiggsTauTau/PFEmbeddingSource_Wemb_cff.py nevts:-1'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('embedded.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

# Additional output definition

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup_7E33v3', '')

process.filterEmptyEv = cms.EDFilter("EmptyEventsFilter",
    src = cms.untracked.InputTag("generator","","EmbeddedRECO"),
    target = cms.untracked.int32(1),
    minEvents = cms.untracked.int32(1)
)


process.newSource = cms.EDProducer("MCParticleReplacer",
    src = cms.InputTag("selectedMuons"),
    beamSpotSrc = cms.InputTag("dummy"),
    hepMcMode = cms.string('new'),
    verbose = cms.bool(False),
    algorithm = cms.string('ZTauTau'),
    hepMcSrc = cms.InputTag("generator"),
    primaryVertexLabel = cms.InputTag("dummy"),
    ZTauTau = cms.PSet(
        TauolaOptions = cms.PSet(
            UseTauolaPolarization = cms.bool(True),
            InputCards = cms.PSet(
                mdtau = cms.int32(0),
                pjak2 = cms.int32(0),
                pjak1 = cms.int32(0)
            )
        ),
        pythiaHepMCVerbosity = cms.untracked.bool(False),
        generatorMode = cms.string('Tauola'),
        filterEfficiency = cms.untracked.double(1.0),
        minVisibleTransverseMomentum = cms.untracked.string(''),
        transformationMode = cms.untracked.int32(3)
    ),
    ParticleGun = cms.PSet(
        ExternalDecays = cms.PSet(
            Tauola = cms.PSet(
                UseTauolaPolarization = cms.bool(True),
                InputCards = cms.PSet(
                    mdtau = cms.int32(0),
                    pjak2 = cms.int32(0),
                    pjak1 = cms.int32(0)
                )
            ),
            parameterSets = cms.vstring('Tauola')
        ),
        forceTauDecay = cms.string('none'),
        forceTauMinusHelicity = cms.int32(0),
        generatorMode = cms.string('Tauola'),
        gunParticle = cms.int32(15),
        forceTauPolarization = cms.string('W'),
        particleOrigin = cms.string('muonReferencePoint'),
        forceTauPlusHelicity = cms.int32(0),
        PythiaParameters = cms.PSet(
            pythiaUESettings = cms.vstring('MSTJ(11)=3     ! Choice of the fragmentation function', 
                'MSTJ(22)=2     ! Decay those unstable particles', 
                'PARJ(71)=10 .  ! for which ctau  10 mm', 
                'MSTP(2)=1      ! which order running alphaS', 
                'MSTP(33)=0     ! no K factors in hard cross sections', 
                'MSTP(51)=10042 ! structure function chosen (external PDF CTEQ6L1)', 
                'MSTP(52)=2     ! work with LHAPDF', 
                'MSTP(81)=1     ! multiple parton interactions 1 is Pythia default', 
                'MSTP(82)=4     ! Defines the multi-parton model', 
                'MSTU(21)=1     ! Check on possible errors during program execution', 
                'PARP(82)=1.8387   ! pt cutoff for multiparton interactions', 
                'PARP(89)=1960. ! sqrts for which PARP82 is set', 
                'PARP(83)=0.5   ! Multiple interactions: matter distrbn parameter', 
                'PARP(84)=0.4   ! Multiple interactions: matter distribution parameter', 
                'PARP(90)=0.16  ! Multiple interactions: rescaling power', 
                'PARP(67)=2.5    ! amount of initial-state radiation', 
                'PARP(85)=1.0  ! gluon prod. mechanism in MI', 
                'PARP(86)=1.0  ! gluon prod. mechanism in MI', 
                'PARP(62)=1.25   ! ', 
                'PARP(64)=0.2    ! ', 
                'MSTP(91)=1      !', 
                'PARP(91)=2.1   ! kt distribution', 
                'PARP(93)=15.0  ! '),
            pgunTauolaParameters = cms.vstring('MDME(89,1)=0', 
                'MDME(90,1)=0', 
                'MDME(91,1)=0', 
                'MDME(92,1)=0', 
                'MDME(93,1)=0', 
                'MDME(94,1)=0', 
                'MDME(95,1)=0', 
                'MDME(96,1)=0', 
                'MDME(97,1)=0', 
                'MDME(98,1)=0', 
                'MDME(99,1)=0', 
                'MDME(100,1)=0', 
                'MDME(101,1)=0', 
                'MDME(102,1)=0', 
                'MDME(103,1)=0', 
                'MDME(104,1)=0', 
                'MDME(105,1)=0', 
                'MDME(106,1)=0', 
                'MDME(107,1)=0', 
                'MDME(108,1)=0', 
                'MDME(109,1)=0', 
                'MDME(110,1)=0', 
                'MDME(111,1)=0', 
                'MDME(112,1)=0', 
                'MDME(113,1)=0', 
                'MDME(114,1)=0', 
                'MDME(115,1)=0', 
                'MDME(116,1)=0', 
                'MDME(117,1)=0', 
                'MDME(118,1)=0', 
                'MDME(119,1)=0', 
                'MDME(120,1)=0', 
                'MDME(121,1)=0', 
                'MDME(122,1)=0', 
                'MDME(123,1)=0', 
                'MDME(124,1)=0', 
                'MDME(125,1)=0', 
                'MDME(126,1)=0', 
                'MDME(127,1)=0', 
                'MDME(128,1)=0', 
                'MDME(129,1)=0', 
                'MDME(130,1)=0', 
                'MDME(131,1)=0', 
                'MDME(132,1)=0', 
                'MDME(133,1)=0', 
                'MDME(134,1)=0', 
                'MDME(135,1)=0', 
                'MDME(136,1)=0', 
                'MDME(137,1)=0', 
                'MDME(138,1)=0', 
                'MDME(139,1)=0', 
                'MDME(140,1)=0', 
                'MDME(141,1)=0', 
                'MDME(142,1)=0'),
            parameterSets = cms.vstring('pythiaUESettings')
        )
    )
)


process.generator = cms.EDProducer("MCParticleReplacer",
    src = cms.InputTag("adaptedMuonsFromWmunu","zMusExtracted"),
    beamSpotSrc = cms.InputTag("dummy"),
    hepMcMode = cms.string('new'),
    verbose = cms.bool(False),
    algorithm = cms.string('ZTauTau'),
    hepMcSrc = cms.InputTag("generator"),
    primaryVertexLabel = cms.InputTag("dummy"),
    ZTauTau = cms.PSet(
        TauolaOptions = cms.PSet(
            UseTauolaPolarization = cms.bool(True),
            InputCards = cms.PSet(
                mdtau = cms.int32(0),
                pjak2 = cms.int32(0),
                pjak1 = cms.int32(0)
            )
        ),
        pythiaHepMCVerbosity = cms.untracked.bool(False),
        generatorMode = cms.string('Tauola'),
        filterEfficiency = cms.untracked.double(1.0),
        minVisibleTransverseMomentum = cms.untracked.string(''),
        transformationMode = cms.untracked.int32(3)
    ),
    ParticleGun = cms.PSet(
        ExternalDecays = cms.PSet(
            Tauola = cms.PSet(
                UseTauolaPolarization = cms.bool(True),
                InputCards = cms.PSet(
                    mdtau = cms.int32(0),
                    pjak2 = cms.int32(0),
                    pjak1 = cms.int32(0)
                )
            ),
            parameterSets = cms.vstring('Tauola')
        ),
        forceTauDecay = cms.string('none'),
        forceTauMinusHelicity = cms.int32(0),
        generatorMode = cms.string('Tauola'),
        gunParticle = cms.int32(15),
        forceTauPolarization = cms.string('W'),
        particleOrigin = cms.string('muonReferencePoint'),
        forceTauPlusHelicity = cms.int32(0),
        PythiaParameters = cms.PSet(
            pythiaUESettings = cms.vstring('MSTJ(11)=3     ! Choice of the fragmentation function', 
                'MSTJ(22)=2     ! Decay those unstable particles', 
                'PARJ(71)=10 .  ! for which ctau  10 mm', 
                'MSTP(2)=1      ! which order running alphaS', 
                'MSTP(33)=0     ! no K factors in hard cross sections', 
                'MSTP(51)=10042 ! structure function chosen (external PDF CTEQ6L1)', 
                'MSTP(52)=2     ! work with LHAPDF', 
                'MSTP(81)=1     ! multiple parton interactions 1 is Pythia default', 
                'MSTP(82)=4     ! Defines the multi-parton model', 
                'MSTU(21)=1     ! Check on possible errors during program execution', 
                'PARP(82)=1.8387   ! pt cutoff for multiparton interactions', 
                'PARP(89)=1960. ! sqrts for which PARP82 is set', 
                'PARP(83)=0.5   ! Multiple interactions: matter distrbn parameter', 
                'PARP(84)=0.4   ! Multiple interactions: matter distribution parameter', 
                'PARP(90)=0.16  ! Multiple interactions: rescaling power', 
                'PARP(67)=2.5    ! amount of initial-state radiation', 
                'PARP(85)=1.0  ! gluon prod. mechanism in MI', 
                'PARP(86)=1.0  ! gluon prod. mechanism in MI', 
                'PARP(62)=1.25   ! ', 
                'PARP(64)=0.2    ! ', 
                'MSTP(91)=1      !', 
                'PARP(91)=2.1   ! kt distribution', 
                'PARP(93)=15.0  ! '),
            pgunTauolaParameters = cms.vstring('MDME(89,1)=0', 
                'MDME(90,1)=0', 
                'MDME(91,1)=0', 
                'MDME(92,1)=0', 
                'MDME(93,1)=0', 
                'MDME(94,1)=0', 
                'MDME(95,1)=0', 
                'MDME(96,1)=0', 
                'MDME(97,1)=0', 
                'MDME(98,1)=0', 
                'MDME(99,1)=0', 
                'MDME(100,1)=0', 
                'MDME(101,1)=0', 
                'MDME(102,1)=0', 
                'MDME(103,1)=0', 
                'MDME(104,1)=0', 
                'MDME(105,1)=0', 
                'MDME(106,1)=0', 
                'MDME(107,1)=0', 
                'MDME(108,1)=0', 
                'MDME(109,1)=0', 
                'MDME(110,1)=0', 
                'MDME(111,1)=0', 
                'MDME(112,1)=0', 
                'MDME(113,1)=0', 
                'MDME(114,1)=0', 
                'MDME(115,1)=0', 
                'MDME(116,1)=0', 
                'MDME(117,1)=0', 
                'MDME(118,1)=0', 
                'MDME(119,1)=0', 
                'MDME(120,1)=0', 
                'MDME(121,1)=0', 
                'MDME(122,1)=0', 
                'MDME(123,1)=0', 
                'MDME(124,1)=0', 
                'MDME(125,1)=0', 
                'MDME(126,1)=0', 
                'MDME(127,1)=0', 
                'MDME(128,1)=0', 
                'MDME(129,1)=0', 
                'MDME(130,1)=0', 
                'MDME(131,1)=0', 
                'MDME(132,1)=0', 
                'MDME(133,1)=0', 
                'MDME(134,1)=0', 
                'MDME(135,1)=0', 
                'MDME(136,1)=0', 
                'MDME(137,1)=0', 
                'MDME(138,1)=0', 
                'MDME(139,1)=0', 
                'MDME(140,1)=0', 
                'MDME(141,1)=0', 
                'MDME(142,1)=0'),
            parameterSets = cms.vstring('pythiaUESettings')
        )
    )
)


process.adaptedMuonsFromWmunu = cms.EDProducer("PATMuonMETAdapter",
    outputCol = cms.untracked.InputTag("adaptedMuonsFromWmunu","zMusExtracted"),
    patMuCands = cms.untracked.InputTag("goodMuonsPFIso"),
    patMet = cms.untracked.InputTag("pfMet")
)


process.removedInputMuons = cms.EDProducer("ZmumuPFEmbedder",
    selectedMuons = cms.InputTag("goodMuonsPFIso"),
    keepMuonTrack = cms.bool(False),
    useCombinedCandidate = cms.untracked.bool(False),
    tracks = cms.InputTag("generalTracks")
)


process.ProductionFilterSequence = cms.Sequence(process.adaptedMuonsFromWmunu+process.removedInputMuons+process.generator+process.filterEmptyEv)

# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.digitisation_step,process.L1simulation_step,process.digi2raw_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.RECOSIMoutput_step])
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq 

# customisation of the process.

# Automatic addition of the customisation function from HLTrigger.Configuration.customizeHLTforMC
from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforMC 

#call to customisation function customizeHLTforMC imported from HLTrigger.Configuration.customizeHLTforMC
process = customizeHLTforMC(process)

# Automatic addition of the customisation function from UserCode.ICHiggsTauTau.pf_01_customizeAll
from UserCode.ICHiggsTauTau.pf_01_customizeAll import customise 

#call to customisation function customise imported from UserCode.ICHiggsTauTau.pf_01_customizeAll

process.doWmunuSkim = cms.PSet()

process = customise(process)

# End of customisation functions
