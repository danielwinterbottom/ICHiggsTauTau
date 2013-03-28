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

    countStatus3_ = 0;
    countDecay_e_ = 0;
    countDecay_mu_ = 0;
    countRest_ = 0;

    return 0;
  }

  int HinvWDecay::Execute(TreeEvent *event) {

    std::vector<GenParticle*> const& parts = event->GetPtrVec<GenParticle>("genParticles");

    for (unsigned i = 0; i < parts.size(); ++i) {
      if (parts[i]->status() != 3) continue;

      unsigned id = abs(parts[i]->pdgid());

      ////if e or mu found and e or mu channel asked, just pass
      if (id == flavour_ && flavour_ != 15) {
	////std::cout << " -- Found status 3 id " << id << ". Keeping event." << std::endl;
	countStatus3_++;
	return 0;
      }

      //for tau, find decay products
      if (id == 15) {
	if (flavour_==15) countStatus3_++;
	
	//get the specific taus collection with daughters filled
	std::vector<GenParticle*> const& taus = event->GetPtrVec<GenParticle>("genParticlesTaus");
	unsigned counter = 0;
	bool lDecay = false;
	for (unsigned j = 0; j < taus.size(); ++j) {
	  if (taus[j]->status() == 3) counter++;
	  //std::cout << " ---- Tau particle " << j << " id " << taus[j]->pdgid() << " status " << taus[j]->status()  << std::endl;
	  unsigned idDau = abs(taus[j]->pdgid());


	  //if (flavour_ != 15 && idDau == flavour_) {
	  ////std::cout << " -- Found a tau decaying to " << idDau << ". Keeping event." << std::endl;
	    //if (idDau==11) countDecay_e_++;
	    //if (idDau==13) countDecay_mu_++;
	    //return 0;
	  //}
	  //if (flavour_ == 15) {
	  //if (idDau == 11 || idDau == 13) {
	  if (idDau==11) {
	    lDecay=true;
	    countDecay_e_++;
	    if (flavour_!=11) return 1;
	    else return 0;
	  }
	  if (idDau==13) {
	    lDecay=true;
	    countDecay_mu_++;
	    if (flavour_!=13) return 1;
	    else return 0;
	  }
	    //return 1;
	    //}
	  //}

	}//loop on tau particles

	if (counter != 1) {
	  std::cout << " -- Found " << counter << " tau status 3 in genParticlesTaus collection !! Expect 1 ..." << std::endl;
	  throw;
	}
	//if (flavour_ == 15){// && !lDecay) {
	  //std::cout << " -- Found tau decaying hadronically. Keeping event." << std::endl;
	if (!lDecay) {
	  countRest_++;
	  if (flavour_ == 15) return 0;
	  else return 1;
	}
	  //return 0;
	break;
	  //}
      }//found a tau status 3
       
    }//loop on genparticles

    return 1;
  }

  int HinvWDecay::PostAnalysis() {

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PostAnalysis Info for HinvWDecay" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << " -- lep from status 3 : " << countStatus3_ << std::endl
      	      << " -- tau->e decay : " << countDecay_e_ << std::endl
	      << " -- tau->mu decay : " << countDecay_mu_ << std::endl
	      << " -- tau->had decay : " << countRest_ << std::endl
	      << " -- Sum for all tau = " << countRest_+countDecay_e_+countDecay_mu_  << std::endl
	      << " -- Sum for e = " << countStatus3_+countDecay_e_ << std::endl
	      << " -- Sum for mu = " << countStatus3_+countDecay_mu_ << std::endl;



    return 0;
  }

  void HinvWDecay::PrintInfo() {
    ;
  }

}//namespace
