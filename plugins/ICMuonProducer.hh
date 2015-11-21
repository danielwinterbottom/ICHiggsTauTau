#ifndef UserCode_ICHiggsTauTau_ICMuonProducer_h
#define UserCode_ICHiggsTauTau_ICMuonProducer_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

/**
 * @brief See documentation [here](\ref objs-muon)
 */
class ICMuonProducer : public edm::EDProducer {
 public:
  explicit ICMuonProducer(const edm::ParameterSet&);
  ~ICMuonProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  std::vector<ic::Muon>* muons_;
  edm::InputTag input_;
  std::string branch_;
  boost::hash<reco::Muon const*> muon_hasher_;
  boost::hash<reco::PFCandidate const*> pf_hasher_;


  bool is_pf_;

  std::vector<std::pair<std::string, edm::InputTag> > input_vmaps_;
  std::vector<std::pair<std::string, edm::InputTag> > input_double_vmaps_;
  edm::InputTag input_vertices_;
  bool do_vertex_ip_;
  edm::InputTag input_beamspot_;
  bool do_beamspot_ip_;

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
};
#endif
