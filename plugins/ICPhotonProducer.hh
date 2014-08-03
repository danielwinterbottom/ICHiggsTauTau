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
// #include "EgammaAnalysis/ElectronTools/interface/PFIsolationEstimator.h"

/**
 * @brief Produces an ic::Photon collection from any input compatible with
 *an edm::View<reco::Photon>
 *
 * **Example usage**
 * @snippet python/default_producers_cfi.py Photon
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
  edm::InputTag input_had_tow_over_em_;
  bool do_had_tow_over_em_;

  struct IsoTags {
    edm::InputTag charged;
    edm::InputTag neutral;
    edm::InputTag gamma;
    explicit IsoTags(edm::ParameterSet const& pset);
  };

  IsoTags pf_iso_;
  bool do_pf_iso_;
};

#endif
