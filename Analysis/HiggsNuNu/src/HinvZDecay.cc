#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvZDecay.h"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"

namespace ic {

  HinvZDecay::HinvZDecay(std::string const& name,
			 unsigned flavour,
			 double minMll, 
			 double maxMll) : ModuleBase(name) {
    flavour_ = flavour;
    minMll_ = minMll;
    maxMll_ = maxMll;
    countStatus3_ = 0;
    
  }

  HinvZDecay::~HinvZDecay() {
    ;
  }

  int HinvZDecay::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for HinvZDecay" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Flavour: " << flavour_ << std::endl;
    std::cout << " Mass range: " << minMll_ << "-" << maxMll_ << std::endl;

    countStatus3_ = 0;

    return 0;
  }

  int HinvZDecay::Execute(TreeEvent *event) {

    std::vector<GenParticle*> const& parts = event->GetPtrVec<GenParticle>("genParticles");

    GenParticle* lepplus = 0;
    GenParticle* lepminus = 0;

    for (unsigned i = 0; i < parts.size(); ++i) {
      if (parts[i]->status() != 3) continue;

      int id = parts[i]->pdgid();

      ////if e or mu found and e or mu channel asked, just pass
      if (id == static_cast<int>(flavour_)) {
	//std::cout << " -- Found status 3 id " << id << ". Keeping event." << std::endl;
	countStatus3_++;
	lepminus = parts[i];
      }
      if (id == static_cast<int>(-flavour_)) {
	//std::cout << " -- Found status 3 id " << id << ". Keeping event." << std::endl;
	countStatus3_++;
	lepplus = parts[i];
      }
       
    }//loop on genparticles
    
    if (lepplus && lepminus) {
      double mass = (lepplus->vector()+lepminus->vector()).M();
      //std::cout << " -- Found two leptons with mass = " << mass << std::endl;
      if (mass > minMll_ && mass < maxMll_){
	return 0;
      }
    }
 
    return 1;
  }

  int HinvZDecay::PostAnalysis() {

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PostAnalysis Info for HinvZDecay" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << " -- Number of Z->ll: " << countStatus3_/2. << std::endl;



    return 0;
  }

  void HinvZDecay::PrintInfo() {
    ;
  }

}//namespace
