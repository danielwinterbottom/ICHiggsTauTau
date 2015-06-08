#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvWDecay.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"

namespace ic {

  HinvWDecay::HinvWDecay(std::string const& name,
			 unsigned flavour,
			 bool isEmbedded) : ModuleBase(name) {
    flavour_ = flavour;
    isEmbedded_ = isEmbedded;
    do_wgammafilter_=false;
    do_newstatuscodes_=false;
  }

  HinvWDecay::~HinvWDecay() {
    ;
  }

  int HinvWDecay::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for HinvWDecay" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Flavour: " << flavour_ << std::endl;
    if (isEmbedded_) std::cout << "Using embedded genparticles." << std::endl;
    countStatus3_ = 0;
    countDecay_e_ = 0;
    countDecay_mu_ = 0;
    countRest_ = 0;
    return 0;
  }

  int HinvWDecay::Execute(TreeEvent *event) {

    bool debug = false;

    if(do_newstatuscodes_){
      //!!check all e and mu, if has status 22 W as mother event is W->e/mu, then check all taus, if there's a tau with a status 22 W as mother then check if any of the e or mu have that tau as a mother and if not W->tauh
      //!!EVENTUALLY UPDATE TO TAKE NEW STATUS FLAGS
      std::vector<GenParticle*> & parts = event->GetPtrVec<GenParticle>("genParticles");
      if (isEmbedded_) parts = event->GetPtrVec<GenParticle>("genParticlesEmbedded");

      for (unsigned iGenPart = 0; iGenPart < parts.size(); ++iGenPart) {//Loop over gen particles
	int id = parts[iGenPart]->pdgid();
	if(abs(id) == 24&&parts[iGenPart]->status()==22){//Find status 22 W
	  int isnegative;//Get charge of W
	  if(id<0)isnegative=1;
	  else isnegative=-1;
	  
	  std::vector<GenParticle*> daughters=ExtractDaughters(parts[iGenPart],parts);//Get list of daughters
	  for(unsigned iDaughter=0;iDaughter<daughters.size();iDaughter++){//Loop over daughters
	    if(daughters[iDaughter]->pdgid()==(int)flavour_*isnegative){
	      if(flavour_!=15){
		return 0;
	      }
	    }
	    if(daughters[iDaughter]->pdgid()==15*isnegative){
	      bool notauintaudecay=false;
	      GenParticle* tauptr=daughters[iDaughter];//Make pointer to tau updated for each time tau decays in loop below
	      bool ise=false;
	      bool ismu=false;
	      bool istauh=false;
	      while(!notauintaudecay){//Loop through tau decays until now tau found
		bool founde=false;
		bool foundmu=false;
		bool foundtau=false;
		std::vector<GenParticle*> taudaughters=ExtractDaughters(tauptr,parts);//Get tau daughters
		for(unsigned iTauDaughter=0;iTauDaughter<taudaughters.size();iTauDaughter++){//Loop over tau daughters
		  //Check if any leptons are found
		  if(taudaughters[iTauDaughter]->pdgid()==11*isnegative){
		    founde=true;
		    }
		  if(taudaughters[iTauDaughter]->pdgid()==13*isnegative){
		    foundmu=true;
		  }
		  if(taudaughters[iTauDaughter]->pdgid()==15*isnegative){
		    foundtau=true;
		    tauptr=taudaughters[iTauDaughter];//If find a tau update tauptr for next loop through
		  }
		  if(taudaughters[iTauDaughter]->pdgid()==11*isnegative*-1){
		    founde=true;
		    std::cout<<"Warning! Tau decaying to wrong sign lepton"<<std::endl;
		  }
		  if(taudaughters[iTauDaughter]->pdgid()==13*isnegative*-1){
		    foundmu=true;
		    std::cout<<"Warning! Tau decaying to wrong sign lepton"<<std::endl;
		  }
		  if(taudaughters[iTauDaughter]->pdgid()==15*isnegative*-1){
		    foundtau=true;
		    std::cout<<"Warning! Tau decaying to wrong sign lepton"<<std::endl;
		    } 
		}
		//If the tau didn't decay to another tau (i.e. if decay wasn't just radiation) see what type of decay it was
		if(!foundtau){
		  notauintaudecay=true;
		  if(founde&&!foundmu){
		    ise=true;
		  }
		  if(!founde&&foundmu){
		    ismu=true;
		  }
		  if(!founde&&!foundmu){
		    istauh=true;
		  }
		  if(founde&&foundmu){
		    std::cout<<"Warning: Found tau decaying to e and mu!!"<<std::endl;
		  }
		}
	      }
	      //After the final tau decay has been found check if it was the flavour we're looking for
	      if(flavour_==11&&ise){
		return 0;
	      }
	      if(flavour_==13&&ismu){
		return 0;
	      }
	      if(flavour_==15&&istauh){
		return 0;
	      }
	    }
	    if((daughters[iDaughter]->pdgid()==-11*isnegative)||(daughters[iDaughter]->pdgid()==-13*isnegative)||(daughters[iDaughter]->pdgid()==-15*isnegative)){
	      std::cout<<"Warning! W decaying to wrong sign lepton"<<std::endl;
	    }
	  }
	}
      }
      //If we got through all the gen particles and there was no W->lnu reject the event
      std::cout<<"No WToLNu decay found: Rejecting event"<<std::endl;
      return 1;
    }
    else{
      bool debugele=false;
      bool debugmu=false;
      
      std::vector<GenParticle*> & parts = event->GetPtrVec<GenParticle>("genParticles");
      if (isEmbedded_) parts = event->GetPtrVec<GenParticle>("genParticlesEmbedded");
      
      for (unsigned i = 0; i < parts.size(); ++i) {
	
	if (debug) std::cout << i << " " << parts[i]->status() << " " << parts[i]->pdgid() << std::endl;
	
	if(do_wgammafilter_&&parts[i]->status()==2){
	  unsigned id = abs(parts[i]->pdgid());
	  if(id==22){
	    std::cout<<parts[i]->pt()<<std::endl;
	    if(parts[i]->pt()>10) return 1;
	  }
	}
	
	if ((!isEmbedded_ && (parts[i]->status() != 3&&!(parts[i]->status() >= 21&&parts[i]->status()<=29))) || 
	    (isEmbedded_ && parts[i]->status() != 2)
	    ) continue;
	
	unsigned id = abs(parts[i]->pdgid());
      
	
	////if e or mu found and e or mu channel asked, just pass
	if (id == flavour_ && flavour_ != 15) {
	  if (debug) std::cout << " -- Found status 3 id " << id << ". Keeping event." << std::endl;
	  countStatus3_++;
	  return 0;
	}
	if(debug){
	  if(id==11){
	    //std::cout << "    Found status 3 electron. Would have kept in enu channel." << std::endl;
	    debugele=true;
	  }
	  if(id==13){
	    //std::cout << "    Found status 3 muon. Would have kept in munu channel." << std::endl;
	    debugmu=true;
	  }
	}
	
	//for tau, find decay products
	if (id == 15) {
	  if (flavour_==15) countStatus3_++;
	  
	  //get the specific taus collection with daughters filled
	  std::vector<GenParticle*> & taus = event->GetPtrVec<GenParticle>("genParticlesTaus");
	  if (isEmbedded_) taus = event->GetPtrVec<GenParticle>("genParticlesEmbedded");
	  
	  unsigned counter = 0;
	  bool lDecay = false;
	  for (unsigned j = 0; j < taus.size(); ++j) {
	    if ((!isEmbedded_ && (taus[j]->status() == 3||((taus[j]->status()) >= 21&&(taus[j]->status()<=29)))) || 
		(isEmbedded_ && taus[j]->status() == 2 && fabs(taus[j]->pdgid())==15)
		) {
	      counter++;
	      if (debug) std::cout << " ---- Tau particle " << j << " id " << taus[j]->pdgid() << " status " << taus[j]->status()  << std::endl;
	      continue;
	    }
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
	      if (debug) std::cout << " -- Found tau decaying to an electron." << std::endl;
	      if (flavour_!=11) return 1;
	      else return 0;
	    }
	    if (idDau==13) {
	      lDecay=true;
	      countDecay_mu_++;
	      if (debug) std::cout << " -- Found tau decaying to a muon." << std::endl;
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
	    if (debug) std::cout << " -- Found tau decaying hadronically." << std::endl;
	    if (flavour_ == 15) return 0;
	    else return 1;
	  }
	  //return 0;
	  break;
	  //}
	}//found a tau status 3
	
      }//loop on genparticles
      
      if (debug){
	if(debugele) std::cout << " -- Found an electron, not enu rejecting event." << std::endl;
	else if(debugmu) std::cout << " -- Found a muon, not munu rejecting event." << std::endl;
	else std::cout << " -- Found nothing, rejecting event." << std::endl;
      } 
      
      return 1;
    }  
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
