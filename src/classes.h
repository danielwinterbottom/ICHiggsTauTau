#include <vector>
#include <map>
#include <utility>
#include <string>
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/PFCandidate.hh"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "UserCode/ICHiggsTauTau/interface/Jet.hh"
#include "UserCode/ICHiggsTauTau/interface/CaloJet.hh"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/JPTJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Track.hh"
#include "UserCode/ICHiggsTauTau/interface/LightTrack.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"
#include "UserCode/ICHiggsTauTau/interface/SecondaryVertex.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Photon.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/PileupInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/TriggerPath.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/SuperCluster.hh"
#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"
#include "UserCode/ICHiggsTauTau/interface/TH2DAsymErr.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

namespace { struct dictionary {
  ic::Candidate dummy1;
  std::vector<ic::Candidate> dummy2;
  ic::Jet dummy3;
  std::vector<ic::Jet> dummy4;
  std::map<size_t, float> dummy5;
  ic::CaloJet dummy6;
  std::vector<ic::CaloJet> dummy7;
  ic::GenParticle dummy8;
  std::vector<ic::GenParticle> dummy9;
  ic::PFJet dummy10;
  std::vector<ic::PFJet> dummy11;
  ic::JPTJet dummy12;
  std::vector<ic::JPTJet> dummy13;
  ic::Track dummy14;
  std::vector<ic::Track> dummy15;
  ic::GenJet dummy16;
  std::vector<ic::GenJet> dummy17;
  ic::Vertex dummy18;
  std::vector<ic::Vertex> dummy19;
  std::pair<unsigned long, float> dummy20;
  std::vector< std::pair<unsigned long, float> > dummy21;
  ic::Electron dummy22;
  std::vector<ic::Electron> dummy23;
  ic::Muon dummy24;
  std::vector<ic::Muon> dummy25;
  std::pair<std::string, double> dummy26;
  std::vector< std::pair<std::string, double> > dummy27;
  ic::EventInfo dummy28;
  ic::PileupInfo dummy30;
  std::vector<ic::PileupInfo> dummy31;
  ic::TriggerPath dummy32;
  std::vector<ic::TriggerPath> dummy33;
  ic::Met dummy34;
  ic::Met::BasicMet dummy34a;
  std::map<std::size_t, ic::Met::BasicMet> dummy34b;
  ic::LightTrack dummy35;
  std::vector<ic::LightTrack> dummy36;
  ic::Tau dummy37;
  std::vector<ic::Tau> dummy38;
  ic::SuperCluster dummy39;
  std::vector<ic::SuperCluster> dummy40;
  ic::TriggerObject dummy41;
  std::vector<ic::TriggerObject> dummy42;
  std::vector<ic::Met> dummy43;
  std::vector<unsigned long> dummy44;
  edm::Wrapper<std::vector<unsigned long> > dummy45;
  mithep::TH2DAsymErr dummy46;
  ic::Photon dummy47;
  std::vector<ic::Photon> dummy48;
  ic::SecondaryVertex dummy49;
  std::vector<ic::SecondaryVertex> dummy50;
  edm::Wrapper<reco::SecondaryVertexTagInfoRefVector> dummy51;
  edm::Wrapper<reco::VertexRefVector> dummy52;
  ic::PFCandidate dummy53;
  std::vector<ic::PFCandidate> dummy54;
  edm::Wrapper<reco::GsfTrackRefVector> dummy55;
  edm::Wrapper<reco::PFCandidateRefVector> dummy56;
};
}

