#ifndef UserCode_ICHiggsTauTau_ICJetProducer_h
#define UserCode_ICHiggsTauTau_ICJetProducer_h

#include <memory>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
// #include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/CaloJet.hh"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "UserCode/ICHiggsTauTau/plugins/ICJetProducerConfig.hh"


template <class T, class U>
class ICJetProducer : public edm::EDProducer {
 public:
  explicit ICJetProducer(const edm::ParameterSet &);
  ~ICJetProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();
  void constructSpecific(edm::Handle<edm::View<U> > const& jets_handle,
                         edm::Event& event, const edm::EventSetup& setup);

  std::vector<T> *jets_;
  edm::InputTag input_;
  std::string branch_;
  boost::hash<U const*> jet_hasher_;

  edm::InputTag input_jet_flavour_;
  bool do_jet_flavour_;

  SpecificConfig<U> cfg_;
};

// =============================
// Template class implementation
// =============================
template <class T, class U>
ICJetProducer<T, U>::ICJetProducer(const edm::ParameterSet &config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      do_jet_flavour_(false) {
  jets_ = new std::vector<T>();

  if (config.exists("specificConfig")) {
    cfg_.Set(config.getParameterSet("specificConfig"));
  }

  if (config.exists("includeJetFlavour")) {
    input_jet_flavour_ =
        config.getParameter<edm::InputTag>("includeJetFlavour");
    do_jet_flavour_ = true;
  }
}

template <class T, class U>
ICJetProducer<T, U>::~ICJetProducer() { delete jets_; }

// ============
// Main produce
// ============
template <class T, class U>
void ICJetProducer<T, U>::produce(edm::Event& event,
                                 const edm::EventSetup& setup) {
  edm::Handle<edm::View<U> > jets_handle;
  event.getByLabel(input_, jets_handle);

  edm::Handle<edm::ValueMap<int> > jet_flavour_handle;
  if (do_jet_flavour_) event.getByLabel(input_jet_flavour_, jet_flavour_handle);

  jets_->clear();
  jets_->resize(jets_handle->size(), T());

  for (unsigned i = 0; i < jets_handle->size(); ++i) {
    U const& src = jets_handle->at(i);
    T & dest = jets_->at(i);
    dest.set_id(jet_hasher_(&src));
    dest.set_pt(src.pt());
    dest.set_eta(src.eta());
    dest.set_phi(src.phi());
    dest.set_energy(src.energy());
    dest.set_charge(src.charge());
    dest.set_jet_area(src.jetArea());
    if (do_jet_flavour_) {
      dest.set_parton_flavour((*jet_flavour_handle)[jets_handle->refAt(i)]);
    }
  }
  constructSpecific(jets_handle, event, setup);
}

// ==================
// Specific producers
// ==================
template <class T, class U>
void ICJetProducer<T, U>::constructSpecific(
    edm::Handle<edm::View<U> > const& jets_handle, edm::Event& event,
    const edm::EventSetup& setup) {}

template <>
void ICJetProducer<ic::CaloJet, reco::CaloJet>::constructSpecific(
    edm::Handle<edm::View<reco::CaloJet> > const& jets_handle,
    edm::Event& event, const edm::EventSetup& setup) {
  edm::Handle<reco::JetIDValueMap> jet_id_handle;
  if (cfg_.do_jet_id) event.getByLabel(cfg_.input_jet_id, jet_id_handle);
  for (unsigned i = 0; i < jets_handle->size(); ++i) {
    reco::CaloJet const& src = jets_handle->at(i);
    ic::CaloJet & dest = jets_->at(i);
    dest.set_max_em_tower_energy(src.maxEInEmTowers());
    dest.set_max_had_tower_energy(src.maxEInHadTowers());
    dest.set_energy_frac_had(src.energyFractionHadronic());
    dest.set_energy_frac_em(src.emEnergyFraction());
    dest.set_had_energy_HB(src.hadEnergyInHB());
    dest.set_had_energy_HO(src.hadEnergyInHO());
    dest.set_had_energy_HE(src.hadEnergyInHE());
    dest.set_had_energy_HF(src.hadEnergyInHF());
    dest.set_em_energy_EB(src.emEnergyInEB());
    dest.set_em_energy_EE(src.emEnergyInEE());
    dest.set_em_energy_HF(src.emEnergyInHF());
    dest.set_towers_area(src.towersArea());
    dest.set_n90(src.n90());
    dest.set_n60(src.n60());
    if (cfg_.do_jet_id) {
      reco::JetID const & jet_id = (*jet_id_handle)[jets_handle->refAt(i)];
      dest.set_fHPD(jet_id.fHPD);
      dest.set_fRBX(jet_id.fRBX);
      dest.set_id_loose((*(cfg_.loose_id))(src, jet_id));
      dest.set_id_tight((*(cfg_.tight_id))(src, jet_id));
    }
  }
}

template <class T, class U>
void ICJetProducer<T, U>::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &jets_);
}

template <class T, class U>
void ICJetProducer<T, U>::endJob() {
}


#endif
