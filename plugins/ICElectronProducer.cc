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

#include "boost/format.hpp"


ICElectronProducer::ICElectronProducer(const edm::ParameterSet& iConfig) {
  produces<std::vector<unsigned> >("selectGenParticles");
  input_ = iConfig.getParameter<edm::InputTag>("input");
  branch_name_ = iConfig.getParameter<std::string>("branchName");
  pfiso_postfix_ = iConfig.getParameter<std::string>("pfIsoPostfix");
  vertex_input_ = iConfig.getParameter<edm::InputTag>("vertexCollection");
  is_pf_ = iConfig.getParameter<bool>("isPF");
  min_pt_ = iConfig.getParameter<double>("minPt");
  max_eta_ = iConfig.getParameter<double>("maxEta");
  electrons_ = new std::vector<ic::Electron>();
  if (is_pf_) {
    std::cout << "Warning, PF Electrons not currently supported by ICElectronProducer" << std::endl;
  }
}

    
ICElectronProducer::~ICElectronProducer() {
  delete electrons_;
}

// ------------ method called to produce the data  ------------
void ICElectronProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  // boost::hash<reco::GenParticle const*> particle_hasher;

  if (is_pf_) return;

  boost::hash<reco::GsfElectron const*> gsf_electron_hasher;

  // Get input PAT collection
  edm::Handle<std::vector<reco::GsfElectron> > eleCollection;

  iEvent.getByLabel(input_, eleCollection);
  std::vector<reco::GsfElectron>::const_iterator iter;

  // Get other inputs
  edm::Handle<reco::BeamSpot> beamspot;
  iEvent.getByLabel(edm::InputTag("offlineBeamSpot"), beamspot);
  edm::Handle<reco::ConversionCollection> hConversions;
  iEvent.getByLabel("allConversions", hConversions);
  edm::Handle<reco::GenParticleCollection> partCollection;
  iEvent.getByLabel("genParticles",partCollection);
  edm::Handle<CaloTowerCollection> towerCollection;
  iEvent.getByLabel("towerMaker",towerCollection);
  edm::Handle<reco::VertexCollection> vertexCollection;
  iEvent.getByLabel(vertex_input_, vertexCollection);

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

  edm::Handle<edm::ValueMap<float> > mvaTrigV0;
  edm::Handle<edm::ValueMap<float> > mvaNonTrigV0;

  iEvent.getByLabel(edm::InputTag("elPFIsoValueChargedAll03PFId"+pfiso_postfix_),charged_all_iso_03);
  iEvent.getByLabel(edm::InputTag("elPFIsoValueCharged03PFId"+pfiso_postfix_),charged_iso_03);
  iEvent.getByLabel(edm::InputTag("elPFIsoValueNeutral03PFId"+pfiso_postfix_),neutral_iso_03);
  iEvent.getByLabel(edm::InputTag("elPFIsoValueGamma03PFId"+pfiso_postfix_),gamma_iso_03);
  iEvent.getByLabel(edm::InputTag("elPFIsoValuePU03PFId"+pfiso_postfix_),pu_iso_03);

  iEvent.getByLabel(edm::InputTag("elPFIsoValueChargedAll04PFId"+pfiso_postfix_),charged_all_iso_04);
  iEvent.getByLabel(edm::InputTag("elPFIsoValueCharged04PFId"+pfiso_postfix_),charged_iso_04);
  iEvent.getByLabel(edm::InputTag("elPFIsoValueNeutral04PFId"+pfiso_postfix_),neutral_iso_04);
  iEvent.getByLabel(edm::InputTag("elPFIsoValueGamma04PFId"+pfiso_postfix_),gamma_iso_04);
  iEvent.getByLabel(edm::InputTag("elPFIsoValuePU04PFId"+pfiso_postfix_),pu_iso_04);

  iEvent.getByLabel(edm::InputTag("mvaTrigV0"),mvaTrigV0);
  iEvent.getByLabel(edm::InputTag("mvaNonTrigV0"),mvaNonTrigV0);

  // Add a flag to the idiso set if we find a reco muon with pT > 3
  // with deltaR < 0.3 of this electron
  edm::Handle<std::vector<reco::Muon> > recomuonCollection;
  iEvent.getByLabel("muons",recomuonCollection);

  // Try and get the collection of pfChargedAll particles so we can
  // figure out if the pf electron ends up in the isolation cone
  edm::Handle<std::vector<reco::PFCandidate> > pfChargedAll;
  iEvent.getByLabel("pfAllChargedParticles",pfChargedAll);


  EcalClusterLazyTools lazyTool( iEvent, iSetup, edm::InputTag("reducedEcalRecHitsEB"),edm::InputTag("reducedEcalRecHitsEE"));
  EgammaTowerIsolation hadDepth1Isolation03(0.3,0.0,0.0,1,towerCollection.product());
  EgammaTowerIsolation hadDepth2Isolation03(0.3,0.0,0.0,2,towerCollection.product());

  // Prepare output collection
  electrons_->resize(0);
  electrons_->reserve(eleCollection->size());

  // Prepare ouput requests
  std::auto_ptr<std::vector<unsigned> > ele_particles(new std::vector<unsigned>());
  
  unsigned idx = 0;
  for (iter = eleCollection->begin(); iter != eleCollection->end(); ++iter, ++idx) {

    if (! (iter->pt() > min_pt_ && fabs(iter->eta()) < max_eta_) ) continue;

    electrons_->push_back(ic::Electron());
    ic::Electron & ele = electrons_->back();

    ele.set_id(gsf_electron_hasher(&(*iter)));
    //std::cout << "Electron " << unsigned(iter - eleCollection->begin()) << ": " << pat_electron_hasher(&(*iter)) << std::endl;
    ele.set_pt(iter->pt());
    ele.set_eta(iter->eta());
    ele.set_phi(iter->phi());
    ele.set_energy(iter->energy());
    ele.set_charge(iter->charge());

    ele.set_dr03_tk_sum_pt(iter->dr03TkSumPt());
    ele.set_dr03_ecal_rechit_sum_et(iter->dr03EcalRecHitSumEt());
    ele.set_dr03_hcal_tower_sum_et(iter->dr03HcalTowerSumEt());
    
    reco::GsfElectronRef elec_ref(eleCollection, idx);
    float dr03_pfiso_charged_all = charged_all_iso_03.isValid() ? (*charged_all_iso_03)[elec_ref] : -1.0;
    float dr03_pfiso_charged  = charged_iso_03.isValid() ? (*charged_iso_03)[elec_ref] : -1.0;
    float dr03_pfiso_neutral = neutral_iso_03.isValid() ? (*neutral_iso_03)[elec_ref] : -1.0;
    float dr03_pfiso_gamma = gamma_iso_03.isValid() ? (*gamma_iso_03)[elec_ref] : -1.0;
    float dr03_pfiso_pu = pu_iso_03.isValid() ? (*pu_iso_03)[elec_ref] : -1.0;

    float dr04_pfiso_charged_all = charged_all_iso_04.isValid() ? (*charged_all_iso_04)[elec_ref] : -1.0;
    float dr04_pfiso_charged  = charged_iso_04.isValid() ? (*charged_iso_04)[elec_ref] : -1.0;
    float dr04_pfiso_neutral = neutral_iso_04.isValid() ? (*neutral_iso_04)[elec_ref] : -1.0;
    float dr04_pfiso_gamma = gamma_iso_04.isValid() ? (*gamma_iso_04)[elec_ref] : -1.0;
    float dr04_pfiso_pu = pu_iso_04.isValid() ? (*pu_iso_04)[elec_ref] : -1.0;

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

    if (pfChargedAll.isValid()) {
      // std::cout << "Electron has track id: " << iter->closestCtfTrackRef().index() << "\t" << iter->closestCtfTrackRef()->pt() << std::endl;
      for (unsigned pf = 0; pf < pfChargedAll->size(); ++pf) {
        reco::PFCandidate const& pfcand = pfChargedAll->at(pf);
        bool barrel = fabs(pfcand.positionAtECALEntrance().eta())<1.479;
        double dr_veto = barrel ? 0.01 : 0.015;
        if (reco::deltaR(*iter, pfcand) < dr_veto || reco::deltaR(*iter, pfcand) > 0.4) continue;
        if (pfcand.trackRef().isNonnull()) { // PF candidate has a track
          // std::cout << "Found pf cand with track id: " << pfcand.trackRef().index() << "\t" << pfcand.trackRef()->pt() << "\t" << pfcand.pt() << std::endl;
          if (pfcand.trackRef().index() == iter->closestCtfTrackRef().index()) {
            // std::cout << "Warning! Found electron track in isolation sum!" << std::endl;
            ele.SetIdIso("trackInIsoSum", pfcand.pt());
            observed_idiso_["trackInIsoSum"] = CityHash64("trackInIsoSum");
          }
        }
      }
    }

    if (mvaTrigV0.isValid()) {
      ele.SetIdIso("mvaTrigV0", (*mvaTrigV0)[elec_ref]);
      observed_idiso_["mvaTrigV0"] = CityHash64("mvaTrigV0");

    }
    if (mvaNonTrigV0.isValid()) {
      ele.SetIdIso("mvaNonTrigV0", (*mvaNonTrigV0)[elec_ref]);
      observed_idiso_["mvaNonTrigV0"] = CityHash64("mvaNonTrigV0");
    }

    for (unsigned mu = 0; mu < recomuonCollection->size(); ++mu) {
      if (recomuonCollection->at(mu).pt() > 3. && fabs(recomuonCollection->at(mu).eta())) {
        if (reco::deltaR(recomuonCollection->at(mu), *iter) < 0.3) {
          ele.SetIdIso("matchedRecoMuon", 1.0);
          observed_idiso_["matchedRecoMuon"] = CityHash64("matchedRecoMuon");
          break;
        }
      }
    }

    ele.set_hadronic_over_em(iter->hadronicOverEm());
    ele.set_sigma_IetaIeta(iter->sigmaIetaIeta());
    ele.set_dphi_sc_tk_at_vtx(iter->deltaPhiSuperClusterTrackAtVtx());
    ele.set_deta_sc_tk_at_vtx(iter->deltaEtaSuperClusterTrackAtVtx());
    ele.set_gsf_tk_nhits(iter->gsfTrack()->trackerExpectedHitsInner().numberOfHits());
    ele.set_conv_dist(iter->convDist());
    ele.set_conv_dcot(iter->convDcot());
    bool has_match = ConversionTools::hasMatchedConversion((*iter), hConversions, beamspot->position(), true, 2.0, 1e-6, 0);
    ele.set_has_matched_conversion(has_match);
 
    ele.set_f_brem(iter->fbrem());
    ele.set_sc_eta(iter->superCluster()->eta());
    ele.set_sc_theta(iter->superCluster()->position().theta());
    ele.set_sc_energy(iter->superCluster()->energy());
    ele.set_sc_e_over_p(iter->eSuperClusterOverP());

    float e3x3 = lazyTool.e3x3(*(iter->superCluster()));
    float sc_raw_energy = iter->superCluster()->rawEnergy();
    ele.set_r9(e3x3 / sc_raw_energy);

    float hcalDepth1TowerSumEt03 = hadDepth1Isolation03.getTowerEtSum(&(*iter));
    float hcalDepth2TowerSumEt03 = hadDepth2Isolation03.getTowerEtSum(&(*iter));
    ele.set_hcal_sum(hcalDepth1TowerSumEt03+hcalDepth2TowerSumEt03);
    ele.set_ecal_energy(iter->ecalEnergy());

    ele.set_vx(iter->vx());
    ele.set_vy(iter->vy());
    ele.set_vz(iter->vz());


    if (vertexCollection->size() > 0) {
      ele.set_dz_vertex(iter->gsfTrack()->dz(vertexCollection->at(0).position()));
      ele.set_dxy_vertex(iter->gsfTrack()->dxy(vertexCollection->at(0).position()));
    } else {
      ele.set_dz_vertex(9999.);
      ele.set_dxy_vertex(9999.);
    }
    ele.set_dxy_beamspot(iter->gsfTrack()->dxy(*beamspot));


    std::vector<std::size_t> gen_index;
    ele.set_gen_particles(gen_index); 
  }
  
  iEvent.put(ele_particles, "selectGenParticles");
}

// ------------ method called once each job just before starting event loop  ------------
void ICElectronProducer::beginJob() {
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Info in <ICElectronProducer>: BeginJob Summary for input: " << input_.label() << std::endl;
  std::cout << "-----------------------------------------------------------------" << std::endl;

  ic::StaticTree::tree_->Branch(branch_name_.c_str(), &electrons_);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICElectronProducer::endJob() {
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Info in <ICElectronProducer>: EndJob Summary for input: " << input_.label() << std::endl;
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "HLT Match Paths Hash Summary:" << std::endl;
  std::map<std::string, std::size_t>::const_iterator iter;
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

// bool ICElectronProducer::electronIDForMVA(reco::GsfElectron const& elec, reco::Vertex const& vtx) {

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

// bool ICElectronProducer::muonIDForMVA(reco::Muon const& muon) {
//   if(!(muon.innerTrack().isNonnull())) {
//     return false;
//   } 
//   if(!(muon.isGlobalMuon() || muon.isTrackerMuon())) return false;
//   if(muon.innerTrack()->numberOfValidHits() < 11 ) return false;
//   return true;
// }

//define this as a plug-in
DEFINE_FWK_MODULE(ICElectronProducer);
