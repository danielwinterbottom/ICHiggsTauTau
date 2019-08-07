#include "UserCode/ICHiggsTauTau/plugins/ICPi0SuperClusterProducer.hh"
#include <memory>
#include <string>
#include <vector>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/SuperCluster.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"
#include "DataFormats/CaloRecHit/interface/CaloCluster.h"

ICPi0SuperClusterProducer::ICPi0SuperClusterProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")) {
  consumes<edm::View<reco::SuperCluster>>(input_);
  scs_ = new std::vector<ic::SuperCluster>();
  PrintHeaderWithBranch(config, branch_);
}

ICPi0SuperClusterProducer::~ICPi0SuperClusterProducer() { delete scs_; }

void ICPi0SuperClusterProducer::produce(edm::Event& event,
                                     const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::SuperCluster> > sc_handle;

  event.getByLabel(input_, sc_handle);

  scs_->clear();
  scs_->resize(sc_handle->size());

  for (unsigned i = 0; i < sc_handle->size(); ++i) {
    reco::SuperCluster const& src = sc_handle->at(i);
    ic::SuperCluster& dest = scs_->at(i);
    dest.set_vx(src.x());
    dest.set_vy(src.y());
    dest.set_vz(src.z());
    dest.set_energy(src.energy());
    dest.set_raw_energy(src.rawEnergy());
    dest.set_id(sc_hasher_(&src));
    dest.set_phiWidth(src.phiWidth());
    dest.set_etaWidth(src.etaWidth());
    
    std::vector<ic::Candidate> cand_vec;

//edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> >    "reducedEgamma"             "reducedEBRecHits"   "PAT"     
//edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> >    "reducedEgamma"             "reducedEERecHits"   "PAT"
    //reco::CaloClusterPtrVector clusters = src.clusters();
    //for (reco::CaloCluster_iterator bit = src.clustersBegin(); bit != src.clustersEnd(); ++bit) {
    //  const reco::CaloClusterPtr c = *bit;
    //  double E = c->correctedEnergy();
    //  std::cout << E << std::endl;
    //  double theta = atan(exp(-c->eta()))*2;
    //  double pt = E*sin(theta);
    //  ic::Candidate new_cand;
    //  new_cand.set_pt(pt);
    //  new_cand.set_eta(c->eta());
    //  new_cand.set_phi(c->phi());
    //  new_cand.set_energy(E);
    //  cand_vec.push_back(new_cand);
    //}
    dest.set_clusters(cand_vec);   
    ic::Candidate seed_cand;
    double E = src.seed()->correctedEnergy();
    double theta = atan(exp(-src.seed()->eta()))*2;
    double pt = E*sin(theta);
    ic::Candidate new_cand;
    seed_cand.set_pt(pt);
    seed_cand.set_eta(src.seed()->eta());
    seed_cand.set_phi(src.seed()->phi());
    seed_cand.set_energy(E);
    dest.set_seed_cluster(seed_cand);
  }

}

void ICPi0SuperClusterProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &scs_);
}

void ICPi0SuperClusterProducer::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICPi0SuperClusterProducer);
