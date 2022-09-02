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
#include "DataFormats/Common/interface/SortedCollection.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/Tau.h"


ICPi0SuperClusterProducer::ICPi0SuperClusterProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      input_EERecHits_(config.getParameter<edm::InputTag>("input_EERecHits")),
      input_EBRecHits_(config.getParameter<edm::InputTag>("input_EBRecHits")),
      input_taus_(config.getParameter<edm::InputTag>("input_taus")),
      branch_(config.getParameter<std::string>("branch")){
  consumes<edm::View<reco::SuperCluster>>(input_);
  consumes<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> >>(input_EERecHits_);
  consumes<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> >>(input_EBRecHits_);
  consumes<edm::View<pat::Tau>>(input_taus_); 
#if CMSSW_MAJOR_VERSION >= 12
  tok_caloTopology_ = esConsumes<CaloTopology, CaloTopologyRecord>();
#endif
  scs_ = new std::vector<ic::SuperCluster>();
  PrintHeaderWithBranch(config, branch_);
}

ICPi0SuperClusterProducer::~ICPi0SuperClusterProducer() { delete scs_; }

void ICPi0SuperClusterProducer::produce(edm::Event& event,
                                     const edm::EventSetup& setup) {
#if (CMSSW_MAJOR_VERSION >= 9 && CMSSW_MINOR_VERSION >= 4 && CMSSW_REVISION > 4) || CMSSW_MAJOR_VERSION >= 10
  edm::Handle<edm::View<reco::SuperCluster> > sc_handle;
  edm::Handle<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> >> eehits_handle;
  edm::Handle<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> >> ebhits_handle;
  edm::Handle<edm::View<pat::Tau> > taus_handle;

  event.getByLabel(input_, sc_handle);
  event.getByLabel(input_EERecHits_, eehits_handle);
  event.getByLabel(input_EBRecHits_, ebhits_handle);
  event.getByLabel(input_taus_, taus_handle);

  scs_->clear();
  //scs_->resize(sc_handle->size());
#if CMSSW_MAJOR_VERSION < 12
  setup.get<CaloTopologyRecord>().get(theCaloTopo_);
  const CaloTopology* topology = theCaloTopo_.product();
#else
  const CaloTopology* topology = &setup.getData(tok_caloTopology_); 
#endif 

  for (unsigned i = 0; i < sc_handle->size(); ++i) {

    reco::SuperCluster const& src = sc_handle->at(i);

    // check is SC is close to a tau if not then discard it
    bool keep = false;
    for(unsigned j = 0; j < taus_handle->size(); ++j) {
      pat::Tau const& t = taus_handle->at(j);
      double dR = sqrt(deltaR2(t.eta(), t.phi(), src.eta(), src.phi()));
      if (dR<0.5) {keep = true; break;}
    }
 
    if(!keep) continue;

    ic::SuperCluster dest;// = scs_->at(i);
    dest.set_vx(src.x());
    dest.set_vy(src.y());
    dest.set_vz(src.z());
    dest.set_energy(src.energy());
    dest.set_raw_energy(src.rawEnergy());
    dest.set_id(sc_hasher_(&src));
    dest.set_phiWidth(src.phiWidth());
    dest.set_etaWidth(src.etaWidth());

    int subdet = src.seed()->hitsAndFractions()[0].first.subdetId();
    const EcalRecHitCollection* hits = nullptr;
    if(subdet == 1) hits = ebhits_handle.product();
    else hits = eehits_handle.product();

    float e3x3 = EcalClusterTools::e3x3(*(src.seed()), &(*hits), &(*topology)); 
    float full5x5_e3x3 = noZS::EcalClusterTools::e3x3(*(src.seed()), &(*hits), &(*topology));
    float r9 = e3x3 / (src.rawEnergy());
    float r9_full5x5 = full5x5_e3x3 / (src.rawEnergy());

    auto locCov = EcalClusterTools::localCovariances(*(src.seed()), &(*hits), &(*topology));
    auto full5x5_locCov = noZS::EcalClusterTools::localCovariances(*(src.seed()), &(*hits), &(*topology));

    float sigmaIetaIeta = sqrt(locCov[0]);
    float sigmaIetaIeta_full5x5 = sqrt(full5x5_locCov[0]);
  
    dest.set_r9(r9);
    dest.set_r9_full5x5(r9_full5x5);
    dest.set_sigmaIetaIeta(sigmaIetaIeta);
    dest.set_sigmaIetaIeta_full5x5(sigmaIetaIeta_full5x5);
 
    std::vector<ic::Candidate> cand_vec;

    reco::CaloClusterPtrVector clusters = src.clusters();
    dest.set_Nclusters(clusters.size());
    if(clusters.isAvailable()) {
      for(auto c : clusters) {
        double E = c->correctedEnergy();
        double theta = atan(exp(-c->eta()))*2;
        double pt = E*sin(theta);
        ic::Candidate new_cand;
        new_cand.set_pt(pt);
        new_cand.set_eta(c->eta());
        new_cand.set_phi(c->phi());
        new_cand.set_energy(E);
        cand_vec.push_back(new_cand);
      }
    }
    dest.set_clusters(cand_vec);  

    std::vector<ic::Candidate> ps_cand_vec;

    reco::CaloClusterPtrVector ps_clusters = src.preshowerClusters();
    if(ps_clusters.isAvailable()) {
      for(auto c : ps_clusters) {
        double E = c->energy();
        double theta = atan(exp(-c->eta()))*2;
        double pt = E*sin(theta);
        ic::Candidate new_cand;
        new_cand.set_pt(pt);
        new_cand.set_eta(c->eta());
        new_cand.set_phi(c->phi());
        new_cand.set_energy(E);
        ps_cand_vec.push_back(new_cand);
      }
    }
    dest.set_ps_clusters(ps_cand_vec);
 
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

    scs_->push_back(dest);
  }
#endif
}

void ICPi0SuperClusterProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &scs_);
}

void ICPi0SuperClusterProducer::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICPi0SuperClusterProducer);
