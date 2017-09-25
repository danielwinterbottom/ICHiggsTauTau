#ifndef UserCode_ICHiggsTauTau_ICTauSpinnerProducer_h
#define UserCode_ICHiggsTauTau_ICTauSpinnerProducer_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"

#include "TauSpinner/SimpleParticle.h"
#include "TauSpinner/tau_reweight_lib.h"
//#include "GeneratorInterface/TauolaInterface/interface/TauSpinnerCMS.h"
#include "LHAPDF/LHAPDF.h"
#include "Tauola/Tauola.h"

/**
 * @brief See documentation [here](\ref objs-candidate)
 */
class ICTauSpinnerProducer : public edm::EDProducer {
 public:
  explicit ICTauSpinnerProducer(const edm::ParameterSet &);
  ~ICTauSpinnerProducer();

 private:
  virtual void beginJob();
  virtual void Init();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  edm::InputTag input_;
  std::string branch_;
  boost::hash<reco::Candidate const*> cand_hasher_;
  
  std::string TauSpinnerSettingsPDF;
  bool TauSpinnerSettingsIpp;
  int TauSpinnerSettingsIpol;
  int TauSpinnerSettingsNonSM2;
  int TauSpinnerSettingsNonSMN;
  double TauSpinnerSettingsCmsEnergy;
};

#endif
