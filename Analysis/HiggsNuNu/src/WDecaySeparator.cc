#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/WDecaySeparator.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"

namespace ic {

  WDecaySeparator::WDecaySeparator(std::string const& name,
				   unsigned flavour) : ModuleBase(name) {
    flavour_ = flavour;
    do_wgammafilter_=false;
  }

  WDecaySeparator::~WDecaySeparator() {
    ;
  }

  int WDecaySeparator::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for WDecaySeparator" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Flavour: " << flavour_ << std::endl;
    countW_ = 0;
    countPrompt_e_ = 0;
    countPrompt_mu_ = 0;
    countPrompt_tau_ = 0;
    countTau_e_ = 0;
    countTau_mu_ = 0;
    countTau_had_ = 0;
    return 0;
  }

  int WDecaySeparator::Execute(TreeEvent *event) {

    unsigned evtNum = event->GetPtr<EventInfo>("eventInfo")->event();
    bool debug = true;//evtNum==71767501;

    bool iswenu=false;
    bool iswmunu=false;
    //bool iswtaunu=false;
    unsigned id_W = 0;
    std::vector<GenParticle*> & parts = event->GetPtrVec<GenParticle>("genParticles");

    //if (debug) std::cout << " -- Event " << event->GetPtr<EventInfo>("eventInfo")->event() << std::endl;
    for (unsigned iGenPart = 0; iGenPart < parts.size(); ++iGenPart) {//Loop over gen particles
      int id = parts[iGenPart]->pdgid();
      std::vector<bool> flags=parts[iGenPart]->statusFlags();
      if (flags[GenStatusBits::IsHardProcess] && 
	  (abs(id)==11 || abs(id)==13 || abs(id)==15)
	  ) {
	id_W=abs(id);
	/*if (debug) {
	  std::cout << " Found W candidate: ";
	  parts[iGenPart]->Print();
	  for (unsigned iflag(0); iflag<flags.size();++iflag){
	    std::cout << iflag << ":" << flags[iflag] << " ";
	  }
	  std::cout << std::endl;
	  }*/
	countW_++;
	//continue;
      }
      if (parts[iGenPart]->status()!=1) continue;
      //do ele
      if (abs(id)==11 && !iswenu && (id_W==11 || id_W==15)) {
	iswenu = processParticle(flags,countPrompt_e_,countTau_e_,countPrompt_tau_);
	/*if (iswenu && debug) {
	  std::cout << " Found W->e candidate: ";
	  parts[iGenPart]->Print();
	  for (unsigned iflag(0); iflag<flags.size();++iflag){
	    std::cout << iflag << ":" << flags[iflag] << " ";
	  }
	  std::cout << std::endl;
	  }*/
      }
      else if (abs(id)==13 && !iswmunu && (id_W==13 || id_W==15)) {
	iswmunu = processParticle(flags,countPrompt_mu_,countTau_mu_,countPrompt_tau_);
	/*if (iswmunu && debug) {
	  std::cout << " Found W->mu candidate: ";
	  parts[iGenPart]->Print();
	  for (unsigned iflag(0); iflag<flags.size();++iflag){
	    std::cout << iflag << ":" << flags[iflag] << " ";
	  }
	  std::cout << std::endl;
	  }*/
      }

      if (debug && iswenu && iswmunu){
	std::cout << " ---- Error, event " << evtNum << ", containing " << parts.size() << " genparticles, found both e and mu decays!"
		  << std::endl;
	for (unsigned jGenPart = 0; jGenPart < parts.size(); ++jGenPart) {//Loop over gen particles
	  parts[jGenPart]->Print();
	  std::vector<bool> flagvec=parts[jGenPart]->statusFlags();
	  for (unsigned iflag(0); iflag<flagvec.size();++iflag){
	    std::cout << iflag << ":" << flagvec[iflag] << " ";
	  }
	  std::cout << std::endl;
	}
	//exit(1);
      }

      if (iswenu){
	if (flavour_==11) return 0;
	else return 1;
      }
      if (iswmunu){
	if (flavour_==13) return 0;
	else return 1;
      }

    }//Loop over gen particles
    
    if (!iswenu && !iswmunu) {
      //iswtaunu=true;
      countPrompt_tau_++;
      countTau_had_++;
      if (flavour_==15) return 0;
      else return 1;
    }


    //If we got through all the gen particles and there was no W->lnu reject the event
    /*if(!iswenu&&!iswmunu&&!iswtaunu){
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
      }*/
    return 1;
    
    //if(do_wgammafilter_&&parts[i]->status()==2){
    //unsigned id = abs(parts[i]->pdgid());
    //if(id==22){
    //	std::cout<<parts[i]->pt()<<std::endl;
    //	if(parts[i]->pt()>10) return 1;
    //}
    //}

  }//execute method
    
  bool WDecaySeparator::processParticle(const std::vector<bool> & flags,
					unsigned & countPrompt,
					unsigned & countTau,
					unsigned & countPromptTau){

    if (flags[GenStatusBits::IsPrompt] && flags[GenStatusBits::FromHardProcess]) {
      countPrompt++;
      return true;
    }
    else if (flags[GenStatusBits::IsDirectPromptTauDecayProduct]) {
      countTau++;
      countPromptTau++;
      return true;
    }
    return false;
    
  }



  int WDecaySeparator::PostAnalysis() {

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PostAnalysis Info for WDecaySeparator" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << " -- Number of W : " << countW_ << std::endl
      	      << " -- W->e decay : " << countPrompt_e_ << std::endl
	      << " -- W->mu decay : " << countPrompt_mu_ << std::endl
	      << " -- W->tau decay : " << countPrompt_tau_ << std::endl
	      << " -- W->lep : " << countPrompt_e_+countPrompt_mu_+countPrompt_tau_ << std::endl
	      << " -- W->tau->e decay : " << countTau_e_ << std::endl
	      << " -- W->tau->mu decay : " << countTau_mu_ << std::endl
	      << " -- W->tau->had decay : " << countTau_had_ << std::endl
	      << " -- W->tau check: " << countTau_e_+countTau_mu_+countTau_had_ << std::endl;

    return 0;
  }

  void WDecaySeparator::PrintInfo() {
    ;
  }

}//namespace
