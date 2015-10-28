#ifdef __CINT__
#include <utility>
#include <string>

#pragma link off all globals;
#pragma link off all classes;
// #pragma link C++ all typedef;
#pragma link off all functions;
// #pragma link off nestedtypedef;
#pragma link C++ nestedclasses;

#pragma link C++ class std::pair<std::string, bool>+;
#pragma link C++ class std::pair<unsigned long, float>+;

#pragma link C++ class std::pair<std::string,ic::Candidate>+;
#pragma link C++ class std::vector<std::pair<std::string,ic::Candidate> >+;

#pragma link C++ class ic::Candidate+;
#pragma link C++ class std::vector<ic::Candidate>+;

#pragma link C++ class ic::PFCandidate+;
#pragma link C++ class std::vector<ic::PFCandidate>+;

#pragma link C++ class ic::Electron+;
#pragma link C++ class std::vector<ic::Electron>+;

#pragma link C++ class ic::Muon+;
#pragma link C++ class std::vector<ic::Muon>+;

#pragma link C++ class ic::Tau+;
#pragma link C++ class std::vector<ic::Tau>+;

#pragma link C++ class ic::Photon+;
#pragma link C++ class std::vector<ic::Photon>+;

#pragma link C++ class ic::Jet+;
#pragma link C++ class std::vector<ic::Jet>+;

#pragma link C++ class ic::CaloJet+;
#pragma link C++ class std::vector<ic::CaloJet>+;

#pragma link C++ class ic::JPTJet+;
#pragma link C++ class std::vector<ic::JPTJet>+;

#pragma link C++ class ic::PFJet+;
#pragma link C++ class std::vector<ic::PFJet>+;

#pragma link C++ class ic::Met+;
#pragma link C++ class ic::Met::BasicMet+;
#pragma link C++ class std::vector<ic::Met>+;
#pragma link C++ class std::pair<unsigned long, ic::Met::BasicMet>+;

#pragma link C++ class ic::SuperCluster+;
#pragma link C++ class std::vector<ic::SuperCluster>+;

#pragma link C++ class ic::Track+;
#pragma link C++ class std::vector<ic::Track>+;

#pragma link C++ class ic::LightTrack+;
#pragma link C++ class std::vector<ic::LightTrack>+;

#pragma link C++ class ic::Vertex+;
#pragma link C++ class std::vector<ic::Vertex>+;

#pragma link C++ class ic::SecondaryVertex+;
#pragma link C++ class std::vector<ic::SecondaryVertex>+;

#pragma link C++ class ic::TriggerObject+;
#pragma link C++ class std::vector<ic::TriggerObject>+;

#pragma link C++ class ic::TriggerPath+;
#pragma link C++ class std::vector<ic::TriggerPath>+;

#pragma link C++ class ic::PileupInfo+;
#pragma link C++ class std::vector<ic::PileupInfo>+;

#pragma link C++ class ic::GenParticle+;
#pragma link C++ class std::vector<ic::GenParticle>+;

#pragma link C++ class ic::GenJet+;
#pragma link C++ class std::vector<ic::GenJet>+;

#pragma link C++ class ic::EventInfo+;

#pragma link C++ class mithep::TH2DAsymErr+;
#endif
