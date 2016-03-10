#ifndef UserCode_ICHiggsTauTau_GenChannelFilter_h
#define UserCode_ICHiggsTauTau_GenChannelFilter_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"


class GenChannelFilter : public edm::EDFilter {
 public:
  explicit GenChannelFilter(const edm::ParameterSet&);
  ~GenChannelFilter();

 private:
  virtual void beginJob();
  virtual bool filter(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  edm::InputTag input_;
  int channel_;

};

#endif
