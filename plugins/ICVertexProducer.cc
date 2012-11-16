#include "UserCode/ICHiggsTauTau/plugins/ICVertexProducer.hh"
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

#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"

#include "UserCode/ICHiggsTauTau/interface/city.h"

#include "boost/format.hpp"


ICVertexProducer::ICVertexProducer(const edm::ParameterSet& iConfig) {
  produces<std::vector<unsigned> >("selectTracks");
  input_label_ = iConfig.getParameter<edm::InputTag>("inputLabel");
  branch_name_ = iConfig.getUntrackedParameter<std::string>("branchName");
  track_pt_threshold_ = iConfig.getUntrackedParameter<double>("trackPtThreshold");
  store_ids_ = iConfig.getParameter<bool>("StoreTrackIds");
  first_only_ = iConfig.getParameter<bool>("FirstVertexOnly");
  if (store_ids_) std::cout << "Info in <ICVertexProducer>: All track IDs stored, but only tracks with pT > " << track_pt_threshold_ 
    << " GeV will be requested." << std::endl;
  cand_vec = new std::vector<ic::Vertex>();
}


ICVertexProducer::~ICVertexProducer() {
 // do anything here that needs to be done at desctruction time
 // (e.g. close files, deallocate resources etc.)
 delete cand_vec;
}



// ------------ method called to produce the data  ------------
void ICVertexProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  boost::hash<reco::Vertex const*> hasher;
  boost::hash<reco::Track const*> trk_hasher;
  edm::Handle<reco::VertexCollection> vertexCollection;
  iEvent.getByLabel(input_label_,vertexCollection);
  std::auto_ptr<std::vector<unsigned> > vtx_tracks(new std::vector<unsigned>());

  edm::Handle<reco::TrackCollection> trackCollection;
  iEvent.getByLabel("generalTracks",trackCollection);
  reco::Track const* ptr_to_first = 0;
  if (trackCollection->size() > 0) ptr_to_first = &(trackCollection->at(0));

  std::vector<reco::Vertex>::const_iterator iter;
  cand_vec->resize(0);
  
  if (!first_only_) {
    cand_vec->reserve(vertexCollection->size());
  } else {
    cand_vec->reserve(1);
  }

  for (iter = vertexCollection->begin(); iter != vertexCollection->end(); ++iter) {
      cand_vec->push_back(ic::Vertex());
      ic::Vertex & cand = cand_vec->back();
      //Set candidate data
      cand.set_id(hasher(&(*iter)));
      cand.set_vx(iter->x());
      // cand.set_vx_error(iter->xError());
      cand.set_vy(iter->y());
      // cand.set_vy_error(iter->yError());
      cand.set_vz(iter->z());
      // cand.set_vz_error(iter->zError());
      cand.set_chi2(iter->chi2());
      cand.set_ndof(iter->ndof());
        if (trackCollection->size() > 0 && store_ids_) {
          for (std::vector<reco::TrackBaseRef>::const_iterator trk_iter = iter->tracks_begin();
           trk_iter != iter->tracks_end(); ++trk_iter) {
            float weight = iter->trackWeight(*trk_iter);
          reco::Track const* ptr_to_trk = dynamic_cast<reco::Track const*>(&(**trk_iter));
          if (ptr_to_trk->pt() < track_pt_threshold_) continue;
          cand.AddTrack(trk_hasher(ptr_to_trk),weight);
          vtx_tracks->push_back(unsigned(ptr_to_trk - ptr_to_first));
        }
      }
      if (first_only_) break;
}
  iEvent.put(vtx_tracks, "selectTracks");

}

// ------------ method called once each job just before starting event loop  ------------
void ICVertexProducer::beginJob() {
 ic::StaticTree::tree_->Branch(branch_name_.c_str(),&cand_vec);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICVertexProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void ICVertexProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICVertexProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICVertexProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICVertexProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICVertexProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICVertexProducer);
