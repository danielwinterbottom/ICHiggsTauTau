#include "UserCode/ICHiggsTauTau/plugins/ICSecondaryVertexProducer.hh"
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
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"

#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"

#include "UserCode/ICHiggsTauTau/interface/city.h"

#include "boost/format.hpp"


ICSecondaryVertexProducer::ICSecondaryVertexProducer(const edm::ParameterSet& iConfig) {
  produces<std::vector<unsigned> >("selectTracks");
  input_label_ = iConfig.getParameter<edm::InputTag>("inputLabel");
  branch_name_ = iConfig.getUntrackedParameter<std::string>("branchName");
  track_pt_threshold_ = iConfig.getUntrackedParameter<double>("trackPtThreshold");
  store_ids_ = iConfig.getParameter<bool>("StoreTrackIds");
  if (store_ids_) std::cout << "Info in <ICSecondaryVertexProducer>: All track IDs stored, but only tracks with pT > " << track_pt_threshold_ 
    << " GeV will be requested." << std::endl;

  merge_labels_ = iConfig.getParameter<std::vector<std::string> >("mergeLabels");
  std::cout << "Info in <ICSecondaryVertexProducer>: Picking up SecondaryVertex requests from the following modules:" << std::endl;
  for (unsigned i = 0; i < merge_labels_.size(); ++i) {
    std::cout << "-- " << merge_labels_[i] << std::endl;
  }
  cand_vec = new std::vector<ic::SecondaryVertex>();
}


ICSecondaryVertexProducer::~ICSecondaryVertexProducer() {
 // do anything here that needs to be done at desctruction time
 // (e.g. close files, deallocate resources etc.)
 delete cand_vec;
}



// ------------ method called to produce the data  ------------
void ICSecondaryVertexProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  boost::hash<reco::Vertex const*> vertex_hasher;
  boost::hash<reco::Track const*> track_hasher;
  
  edm::Handle<std::vector<reco::SecondaryVertexTagInfo> > tag_info_collection;
  iEvent.getByLabel(input_label_, tag_info_collection);

  std::auto_ptr<std::vector<unsigned> > vtx_tracks(new std::vector<unsigned>());

  edm::Handle<reco::TrackCollection> track_collection;
  iEvent.getByLabel("generalTracks", track_collection);
  reco::Track const* ptr_to_first = 0;
  if (track_collection->size() > 0) ptr_to_first = &(track_collection->at(0));
  
  std::set<unsigned> request_set;
  std::vector<edm::Handle<std::vector<unsigned> > > merge_handles;
  merge_handles.resize(merge_labels_.size());
  for (unsigned i = 0; i < merge_handles.size(); ++i) {
    iEvent.getByLabel(merge_labels_[i],"selectSecondaryVertices", merge_handles[i]);
    for (unsigned j = 0; j < merge_handles[i]->size(); ++j) {
      request_set.insert(merge_handles[i]->at(j));
    }
  }

  cand_vec->resize(0);
  
  std::vector<reco::SecondaryVertexTagInfo>::const_iterator iter;
  unsigned iT = 0;
  for (iter = tag_info_collection->begin(); iter != tag_info_collection->end(); ++iter, ++iT) {
    if (!request_set.count(iT)) continue;
    for (unsigned i = 0; i < iter->nVertices(); ++i) {
      cand_vec->push_back(ic::SecondaryVertex());
      ic::SecondaryVertex & cand = cand_vec->back();
      //Set candidate data
      reco::Vertex const& sv = iter->secondaryVertex(i);
      cand.set_id(vertex_hasher(&sv));
      cand.set_vx(sv.x());
      cand.set_vy(sv.y());
      cand.set_vz(sv.z());
      cand.set_chi2(sv.chi2());
      cand.set_ndof(sv.ndof());
      cand.set_distance_3d(iter->flightDistance(i, true).value());
      cand.set_distance_err_3d(iter->flightDistance(i, true).error());

      if (track_collection->size() > 0 && store_ids_) {
        for (std::vector<reco::TrackBaseRef>::const_iterator trk_iter = sv.tracks_begin();
            trk_iter != sv.tracks_end(); ++trk_iter) {
          float weight = sv.trackWeight(*trk_iter);
          reco::Track const* ptr_to_trk = dynamic_cast<reco::Track const*>(&(**trk_iter));
          if (ptr_to_trk->pt() < track_pt_threshold_) continue;
          cand.AddTrack(track_hasher(ptr_to_trk), weight);
          vtx_tracks->push_back(unsigned(ptr_to_trk - ptr_to_first));
        }
      }
    } // end loop i : nVertices
  }
  iEvent.put(vtx_tracks, "selectTracks");
}

// ------------ method called once each job just before starting event loop  ------------
void ICSecondaryVertexProducer::beginJob() {
 ic::StaticTree::tree_->Branch(branch_name_.c_str(),&cand_vec);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICSecondaryVertexProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void ICSecondaryVertexProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICSecondaryVertexProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICSecondaryVertexProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICSecondaryVertexProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICSecondaryVertexProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICSecondaryVertexProducer);
