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
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "DataFormats/VertexReco/interface/VertexFwd.h"


#include "boost/format.hpp"


ICMuonProducer::ICMuonProducer(const edm::ParameterSet& iConfig) {
  produces<std::vector<unsigned> >("selectGenParticles");
  input_ = iConfig.getParameter<edm::InputTag>("input");
  branch_name_ = iConfig.getParameter<std::string>("branchName");
  pfiso_postfix_ = iConfig.getParameter<std::string>("pfIsoPostfix");
  vertex_input_ = iConfig.getParameter<edm::InputTag>("vertexCollection");
  is_pf_ = iConfig.getParameter<bool>("isPF");
  muons_ = new std::vector<ic::Muon>();
  min_pt_ = iConfig.getParameter<double>("minPt");
  max_eta_ = iConfig.getParameter<double>("maxEta");
}


ICMuonProducer::~ICMuonProducer() {
  delete muons_;
}

// ------------ method called to produce the data  ------------
void ICMuonProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  // boost::hash<reco::GenParticle const*> particle_hasher;
  boost::hash<reco::Muon const*> recomuon_hasher;
  boost::hash<reco::PFCandidate const*> pfmuon_hasher;

  // Get input PAT collection
  edm::Handle<std::vector<reco::Muon> > recomuonCollection;

  edm::Handle<std::vector<reco::PFCandidate> > pfmuonCollection;

  unsigned nmuons = 0;

  if (is_pf_) {
    iEvent.getByLabel(input_,pfmuonCollection);
    nmuons = pfmuonCollection->size();
  } else {    
    iEvent.getByLabel(input_,recomuonCollection);
    nmuons = recomuonCollection->size();
  }

  // Get other inputs
  edm::Handle<reco::BeamSpot> beamspot;
  iEvent.getByLabel(edm::InputTag("offlineBeamSpot"), beamspot);
  edm::Handle<reco::GenParticleCollection> partCollection;
  iEvent.getByLabel("genParticles",partCollection);
  edm::Handle<reco::VertexCollection> vertexCollection;
  iEvent.getByLabel(vertex_input_,vertexCollection);

  edm::Handle<edm::ValueMap<double> > charged_all_iso_03;
  edm::Handle<edm::ValueMap<double> > charged_iso_03;
  edm::Handle<edm::ValueMap<double> > neutral_iso_03;
  edm::Handle<edm::ValueMap<double> > gamma_iso_03;
  edm::Handle<edm::ValueMap<double> > pu_iso_03;

  edm::Handle<edm::ValueMap<double> > charged_all_iso_04;
  edm::Handle<edm::ValueMap<double> > charged_iso_04;
  edm::Handle<edm::ValueMap<double> > neutral_iso_04;
  edm::Handle<edm::ValueMap<double> > gamma_iso_04;
  edm::Handle<edm::ValueMap<double> > pu_iso_04;

  iEvent.getByLabel(edm::InputTag("muPFIsoValueChargedAll03"+pfiso_postfix_),charged_all_iso_03);
  iEvent.getByLabel(edm::InputTag("muPFIsoValueCharged03"+pfiso_postfix_),charged_iso_03);
  iEvent.getByLabel(edm::InputTag("muPFIsoValueNeutral03"+pfiso_postfix_),neutral_iso_03);
  iEvent.getByLabel(edm::InputTag("muPFIsoValueGamma03"+pfiso_postfix_),gamma_iso_03);
  iEvent.getByLabel(edm::InputTag("muPFIsoValuePU03"+pfiso_postfix_),pu_iso_03);

  iEvent.getByLabel(edm::InputTag("muPFIsoValueChargedAll04"+pfiso_postfix_),charged_all_iso_04);
  iEvent.getByLabel(edm::InputTag("muPFIsoValueCharged04"+pfiso_postfix_),charged_iso_04);
  iEvent.getByLabel(edm::InputTag("muPFIsoValueNeutral04"+pfiso_postfix_),neutral_iso_04);
  iEvent.getByLabel(edm::InputTag("muPFIsoValueGamma04"+pfiso_postfix_),gamma_iso_04);
  iEvent.getByLabel(edm::InputTag("muPFIsoValuePU04"+pfiso_postfix_),pu_iso_04);

  // Prepare output collection
  muons_->resize(0);
  muons_->reserve(nmuons);

  // Prepare ouput requests
  std::auto_ptr<std::vector<unsigned> > muo_particles(new std::vector<unsigned>());

  for (unsigned idx = 0; idx < nmuons; ++idx) {
    if (is_pf_) {
      if (! (pfmuonCollection->at(idx).pt() > min_pt_ && fabs(pfmuonCollection->at(idx).eta()) < max_eta_) ) continue;
    } else {
      if (! (recomuonCollection->at(idx).pt() > min_pt_ && fabs(recomuonCollection->at(idx).eta()) < max_eta_) ) continue;
    }
    muons_->push_back(ic::Muon());
    ic::Muon & muo = muons_->back();

    reco::MuonRef reco_muon;
    reco::PFCandidateRef pf_muon_ref;

    if (is_pf_) {
      muo.set_id(pfmuon_hasher(&(pfmuonCollection->at(idx))));
      muo.set_pt(pfmuonCollection->at(idx).pt());
      muo.set_eta(pfmuonCollection->at(idx).eta());
      muo.set_phi(pfmuonCollection->at(idx).phi());
      muo.set_energy(pfmuonCollection->at(idx).energy());
      muo.set_charge(pfmuonCollection->at(idx).charge());
      reco_muon = pfmuonCollection->at(idx).muonRef();
      pf_muon_ref = reco::PFCandidateRef(pfmuonCollection, idx);

    } else {
      muo.set_id(recomuon_hasher(&(recomuonCollection->at(idx))));
      muo.set_pt(recomuonCollection->at(idx).pt());
      muo.set_eta(recomuonCollection->at(idx).eta());
      muo.set_phi(recomuonCollection->at(idx).phi());
      muo.set_energy(recomuonCollection->at(idx).energy());
      muo.set_charge(recomuonCollection->at(idx).charge());
      reco_muon = reco::MuonRef(recomuonCollection, idx);
    }

    muo.set_dr03_tk_sum_pt(reco_muon->isolationR03().sumPt);
    muo.set_dr03_ecal_rechit_sum_et(reco_muon->isolationR03().emEt);
    muo.set_dr03_hcal_tower_sum_et(reco_muon->isolationR03().hadEt);


    float dr03_pfiso_charged_all  = -1.0;
    float dr03_pfiso_charged      = -1.0;
    float dr03_pfiso_neutral      = -1.0;
    float dr03_pfiso_gamma        = -1.0;
    float dr03_pfiso_pu           = -1.0;

    float dr04_pfiso_charged_all  = -1.0;
    float dr04_pfiso_charged      = -1.0;
    float dr04_pfiso_neutral      = -1.0;
    float dr04_pfiso_gamma        = -1.0;
    float dr04_pfiso_pu           = -1.0;

    if (is_pf_) {
      dr03_pfiso_charged_all  = (*charged_all_iso_03)[pf_muon_ref];
      dr03_pfiso_charged      = (*charged_iso_03)[pf_muon_ref];
      dr03_pfiso_neutral      = (*neutral_iso_03)[pf_muon_ref];
      dr03_pfiso_gamma        = (*gamma_iso_03)[pf_muon_ref];
      dr03_pfiso_pu           = (*pu_iso_03)[pf_muon_ref];
      dr04_pfiso_charged_all  = (*charged_all_iso_04)[pf_muon_ref];
      dr04_pfiso_charged      = (*charged_iso_04)[pf_muon_ref];
      dr04_pfiso_neutral      = (*neutral_iso_04)[pf_muon_ref];
      dr04_pfiso_gamma        = (*gamma_iso_04)[pf_muon_ref];
      dr04_pfiso_pu           = (*pu_iso_04)[pf_muon_ref];
    } else {
      dr03_pfiso_charged_all  = (*charged_all_iso_03)[reco_muon];
      dr03_pfiso_charged      = (*charged_iso_03)[reco_muon];
      dr03_pfiso_neutral      = (*neutral_iso_03)[reco_muon];
      dr03_pfiso_gamma        = (*gamma_iso_03)[reco_muon];
      dr03_pfiso_pu           = (*pu_iso_03)[reco_muon];
      dr04_pfiso_charged_all  = (*charged_all_iso_04)[reco_muon];
      dr04_pfiso_charged      = (*charged_iso_04)[reco_muon];
      dr04_pfiso_neutral      = (*neutral_iso_04)[reco_muon];
      dr04_pfiso_gamma        = (*gamma_iso_04)[reco_muon];
      dr04_pfiso_pu           = (*pu_iso_04)[reco_muon];
    }

    muo.set_dr03_pfiso_charged_all(dr03_pfiso_charged_all);
    muo.set_dr03_pfiso_charged(dr03_pfiso_charged);
    muo.set_dr03_pfiso_neutral(dr03_pfiso_neutral);
    muo.set_dr03_pfiso_gamma(dr03_pfiso_gamma);
    muo.set_dr03_pfiso_pu(dr03_pfiso_pu);
    muo.set_dr04_pfiso_charged_all(dr04_pfiso_charged_all);
    muo.set_dr04_pfiso_charged(dr04_pfiso_charged);
    muo.set_dr04_pfiso_neutral(dr04_pfiso_neutral);
    muo.set_dr04_pfiso_gamma(dr04_pfiso_gamma);
    muo.set_dr04_pfiso_pu(dr04_pfiso_pu);

    muo.set_is_standalone(reco_muon->isStandAloneMuon());
    muo.set_is_global(reco_muon->isGlobalMuon());
    muo.set_is_tracker(reco_muon->isTrackerMuon());
    muo.set_is_calo(reco_muon->isCaloMuon());
    if (reco_muon->isGlobalMuon()) {
        muo.set_gt_normalized_chi2(reco_muon->globalTrack()->normalizedChi2());
        muo.set_gt_valid_muon_hits(reco_muon->globalTrack()->hitPattern().numberOfValidMuonHits());
    } else {
     muo.set_gt_normalized_chi2(0);
     muo.set_gt_valid_muon_hits(0); 
    }
    muo.set_matched_stations(reco_muon->numberOfMatchedStations());
    if (reco_muon->isTrackerMuon()) {
        muo.set_it_pixel_hits(reco_muon->innerTrack()->hitPattern().numberOfValidPixelHits());
        muo.set_it_tracker_hits(reco_muon->innerTrack()->hitPattern().numberOfValidTrackerHits());
        muo.set_it_layers_with_measurement(reco_muon->innerTrack()->hitPattern().trackerLayersWithMeasurement());
    } else {
      muo.set_it_pixel_hits(0);
      muo.set_it_tracker_hits(0);
      muo.set_it_layers_with_measurement(0);
    }

    muo.set_vx(reco_muon->vx());
    muo.set_vy(reco_muon->vy());
    muo.set_vz(reco_muon->vz());

    muo.set_dxy_beamspot(reco_muon->innerTrack()->dxy(*beamspot));
    if (vertexCollection->size() > 0 && reco_muon->isGlobalMuon()) {
      muo.set_dz_vertex(reco_muon->innerTrack()->dz(vertexCollection->at(0).position()));
      muo.set_dxy_vertex(reco_muon->innerTrack()->dxy(vertexCollection->at(0).position()));
    } else {
      muo.set_dz_vertex(9999.);
      muo.set_dxy_vertex(9999.);
    }

    std::vector<std::size_t> gen_index;
    muo.set_gen_particles(gen_index); 

  }


  iEvent.put(muo_particles, "selectGenParticles");
}

// ------------ method called once each job just before starting event loop  ------------
void ICMuonProducer::beginJob() {
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Info in <ICMuonProducer>: BeginJob Summary for input: " << input_.label() << std::endl;
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
  std::cout << "Info in <ICMuonProducer>: EndJob Summary for input: " << input_.label() << std::endl;
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "HLT Match Paths Hash Summary:" << std::endl;
  std::map<std::string, std::size_t>::const_iterator iter;
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

// bool ICMuonProducer::electronIDForMVA(reco::GsfElectron const& elec, reco::Vertex const& vtx) {

//     double electronTrackZ = 0;
//     if (elec.gsfTrack().isNonnull()) {
//       electronTrackZ = elec.gsfTrack()->dz(vtx.position());
//     } else if (elec.closestCtfTrackRef().isNonnull()) {
//       electronTrackZ = elec.closestCtfTrackRef()->dz(vtx.position());
//     }    
//     if(fabs(electronTrackZ) > 0.2)  return false;

//     if(fabs(elec.superCluster()->eta())<1.479) {     
//       if(elec.pt() > 20) {
//         if(elec.sigmaIetaIeta()       > 0.01)  return false;
//         if(fabs(elec.deltaEtaSuperClusterTrackAtVtx()) > 0.007) return false;
//         if(fabs(elec.deltaPhiSuperClusterTrackAtVtx()) > 0.8)  return false;
//         if(elec.hadronicOverEm()       > 0.15)  return false;    
//       } else {
//         if(elec.sigmaIetaIeta()       > 0.012)  return false;
//         if(fabs(elec.deltaEtaSuperClusterTrackAtVtx()) > 0.007) return false;
//         if(fabs(elec.deltaPhiSuperClusterTrackAtVtx()) > 0.8)  return false;
//         if(elec.hadronicOverEm()       > 0.15) return false;    
//       } 
//     } else {     
//       if(elec.pt() > 20) {
//         if(elec.sigmaIetaIeta()       > 0.03)  return false;
//         if(fabs(elec.deltaEtaSuperClusterTrackAtVtx()) > 0.010) return false;
//         if(fabs(elec.deltaPhiSuperClusterTrackAtVtx()) > 0.8)  return false;
//       } else {
//         if(elec.sigmaIetaIeta()       > 0.032)  return false;
//         if(fabs(elec.deltaEtaSuperClusterTrackAtVtx()) > 0.010) return false;
//         if(fabs(elec.deltaPhiSuperClusterTrackAtVtx()) > 0.8)  return false;
//       }
//     }
//     return true;
// }

// bool ICMuonProducer::muonIDForMVA(reco::Muon const& muon) {
//   if(!(muon.innerTrack().isNonnull())) {
//     return false;
//   } 
//   if(!(muon.isGlobalMuon() || muon.isTrackerMuon())) return false;
//   if(muon.innerTrack()->numberOfValidHits() < 11 ) return false;
//   return true;
// }

//define this as a plug-in
DEFINE_FWK_MODULE(ICMuonProducer);
