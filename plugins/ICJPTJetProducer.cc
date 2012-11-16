#include "UserCode/ICHiggsTauTau/plugins/ICJPTJetProducer.hh"
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


#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"

#include "UserCode/ICHiggsTauTau/interface/PATConstruction.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"

#include "boost/format.hpp"


ICJPTJetProducer::ICJPTJetProducer(const edm::ParameterSet& iConfig) {
  produces<std::vector<unsigned> >("selectGenParticles");
  produces<std::vector<unsigned> >("selectTracks");
  input_label_ = iConfig.getParameter<edm::InputTag>("inputLabel");
  branch_name_ = iConfig.getUntrackedParameter<std::string>("branchName");
  store_ids_ = iConfig.getParameter<bool>("StoreTrackIds");
  jptjets_ = new std::vector<ic::JPTJet>();
  loose_id_ = new JetIDSelectionFunctor( JetIDSelectionFunctor::PURE09, JetIDSelectionFunctor::LOOSE);
  tight_id_ = new JetIDSelectionFunctor( JetIDSelectionFunctor::PURE09, JetIDSelectionFunctor::TIGHT);
}


ICJPTJetProducer::~ICJPTJetProducer() {
 delete jptjets_;
 delete loose_id_;
 delete tight_id_;
}

// ------------ method called to produce the data  ------------
void ICJPTJetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  boost::hash<reco::GenParticle const*> hasher;

  edm::Handle<std::vector<pat::Jet> > jetCollection;
  iEvent.getByLabel(input_label_,jetCollection);
  std::vector<pat::Jet>::const_iterator iter;
  //Set the Jet vector size to zero, then allocate enough space
  //for at least as many elements in the PAT::Jet vector
  jptjets_->resize(0);
  jptjets_->reserve(jetCollection->size());

  edm::Handle<reco::GenParticleCollection> partCollection;
  iEvent.getByLabel("genParticles",partCollection);
  edm::Handle<reco::TrackCollection> trackCollection;
  iEvent.getByLabel("generalTracks",trackCollection);
  edm::Handle<reco::VertexCollection> vertexCollection;
  iEvent.getByLabel("offlinePrimaryVertices",vertexCollection);

  // Build map to associate tracks->vertices
  std::vector<reco::TrackBaseRef>::const_iterator trk_it;
  std::map<unsigned, unsigned> trk_vtx_map;
  reco::Track const* ptr_to_first = 0;
  if (trackCollection->size() > 0) {
    ptr_to_first = &(trackCollection->at(0));
    for (unsigned i = 0; i < vertexCollection->size(); ++i) {
      for (trk_it = vertexCollection->at(i).tracks_begin(); trk_it != vertexCollection->at(i).tracks_end(); ++trk_it) {
        reco::Track const* ptr_to_trk = dynamic_cast<reco::Track const*>(&(**trk_it));
        unsigned trk_idx = (unsigned(ptr_to_trk - ptr_to_first));
        trk_vtx_map[trk_idx] =  i;
      }
    }
  }


  std::auto_ptr<std::vector<unsigned> > jet_particles(new std::vector<unsigned>());
  std::auto_ptr<std::vector<unsigned> > jet_tracks(new std::vector<unsigned>());

  // edm::Handle<reco::JPTJetCollection> jets;
  // iEvent.getByLabel("ak5JPTJetsL1L2L3", jets);
  // std::vector<reco::JPTJet>::const_iterator jiter;
  // std::cout << "Begin loop through JPT jets:" << std::endl;
  // for (jiter = jets->begin(); jiter != jets->end(); ++jiter) {
  //   if (jiter->pt() > 15.) std::cout << "-- pt: " << jiter->pt() << std::endl;
  // }

  // std::cout << "Begin loop through PAT jets:" << std::endl;
  for (iter = jetCollection->begin(); iter != jetCollection->end(); ++iter) {
    // std::cout << "-- pt: " << iter->pt() << std::endl;
    // std::cout << "-- uncorrected pt: " << ((iter->jecSetsAvailable() 
    //   ? iter->jecFactor(0) : 1.)*iter->energy()) << std::endl;
    jptjets_->push_back(ic::JPTJet());
    ic::JPTJet & jet = jptjets_->back();
    ic::ConstructJPTJet(&jet, &(*iter));
    //Set candidate data


    if (iter->genParton()) {
      unsigned idx = unsigned((reco::GenParticle*)iter->genParton() - &(partCollection->at(0)));
      jet_particles->push_back(idx);
      std::vector<std::size_t> gen_index;
      gen_index.push_back(hasher(iter->genParton()));
      jet.set_gen_particles(gen_index); 
    } 


    std::vector<reco::Track const*> all_tracks;
    std::vector<reco::Track const*> current_tracks;

    double beta = -1.0;
    double beta_max = -1.0;
    double trk_pt_total = 0.0;


    if (trackCollection->size() > 0) {
      if (store_ids_) jet.set_pions_in_vtx_in_calo(makeTrackIDVec(iter->pionsInVertexInCalo(), 
        *jet_tracks, &(trackCollection->at(0))));
      current_tracks = TrackPtrVec(iter->pionsInVertexInCalo());
      all_tracks.insert(all_tracks.end(),current_tracks.begin(),current_tracks.end());
      
      if (store_ids_) jet.set_pions_in_vtx_out_calo(makeTrackIDVec(iter->pionsInVertexOutCalo(), 
        *jet_tracks, &(trackCollection->at(0))));
      current_tracks = TrackPtrVec(iter->pionsInVertexOutCalo());
      all_tracks.insert(all_tracks.end(),current_tracks.begin(),current_tracks.end());

      
      if (store_ids_) jet.set_pions_out_vtx_in_calo(makeTrackIDVec(iter->pionsOutVertexInCalo(), 
        *jet_tracks, &(trackCollection->at(0))));
      current_tracks = TrackPtrVec(iter->pionsOutVertexInCalo());
      all_tracks.insert(all_tracks.end(),current_tracks.begin(),current_tracks.end());

      
      if (store_ids_) jet.set_muons_in_vtx_in_calo(makeTrackIDVec(iter->muonsInVertexInCalo(), 
        *jet_tracks, &(trackCollection->at(0))));
      current_tracks = TrackPtrVec(iter->muonsInVertexInCalo());
      all_tracks.insert(all_tracks.end(),current_tracks.begin(),current_tracks.end());

      
      if (store_ids_) jet.set_muons_in_vtx_out_calo(makeTrackIDVec(iter->muonsInVertexOutCalo(), 
        *jet_tracks, &(trackCollection->at(0))));
      current_tracks = TrackPtrVec(iter->muonsInVertexOutCalo());
      all_tracks.insert(all_tracks.end(),current_tracks.begin(),current_tracks.end());

      
      if (store_ids_) jet.set_muons_out_vtx_in_calo(makeTrackIDVec(iter->muonsOutVertexInCalo(), 
        *jet_tracks, &(trackCollection->at(0))));
      current_tracks = TrackPtrVec(iter->muonsOutVertexInCalo());
      all_tracks.insert(all_tracks.end(),current_tracks.begin(),current_tracks.end());

      
      if (store_ids_) jet.set_elecs_in_vtx_in_calo(makeTrackIDVec(iter->elecsInVertexInCalo(), 
        *jet_tracks, &(trackCollection->at(0))));
      current_tracks = TrackPtrVec(iter->elecsInVertexInCalo());
      all_tracks.insert(all_tracks.end(),current_tracks.begin(),current_tracks.end());

      
      if (store_ids_) jet.set_elecs_in_vtx_out_calo(makeTrackIDVec(iter->elecsInVertexOutCalo(), 
        *jet_tracks, &(trackCollection->at(0))));
      current_tracks = TrackPtrVec(iter->elecsInVertexOutCalo());
      all_tracks.insert(all_tracks.end(),current_tracks.begin(),current_tracks.end());

      
      if (store_ids_) jet.set_elecs_out_vtx_in_calo(makeTrackIDVec(iter->elecsOutVertexInCalo(), 
        *jet_tracks, &(trackCollection->at(0))));
      current_tracks = TrackPtrVec(iter->elecsOutVertexInCalo());
      all_tracks.insert(all_tracks.end(),current_tracks.begin(),current_tracks.end());


      std::vector<double> pt_at_vtx_vec(vertexCollection->size(), 0.0);


      if (trackCollection->size() > 0) {
        reco::Track const* ptr_first = &(trackCollection->at(0));
        for (unsigned i = 0; i < all_tracks.size(); ++i) {
            unsigned idx = unsigned(all_tracks[i] - ptr_first);
            trk_pt_total += all_tracks[i]->pt();
            // Is track associated to a vertex?
            if (trk_vtx_map.count(idx) > 0) {
              pt_at_vtx_vec[trk_vtx_map.find(idx)->second] += all_tracks[i]->pt();
            } else { // No, so is it within 0.2 cm wrt the closest vertex in z
              std::vector<double> dz_with_vtx(vertexCollection->size(), 0.0);
              for (unsigned j = 0; j < vertexCollection->size(); ++j) {
                dz_with_vtx[j] = fabs(vertexCollection->at(j).z() - all_tracks[i]->vz());
              }
              std::vector<double>::const_iterator min = std::min_element(dz_with_vtx.begin(),dz_with_vtx.end());
              if (min != dz_with_vtx.end()) {
                if (*min < 0.2) {
                  pt_at_vtx_vec[unsigned(min-dz_with_vtx.begin())] += all_tracks[i]->pt();
                }
              } 
            }
        }
        if (vertexCollection->size() > 0 && trk_pt_total > 0.0) {
          for (unsigned i = 0; i < pt_at_vtx_vec.size(); ++i) {
            pt_at_vtx_vec[i] = pt_at_vtx_vec[i] / trk_pt_total;
          }
          beta = pt_at_vtx_vec[0];
          beta_max = *std::max_element(pt_at_vtx_vec.begin(), pt_at_vtx_vec.end());
        }
      }
    }

    jet.set_beta(beta);
    jet.set_beta_max(beta_max);
    jet.set_track_pt_total(trk_pt_total);

    //Set observed jet energy correction factors
    std::vector<std::string> jec_levels = iter->availableJECLevels();
    for (unsigned i = 0; i < jec_levels.size(); ++i) {
      observed_jec_[jec_levels[i]] = CityHash64(jec_levels[i]);
    }

    //Set observed b-tagging discriminators
    std::vector<std::pair<std::string, float> > const& b_discr = iter->getPairDiscri();
    for (unsigned i = 0; i < b_discr.size(); ++i) {
      observed_btag_[b_discr.at(i).first] = CityHash64(b_discr.at(i).first);
    }

    jet.set_id_loose((*loose_id_)(*iter));
    jet.set_id_tight((*tight_id_)(*iter));
  }
  iEvent.put(jet_particles, "selectGenParticles");
  iEvent.put(jet_tracks, "selectTracks");

}

// ------------ method called once each job just before starting event loop  ------------
void ICJPTJetProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_name_.c_str(), &jptjets_);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICJPTJetProducer::endJob() {
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Info in <ICJPTJetProducer>: EndJob Summary" << std::endl;
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Jet Energy Correction Hash Summary:" << std::endl;
  std::map<std::string, std::size_t>::const_iterator iter;
  for (iter = observed_jec_.begin(); iter != observed_jec_.end(); ++iter) {
    std::cout << boost::format("%-30s %-30s\n") % iter->first % iter->second;
  }
  std::cout << "b-Tag Discriminator Hash Summary:" << std::endl;
  for (iter = observed_btag_.begin(); iter != observed_btag_.end(); ++iter) {
    std::cout << boost::format("%-45s %-20s\n") % iter->first % iter->second;
  }
}

// ------------ method called when starting to processes a run  ------------
void ICJPTJetProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICJPTJetProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICJPTJetProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICJPTJetProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void ICJPTJetProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

std::vector<std::size_t> ICJPTJetProducer::makeTrackIDVec(reco::TrackRefVector const& ref_vec, 
                                                          std::vector<unsigned> & idx_vec,
                                                          reco::Track const* ptr_first) {
    static boost::hash<reco::Track const*> track_hasher;
    std::vector<std::size_t> id_vec;
    id_vec.resize(ref_vec.size());
    for (unsigned i = 0; i < ref_vec.size(); ++i) {
      unsigned idx = unsigned(&(*(ref_vec[i])) - ptr_first);
      idx_vec.push_back(idx);
      id_vec[i] = track_hasher(&(*(ref_vec[i])));
    }
    return id_vec;
}

std::vector<reco::Track const*> ICJPTJetProducer::TrackPtrVec(reco::TrackRefVector const& ref_vec) {
    std::vector<reco::Track const*> result;
    result.resize(ref_vec.size());
    for (unsigned i = 0; i < ref_vec.size(); ++i) {
      result[i] = &(*(ref_vec[i]));
    }
    return result;
}



//define this as a plug-in
DEFINE_FWK_MODULE(ICJPTJetProducer);
