#include "UserCode/ICHiggsTauTau/plugins/ICPFTauProducer.hh"
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

#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

#include "UserCode/ICHiggsTauTau/interface/PATConstruction.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"

#include "boost/format.hpp"


ICPFTauProducer::ICPFTauProducer(const edm::ParameterSet& iConfig) {
  //produces<std::vector<unsigned> >("selectGenParticles");
  produces<std::vector<unsigned> >("selectTracks");
  input_ = iConfig.getParameter<edm::InputTag>("input");
  vertex_input_ = iConfig.getParameter<edm::InputTag>("vertexCollection");
  track_input_ = iConfig.getParameter<edm::InputTag>("trackCollection");
  branch_name_ = iConfig.getParameter<std::string>("branchName");
  store_ids_ = iConfig.getParameter<bool>("storeAndRequestTrackIds");
  edm::ParameterSet id_params = iConfig.getParameter<edm::ParameterSet>("tauIDs");
  std::vector<std::string> id_names = id_params.getParameterNamesForType<edm::InputTag>();
  for (std::vector<std::string>::const_iterator it = id_names.begin(); it != id_names.end(); ++it) {
   tau_ids_.push_back(std::make_pair(*it, id_params.getParameter<edm::InputTag>(*it)));
  }
  if (iConfig.exists("tauIDCuts")) {
    edm::ParameterSet idcut_params = iConfig.getParameter<edm::ParameterSet>("tauIDCuts");
    std::vector<std::string> idcut_names = idcut_params.getParameterNamesForType<double>();
    for (std::vector<std::string>::const_iterator it = idcut_names.begin(); it != idcut_names.end(); ++it) {
     idcuts_[*it] = idcut_params.getParameter<double>(*it);
      std::cout << "Info in <ICPFTauProducer>: Require " << *it << " > " << idcuts_[*it] << std::endl;
    }    
  }
  min_pt_ = iConfig.getParameter<double>("minPt");
  max_eta_ = iConfig.getParameter<double>("maxEta");
  require_decay_mode_ = true;
  if (iConfig.exists("requireDecayMode")) require_decay_mode_ = iConfig.getParameter<bool>("requireDecayMode");
  std::cout << "Info in <ICPFTauProducer>: Require tau decay mode: " << require_decay_mode_ << std::endl;
  taus_ = new std::vector<ic::Tau>();
}


ICPFTauProducer::~ICPFTauProducer() {
 //delete calojets_;
}

// ------------ method called to produce the data  ------------
void ICPFTauProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  boost::hash<reco::PFTau const*> tau_hasher;

  // Get input PAT collection
  edm::Handle<std::vector<reco::PFTau> > tauCollection;
  iEvent.getByLabel(input_,tauCollection);
  std::vector<reco::PFTau>::const_iterator iter;

  // Get other inputs
  edm::Handle<reco::TrackCollection> trackCollection;
  iEvent.getByLabel(track_input_,trackCollection);
  edm::Handle<reco::VertexCollection> vertexCollection;
  iEvent.getByLabel(vertex_input_,vertexCollection);
  
  // Prepare output collection
  taus_->resize(0);
  taus_->reserve(tauCollection->size());
  
  // Prepare ouput requests
  std::auto_ptr<std::vector<unsigned> > jet_tracks(new std::vector<unsigned>());

  unsigned idx = 0;
  for (iter = tauCollection->begin(); iter != tauCollection->end(); ++iter, ++idx) {
    
    if (iter->decayMode() < 0.5 && require_decay_mode_) continue;
    if (! (iter->pt() > min_pt_ && fabs(iter->eta()) < max_eta_) ) continue;

    bool failed_id = false;
    for (auto idit = idcuts_.begin(); idit != idcuts_.end(); ++idit) {
      edm::Handle<reco::PFTauDiscriminator> discr;
      iEvent.getByLabel(idit->first, discr);
      reco::PFTauRef tau_ref(tauCollection, idx);
      if ((*discr)[tau_ref] <= idit->second ) {
        failed_id = true;
        break;
      }
    }
    if (failed_id) continue;


    taus_->push_back(ic::Tau());
    ic::Tau & tau = taus_->back();

    //Set candidate data
    tau.set_id(tau_hasher(&(*iter)));
    tau.set_pt(iter->pt());
    tau.set_eta(iter->eta());
    tau.set_phi(iter->phi());
    tau.set_energy(iter->energy());
    tau.set_charge(iter->charge());

    for (unsigned i = 0; i < tau_ids_.size(); ++i) {
      edm::Handle<reco::PFTauDiscriminator> discr;
      iEvent.getByLabel(tau_ids_[i].second, discr);
      reco::PFTauRef tau_ref(tauCollection, idx);
      tau.SetTauID(tau_ids_[i].first, (*discr)[tau_ref]);
      observed_id_[tau_ids_[i].first] = CityHash64(tau_ids_[i].first);
    }
    tau.set_decay_mode(iter->decayMode());
    if (iter->leadPFChargedHadrCand().isNonnull()) {
      tau.set_lead_ecal_energy(iter->leadPFChargedHadrCand()->ecalEnergy());
      tau.set_lead_hcal_energy(iter->leadPFChargedHadrCand()->hcalEnergy());
      tau.set_lead_p(iter->leadPFChargedHadrCand()->p());      
      if(iter->leadPFChargedHadrCand()->trackRef().isNonnull() && vertexCollection->size() > 0) {
        tau.set_lead_dxy_vertex(iter->leadPFChargedHadrCand()->trackRef()->dxy(vertexCollection->at(0).position()));
        tau.set_lead_dz_vertex(iter->leadPFChargedHadrCand()->trackRef()->dz(vertexCollection->at(0).position()));
      } else {
        tau.set_lead_dxy_vertex(9999.0);
        tau.set_lead_dz_vertex(9999.0);
      }
    } else {
      tau.set_lead_ecal_energy(-9999.0);
      tau.set_lead_hcal_energy(-9999.0);
      tau.set_lead_p(-9999.0);    
    }
    

    tau.set_vx(iter->vx());
    tau.set_vy(iter->vy());
    tau.set_vz(iter->vz());

    if (trackCollection->size() > 0) {
      reco::Track const* ptr_first = &(trackCollection->at(0));
      static boost::hash<reco::Track const*> track_hasher;
      if (iter->signalPFChargedHadrCands().isNonnull()) {
        reco::PFCandidateRefVector const& charged_pf = iter->signalPFChargedHadrCands();
        std::vector<std::size_t> trk_ids;
        for (unsigned i = 0; i <charged_pf.size(); ++i) {
          if (charged_pf[i]->trackRef().isNonnull()) {
            unsigned idx = unsigned(&(*(charged_pf[i]->trackRef())) - ptr_first);
            jet_tracks->push_back(idx);
            trk_ids.push_back(track_hasher(&(*(charged_pf[i]->trackRef()))));
          }
        }
        if (store_ids_) tau.set_constituent_tracks(trk_ids);        
      }
    }
  }
  iEvent.put(jet_tracks, "selectTracks");
}

// ------------ method called once each job just before starting event loop  ------------
void ICPFTauProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_name_.c_str(), &taus_);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICPFTauProducer::endJob() {
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Info in <ICPFTauProducer>: EndJob Summary" << std::endl;
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::map<std::string, std::size_t>::const_iterator iter;
  std::cout << "Tau ID Hash Summary:" << std::endl;
  for (iter = observed_id_.begin(); iter != observed_id_.end(); ++iter) {
    std::cout << boost::format("%-30s %-30s\n") % iter->first % iter->second;
  }
}

// ------------ method called when starting to processes a run  ------------
void ICPFTauProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICPFTauProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICPFTauProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICPFTauProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICPFTauProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICPFTauProducer);
