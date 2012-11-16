#include "UserCode/ICHiggsTauTau/plugins/ICSuperClusterProducer.hh"
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


#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"

#include "UserCode/ICHiggsTauTau/interface/PATConstruction.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"

#include "boost/format.hpp"


ICSuperClusterProducer::ICSuperClusterProducer(const edm::ParameterSet& iConfig) {
  branch_name_ = iConfig.getUntrackedParameter<std::string>("branchName");
  min_energy_ = iConfig.getUntrackedParameter<double>("minEnergy");
  scs_ = new std::vector<ic::SuperCluster>();
}


ICSuperClusterProducer::~ICSuperClusterProducer() {
 delete scs_;

}

// ------------ method called to produce the data  ------------
void ICSuperClusterProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  boost::hash<reco::SuperCluster const*> hasher;


  edm::Handle<std::vector<reco::SuperCluster> > scBarrelCollection;
  edm::Handle<std::vector<reco::SuperCluster> > scEndCapCollection;

  iEvent.getByLabel("correctedHybridSuperClusters",scBarrelCollection);
  iEvent.getByLabel("correctedMulti5x5SuperClustersWithPreshower",scEndCapCollection);

  std::vector<reco::SuperCluster>::const_iterator iter;
  //Set the Jet vector size to zero, then allocate enough space
  //for at least as many elements in the PAT::Jet vector
  scs_->resize(0);
  scs_->reserve(scBarrelCollection->size() + scEndCapCollection->size());

  for (iter = scBarrelCollection->begin(); iter != scBarrelCollection->end(); ++iter) {
    if (iter->energy() < min_energy_) continue;
    scs_->push_back(ic::SuperCluster());
    ic::SuperCluster & sc = scs_->back();
    sc.set_vx(iter->x());
    sc.set_vy(iter->y());
    sc.set_vz(iter->z());
    sc.set_energy(iter->energy());
    sc.set_raw_energy(iter->rawEnergy());
    sc.set_is_barrel(true);
    sc.set_id(hasher(&(*iter)));
  }

  for (iter = scEndCapCollection->begin(); iter != scEndCapCollection->end(); ++iter) {
    if (iter->energy() < min_energy_) continue;
    scs_->push_back(ic::SuperCluster());
    ic::SuperCluster & sc = scs_->back();
    sc.set_vx(iter->x());
    sc.set_vy(iter->y());
    sc.set_vz(iter->z());
    sc.set_energy(iter->energy());
    sc.set_raw_energy(iter->rawEnergy());
    sc.set_is_barrel(false);
    sc.set_id(hasher(&(*iter)));
  }


}

// ------------ method called once each job just before starting event loop  ------------
void ICSuperClusterProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_name_.c_str(), &scs_);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICSuperClusterProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void ICSuperClusterProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICSuperClusterProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICSuperClusterProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICSuperClusterProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICSuperClusterProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICSuperClusterProducer);
