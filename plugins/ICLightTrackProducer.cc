#include "UserCode/ICHiggsTauTau/plugins/ICLightTrackProducer.hh"
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
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "UserCode/ICHiggsTauTau/interface/LightTrack.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"


#include "UserCode/ICHiggsTauTau/interface/city.h"

#include "boost/format.hpp"


ICLightTrackProducer::ICLightTrackProducer(const edm::ParameterSet& iConfig) {
 cand_vec = new std::vector<ic::LightTrack>();
 merge_labels_ = iConfig.getUntrackedParameter<std::vector<std::string> >("mergeLabels");
 std::cout << "Info in <ICLightTrackProducer>: Picking up Track requests from the following modules:" << std::endl;
 for (unsigned i = 0; i < merge_labels_.size(); ++i) {
   std::cout << "-- " << merge_labels_[i] << std::endl;
 }
}


ICLightTrackProducer::~ICLightTrackProducer() {
 // do anything here that needs to be done at desctruction time
 // (e.g. close files, deallocate resources etc.)
 delete cand_vec;
}



// ------------ method called to produce the data  ------------
void ICLightTrackProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  boost::hash<reco::Track const*> hasher;

  edm::Handle<reco::TrackCollection> trackCollection;
  iEvent.getByLabel("generalTracks",trackCollection);
  if (trackCollection->size() == 0) return;
  const reco::Track *ptr_to_first  = &((trackCollection->at(0)));
  const reco::Track *ptr_to_last  = &((trackCollection->at(trackCollection->size() -1)));

  std::set<reco::Track const*> ptr_set;
  std::vector<edm::Handle<std::vector<unsigned> > > merge_handles;
  merge_handles.resize(merge_labels_.size());
  for (unsigned i = 0; i < merge_handles.size(); ++i) {
    iEvent.getByLabel(merge_labels_[i],"selectTracks", merge_handles[i]);
    for (unsigned j = 0; j < merge_handles[i]->size(); ++j) {
      const reco::Track *ptr = &(trackCollection->at(merge_handles[i]->at(j)));
      ptr_set.insert(ptr);
    }
  }


  //reco::GenParticleCollection::const_iterator iter;
  std::set<reco::Track const*>::const_iterator iter;
  cand_vec->resize(0);
  cand_vec->reserve(trackCollection->size());

  for (iter = ptr_set.begin(); iter != ptr_set.end(); ++iter) {
    if (!(*iter >= ptr_to_first && *iter <= ptr_to_last)) {
      std::cerr << "Warning: Track pointer outside generalTracks vector!" << std::endl;
      continue;
    }
    cand_vec->push_back(ic::LightTrack());
    ic::LightTrack & cand = cand_vec->back();
    //Set candidate data
    cand.set_id(hasher(*iter));
    cand.set_pt((*iter)->pt());
    cand.set_vz((*iter)->vz());
  }
}

// ------------ method called once each job just before starting event loop  ------------
void ICLightTrackProducer::beginJob() {
 ic::StaticTree::tree_->Branch("lightTracks",&cand_vec);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICLightTrackProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void ICLightTrackProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICLightTrackProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICLightTrackProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICLightTrackProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICLightTrackProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICLightTrackProducer);
