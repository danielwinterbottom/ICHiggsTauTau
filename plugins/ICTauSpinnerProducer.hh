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
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "UserCode/ICHiggsTauTau/TAUOLA/TauSpinner/include/TauSpinner/SimpleParticle.h"
#include "UserCode/ICHiggsTauTau/TAUOLA/TauSpinner/include/TauSpinner/tau_reweight_lib.h"
//#include "TauSpinner/SimpleParticle.h"
//#include "TauSpinner/tau_reweight_lib.h"
//#include "GeneratorInterface/TauolaInterface/interface/TauSpinnerCMS.h"
//#include "LHAPDF/LHAPDF.h"
//#include "Tauola/Tauola.h"


enum pdgId {
  Gamma = 22,
  PiZero = 111,
  PiPlus = 211,
  KPlus = 321,
  KLong = 130,
  KShort = 310,
  Electron = 11,
  NuE = 12,
  Muon = 13,
  NuMu = 14,
  NuTau = 16
};

/**
 * @brief See documentation [here](\ref objs-candidate)
 */
class ICTauSpinnerProducer : public edm::EDProducer {
 public:
  explicit ICTauSpinnerProducer(const edm::ParameterSet &);
  ~ICTauSpinnerProducer();

 private:
  virtual void beginJob();
  virtual void initialize();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();
  reco::GenParticle getBoson(edm::Handle<edm::View<reco::GenParticle> > parts_handle);
  std::vector<reco::GenParticle> getTaus(reco::GenParticle boson, edm::Handle<edm::View<reco::GenParticle> > parts_handle);
  void getTauDaughters(std::vector<reco::GenParticle> &tau_daughters, reco::GenParticle tau, edm::Handle<edm::View<reco::GenParticle> > parts_handle);
  TauSpinner::SimpleParticle ConvertToSimplePart(reco::GenParticle input_part);

  edm::InputTag input_;
  std::string branch_;
  double theta_;
  int bosonPdgId_;
  double weight_;
  
  std::string TauSpinnerSettingsPDF;
  bool TauSpinnerSettingsIpp;
  int TauSpinnerSettingsIpol;
  int TauSpinnerSettingsNonSM2;
  int TauSpinnerSettingsNonSMN;
  double TauSpinnerSettingsCmsEnergy;
};

#endif
