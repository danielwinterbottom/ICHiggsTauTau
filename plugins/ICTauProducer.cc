#include "UserCode/ICHiggsTauTau/plugins/ICTauProducer.hh"
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
#include "UserCode/ICHiggsTauTau/interface/Jet.hh"


#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

#include "UserCode/ICHiggsTauTau/interface/PATConstruction.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"

#include "boost/format.hpp"


ICTauProducer::ICTauProducer(const edm::ParameterSet& iConfig) {
  //produces<std::vector<unsigned> >("selectGenParticles");
  produces<std::vector<unsigned> >("selectTracks");
  input_label_ = iConfig.getParameter<edm::InputTag>("inputLabel");
  branch_name_ = iConfig.getUntrackedParameter<std::string>("branchName");
  store_ids_ = iConfig.getParameter<bool>("StoreTrackIds");
  taus_ = new std::vector<ic::Tau>();
}


ICTauProducer::~ICTauProducer() {
 //delete calojets_;
}

// ------------ method called to produce the data  ------------
void ICTauProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  boost::hash<pat::Tau const*> pat_tau_hasher;

  // Get input PAT collection
  edm::Handle<std::vector<pat::Tau> > tauCollection;
  iEvent.getByLabel(input_label_,tauCollection);
  std::vector<pat::Tau>::const_iterator iter;

  // Get other inputs
  edm::Handle<reco::TrackCollection> trackCollection;
  iEvent.getByLabel("generalTracks",trackCollection);
  edm::Handle<reco::VertexCollection> vertexCollection;
  iEvent.getByLabel("goodOfflinePrimaryVertices",vertexCollection);
  
  // Prepare output collection
  taus_->resize(0);
  taus_->reserve(tauCollection->size());
  
  // Prepare ouput requests
  std::auto_ptr<std::vector<unsigned> > jet_tracks(new std::vector<unsigned>());
  
  for (iter = tauCollection->begin(); iter != tauCollection->end(); ++iter) {
    
    if (iter->decayMode() < -0.5) continue;

    taus_->push_back(ic::Tau());
    ic::Tau & tau = taus_->back();
    //Set candidate data

    tau.set_id(pat_tau_hasher(&(*iter)));
    tau.set_pt(iter->pt());
    tau.set_eta(iter->eta());
    tau.set_phi(iter->phi());
    tau.set_energy(iter->energy());
    tau.set_charge(iter->charge());
    // std::cout << tau.vector() << std::endl;

    std::vector<std::pair<std::string, float> > tau_ids = iter->tauIDs();
    for (unsigned i = 0; i < tau_ids.size(); ++i) {
      // std::cout << i << " " << tau_ids[i].first << "\t" << tau_ids[i].second << std::endl;
      tau.SetTauID(tau_ids[i].first, tau_ids[i].second);
      observed_id_[tau_ids[i].first] = CityHash64(tau_ids[i].first);
    }
    tau.set_decay_mode(iter->decayMode());

    // tau.set_elec_preid_output(iter->electronPreIDOutput());
    // tau.set_elec_preid_decision(iter->electronPreIDDecision());

    if(iter->leadPFChargedHadrCand().isNonnull() ){
        tau.set_lead_ecal_energy(iter->leadPFChargedHadrCand()->ecalEnergy());
        tau.set_lead_hcal_energy(iter->leadPFChargedHadrCand()->hcalEnergy());
        tau.set_lead_p(iter->leadPFChargedHadrCand()->p());
        if(iter->leadPFChargedHadrCand()->trackRef().isNonnull() && vertexCollection->size() > 0) {
          tau.set_lead_dxy_vertex(iter->leadPFChargedHadrCand()->trackRef()->dxy(vertexCollection->at(0).position()));
          tau.set_lead_dz_vertex(iter->leadPFChargedHadrCand()->trackRef()->dz(vertexCollection->at(0).position()));
        } else if (iter->leadPFChargedHadrCand()->gsfTrackRef().isNonnull() && vertexCollection->size() > 0) {
          tau.set_lead_dxy_vertex(iter->leadPFChargedHadrCand()->gsfTrackRef()->dxy(vertexCollection->at(0).position()));
          tau.set_lead_dz_vertex(iter->leadPFChargedHadrCand()->gsfTrackRef()->dz(vertexCollection->at(0).position()));      
        } else {
          tau.set_lead_dxy_vertex(9999.0);
          tau.set_lead_dz_vertex(9999.0);
        }
    } else { 
        tau.set_lead_ecal_energy(0.0);
        tau.set_lead_hcal_energy(0.0);
        tau.set_lead_dxy_vertex(0.0);
        tau.set_lead_dz_vertex(0.0);
    }



    tau.set_vx(iter->vx());
    tau.set_vy(iter->vy());
    tau.set_vz(iter->vz());

    #ifdef CMSSW_5_3_13
    
    if (trackCollection->size() > 0) {
      reco::Track const* ptr_first = &(trackCollection->at(0));
      static boost::hash<reco::Track const*> track_hasher;

      std::vector<edm::Ptr<reco::PFCandidate> > const& charged_pf = iter->signalPFChargedHadrCands();
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
    
    #else
    if (trackCollection->size() > 0) {
      reco::Track const* ptr_first = &(trackCollection->at(0));
      static boost::hash<reco::Track const*> track_hasher;

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
    
    #endif

    // if (iter->triggerObjectMatches().size() > 0) {
    //   std::vector<std::size_t> hlt_paths;
    //   std::vector<std::size_t> hlt_filters;
    //   std::vector<std::string> const& hlt_path_strings = iter->triggerObjectMatches().at(0).pathNames(0,0);
    //   std::vector<std::string> const& hlt_filter_strings = iter->triggerObjectMatches().at(0).filterLabels();     
    //   hlt_paths.resize(hlt_path_strings.size());
    //   hlt_filters.resize(hlt_filter_strings.size());
    //   for (unsigned i = 0; i < hlt_paths.size(); ++i) {
    //     hlt_paths[i] = CityHash64(hlt_path_strings[i]);
    //     observed_paths_[hlt_path_strings[i]] = CityHash64(hlt_path_strings[i]);
    //   }
    //   for (unsigned i = 0; i < hlt_filters.size(); ++i) {
    //    hlt_filters[i] = CityHash64(hlt_filter_strings[i]);
    //    observed_filters_[hlt_filter_strings[i]] = CityHash64(hlt_filter_strings[i]);
    //   }
    //  tau.set_hlt_match_paths(hlt_paths);
    //  tau.set_hlt_match_filters(hlt_filters);
    // }

  }
  iEvent.put(jet_tracks, "selectTracks");
}

// ------------ method called once each job just before starting event loop  ------------
void ICTauProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_name_.c_str(), &taus_);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICTauProducer::endJob() {
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Info in <ICTauProducer>: EndJob Summary" << std::endl;
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::map<std::string, std::size_t>::const_iterator iter;
  std::cout << "Tau ID Hash Summary:" << std::endl;
  for (iter = observed_id_.begin(); iter != observed_id_.end(); ++iter) {
    std::cout << boost::format("%-30s %-30s\n") % iter->first % iter->second;
  }
  std::cout << "HLT Match Paths Hash Summary:" << std::endl;
  for (iter = observed_paths_.begin(); iter != observed_paths_.end(); ++iter) {
    std::cout << boost::format("%-60s %-30s\n") % iter->first % iter->second;
  }
  std::cout << "HLT Match Filters Hash Summary:" << std::endl;
  for (iter = observed_filters_.begin(); iter != observed_filters_.end(); ++iter) {
    std::cout << boost::format("%-60s %-20s\n") % iter->first % iter->second;
  }
}

// ------------ method called when starting to processes a run  ------------
void ICTauProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICTauProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICTauProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICTauProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICTauProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICTauProducer);
