#ifndef UserCode_ICHiggsTauTau_ICEventInfoProducer_h
#define UserCode_ICHiggsTauTau_ICEventInfoProducer_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"

/**
 * @brief Produces an ic::EventInfo object
 *
 * **Example usage**
 * @snippet python/default_producers_cfi.py EventInfo
 */
class ICEventInfoProducer : public edm::EDProducer {
 public:
  explicit ICEventInfoProducer(const edm::ParameterSet &);
  ~ICEventInfoProducer();

 private:
  virtual void beginJob();
  virtual void endRun(edm::Run const& run, edm::EventSetup const& es);
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  ic::EventInfo *info_;
  std::string branch_;
  edm::InputTag lhe_collection_;
  bool do_jets_rho_;
  edm::InputTag input_jets_rho_;
  bool do_leptons_rho_;
  edm::InputTag input_leptons_rho_;
  bool do_vertex_count_;
  edm::InputTag input_vertices_;
  bool do_csc_filter_;
  edm::InputTag input_csc_filter_;
  bool do_lhe_weights_;
  bool do_ht_;
  bool do_filtersfromtrig_;
  std::vector<std::pair<std::string, edm::InputTag> > filters_;
  std::vector<std::string> filtersfromtrig_;
  edm::InputTag filtersfromtrig_input_;
  std::vector<std::pair<std::string, edm::InputTag> > weights_;
  std::vector<std::pair<std::string, edm::InputTag> > gen_weights_;
  std::set<std::string> invert_filter_logic_;
  std::map<std::string, std::size_t> observed_filters_;
  std::vector<std::string> lhe_weight_labels_;
};

#endif
