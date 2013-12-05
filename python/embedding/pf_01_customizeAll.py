# -*- coding: utf-8 -*-
import FWCore.ParameterSet.Config as cms
import os
import sys

from FWCore.ParameterSet.Modules import _Module


# Searches for self.lookFor module in cms.Path. When found, next and prev module is stored
class SeqVisitor(object):
    def __init__(self, lookFor):
	self.lookFor=lookFor
	self.nextInChain="NONE"
	self.prevInChain="NONE"
	self.prevInChainCandidate="NONE"
	self.catch=0   # 1 - we have found self.lookFor, at next visit write visitee
	self.found=0

    def prepareSearch(self): # this should be called on beggining of each iteration 
	self.found=0 
      
    def setLookFor(self, lookFor):
	self.lookFor = lookFor
      
    def giveNext(self):
	return self.nextInChain
    def givePrev(self):
	return self.prevInChain
      
    def enter(self,visitee):
	if isinstance(visitee, _Module):
	  if self.catch == 1:
	      self.catch=0
	      self.nextInChain=visitee
	      self.found=1
	  if visitee == self.lookFor:
	      self.catch=1
	      self.prevInChain=self.prevInChainCandidate
	      
	  self.prevInChainCandidate=visitee
	
    def leave(self,visitee):
        pass

  

def customise(process):
    # Catch the case when this config is run from cmsDriver, it won't work due to VarParsing
    # First protect against crab job creation, then the no-argument case
    if hasattr(sys, "argv") and len(sys.argv) > 0:
        if "cmsDriver" in sys.argv[0]:
            print "Running pf_customise from cmsDriver, not executing running() further due to VarParsing"
            return
        else:
            print "Running pf_customise"
         
    # Command line arguments
    import FWCore.ParameterSet.VarParsing as VarParsing
    options = VarParsing.VarParsing ('analysis')
    options.register ('mdtau',
                      0, # default value
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.int,         
                      "mdtau value for tauola")
    
    options.register ('transformationMode',
                      1, #default value
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.int,
                      "transformation mode. 0=mumu->mumu, 1=mumu->tautau, 2=mumu->taunu, 3=munu->taunu")
    
    options.register ('minVisibleTransverseMomentum',
                      "", #default value
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.string,
                      "generator level cut on visible transverse momentum (typeN:pT,[...];[...])")
    
    options.register ('useJson',
                      0, # default value, false
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.int,         
                      "should I enable json usage?")
    
    options.register ('overrideBeamSpot',
                      0, # default value, false
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.int,         
                      "should I override beamspot in globaltag?")
    
    #  options.register ('primaryProcess',
    #                    'RECO', # default value
    #                     VarParsing.VarParsing.multiplicity.singleton,
    #                     VarParsing.VarParsing.varType.string,
    #                     "original processName")
    
    process._Process__name="EmbeddedRECO"
    process.TFileService = cms.Service("TFileService",  fileName = cms.string("histo_embedded_data.root")          )


    hltProcessName = "HLT"
    recoProcessName = "RECO"
    processName = process.name_()
 
 
    
    try:
        outputModule = process.output
    except:
        pass
    try:
        outputModule = getattr(process,str(getattr(process,list(process.endpaths)[-1])))
    except:
        pass
    
    print "Changing eventcontent to AODSIM + misc. "
    outputModule.outputCommands = cms.untracked.vstring("drop *")
    #outputModule.outputCommands.extend(process.RAWEventContent.outputCommands )
    outputModule.outputCommands.extend(process.AODSIMEventContent.outputCommands )
    
    keepMC = cms.untracked.vstring(
        "keep *_removedInputMuons_*_*",
        "keep *_generator_*_*",
        "keep *_PhotonIDProd_*_*",
        "keep *_goodMuons*_*_*",
        "keep *_genParticles_*_%s" % hltProcessName,
        "keep recoGenJets_*_*_%s" % hltProcessName,
        "keep recoGenMETs_*_*_%s" % hltProcessName,
        "keep *_pfMet_*_%s" % recoProcessName,
        "keep *_offlinePrimaryVertices_*_%s" % recoProcessName,
        "keep *_offlineBeamSpot_*_%s" % recoProcessName,
        "keep *_gtDigis_*_%s" % recoProcessName,
        "keep *_l1GtTriggerMenuLite_*_%s" % recoProcessName, # in run block
        "keep *_conditionsInEdm_*_%s" % recoProcessName, # in run block
        "keep *_addPileupInfo*_*_%s" % recoProcessName, # for MC
        "keep HcalNoiseSummary_*_*_%s" % recoProcessName,
        "keep *_adaptedMuonsFromWmunu_*_%s" % processName,
        "keep *_generator_weight_%s" % processName,
        "keep *_genParticles_*_%s" % processName,
        "keep recoGenJets_*_*_%s" % processName,
        "keep recoGenMETs_*_*_%s" % processName,
        "keep edmMergeableCounter_*_*_%s" % processName,
        "keep *_tmfTracks_*_%s" % processName,
        "keep *_offlinePrimaryVertices_*_%s" % processName,
        "keep *_offlinePrimaryVerticesWithBS_*_%s" % processName,
        "keep *_generalTracks_*_%s" % recoProcessName,
        "keep *_muons_*_%s" % recoProcessName,
        "keep *_globalMuons_*_%s" % recoProcessName,
        "keep recoGsfElectronCores_*_*_%s" % recoProcessName,
        "keep *_gsfElectrons_*_%s" % recoProcessName,
        "keep *_photons_*_%s" % recoProcessName,
        "keep *_photonCore_*_%s" % recoProcessName,
        "keep *_particleFlow*_*_%s" % processName,
        "keep *_generalTracks_*_%s" % processName,
        "keep *_muons_*_%s" % processName,
        "keep *_globalMuons_*_%s" % processName,
        "keep *_*Electron*_*_%s" % processName,
        "keep *_eid*_*_*",
        
        )
    outputModule.outputCommands.extend(keepMC)
 
    # getRid of second "drop *"
    index = 0
    for item in outputModule.outputCommands[:]:
        if item == "drop *" and index != 0:
            #print index," ",outputModule.outputCommands[index]
            del outputModule.outputCommands[index]
            index -= 1
        index += 1  

    # Disable gen vertex smearing
 
    process.VtxSmeared = cms.EDProducer("FlatEvtVtxGenerator", 
                                        MaxZ = cms.double(0.0),
                                        MaxX = cms.double(0.0),
                                        MaxY = cms.double(0.0),
                                        MinX = cms.double(0.0),
                                        MinY = cms.double(0.0),
                                        MinZ = cms.double(0.0),
                                        TimeOffset = cms.double(0.0),
                                        src = cms.InputTag("generator")
                                        )
    
    
    
    print "Setting mdtau to ", options.mdtau
    process.generator.ZTauTau.TauolaOptions.InputCards.mdtau = options.mdtau 
    process.newSource.ZTauTau.TauolaOptions.InputCards.mdtau = options.mdtau
    process.generator.ParticleGun.ExternalDecays.Tauola.InputCards.mdtau = options.mdtau 
    process.newSource.ParticleGun.ExternalDecays.Tauola.InputCards.mdtau = options.mdtau 

    print "Setting minVisibleTransverseMomentum to ", options.minVisibleTransverseMomentum
    process.newSource.ZTauTau.minVisibleTransverseMomentum = cms.untracked.string(options.minVisibleTransverseMomentum)
    process.generator.ZTauTau.minVisibleTransverseMomentum = cms.untracked.string(options.minVisibleTransverseMomentum)

    print "Setting transformationMode to ", options.transformationMode
    process.generator.ZTauTau.transformationMode = cms.untracked.int32(options.transformationMode)
    process.newSource.ZTauTau.transformationMode = cms.untracked.int32(options.transformationMode)

    print "options.overrideBeamSpot", options.overrideBeamSpot
    if options.overrideBeamSpot != 0:
        bs = cms.string("BeamSpotObjects_2009_LumiBased_SigmaZ_v26_offline") # 52x data PR gt
        # bs = cms.string("BeamSpotObjects_2009_LumiBased_SigmaZ_v21_offline") # 42x data PR gt
        # bs = cms.string("BeamSpotObjects_2009_LumiBased_SigmaZ_v18_offline") # 41x data PR gt
        # bs = cms.string("BeamSpotObjects_2009_LumiBased_v17_offline") # 38x data gt
        #bs = cms.string("BeamSpotObjects_2009_v14_offline") # 36x data gt
        #  tag = cms.string("Early10TeVCollision_3p8cm_31X_v1_mc_START"), # 35 default
        #  tag = cms.string("Realistic900GeVCollisions_10cm_STARTUP_v1_mc"), # 36 default
        process.GlobalTag.toGet = cms.VPSet(
            cms.PSet(record = cms.string("BeamSpotObjectsRcd"),
                     tag = bs,
                     connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_BEAMSPOT")
                     )
            )
        print "BeamSpot in globaltag set to ", bs 
    else:
        print "BeamSpot in globaltag not changed"

    if options.useJson !=  0:
      print "Enabling json usage"
      import PhysicsTools.PythonAnalysis.LumiList as LumiList
      import FWCore.ParameterSet.Types as CfgTypes
      myLumis = LumiList.LumiList(filename = 'my.json').getCMSSWString().split(',')
      process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
      process.source.lumisToProcess.extend(myLumis)

    # Merge tracks
    process.tmfTracks = cms.EDProducer("RecoTracksMixer",
                                       trackCol1 = cms.InputTag("removedInputMuons","tracks"),
                                       trackCol2 = cms.InputTag("generalTracks","", processName)
                                       )
    
    process.offlinePrimaryVerticesWithBS.TrackLabel = cms.InputTag("tmfTracks")
    process.offlinePrimaryVertices.TrackLabel = cms.InputTag("tmfTracks")
    if hasattr(process.muons, "TrackExtractorPSet"):
        process.muons.TrackExtractorPSet.inputTrackCollection = cms.InputTag("tmfTracks")
    elif hasattr(process, "muons1stStep") and hasattr(process.muons1stStep, "TrackExtractorPSet"):
        process.muons1stStep.TrackExtractorPSet.inputTrackCollection = cms.InputTag("tmfTracks")
    else:
        raise Exception("Problem in overriding track collection for reco::Muon producer")
    
    # Ensure that tmfTracks is always run after generalTracks (to mix the original and embedded tracks)
    for p in process.paths:
        pth = getattr(process, p)
        if "generalTracks" in pth.moduleNames():
            pth.replace(process.generalTracks, process.generalTracks*process.tmfTracks)
            
    # it should be the best solution to take the original beam spot for the
    # reconstruction of the new primary vertex
    # use the  one produced earlier, do not produce your own
    for s in process.sequences:
        seq =  getattr(process,s)
        seq.remove(process.offlineBeamSpot) 


    # Remove beam halo Id
    try:
        process.metreco.remove(process.BeamHaloId)
    except:
        pass
    
    try:
        outputModule = process.output
    except:
        pass
    try:
        outputModule = getattr(process,str(getattr(process,list(process.endpaths)[-1])))
    except:
        pass

    # Disable lumi producer
    process.localreco_HcalNZS.remove(process.lumiProducer)
    process.localreco.remove(process.lumiProducer)
    
    process.particleFlowORG = process.particleFlow.clone()

    # Since CMSSW 4_4 the particleFlow reco works a bit differently. The step is
    # twofold, first particleFlowTmp is created and then the final particleFlow
    # collection. What we do in this case is that we merge the final ParticleFlow
    # collection. For the muon reconstruction, we also merge particleFlowTmp in
    # order to get PF-based isolation right.
    if hasattr(process, 'particleFlowTmp'):
        process.particleFlowTmpMixed = cms.EDProducer('PFCandidateMixer',
                                                      col1 = cms.untracked.InputTag("removedInputMuons","pfCands"),
                                                      col2 = cms.untracked.InputTag("particleFlowTmp", ""),
                                                      trackCol = cms.untracked.InputTag("tmfTracks"),
                                                      # Don't produce value maps:
                                                      muons = cms.untracked.InputTag(""),
                                                      gsfElectrons = cms.untracked.InputTag("")                  
                                                      )
        process.muons.PFCandidates = cms.InputTag("particleFlowTmpMixed")

        for p in process.paths:
            pth = getattr(process, p)
            if "particleFlow" in pth.moduleNames():
                pth.replace(process.particleFlow, process.particleFlowORG*process.particleFlow)
            if "muons" in pth.moduleNames():
                pth.replace(process.muons, process.particleFlowTmpMixed*process.muons)
    else:
        # CMSSW_4_2
        if hasattr(process,"famosParticleFlowSequence"):
            process.famosParticleFlowSequence.remove(process.pfPhotonTranslatorSequence)
            process.famosParticleFlowSequence.remove(process.pfElectronTranslatorSequence)
            process.famosParticleFlowSequence.remove(process.particleFlow)
            process.famosParticleFlowSequence.__iadd__(process.particleFlowORG)
            process.famosParticleFlowSequence.__iadd__(process.particleFlow)
            process.famosParticleFlowSequence.__iadd__(process.pfElectronTranslatorSequence)
            process.famosParticleFlowSequence.__iadd__(process.pfPhotonTranslatorSequence)
        elif hasattr(process,"particleFlowReco"):
            process.particleFlowReco.remove(process.pfPhotonTranslatorSequence)
            process.particleFlowReco.remove(process.pfElectronTranslatorSequence)
            process.particleFlowReco.remove(process.particleFlow)
            process.particleFlowReco.__iadd__(process.particleFlowORG)
            process.particleFlowReco.__iadd__(process.particleFlow)
            process.particleFlowReco.__iadd__(process.pfElectronTranslatorSequence)
            process.particleFlowReco.__iadd__(process.pfPhotonTranslatorSequence)
        else :
            raise "Cannot find particleFlow sequence"

        process.pfSelectedElectrons.src = cms.InputTag("particleFlowORG")
        process.pfSelectedPhotons.src   = cms.InputTag("particleFlowORG")

        
    process.particleFlow =  cms.EDProducer('PFCandidateMixer',
                                           col1 = cms.untracked.InputTag("removedInputMuons","pfCands"),
                                           col2 = cms.untracked.InputTag("particleFlowORG", ""),
                                           trackCol = cms.untracked.InputTag("tmfTracks"),
                                           muons = cms.untracked.InputTag("muons"),
                                           gsfElectrons = cms.untracked.InputTag("gsfElectrons","",recoProcessName) # FIXME does this work?
                                           )

    # Set the empty event filter source
    process.filterEmptyEv.src.setProcessName(processName)



    # Find all modules having particleFlow as their input
    pfInputNeeded = {}
    for p in process.paths:
        i =  getattr(process,p)
        target = process.particleFlow
        
        lookForPFInput = ["particleFlow"]
        
        seqVis = SeqVisitor(target)
        seqVis.prepareSearch()
        seqVis.setLookFor(target)
        i.visit(seqVis)
        while seqVis.catch != 1 and seqVis.found == 1: 
            target = seqVis.giveNext()
          
            pfInput = []
          
            targetAttributes =  dir(target)
            for targetAttribute in targetAttributes:
                attr=getattr(target,targetAttribute) # get actual attribute, not just  the name
                if isinstance(attr, cms.InputTag):
                    if attr.getModuleLabel()=="particleFlow" and attr.getProductInstanceLabel()!="":
                        print "Changing: ", target, " ", targetAttribute, " ", attr, " to particleFlowORG"
                        attr.setModuleLabel("particleFlowORG")
                    if attr.getModuleLabel() in lookForPFInput:
                        pfInput.append(attr)
                          
            if len(pfInput) > 0:
                lookForPFInput.append(target.label())
                pfInputNeeded[target.label()] = pfInput
                              
                              
            #i.replace(target, source) 
            seqVis.prepareSearch()
            seqVis.setLookFor(target)
            i.visit(seqVis)
            
        #if (seqVis.catch==1):
            #seqVis.catch=0
            #i.__iadd__(source)
            
    pfOutputCommands = []
    for label in pfInputNeeded.keys():
        print "particleFlow as input in module %s, InputTags: %s" % (label, ", ".join(str(x) for x in pfInputNeeded[label]))
        pfOutputCommands.append("keep *_%s_*_%s" % (label, processName))

    outputModule.outputCommands.extend(pfOutputCommands)
        
        
    # Setup/remove some HLT/DQM stuff whcih doesn't work
    if hasattr(process, "hltTrigReport"):
        process.hltTrigReport.HLTriggerResults.setProcessName(processName)
    if hasattr(process, "DQM_FEDIntegrity_v2"):
        process.schedule.remove(process.DQM_FEDIntegrity_v2)
    if hasattr(process, "DQM_FEDIntegrity_v3"):
        process.schedule.remove(process.DQM_FEDIntegrity_v3)
    if hasattr(process, "DQM_FEDIntegrity_v5"):
        process.schedule.remove(process.DQM_FEDIntegrity_v5)
    if hasattr(process, "HLTAnalyzerEndpath"):
        process.schedule.remove(process.HLTAnalyzerEndpath)
        del process.HLTAnalyzerEndpath




    skimEnabled = False
    if hasattr(process,"doZmumuSkim"):
        print "Enabling Zmumu skim"
        skimEnabled = True

        cmssw_ver = os.environ["CMSSW_VERSION"]
        if cmssw_ver.find("CMSSW_4_2") != -1:
            print
            print "Using legacy version of Zmumu skim. Note, that muon isolation is disabled"
            print
            process.load("TauAnalysis/MCEmbeddingTools/ZmumuStandalonSelectionLegacy_cff")
        else:
            process.load("TauAnalysis/MCEmbeddingTools/ZmumuStandalonSelection_cff")

        #process.load("TauAnalysis/Skimming/goldenZmmSelectionVBTFrelPFIsolation_cfi")
        process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")

        # we are allready selecting events from generation step, so following way is ok
        for path in process.paths:
            getattr(process,path)._seq = process.goldenZmumuSelectionSequence * getattr(process,path)._seq

        #process.options = cms.untracked.PSet(
        #  wantSummary = cms.untracked.bool(True)
      #)
      
    if hasattr(process,"doWmunuSkim"):
        print "Enabling Wmunu skim"
        skimEnabled = True
        process.load("UserCode/ICHiggsTauTau/WmunuStandalonSelection_cff")
        #process.load("TauAnalysis/Skimming/goldenZmmSelectionVBTFrelPFIsolation_cfi")
        process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")

        # we are allready selecting events from generation step, so following way is ok
        for path in process.paths:
            getattr(process,path)._seq = process.WmunuSelectionSequence * getattr(process,path)._seq

        #process.options = cms.untracked.PSet(
        #  wantSummary = cms.untracked.bool(True)
      #)


    if not skimEnabled:
        print "Zmumu or Wmunu skim not enabled"


    print "# ######################################################################################"
    print "  Following parameters can be added before customize function "
    print "  call in order to controll process  customization: "
    print "     process.doNotParse =  cms.PSet() # disables CL parsing for crab compat"
    print "     process.doZmumuSkim = cms.PSet() # adds Zmumu skimming before embedding is run"
    print "     process.doWmunuSkim = cms.PSet() # adds Wmunu skimming before embedding is run"
    print "# ######################################################################################"




    print "#############################################################"
    print " Warning! PFCandidates 'electron' collection is not mixed, "
    print "  and probably shouldnt be used. "
    print "#############################################################"
    
    
    f = open("skimAndEmbed_data_dump.py", "w")
    f.write(process.dumpPython())
    f.close()
    
    
    return(process)
