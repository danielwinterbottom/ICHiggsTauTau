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
      if(do_statusflags_){
	//!!EVENTUALLY UPDATE TO TAKE NEW STATUS FLAGS
	bool iswenu=false;
	bool iswmunu=false;
	bool iswtaunu=false;
	std::vector<GenParticle*> & parts = event->GetPtrVec<GenParticle>("genParticles");
	int npromptele=0;
	int npromptmu=0;
	int ntaudecele=0;
	int ntaudecmu=0;
	int ntaudechad=0;
	std::vector<std::size_t> tauids;
	for (unsigned iGenPart = 0; iGenPart < parts.size(); ++iGenPart) {//Loop over gen particles
	  int id = parts[iGenPart]->pdgid();
	  int status = parts[iGenPart]->status();
	  //std::cout<<"  "<<parts[iGenPart]->id()<<"  "<<id<<"  "<<status<<std::endl;
	  std::vector<bool> flags=parts[iGenPart]->statusFlags();
	  if(flags[IsPrompt]){
	    //find prompt final state ele or mu 
	    if(status==1){
	      if(abs(id)==11)npromptele++;
	      if(abs(id)==13)npromptmu++;
	    }
	    //if tau go through to find what it decayed to
	    if(abs(id)==15){
	      //make sure this tau hasn't been considered in the decay chain of a previous tau
	      bool notconsidered=true;
	      for(unsigned itau=0;itau<tauids.size();itau++){
		if(parts[iGenPart]->id()==tauids[itau])notconsidered=false;
	      }
	      tauids.push_back(parts[iGenPart]->id());
	      if(notconsidered){
		bool notauintaudecay=false;
		GenParticle* tauptr=parts[iGenPart];//Make pointer to tau updated for each time tau decays in loop below
		while(!notauintaudecay){//Loop through tau decays until now tau found
		  bool founde=false;
		  bool foundmu=false;
		  bool foundtau=false;
		  
		  std::vector<GenParticle*> taudaughters=ExtractDaughters(tauptr,parts);//Get tau daughters
		  for(unsigned iTauDaughter=0;iTauDaughter<taudaughters.size();iTauDaughter++){//Loop over tau daughters
		    int daughterid=taudaughters[iTauDaughter]->pdgid();
		    if(abs(daughterid)==11){
		      founde=true;
		    }
		    if(abs(daughterid)==13){
		      foundmu=true;
		    }
		    if(abs(daughterid)==15){
		      foundtau=true;
		      tauids.push_back(taudaughters[iTauDaughter]->id());
		      tauptr=taudaughters[iTauDaughter];//If find a tau update tauptr for next loop through
		    }
		  }
		  //If the tau didn't decay to another tau (i.e. if decay wasn't just radiation) see what type of decay it was
		  if(!foundtau){
		    notauintaudecay=true;
		    if(founde&&!foundmu){
		      ntaudecele++;
		    }
		    if(!founde&&foundmu){
		      ntaudecmu++;
		    }
		    if(!founde&&!foundmu){
		      ntaudechad++;		    
		    }
		  }
		}
	      }
	    }
	  }
	}
	//check what leptons were present in the event, if only one easy to decide
	bool oneleptononly=true;
	if(npromptele>0){
	  if(npromptele==1&&npromptmu==0&&ntaudecele==0&&ntaudecmu==0&&ntaudechad==0){
	    iswenu=true;
	  }
	  else oneleptononly=false;
	}
	else if(npromptmu>0){
	  if(npromptmu==1&&npromptele==0&&ntaudecele==0&&ntaudecmu==0&&ntaudechad==0){
	    iswmunu=true;
	  }
	  else oneleptononly=false;
	}
	else if(ntaudecele>0){
	  if(npromptmu==0&&npromptele==0&&ntaudecele==1&&ntaudecmu==0&&ntaudechad==0){
	    iswenu=true;
	  }
	  else oneleptononly=false;
	}
	else if(ntaudecmu>0){
	  if(npromptmu==0&&npromptele==0&&ntaudecele==0&&ntaudecmu==1&&ntaudechad==0){
	    iswmunu=true;
	  }
	  else oneleptononly=false;
	}
	else if(ntaudechad>0){
	  if(npromptmu==0&&npromptele==0&&ntaudecele==0&&ntaudecmu==0&&ntaudechad==1){
	    iswtaunu=true;
	  }
	  else oneleptononly=false;
	}
	else std::cout<<"No, lepton found"<<std::endl;
	//if more than one lepton pick the flavour with 
	if(!oneleptononly){
	  bool oddele=false;
	  bool oddmu=false;
	  bool oddtau=false;
	  if((npromptele+ntaudecele)%2!=0)oddele=true;
	  if((npromptmu+ntaudecmu)%2!=0)oddmu=true;
	  if((ntaudechad)%2!=0)oddtau=true;
	  if(oddele&&!(oddmu||oddtau)){
	    iswenu=true;
	  }
	  else if(oddmu&&!(oddele||oddtau)){
	    iswmunu=true;
	  }
	  else if(oddtau&&!(oddele||oddmu)){
	    iswtaunu=true;
	  }
	  else{
	    std::cout<<"!!either no odd numbers or more than one flavour with odd numbers"<<std::endl;
	    std::cout<<"    "<<npromptele<<" prompt eles, "<<npromptmu<<" prompt mus, "<<ntaudecele<<" taudec eles, "<<ntaudecmu<<" taudec mus, "<<ntaudechad<<" hadronic taus"<<std::endl;
	  }
	}
	if(flavour_==11){
	  if(iswenu)return 0;
	  else return 1;
	}
	else if(flavour_==13){
	  if(iswmunu)return 0;
	  else return 1;
	}
	else if(flavour_==15){
	  if(iswtaunu)return 0;
	  else return 1;
	}
	else std::cout<<"Flavour "<<flavour_<<" not supported exiting!"<<std::endl;
	return 1;
      }
      else{
      bool iswenu=false;
      bool iswmunu=false;
      bool iswtaunu=false;
      std::vector<GenParticle*> & parts = event->GetPtrVec<GenParticle>("genParticles");
      if (isEmbedded_) parts = event->GetPtrVec<GenParticle>("genParticlesEmbedded");


      for (unsigned iGenPart = 0; iGenPart < parts.size(); ++iGenPart) {//Loop over gen particles
	int id = parts[iGenPart]->pdgid();
	if(abs(id) == 24&&parts[iGenPart]->status()==22){//Find status 22 W
	  int isnegative;//Get charge of W
	  if(id<0)isnegative=1;
	  else isnegative=-1;
	  
	  //get final w
	  bool nowinwdecay=false;
	  std::vector<GenParticle*> daughters=ExtractDaughters(parts[iGenPart],parts);//Get list of daughters
	  while(!nowinwdecay){
	    bool foundw=false;
	    for(unsigned iDaughter=0;iDaughter<daughters.size();iDaughter++){//Loop over daughters
	      if(abs(daughters[iDaughter]->pdgid())==24){
		foundw=true;
		daughters=ExtractDaughters(daughters[iDaughter],parts);//Get list of daughters
		break;
	      }
	    }
	    if(!foundw)nowinwdecay=true;
	  }
	  for(unsigned iDaughter=0;iDaughter<daughters.size();iDaughter++){//Loop over daughters
	    if(daughters[iDaughter]->pdgid()==11*isnegative){
	      iswenu=true;
	      if(flavour_==11)return 0;
	    }
	    if(daughters[iDaughter]->pdgid()==13*isnegative){
	      iswmunu=true;
	      if(flavour_==13)return 0;
	    }
	    if(daughters[iDaughter]->pdgid()==15*isnegative){
	      bool notauintaudecay=false;
	      GenParticle* tauptr=daughters[iDaughter];//Make pointer to tau updated for each time tau decays in loop below
	      while(!notauintaudecay){//Loop through tau decays until now tau found
		bool founde=false;
		bool foundmu=false;
		bool foundtau=false;
		int noppleps=0;
		int nssleps=0;
		
		std::vector<GenParticle*> taudaughters=ExtractDaughters(tauptr,parts);//Get tau daughters
		for(unsigned iTauDaughter=0;iTauDaughter<taudaughters.size();iTauDaughter++){//Loop over tau daughters
		  //std::cout<<iTauDaughter<<" "<<taudaughters[iTauDaughter]->status()<<" "<<taudaughters[iTauDaughter]->pdgid()<<std::endl;
		  //Check if any leptons are found
		  if(taudaughters[iTauDaughter]->pdgid()==11*isnegative){
		    founde=true;
		    countDecay_e_++;
		    nssleps++;
		    }
		  if(taudaughters[iTauDaughter]->pdgid()==13*isnegative){
		    foundmu=true;
		    countDecay_mu_++;
		    nssleps++;
		  }
		  if(taudaughters[iTauDaughter]->pdgid()==15*isnegative){
		    foundtau=true;
		    tauptr=taudaughters[iTauDaughter];//If find a tau update tauptr for next loop through
		    nssleps++;
		  }
		  if(taudaughters[iTauDaughter]->pdgid()==11*isnegative*-1){
		    noppleps++;
		  }
		  if(taudaughters[iTauDaughter]->pdgid()==13*isnegative*-1){
		    noppleps++;
		  }
		  if(taudaughters[iTauDaughter]->pdgid()==15*isnegative*-1){
		    noppleps++;
		  } 
		}
		if(noppleps>nssleps)std::cout<<"Warning: more leptons of opposite sign than same sign as initial tau"<<std::endl;
		//If the tau didn't decay to another tau (i.e. if decay wasn't just radiation) see what type of decay it was
		if(!foundtau){
		  notauintaudecay=true;
		  if(founde&&!foundmu){
		    iswenu=true;
		  }
		  if(!founde&&foundmu){
		    iswmunu=true;
		  }
		  if(!founde&&!foundmu){
		    iswtaunu=true;
		    countRest_++;
		  }
		  if(founde&&foundmu){
		    std::cout<<"Warning: Found tau decaying to e and mu!!"<<std::endl;
		  }
		}
	      }
	      //After the final tau decay has been found check if it was the flavour we're looking for
	      if(flavour_==11&&iswenu){
		return 0;
	      }
	      if(flavour_==13&&iswmunu){
		return 0;
	      }
	      if(flavour_==15&&iswtaunu){
		return 0;
	      }
	    }
	  }
	}
      }
      //If we got through all the gen particles and there was no W->lnu reject the event
      if(!iswenu&&!iswmunu&&!iswtaunu){
	std::cout<<"Warning: Found no WToLNu decay rejecting event"<<std::endl;
	std::cout<<"The following gen particles were present"<<std::endl;
	for (unsigned iGenPart = 0; iGenPart < parts.size(); ++iGenPart) {//Loop over gen particles//!!
	  int id = parts[iGenPart]->pdgid();
	  std::cout<<"  "<<iGenPart<<"  "<<id<<"  "<<parts[iGenPart]->status()<<std::endl;
	  if(abs(id)==24){
	    std::cout<<"    "<<"W daughters id and status are:"<<std::endl;
	    std::vector<GenParticle*> wdaughters=ExtractDaughters(parts[iGenPart],parts);//Get list of daughters
	    for(unsigned iDaughter=0;iDaughter<wdaughters.size();iDaughter++){//Loop over daughters
	      std::cout<<"     "<<wdaughters[iDaughter]->pdgid()<<" "<<wdaughters[iDaughter]->status()<<std::endl;
	    }
	  }
	}
      }
      return 1;
      }
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
