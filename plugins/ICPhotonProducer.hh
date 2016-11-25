#ifndef UserCode_ICHiggsTauTau_ICPhotonProducer_h
#define UserCode_ICHiggsTauTau_ICPhotonProducer_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "UserCode/ICHiggsTauTau/interface/Photon.hh"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

// #include "EgammaAnalysis/ElectronTools/interface/PFIsolationEstimator.h"

/**
 * @brief See documentation [here](\ref objs-photon)
 */
class ICPhotonProducer : public edm::EDProducer {
 public:
  explicit ICPhotonProducer(const edm::ParameterSet &);
  ~ICPhotonProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  std::vector<ic::Photon> *photons_;
  edm::InputTag input_;
  std::string branch_;
  boost::hash<reco::Photon const*> photon_hasher_;
  // std::string pfiso_postfix_;
  // edm::InputTag vertex_input_;
  // PFIsolationEstimator *isolator_;

  edm::InputTag input_electron_veto_;
  bool do_electron_veto_;
  bool do_had_tow_over_em_;

  struct IsoTags {
    edm::InputTag charged_all;
    edm::InputTag charged;
    edm::InputTag neutral;
    edm::InputTag gamma;
    edm::InputTag pu;
    explicit IsoTags(edm::ParameterSet const& pset, edm::ConsumesCollector && collector);
  };

  IsoTags pf_iso_03_;
  IsoTags pf_iso_04_;
  bool do_pf_iso_03_;
  bool do_pf_iso_04_;
  bool do_iso_from_pat_;
};

#endif
