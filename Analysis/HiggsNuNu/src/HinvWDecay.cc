#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvWDecay.h"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"

namespace ic {

  HinvWDecay::HinvWDecay(std::string const& name,
			 unsigned flavour) : ModuleBase(name) {
    flavour_ = flavour;
  }

  HinvWDecay::~HinvWDecay() {
    ;
  }

  int HinvWDecay::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for HinvWDecay" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Flavour: " << flavour_ << std::endl;
    return 0;
  }

  int HinvWDecay::Execute(TreeEvent *event) {

    std::vector<GenParticle*> const& parts = event->GetPtrVec<GenParticle>("genParticles");

    for (unsigned i = 0; i < parts.size(); ++i) {
      if (parts[i]->status() != 3) continue;

      unsigned id = abs(parts[i]->pdgid());

      //if e or mu found and e or mu channel asked, just pass
      if (id == flavour_ && flavour_ != 15) {
	//std::cout << " -- Found status 3 id " << id << ". Keeping event." << std::endl;
	return 0;
      }
      //for tau, find decay products
      if (id == 15) {
	bool lDecay = false;
	//get the specific taus collection with daughters filled
	std::vector<GenParticle*> const& taus = event->GetPtrVec<GenParticle>("genParticlesTaus");
	unsigned counter = 0;
	for (unsigned j = 0; j < taus.size(); ++j) {
	  if (taus[j]->status() == 3) counter++;
	  //std::cout << " ---- Tau particle " << j << " id " << taus[j]->pdgid() << " status " << taus[j]->status()  << std::endl;
	  unsigned idDau = abs(taus[j]->pdgid());
	  if (flavour_ != 15 && idDau == flavour_) {
	    //std::cout << " -- Found a tau decaying to " << idDau << ". Keeping event." << std::endl;
	    return 0;
	  }
	  if (flavour_ == 15) {
	    if (idDau == 11 || idDau == 13) {
	      lDecay = true;
	      break;
	    }
	  }

	}

	//unsigned idTau = abs(taus[j]->pdgid());
	//  if (idTau == 15) {
	//    const std::vector<int> & daughters = taus[j]->daughters();
// 	    std::cout << " -- Found a tau with " << daughters.size() << " daughters! " << std::endl;
// 	    for (unsigned k(0); k<daughters.size(); ++k){
// 	      //reloop to find daughter index
// 	      GenParticle * daughter = 0;
// 	      for (unsigned jj = 0; jj < taus.size(); ++jj) {
// 		if (taus[jj]->index() == daughters[k]){
// 		  daughter = taus[jj];
// 		  break;
// 		}
// 	      }
// 	      //consider only status 2:
	     
// 	      unsigned idDau = abs(daughter->pdgid());
	      
// 	      if (daughter->status() != 2) continue;
	      
// 	    }//loop on daughters
// 	  }
// 	  else {
// 	    std::cout << " -- Found other particle with status 3 in genParticlesTaus collection , id = " << idTau << std::endl;
// 	    throw;
// 	  }
	//}
	if (counter != 1) {
	  std::cout << " -- Found " << counter << " tau status 3 in genParticlesTaus collection !! Expect 1 ..." << std::endl;
	  throw;
	}
	if (flavour_ == 15 && !lDecay) {
	  //std::cout << " -- Found tau decaying hadronically. Keeping event." << std::endl;
	  return 0;
	}
      }//found a tau status 3
       
    }//loop on genparticles

    return 1;
  }

  int HinvWDecay::PostAnalysis() {
    return 0;
  }

  void HinvWDecay::PrintInfo() {
    ;
  }

}//namespace
