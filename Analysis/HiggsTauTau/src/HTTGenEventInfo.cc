#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTGenEventInfo.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
  
namespace ic {

  HTTGenEventInfo::HTTGenEventInfo(std::string const& name) : ModuleBase(name) {
  }

  HTTGenEventInfo::~HTTGenEventInfo() {
    ;
  }

  int HTTGenEventInfo::PreAnalysis() {
    return 0;
  }

  int HTTGenEventInfo::Execute(TreeEvent *event) {

    std::vector<ic::GenParticle*> lhe_particles = event->GetPtrVec<ic::GenParticle>("lheParticles");
    double n_incoming_gluons=0;
    double n_incoming_quarks=0;
    double n_outgoing_gluons=0;
    double n_outgoing_quarks=0;
    for(unsigned i=0; i<2;++i){
      // first 2 elements are incoming partons - could store the mother id's in future to make sure this works if the lhe_particles are store in a different order!
      if(fabs(lhe_particles[i]->pdgid())==21) n_incoming_gluons++;
      if (fabs(lhe_particles[i]->pdgid()) <=6) n_incoming_quarks++; 
    }
    for(unsigned i=2; i<lhe_particles.size();++i){
      if(fabs(lhe_particles[i]->pdgid())==25) continue;
      if(fabs(lhe_particles[i]->pdgid())==21) n_outgoing_gluons++;
      if (fabs(lhe_particles[i]->pdgid()) <=6) n_outgoing_quarks++;
    }

    event->Add("n_incoming_gluons", n_incoming_gluons);
    event->Add("n_incoming_quarks", n_incoming_quarks);
    event->Add("n_outgoing_gluons", n_outgoing_gluons);
    event->Add("n_outgoing_quarks", n_outgoing_quarks);
    
    return 0;
  }
  int HTTGenEventInfo::PostAnalysis() {

    return 0;
  }

  void HTTGenEventInfo::PrintInfo() {
    ;
  }

}
