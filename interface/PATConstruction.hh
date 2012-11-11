#ifndef ICHiggsTauTau_PATConstruction_hh
#define ICHiggsTauTau_PATConstruction_hh
#include <map>
#include <string>
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "UserCode/ICHiggsTauTau/interface/Jet.hh"
#include "UserCode/ICHiggsTauTau/interface/CaloJet.hh"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/JPTJet.hh"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"


namespace ic {

  void ConstructJet(ic::Jet * jet, pat::Jet const* pat_jet);

  void ConstructCaloJet(ic::CaloJet * jet, pat::Jet const* pat_jet);

  void ConstructPFJet(ic::PFJet * jet, pat::Jet const* pat_jet);

  void ConstructJPTJet(ic::JPTJet * jet, pat::Jet const* pat_jet);

  void ConstructGenParticle(ic::GenParticle * particle, 
                            reco::GenParticle const* reco_particle);

  void ConstructElectron(ic::Electron *ele, pat::Electron const* pat_ele);

  void ConstructMuon(ic::Muon *muo, pat::Muon const* pat_muo);

}
#endif