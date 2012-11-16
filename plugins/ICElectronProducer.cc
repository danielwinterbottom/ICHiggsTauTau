#include "UserCode/ICHiggsTauTau/plugins/ICElectronProducer.hh"
#include <boost/functional/hash.hpp>
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaTowerIsolation.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

#include "UserCode/ICHiggsTauTau/interface/PATConstruction.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "EGamma/EGammaAnalysisTools/interface/EGammaMvaEleEstimator.h"

#include "boost/format.hpp"


ICElectronProducer::ICElectronProducer(const edm::ParameterSet& iConfig) {
  produces<std::vector<unsigned> >("selectGenParticles");
  input_label_ = iConfig.getParameter<edm::InputTag>("inputLabel");
  branch_name_ = iConfig.getUntrackedParameter<std::string>("branchName");
  add_pfiso_postfix_ = iConfig.getParameter<std::string>("AddPFIsoPostfix");
  rho_jets_name_ = iConfig.getParameter<std::string>("RhoJetsName");
  eff_areas_ = iConfig.getParameter<std::string>("EffAreas");
  pfnopu_rho_jets_name_ = iConfig.getParameter<std::string>("PFNoPURhoJetsName");
  is_pf_ = iConfig.getParameter<bool>("IsPF");
  //eff_area_enum = ElectronEffectiveArea::kEleEANoCorr;
  electrons_ = new std::vector<ic::Electron>();

}

    
ICElectronProducer::~ICElectronProducer() {
  delete electrons_;
}

// ------------ method called to produce the data  ------------
void ICElectronProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  boost::hash<reco::GenParticle const*> particle_hasher;
  boost::hash<pat::Electron const*> pat_electron_hasher;

  // Get input PAT collection
  edm::Handle<std::vector<pat::Electron> > eleCollection;
  iEvent.getByLabel(input_label_,eleCollection);
  std::vector<pat::Electron>::const_iterator iter;

  // Get other inputs
  edm::Handle<reco::BeamSpot> beamspot;
  iEvent.getByLabel(edm::InputTag("offlineBeamSpot"), beamspot);
  edm::Handle<double> rho_handle;
  iEvent.getByLabel(edm::InputTag(rho_jets_name_,"rho"),rho_handle);
  edm::Handle<double> pfnopu_rho_handle;
  iEvent.getByLabel(edm::InputTag(pfnopu_rho_jets_name_,"rho"),pfnopu_rho_handle);
  edm::Handle<reco::ConversionCollection> hConversions;
  iEvent.getByLabel("allConversions", hConversions);
  edm::Handle<reco::GenParticleCollection> partCollection;
  iEvent.getByLabel("genParticles",partCollection);
  edm::Handle<CaloTowerCollection> towerCollection;
  iEvent.getByLabel("towerMaker",towerCollection);
  edm::Handle<reco::VertexCollection> vertexCollection;
  iEvent.getByLabel("goodOfflinePrimaryVertices",vertexCollection);
  // edm::Handle<double> rho_for_mva_iso_handle;
  // iEvent.getByLabel(edm::InputTag("kt6PFJets","rho"),rho_for_mva_iso_handle);
  // edm::Handle<reco::PFCandidateCollection> pf_cands_for_mva_iso_handle;
  // iEvent.getByLabel("particleFlow", pf_cands_for_mva_iso_handle); 
  // const reco::PFCandidateCollection &pf_cands_for_mva_iso = *(pf_cands_for_mva_iso_handle.product());
  // edm::Handle<reco::GsfElectronCollection> elecs_for_mva_iso_handle;
  // iEvent.getByLabel("gsfElectrons",elecs_for_mva_iso_handle);
  // edm::Handle<reco::MuonCollection> muons_for_mva_iso_handle;
  // iEvent.getByLabel("muons",muons_for_mva_iso_handle);

  // reco::MuonCollection IdentifiedMuons;
  // reco::GsfElectronCollection IdentifiedElectrons;

  // if (!is_pf_ && vertexCollection->size() > 0) {
  //   for (reco::GsfElectronCollection::const_iterator iE = elecs_for_mva_iso_handle->begin(); 
  //        iE != elecs_for_mva_iso_handle->end(); ++iE) {
  //     if (electronIDForMVA(*iE, vertexCollection->at(0))) IdentifiedElectrons.push_back(*iE);
  //   }
  //   // Muon veto is no longer recommended
  //   // for (reco::MuonCollection::const_iterator iM = muons_for_mva_iso_handle->begin(); 
  //   //       iM != muons_for_mva_iso_handle->end(); ++iM) {
  //   //   if (muonIDForMVA(*iM)) IdentifiedMuons.push_back(*iM);
  //   // }
  // }

  // By default, we get R04 PF-isolation variables, add R03 values
  // NB PAT userIsolation appears to be broken at the moment so 
  // this is a work-around
  edm::Handle<edm::ValueMap<double> > charged_all_iso;
  edm::Handle<edm::ValueMap<double> > charged_iso;
  edm::Handle<edm::ValueMap<double> > neutral_iso;
  edm::Handle<edm::ValueMap<double> > gamma_iso;
  edm::Handle<edm::ValueMap<double> > pu_iso;

  // In latest recommendation, we don't need to use the NoPFId versions
  // of the isolations when using gsfElectrons- keep it commented here
  // in case this changes again
     edm::Handle<edm::ValueMap<double> > charged_all_iso_04_id;
     edm::Handle<edm::ValueMap<double> > charged_iso_04_id;
     edm::Handle<edm::ValueMap<double> > neutral_iso_04_id;
     edm::Handle<edm::ValueMap<double> > gamma_iso_04_id;
     edm::Handle<edm::ValueMap<double> > pu_iso_04_id;
  // edm::Handle<edm::ValueMap<double> > charged_iso_04_noid;
  // edm::Handle<edm::ValueMap<double> > neutral_iso_04_noid;
  // edm::Handle<edm::ValueMap<double> > gamma_iso_04_noid;
  // edm::Handle<edm::ValueMap<double> > pu_iso_04_noid;
  // edm::Handle<edm::ValueMap<double> > charged_iso_03_noid;
  // edm::Handle<edm::ValueMap<double> > neutral_iso_03_noid;
  // edm::Handle<edm::ValueMap<double> > gamma_iso_03_noid;
  // edm::Handle<edm::ValueMap<double> > pu_iso_03_noid;
  if (add_pfiso_postfix_ != "") {
    iEvent.getByLabel(edm::InputTag("elPFIsoValueChargedAll"+add_pfiso_postfix_),charged_all_iso);
    iEvent.getByLabel(edm::InputTag("elPFIsoValueCharged"+add_pfiso_postfix_),charged_iso);
    iEvent.getByLabel(edm::InputTag("elPFIsoValueNeutral"+add_pfiso_postfix_),neutral_iso);
    iEvent.getByLabel(edm::InputTag("elPFIsoValueGamma"+add_pfiso_postfix_),gamma_iso);
    iEvent.getByLabel(edm::InputTag("elPFIsoValuePU"+add_pfiso_postfix_),pu_iso);
     if (!is_pf_) {
      iEvent.getByLabel(edm::InputTag("elPFIsoValueChargedAll04PFIdPFIso"),charged_all_iso_04_id);
      iEvent.getByLabel(edm::InputTag("elPFIsoValueCharged04PFIdPFIso"),charged_iso_04_id);
      iEvent.getByLabel(edm::InputTag("elPFIsoValueNeutral04PFIdPFIso"),neutral_iso_04_id);
      iEvent.getByLabel(edm::InputTag("elPFIsoValueGamma04PFIdPFIso"),gamma_iso_04_id);
      iEvent.getByLabel(edm::InputTag("elPFIsoValuePU04PFIdPFIso"),pu_iso_04_id);
    //   iEvent.getByLabel(edm::InputTag("elPFIsoValueCharged04NoPFIdPFIso"),charged_iso_04_noid);
    //   iEvent.getByLabel(edm::InputTag("elPFIsoValueNeutral04NoPFIdPFIso"),neutral_iso_04_noid);
    //   iEvent.getByLabel(edm::InputTag("elPFIsoValueGamma04NoPFIdPFIso"),gamma_iso_04_noid);
    //   iEvent.getByLabel(edm::InputTag("elPFIsoValuePU04NoPFIdPFIso"),pu_iso_04_noid);
    //   iEvent.getByLabel(edm::InputTag("elPFIsoValueCharged03NoPFIdPFIso"),charged_iso_03_noid);
    //   iEvent.getByLabel(edm::InputTag("elPFIsoValueNeutral03NoPFIdPFIso"),neutral_iso_03_noid);
    //   iEvent.getByLabel(edm::InputTag("elPFIsoValueGamma03NoPFIdPFIso"),gamma_iso_03_noid);
    //   iEvent.getByLabel(edm::InputTag("elPFIsoValuePU03NoPFIdPFIso"),pu_iso_03_noid);
     }
  }


  EcalClusterLazyTools lazyTool( iEvent, iSetup, edm::InputTag("reducedEcalRecHitsEB"),edm::InputTag("reducedEcalRecHitsEE"));
  EgammaTowerIsolation hadDepth1Isolation03(0.3,0.0,0.0,1,towerCollection.product());
  EgammaTowerIsolation hadDepth2Isolation03(0.3,0.0,0.0,2,towerCollection.product());
  
  // edm::ESHandle<TransientTrackBuilder> builder;
  // iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", builder);

 
  // Prepare output collection
  electrons_->resize(0);
  electrons_->reserve(eleCollection->size());

  // Prepare ouput requests
  std::auto_ptr<std::vector<unsigned> > ele_particles(new std::vector<unsigned>());
  
  for (iter = eleCollection->begin(); iter != eleCollection->end(); ++iter) {
    electrons_->push_back(ic::Electron());
    ic::Electron & ele = electrons_->back();

    ele.set_id(pat_electron_hasher(&(*iter)));
    //std::cout << "Electron " << unsigned(iter - eleCollection->begin()) << ": " << pat_electron_hasher(&(*iter)) << std::endl;
    ele.set_pt(iter->pt());
    ele.set_eta(iter->eta());
    ele.set_phi(iter->phi());
    ele.set_energy(iter->energy());
    ele.set_charge(iter->charge());

    ele.set_dr03_tk_sum_pt(iter->dr03TkSumPt());
    ele.set_dr03_ecal_rechit_sum_et(iter->dr03EcalRecHitSumEt());
    ele.set_dr03_hcal_tower_sum_et(iter->dr03HcalTowerSumEt());
    
    float dr03_pfiso_charged_all = -1.0;
    float dr03_pfiso_charged = -1.0;
    float dr03_pfiso_neutral = -1.0;
    float dr03_pfiso_gamma = -1.0;
    float dr03_pfiso_pu = -1.0;
    float dr04_pfiso_charged_all = -1.0;
    float dr04_pfiso_charged = -1.0;
    float dr04_pfiso_neutral = -1.0;
    float dr04_pfiso_gamma = -1.0;
    float dr04_pfiso_pu = -1.0;
    if (add_pfiso_postfix_ != "") {
      if (is_pf_) {
        dr03_pfiso_charged_all = (*charged_all_iso)[iter->pfCandidateRef()->sourceCandidatePtr(0)];
        dr03_pfiso_charged = (*charged_iso)[iter->pfCandidateRef()->sourceCandidatePtr(0)];
        dr03_pfiso_neutral = (*neutral_iso)[iter->pfCandidateRef()->sourceCandidatePtr(0)];
        dr03_pfiso_gamma = (*gamma_iso)[iter->pfCandidateRef()->sourceCandidatePtr(0)];
        dr03_pfiso_pu = (*pu_iso)[iter->pfCandidateRef()->sourceCandidatePtr(0)];

        dr04_pfiso_charged_all = iter->userIsolation(pat::PfChargedAllIso);
        dr04_pfiso_charged = iter->chargedHadronIso();
        dr04_pfiso_neutral = iter->neutralHadronIso();
        dr04_pfiso_gamma = iter->photonIso();
        dr04_pfiso_pu = iter->puChargedHadronIso();
      } else {
          dr03_pfiso_charged_all = (*charged_all_iso)[iter->originalObjectRef()];
          dr03_pfiso_charged = (*charged_iso)[iter->originalObjectRef()];
          dr03_pfiso_neutral = (*neutral_iso)[iter->originalObjectRef()];
          dr03_pfiso_gamma = (*gamma_iso)[iter->originalObjectRef()];
          dr03_pfiso_pu = (*pu_iso)[iter->originalObjectRef()];
          dr04_pfiso_charged_all = (*charged_all_iso_04_id)[iter->originalObjectRef()];
          dr04_pfiso_charged = (*charged_iso_04_id)[iter->originalObjectRef()];
          dr04_pfiso_neutral = (*neutral_iso_04_id)[iter->originalObjectRef()];
          dr04_pfiso_gamma = (*gamma_iso_04_id)[iter->originalObjectRef()];
          dr04_pfiso_pu = (*pu_iso_04_id)[iter->originalObjectRef()];
      }
    }
    ele.set_dr03_pfiso_charged_all(dr03_pfiso_charged_all);
    ele.set_dr03_pfiso_charged(dr03_pfiso_charged);
    ele.set_dr03_pfiso_neutral(dr03_pfiso_neutral);
    ele.set_dr03_pfiso_gamma(dr03_pfiso_gamma);
    ele.set_dr03_pfiso_pu(dr03_pfiso_pu);
    ele.set_dr04_pfiso_charged_all(dr04_pfiso_charged_all);
    ele.set_dr04_pfiso_charged(dr04_pfiso_charged);
    ele.set_dr04_pfiso_neutral(dr04_pfiso_neutral);
    ele.set_dr04_pfiso_gamma(dr04_pfiso_gamma);
    ele.set_dr04_pfiso_pu(dr04_pfiso_pu);

    if (!is_pf_) {
      std::vector< std::pair<std::string, float> > idpairs = iter->electronIDs();
      for (unsigned i = 0; i < idpairs.size(); ++i) {
        ele.SetIdIso(idpairs[i].first, idpairs[i].second);
        observed_idiso_[idpairs[i].first] = CityHash64(idpairs[i].first);
      }

      // double id2011mva = f2011MVA->MVAValue(&(*iter), lazyTool);
      // ele.SetIdIso("id2011mva",id2011mva);
      // observed_idiso_["id2011mva"] = CityHash64("id2011mva");

      // if (vertexCollection->size() > 0) {
      //   double isomva = fElectronIsoMVA->mvaValue(  *iter, 
      //                                             vertexCollection->at(0), 
      //                                             pf_cands_for_mva_iso, 
      //                                             *rho_for_mva_iso_handle, 
      //                                             eff_area_enum,
      //                                             IdentifiedElectrons, 
      //                                             IdentifiedMuons); 
      //   ele.SetIdIso("isomva",isomva);
      // } else {
      //   ele.SetIdIso("isomva",0.0);
      // }
      // observed_idiso_["isomva"] = CityHash64("isomva");
    }

    ele.set_hadronic_over_em(iter->hadronicOverEm());
    ele.set_sigma_IetaIeta(iter->sigmaIetaIeta());
    ele.set_dphi_sc_tk_at_vtx(iter->deltaPhiSuperClusterTrackAtVtx());
    ele.set_deta_sc_tk_at_vtx(iter->deltaEtaSuperClusterTrackAtVtx());
    ele.set_gsf_tk_nhits(iter->gsfTrack()->trackerExpectedHitsInner().numberOfHits());
    ele.set_conv_dist(iter->convDist());
    ele.set_conv_dcot(iter->convDcot());
    bool has_match = ConversionTools::hasMatchedConversion(dynamic_cast<reco::GsfElectron const&>(*(iter->originalObjectRef())), hConversions, beamspot->position(), true, 2.0, 1e-6, 0);
    ele.set_has_matched_conversion(has_match);
 
    ele.set_mva(iter->mva());

    ele.set_f_brem(iter->fbrem());
    ele.set_sc_eta(iter->superCluster()->eta());
    ele.set_sc_theta(iter->superCluster()->position().theta());
    ele.set_sc_energy(iter->superCluster()->energy());
    ele.set_sc_e_over_p(iter->eSuperClusterOverP());

    float e3x3 = lazyTool.e3x3(*(iter->superCluster()));
    float sc_raw_energy = iter->superCluster()->rawEnergy();
    ele.set_r9(e3x3 / sc_raw_energy);

    float hcalDepth1TowerSumEt03 = hadDepth1Isolation03.getTowerEtSum(&(*(iter->originalObjectRef())));
    float hcalDepth2TowerSumEt03 = hadDepth2Isolation03.getTowerEtSum(&(*(iter->originalObjectRef())));
    ele.set_hcal_sum(hcalDepth1TowerSumEt03+hcalDepth2TowerSumEt03);
    ele.set_ecal_energy(iter->ecalEnergy());


    ele.set_rho(*rho_handle);
    ele.set_pfnopu_rho(*pfnopu_rho_handle);
    ele.set_vx(iter->vx());
    ele.set_vy(iter->vy());
    ele.set_vz(iter->vz());

    // pat::Electron::BS2D method is still broken in 4_4_3
    //ele.set_dxy_beamspot(iter->dB(pat::Electron::BS2D));
    if (vertexCollection->size() > 0) {
      ele.set_dz_vertex(iter->gsfTrack()->dz(vertexCollection->at(0).position()));
      ele.set_dxy_vertex(iter->gsfTrack()->dxy(vertexCollection->at(0).position()));
    } else {
      ele.set_dz_vertex(9999.);
      ele.set_dxy_vertex(9999.);
    }
    ele.set_dxy_beamspot(iter->gsfTrack()->dxy(*beamspot));

    if (iter->triggerObjectMatches().size() > 0) {
      std::vector<std::size_t> hlt_paths;
      std::vector<std::size_t> hlt_filters;
      std::vector<std::string> const& hlt_path_strings = iter->triggerObjectMatches().at(0).pathNames(0,0);
      std::vector<std::string> const& hlt_filter_strings = iter->triggerObjectMatches().at(0).filterLabels();     
      hlt_paths.resize(hlt_path_strings.size());
      hlt_filters.resize(hlt_filter_strings.size());
      for (unsigned i = 0; i < hlt_paths.size(); ++i) {
        hlt_paths[i] = CityHash64(hlt_path_strings[i]);
        observed_paths_[hlt_path_strings[i]] = CityHash64(hlt_path_strings[i]);
      }
      for (unsigned i = 0; i < hlt_filters.size(); ++i) {
       hlt_filters[i] = CityHash64(hlt_filter_strings[i]);
       observed_filters_[hlt_filter_strings[i]] = CityHash64(hlt_filter_strings[i]);
      }
     ele.set_hlt_match_paths(hlt_paths);
     ele.set_hlt_match_filters(hlt_filters);
    }

    if (iter->genLepton()) {
      unsigned idx = unsigned((reco::GenParticle*)iter->genLepton() - &(partCollection->at(0)));
      ele_particles->push_back(idx);
      std::vector<std::size_t> gen_index;
      gen_index.push_back(particle_hasher(iter->genLepton()));
      ele.set_gen_particles(gen_index); 
    }
  }
  
  iEvent.put(ele_particles, "selectGenParticles");
}

// ------------ method called once each job just before starting event loop  ------------
void ICElectronProducer::beginJob() {
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Info in <ICElectronProducer>: BeginJob Summary for input: " << input_label_.label() << std::endl;
  std::cout << "-----------------------------------------------------------------" << std::endl;

  // f2011MVA = new ElectronIDMVA();
  // f2011MVA->Initialize("BDTG method",
  //                  edm::FileInPath("UserCode/MitPhysics/data/ElectronMVAWeights/Subdet0LowPt_NoIPInfo_BDTG.weights.xml").fullPath(),
  //                  edm::FileInPath("UserCode/MitPhysics/data/ElectronMVAWeights/Subdet1LowPt_NoIPInfo_BDTG.weights.xml").fullPath(),
  //                  edm::FileInPath("UserCode/MitPhysics/data/ElectronMVAWeights/Subdet2LowPt_NoIPInfo_BDTG.weights.xml").fullPath(),
  //                  edm::FileInPath("UserCode/MitPhysics/data/ElectronMVAWeights/Subdet0HighPt_NoIPInfo_BDTG.weights.xml").fullPath(),
  //                  edm::FileInPath("UserCode/MitPhysics/data/ElectronMVAWeights/Subdet1HighPt_NoIPInfo_BDTG.weights.xml").fullPath(),
  //                  edm::FileInPath("UserCode/MitPhysics/data/ElectronMVAWeights/Subdet2HighPt_NoIPInfo_BDTG.weights.xml").fullPath(),                
  //                  ElectronIDMVA::kNoIPInfo);

  //Electron ISO MVA is not currently needed
  // fElectronIsoMVA = new EGammaMvaEleEstimator();
  // std::vector<std::string> eleiso_weightfiles;
  // eleiso_weightfiles.push_back(edm::FileInPath("UserCode/sixie/EGamma/EGammaAnalysisTools/data/ElectronIso_BDTG_V0_BarrelPt5To10.weights.xml").fullPath());
  // eleiso_weightfiles.push_back(edm::FileInPath("UserCode/sixie/EGamma/EGammaAnalysisTools/data/ElectronIso_BDTG_V0_EndcapPt5To10.weights.xml").fullPath());
  // eleiso_weightfiles.push_back(edm::FileInPath("UserCode/sixie/EGamma/EGammaAnalysisTools/data/ElectronIso_BDTG_V0_BarrelPt10ToInf.weights.xml").fullPath());
  // eleiso_weightfiles.push_back(edm::FileInPath("UserCode/sixie/EGamma/EGammaAnalysisTools/data/ElectronIso_BDTG_V0_EndcapPt10ToInf.weights.xml").fullPath());
  // fElectronIsoMVA->initialize("EleIso_BDTG_IsoRings",
  //                   EGammaMvaEleEstimator::kIsoRings,
  //                   kTRUE,
  //                   eleiso_weightfiles);
  // fElectronIsoMVA->SetPrintMVADebug(false);

  // if (eff_areas_ != "") {
  //   if (eff_areas_ == "kEleEAData2011") {
  //     eff_area_enum = ElectronEffectiveArea::kEleEAData2011;
  //     std::cout << "Using effective areas: " << eff_areas_ << std::endl;
  //   } else if (eff_areas_ == "kEleEASummer11MC") {
  //     eff_area_enum = ElectronEffectiveArea::kEleEASummer11MC;
  //     std::cout << "Using effective areas: " << eff_areas_ << std::endl;
  //   } else if (eff_areas_ == "kEleEAFall11MC") {
  //     eff_area_enum = ElectronEffectiveArea::kEleEAFall11MC;
  //     std::cout << "Using effective areas: " << eff_areas_ << std::endl;
  //   } else if (eff_areas_ == "kEleEAData2012") {
  //     eff_area_enum = ElectronEffectiveArea::kEleEAData2012;
  //     std::cout << "Using effective areas: " << eff_areas_ << std::endl;
  //   } else {
  //     std::cout << "Effective area payload not recognised!" << std::endl;
  //   }
  // } else {
  //   std::cout << "No effective area payload specified!" << std::endl;
  // }

  ic::StaticTree::tree_->Branch(branch_name_.c_str(), &electrons_);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICElectronProducer::endJob() {
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Info in <ICElectronProducer>: EndJob Summary for input: " << input_label_.label() << std::endl;
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "HLT Match Paths Hash Summary:" << std::endl;
  std::map<std::string, std::size_t>::const_iterator iter;
  for (iter = observed_paths_.begin(); iter != observed_paths_.end(); ++iter) {
    std::cout << boost::format("%-60s %-30s\n") % iter->first % iter->second;
  }
  std::cout << "HLT Match Filters Hash Summary:" << std::endl;
  for (iter = observed_filters_.begin(); iter != observed_filters_.end(); ++iter) {
    std::cout << boost::format("%-60s %-20s\n") % iter->first % iter->second;
  }
  std::cout << "ID and Iso Label Hash Summary:" << std::endl;
  for (iter = observed_idiso_.begin(); iter != observed_idiso_.end(); ++iter) {
    std::cout << boost::format("%-60s %-20s\n") % iter->first % iter->second;
  }
}

// ------------ method called when starting to processes a run  ------------
void ICElectronProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICElectronProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICElectronProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICElectronProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICElectronProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

bool ICElectronProducer::electronIDForMVA(reco::GsfElectron const& elec, reco::Vertex const& vtx) {

    double electronTrackZ = 0;
    if (elec.gsfTrack().isNonnull()) {
      electronTrackZ = elec.gsfTrack()->dz(vtx.position());
    } else if (elec.closestCtfTrackRef().isNonnull()) {
      electronTrackZ = elec.closestCtfTrackRef()->dz(vtx.position());
    }    
    if(fabs(electronTrackZ) > 0.2)  return false;

    if(fabs(elec.superCluster()->eta())<1.479) {     
      if(elec.pt() > 20) {
        if(elec.sigmaIetaIeta()       > 0.01)  return false;
        if(fabs(elec.deltaEtaSuperClusterTrackAtVtx()) > 0.007) return false;
        if(fabs(elec.deltaPhiSuperClusterTrackAtVtx()) > 0.8)  return false;
        if(elec.hadronicOverEm()       > 0.15)  return false;    
      } else {
        if(elec.sigmaIetaIeta()       > 0.012)  return false;
        if(fabs(elec.deltaEtaSuperClusterTrackAtVtx()) > 0.007) return false;
        if(fabs(elec.deltaPhiSuperClusterTrackAtVtx()) > 0.8)  return false;
        if(elec.hadronicOverEm()       > 0.15) return false;    
      } 
    } else {     
      if(elec.pt() > 20) {
        if(elec.sigmaIetaIeta()       > 0.03)  return false;
        if(fabs(elec.deltaEtaSuperClusterTrackAtVtx()) > 0.010) return false;
        if(fabs(elec.deltaPhiSuperClusterTrackAtVtx()) > 0.8)  return false;
      } else {
        if(elec.sigmaIetaIeta()       > 0.032)  return false;
        if(fabs(elec.deltaEtaSuperClusterTrackAtVtx()) > 0.010) return false;
        if(fabs(elec.deltaPhiSuperClusterTrackAtVtx()) > 0.8)  return false;
      }
    }
    return true;
}

bool ICElectronProducer::muonIDForMVA(reco::Muon const& muon) {
  if(!(muon.innerTrack().isNonnull())) {
    return false;
  } 
  if(!(muon.isGlobalMuon() || muon.isTrackerMuon())) return false;
  if(muon.innerTrack()->numberOfValidHits() < 11 ) return false;
  return true;
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICElectronProducer);
