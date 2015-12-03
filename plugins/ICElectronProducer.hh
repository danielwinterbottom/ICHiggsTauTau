#ifndef UserCode_ICHiggsTauTau_ICElectronProducer_h
#define UserCode_ICHiggsTauTau_ICElectronProducer_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"

/**
 * @brief See documentation [here](\ref objs-electron)
 */
class ICElectronProducer : public edm::EDProducer {
 public:
  explicit ICElectronProducer(const edm::ParameterSet &);
  ~ICElectronProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  std::vector<ic::Electron> *electrons_;
  edm::InputTag input_;
  std::string branch_;
  boost::hash<reco::GsfElectron const*> gsf_electron_hasher_;

  std::vector<std::pair<std::string, edm::InputTag> > input_vmaps_;
  edm::InputTag input_r9_;
  bool do_r9_;
  edm::InputTag input_hcal_sum_;
  bool do_hcal_sum_;
  edm::InputTag input_vertices_;
  bool do_vertex_ip_;
  edm::InputTag input_beamspot_;
  bool do_beamspot_ip_;
  edm::InputTag input_conversion_matches_;
  bool do_conversion_matches_;

  struct IsoTags {
    edm::InputTag charged_all;
    edm::InputTag charged;
    edm::InputTag neutral;
    edm::InputTag gamma;
    edm::InputTag pu;
    explicit IsoTags(edm::ParameterSet const& pset, edm::ConsumesCollector && collector);
  };

  struct ClusterIsoTags {
    edm::InputTag ecal;
    edm::InputTag hcal;
    explicit ClusterIsoTags(edm::ParameterSet const& pset, edm::ConsumesCollector && collector);
  };


  ClusterIsoTags cluster_iso_;
  IsoTags pf_iso_03_;
  IsoTags pf_iso_04_;
  bool do_cluster_iso_;
  bool do_pf_iso_03_;
  bool do_pf_iso_04_;
};

#endif
