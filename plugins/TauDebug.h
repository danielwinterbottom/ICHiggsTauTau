#ifndef UserCode_ICHiggsTauTau_TauDebug_h
#define UserCode_ICHiggsTauTau_TauDebug_h

#include <memory>
#include <vector>
#include "boost/functional/hash.hpp"
#include "boost/format.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "UserCode/ICHiggsTauTau/interface/PFCandidate.hh"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "RecoParticleFlow/PFProducer/interface/GsfElectronEqual.h"
#include "RecoParticleFlow/PFProducer/interface/PFEGammaFilters.h"

class TauDebug : public edm::EDProducer {
 public:
  explicit TauDebug(const edm::ParameterSet &);
  ~TauDebug();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();

  edm::InputTag input_;

  bool passElectronSelection(const reco::GsfElectron& electron);
  bool isElectron(const reco::GsfElectron& electron);

  unsigned n_cands_ = 0;
  unsigned n_trackRefs_ = 0;
  unsigned n_gsfTrackRefs_ = 0;
  unsigned n_gsfElectronRefs_ = 0;
  unsigned n_conversionRefs_ = 0;
  unsigned n_photonRefs_ = 0;
  unsigned n_photons_with_convs_ = 0;
  unsigned n_photons_with_convsOneLeg_ = 0;
  unsigned n_match_elec_ = 0;
  unsigned n_elec_fail_good_ = 0;
  unsigned n_elec_fail_primary_ = 0;
  unsigned n_elec_fail_both_ = 0;
  unsigned n_with_conv_no_elec_ = 0;
  unsigned n_with_convOneLeg_no_elec_ = 0;
};



#endif
