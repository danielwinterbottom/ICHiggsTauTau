#include "UserCode/ICHiggsTauTau/plugins/ICMuonProducer.hh"
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

#include "DataFormats/PatCandidates/interface/Muon.h"

#include "UserCode/ICHiggsTauTau/interface/PATConstruction.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"

#include "boost/format.hpp"


ICMuonProducer::ICMuonProducer(const edm::ParameterSet& iConfig) {
  produces<std::vector<unsigned> >("selectGenParticles");
  input_label_ = iConfig.getParameter<edm::InputTag>("inputLabel");
  branch_name_ = iConfig.getUntrackedParameter<std::string>("branchName");
  add_pfiso_postfix_ = iConfig.getParameter<std::string>("AddPFIsoPostfix");
  rho_jets_name_ = iConfig.getParameter<std::string>("RhoJetsName");
  pfnopu_rho_jets_name_ = iConfig.getParameter<std::string>("RhoJetsName");
  eff_areas_ = iConfig.getParameter<std::string>("EffAreas");
  is_pf_ = iConfig.getParameter<bool>("IsPF");
  //eff_area_enum = MuonEffectiveArea::kMuEANoCorr;
  muons_ = new std::vector<ic::Muon>();
}


ICMuonProducer::~ICMuonProducer() {
  delete muons_;
}

// ------------ method called to produce the data  ------------
void ICMuonProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  boost::hash<reco::GenParticle const*> particle_hasher;
  boost::hash<pat::Muon const*> pat_muon_hasher;

  // Get input PAT collection
  edm::Handle<std::vector<pat::Muon> > muoCollection;
  iEvent.getByLabel(input_label_,muoCollection);
  std::vector<pat::Muon>::const_iterator iter;



  // Get other inputs
  edm::Handle<double> rho_handle;
  iEvent.getByLabel(edm::InputTag(rho_jets_name_,"rho"),rho_handle);
  edm::Handle<double> pfnopu_rho_handle;
  iEvent.getByLabel(edm::InputTag(pfnopu_rho_jets_name_,"rho"),pfnopu_rho_handle);
  edm::Handle<reco::GenParticleCollection> partCollection;
  iEvent.getByLabel("genParticles",partCollection);
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

  // if (is_pf_ && vertexCollection->size() > 0) {
  //   for (reco::GsfElectronCollection::const_iterator iE = elecs_for_mva_iso_handle->begin(); 
  //        iE != elecs_for_mva_iso_handle->end(); ++iE) {
  //     if (electronIDForMVA(*iE, vertexCollection->at(0))) IdentifiedElectrons.push_back(*iE);
  //   }
  //   for (reco::MuonCollection::const_iterator iM = muons_for_mva_iso_handle->begin(); 
  //         iM != muons_for_mva_iso_handle->end(); ++iM) {
  //     if (muonIDForMVA(*iM)) IdentifiedMuons.push_back(*iM);
  //   }
  // }



  edm::Handle<edm::ValueMap<double> > charged_all_iso;
  edm::Handle<edm::ValueMap<double> > charged_iso;
  edm::Handle<edm::ValueMap<double> > neutral_iso;
  edm::Handle<edm::ValueMap<double> > gamma_iso;
  edm::Handle<edm::ValueMap<double> > pu_iso;
  if (add_pfiso_postfix_ != "") {
    iEvent.getByLabel(edm::InputTag("muPFIsoValueChargedAll"+add_pfiso_postfix_),charged_all_iso);
    iEvent.getByLabel(edm::InputTag("muPFIsoValueCharged"+add_pfiso_postfix_),charged_iso);
    iEvent.getByLabel(edm::InputTag("muPFIsoValueNeutral"+add_pfiso_postfix_),neutral_iso);
    iEvent.getByLabel(edm::InputTag("muPFIsoValueGamma"+add_pfiso_postfix_),gamma_iso);
    iEvent.getByLabel(edm::InputTag("muPFIsoValuePU"+add_pfiso_postfix_),pu_iso);
  }

  // Prepare output collection
  muons_->resize(0);
  muons_->reserve(muoCollection->size());

  // Prepare ouput requests
  std::auto_ptr<std::vector<unsigned> > muo_particles(new std::vector<unsigned>());
  for (iter = muoCollection->begin(); iter != muoCollection->end(); ++iter) {
    muons_->push_back(ic::Muon());
    ic::Muon & muo = muons_->back();

    muo.set_id(pat_muon_hasher(&(*iter)));
    muo.set_pt(iter->pt());
    muo.set_eta(iter->eta());
    muo.set_phi(iter->phi());
    muo.set_energy(iter->energy());
    muo.set_charge(iter->charge());

    muo.set_dr03_tk_sum_pt(iter->isolationR03().sumPt);
    muo.set_dr03_ecal_rechit_sum_et(iter->isolationR03().emEt);
    muo.set_dr03_hcal_tower_sum_et(iter->isolationR03().hadEt);

    float dr03_pfiso_charged_all = -1.0;
    float dr03_pfiso_charged = -1.0;
    float dr03_pfiso_neutral = -1.0;
    float dr03_pfiso_gamma = -1.0;
    float dr03_pfiso_pu = -1.0;
    if (add_pfiso_postfix_ != "") {
      if (is_pf_) {
        dr03_pfiso_charged_all = (*charged_all_iso)[reco::CandidateBaseRef(iter->pfCandidateRef())];
        dr03_pfiso_charged = (*charged_iso)[reco::CandidateBaseRef(iter->pfCandidateRef())];
        dr03_pfiso_neutral = (*neutral_iso)[reco::CandidateBaseRef(iter->pfCandidateRef())];
        dr03_pfiso_gamma = (*gamma_iso)[reco::CandidateBaseRef(iter->pfCandidateRef())];
        dr03_pfiso_pu = (*pu_iso)[reco::CandidateBaseRef(iter->pfCandidateRef())];
      } else {
        dr03_pfiso_charged_all = (*charged_all_iso)[iter->originalObjectRef()];
        dr03_pfiso_charged = (*charged_iso)[iter->originalObjectRef()];
        dr03_pfiso_neutral = (*neutral_iso)[iter->originalObjectRef()];
        dr03_pfiso_gamma = (*gamma_iso)[iter->originalObjectRef()];
        dr03_pfiso_pu = (*pu_iso)[iter->originalObjectRef()];
      }
    }
    muo.set_dr03_pfiso_charged_all(dr03_pfiso_charged_all);
    muo.set_dr03_pfiso_charged(dr03_pfiso_charged);
    muo.set_dr03_pfiso_neutral(dr03_pfiso_neutral);
    muo.set_dr03_pfiso_gamma(dr03_pfiso_gamma);
    muo.set_dr03_pfiso_pu(dr03_pfiso_pu);
    muo.set_dr04_pfiso_charged_all(iter->userIsolation(pat::PfChargedAllIso));
    muo.set_dr04_pfiso_charged(iter->chargedHadronIso());
    muo.set_dr04_pfiso_neutral(iter->neutralHadronIso());
    muo.set_dr04_pfiso_gamma(iter->photonIso());
    muo.set_dr04_pfiso_pu(iter->puChargedHadronIso());
    // if (is_pf_ && iter->isTrackerMuon() && iter->isGlobalMuon() && vertexCollection->size() > 0) {
    //   double isomva = fMuonIsoMVA->mvaValue( *iter, vertexCollection->at(0), 
    //     pf_cands_for_mva_iso, *rho_for_mva_iso_handle, 
    //     eff_area_enum, IdentifiedElectrons, IdentifiedMuons);
    //   double idmva = fMuonIDMVA->mvaValue( *iter, vertexCollection->at(0), 
    //     pf_cands_for_mva_iso, *rho_for_mva_iso_handle, 
    //     eff_area_enum, IdentifiedElectrons, IdentifiedMuons);
    //   double isoringsradmva = fMuonIsoRingsRadMVA->mvaValue( *iter, vertexCollection->at(0), 
    //     pf_cands_for_mva_iso, *rho_for_mva_iso_handle, 
    //     eff_area_enum, IdentifiedElectrons, IdentifiedMuons);
    //   double idisomva = fMuonIDIsoCombinedMVA->mvaValue( *iter, vertexCollection->at(0), 
    //     pf_cands_for_mva_iso, *rho_for_mva_iso_handle, 
    //     eff_area_enum, IdentifiedElectrons, IdentifiedMuons);
    //   muo.SetIdIso("isomva",isomva);
    //   muo.SetIdIso("idmva", idmva);
    //   muo.SetIdIso("isoringsradmva", isoringsradmva);
    //   muo.SetIdIso("idisomva", idisomva);
    // } else {
    //   muo.SetIdIso("isomva", -100.);
    //   muo.SetIdIso("idmva", -100.);
    //   muo.SetIdIso("isoringsradmva", -100.);
    //   muo.SetIdIso("idisomva", -100.);
    // }
    // observed_idiso_["isomva"]         = CityHash64("isomva");
    // observed_idiso_["idmva"]          = CityHash64("idmva");
    // observed_idiso_["isoringsradmva"] = CityHash64("isoringsradmva");
    // observed_idiso_["idisomva"]       = CityHash64("idisomva");



    muo.set_is_standalone(iter->isStandAloneMuon());
    muo.set_is_global(iter->isGlobalMuon());
    muo.set_is_tracker(iter->isTrackerMuon());
    muo.set_is_calo(iter->isCaloMuon());
    if (iter->isGlobalMuon()) {
        muo.set_gt_normalized_chi2(iter->globalTrack()->normalizedChi2());
        muo.set_gt_valid_muon_hits(iter->globalTrack()->hitPattern().numberOfValidMuonHits());
    }
    muo.set_matched_stations(iter->numberOfMatchedStations());
    if (iter->isTrackerMuon()) {
        muo.set_it_pixel_hits(iter->innerTrack()->hitPattern().numberOfValidPixelHits());
        muo.set_it_tracker_hits(iter->innerTrack()->hitPattern().numberOfValidTrackerHits());
        muo.set_it_layers_with_measurement(iter->innerTrack()->hitPattern().trackerLayersWithMeasurement());
    }
    muo.set_rho(*rho_handle);
    muo.set_pfnopu_rho(*pfnopu_rho_handle);

    muo.set_vx(iter->vx());
    muo.set_vy(iter->vy());
    muo.set_vz(iter->vz());

    muo.set_dxy_beamspot(iter->dB(pat::Muon::BS2D));
    if (vertexCollection->size() > 0 && iter->isGlobalMuon()) {
      muo.set_dz_vertex(iter->innerTrack()->dz(vertexCollection->at(0).position()));
      muo.set_dxy_vertex(iter->innerTrack()->dxy(vertexCollection->at(0).position()));
    } else {
      muo.set_dz_vertex(9999.);
      muo.set_dxy_vertex(9999.);
    }

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
     muo.set_hlt_match_paths(hlt_paths);
     muo.set_hlt_match_filters(hlt_filters);
    }
    
    if (iter->genLepton()) {
      unsigned idx = unsigned((reco::GenParticle*)iter->genLepton() - &(partCollection->at(0)));
      muo_particles->push_back(idx);
      std::vector<std::size_t> gen_index;
      gen_index.push_back(particle_hasher(iter->genLepton()));
      muo.set_gen_particles(gen_index); 
    } 
  }

  iEvent.put(muo_particles, "selectGenParticles");
}

// ------------ method called once each job just before starting event loop  ------------
void ICMuonProducer::beginJob() {
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Info in <ICMuonProducer>: BeginJob Summary for input: " << input_label_.label() << std::endl;
  std::cout << "-----------------------------------------------------------------" << std::endl;
  
  // Muon MVA isolation not currently needed
  // fMuonIsoMVA = new MuonMVAEstimator();
  // fMuonIDMVA = new MuonMVAEstimator();
  // fMuonIsoRingsRadMVA = new MuonMVAEstimator();
  // fMuonIDIsoCombinedMVA = new MuonMVAEstimator();

  // std::vector<std::string> muoniso_weightfiles;
  // muoniso_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIsoMVA_sixie-BarrelPt5To10_V0_BDTG.weights.xml").fullPath());
  // muoniso_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIsoMVA_sixie-EndcapPt5To10_V0_BDTG.weights.xml").fullPath());
  // muoniso_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIsoMVA_sixie-BarrelPt10ToInf_V0_BDTG.weights.xml").fullPath());
  // muoniso_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIsoMVA_sixie-EndcapPt10ToInf_V0_BDTG.weights.xml").fullPath());
  // muoniso_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIsoMVA_sixie-Tracker_V0_BDTG.weights.xml").fullPath());
  // muoniso_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIsoMVA_sixie-Global_V0_BDTG.weights.xml").fullPath());

  // std::vector<std::string> muonid_weightfiles;
  // muonid_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIDMVA_sixie-BarrelPt5To10_V0_BDTG.weights.xml").fullPath());
  // muonid_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIDMVA_sixie-EndcapPt5To10_V0_BDTG.weights.xml").fullPath());
  // muonid_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIDMVA_sixie-BarrelPt10ToInf_V0_BDTG.weights.xml").fullPath());
  // muonid_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIDMVA_sixie-EndcapPt10ToInf_V0_BDTG.weights.xml").fullPath());
  // muonid_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIDMVA_sixie-Tracker_V0_BDTG.weights.xml").fullPath());
  // muonid_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIDMVA_sixie-Global_V0_BDTG.weights.xml").fullPath());
  
  // std::vector<std::string> muonisoRingsRad_weightfiles;
  // muonisoRingsRad_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIsoMVA_sixie-BarrelPt5To10_V1_BDTG.weights.xml").fullPath());
  // muonisoRingsRad_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIsoMVA_sixie-EndcapPt5To10_V1_BDTG.weights.xml").fullPath());
  // muonisoRingsRad_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIsoMVA_sixie-BarrelPt10ToInf_V1_BDTG.weights.xml").fullPath());
  // muonisoRingsRad_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIsoMVA_sixie-EndcapPt10ToInf_V1_BDTG.weights.xml").fullPath());
  // muonisoRingsRad_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIsoMVA_sixie-Tracker_V1_BDTG.weights.xml").fullPath());
  // muonisoRingsRad_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIsoMVA_sixie-Global_V1_BDTG.weights.xml").fullPath());
  
  // std::vector<std::string> muonidiso_weightfiles;
  // muonidiso_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIDIsoCombinedMVA_V0_barrel_lowpt.weights.xml").fullPath());
  // muonidiso_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIDIsoCombinedMVA_V0_endcap_lowpt.weights.xml").fullPath());
  // muonidiso_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIDIsoCombinedMVA_V0_barrel_highpt.weights.xml").fullPath());
  // muonidiso_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIDIsoCombinedMVA_V0_endcap_highpt.weights.xml").fullPath());
  // muonidiso_weightfiles.push_back(edm::FileInPath("Muon/MuonAnalysisTools/data/MuonIDIsoCombinedMVA_V0_tracker.weights.xml").fullPath());

  // fMuonIsoMVA->initialize("MuonIso_BDTG_IsoRings",
  //                  MuonMVAEstimator::kIsoRings,
  //                  kTRUE,
  //                  muoniso_weightfiles);
  // fMuonIsoMVA->SetPrintMVADebug(false);

  // fMuonIDMVA->initialize("MuonID_BDTG",
  //                  MuonMVAEstimator::kID,
  //                  kTRUE,
  //                  muonid_weightfiles);
  // fMuonIDMVA->SetPrintMVADebug(false);

  // fMuonIsoRingsRadMVA->initialize("MuonIso_BDTG_IsoRingsRad",
  //                  MuonMVAEstimator::kIsoRingsRadial,
  //                  kTRUE,
  //                  muonisoRingsRad_weightfiles);
  // fMuonIsoRingsRadMVA->SetPrintMVADebug(false);

  // fMuonIDIsoCombinedMVA->initialize("MuonIso_BDTG_IsoRings",
  //                      MuonMVAEstimator::kIDIsoRingsCombined,
  //                      kTRUE,
  //                      muonidiso_weightfiles);
  // fMuonIDIsoCombinedMVA->SetPrintMVADebug(false);



  // if (eff_areas_ != "") {
  //   if (eff_areas_ == "kMuEAData2011") {
  //     eff_area_enum = MuonEffectiveArea::kMuEAData2011;
  //     std::cout << "Using effective areas: " << eff_areas_ << std::endl;
  //   } else if (eff_areas_ == "kMuEASummer11MC") {
  //     eff_area_enum = MuonEffectiveArea::kMuEASummer11MC;
  //     std::cout << "Using effective areas: " << eff_areas_ << std::endl;
  //   } else if (eff_areas_ == "kMuEAFall11MC") {
  //     eff_area_enum = MuonEffectiveArea::kMuEAFall11MC;
  //     std::cout << "Using effective areas: " << eff_areas_ << std::endl;
  //   } else if (eff_areas_ == "kMuEAData2012") {
  //     eff_area_enum = MuonEffectiveArea::kMuEAData2012;
  //     std::cout << "Using effective areas: " << eff_areas_ << std::endl;
  //   } else {
  //     std::cout << "Effective area payload not recognised!" << std::endl;
  //   }
  // } else {
  //   std::cout << "No effective area payload specified!" << eff_area_enum << std::endl;
  // }

  ic::StaticTree::tree_->Branch(branch_name_.c_str(), &muons_);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICMuonProducer::endJob() {
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Info in <ICMuonProducer>: EndJob Summary for input: " << input_label_.label() << std::endl;
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
void ICMuonProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICMuonProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICMuonProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICMuonProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICMuonProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

bool ICMuonProducer::electronIDForMVA(reco::GsfElectron const& elec, reco::Vertex const& vtx) {

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

bool ICMuonProducer::muonIDForMVA(reco::Muon const& muon) {
  if(!(muon.innerTrack().isNonnull())) {
    return false;
  } 
  if(!(muon.isGlobalMuon() || muon.isTrackerMuon())) return false;
  if(muon.innerTrack()->numberOfValidHits() < 11 ) return false;
  return true;
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICMuonProducer);
