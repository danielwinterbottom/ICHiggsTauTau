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
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"

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
    bool do_charged_all;
    bool do_charged;
    bool do_neutral;
    bool do_gamma;
    bool do_pu;
    IsoTags();
    void Set(edm::ParameterSet const& pset);
  };

  IsoTags pf_iso_03_;
  IsoTags pf_iso_04_;

  /*
   [x] remove reco muon overlap check
   [x] remove manual isolation check
   [x] option/config for dr03 pf isolation
   [x] option/config for dr04 pf isolation
   [x] option/config for mva id output
   [x] option/config for conversion
   [x] option/config for vertex/beamspot IPs
   [x] option/config for lazyTool usage (for r9)
   [x] option/config for hcal_sum
   [x] drop gen particles
  */
};

#endif
