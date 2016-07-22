#ifndef UserCode_ICHiggsTauTau_ICHashTreeProducer_h
#define UserCode_ICHiggsTauTau_ICHashTreeProducer_h

#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


class ICHashTreeProducer : public edm::EDProducer {
 public:
  explicit ICHashTreeProducer(const edm::ParameterSet&);
  ~ICHashTreeProducer();

  static void Add(std::size_t const& id, std::string const& str);

 private:
  static std::map<std::size_t, std::string> unhash_map_;
  virtual void beginJob();
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob();
};

#endif
