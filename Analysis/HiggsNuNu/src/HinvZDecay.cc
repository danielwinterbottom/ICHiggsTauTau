#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvZDecay.h"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"

namespace ic {

  HinvZDecay::HinvZDecay(std::string const& name,
 			 unsigned flavour,
 			 double minMll, 
 			 double maxMll) : ModuleBase(name) {
     flavour_ = flavour;
     min_ = minMll;
     max_ = maxMll;
     variable_ = cutVar::mass;
     countStatus3_ = 0;
  }

  HinvZDecay::HinvZDecay(std::string const& name,
			 unsigned flavour,
			 double min, 
			 double max, 
			 cutVar variable,
			 bool is2012) : ModuleBase(name) {
    flavour_ = flavour;
    min_ = min;
    max_ = max;
    countStatus3_ = 0;
    variable_ = variable;
    is2012_ = is2012;
    n0pt_ = 0;
  }

  HinvZDecay::HinvZDecay(std::string const& name,
			 unsigned flavour,
			 double min, 
			 double max, 
			 bool variable,
			 bool is2012) : ModuleBase(name) {
    if (variable) HinvZDecay(name,flavour,min,max,cutVar::pt,is2012);
    else HinvZDecay(name,flavour,min,max,cutVar::mass,is2012);
  }

  HinvZDecay::~HinvZDecay() {
    ;
  }

  int HinvZDecay::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for HinvZDecay" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Flavour: " << flavour_ << std::endl;
    if (variable_==cutVar::mass) std::cout << " Mass range kept: " << min_ << "-" << max_ << std::endl;
    else if (variable_==cutVar::pt) std::cout << " pT range kept: " << min_ << "-" << max_ << std::endl;
    else std::cout << " HT range kept: " << min_ << "-" << max_ << std::endl;
    std::cout << " Do 2012 selection: " << is2012_ << std::endl;
    countStatus3_ = 0;

    return 0;
  }

  int HinvZDecay::Execute(TreeEvent *event) {

    //std::cout << " --New event" << std::endl;

    EventInfo * eventInfo = event->GetPtr<EventInfo>("eventInfo");
    double gen_ht = eventInfo->gen_ht() ;

    std::vector<GenParticle*> const& parts = event->GetPtrVec<GenParticle>("genParticles");

    //if (is2012_){
    GenParticle* lepplus = 0;
    GenParticle* lepminus = 0;
    GenParticle* Z = 0;

    for (unsigned i = 0; i < parts.size(); ++i) {
      int id = parts[i]->pdgid();
      std::vector<bool> flags;
      if (!is2012_) flags = parts[i]->statusFlags();

      /*parts[i]->Print();
      std::cout << " -- Flags: "; 
      for (unsigned j(0);j<flags.size();j++){
	std::cout << flags[j] << " " ;
      }
      std::cout << std::endl;*/

      if (!is2012_ && !(flags[GenStatusBits::IsHardProcess] && 
			flags[GenStatusBits::FromHardProcess] && 
			flags[GenStatusBits::IsFirstCopy] && 
			(id==23 || abs(id)==11 || abs(id)==13 || abs(id) == 15))) continue;

      if (is2012_ && (parts[i]->status() != 3) && 
	  !(parts[i]->status() >= 21&&parts[i]->status() <= 29)) continue;
	
      //get Z if exists
      if (!is2012_){
	if (id==23){
	  Z = parts[i];
	  break;
	}
      }

      ////if e or mu found and e or mu channel asked, just pass
      if ( (is2012_ && id == static_cast<int>(flavour_)) ||
	   (!is2012_ && (id==11 || id==13 || id==15)) ) {
	//std::cout << " -- Found status 3 id " << id << ". Keeping event." << std::endl;
	lepminus = parts[i];
      }
      if ( (is2012_ && id == static_cast<int>(-flavour_)) ||
	   (!is2012_ && (id==-11 || id==-13 || id==-15)) ) {
	//std::cout << " -- Found status 3 id " << id << ". Keeping event." << std::endl;
	lepplus = parts[i];
      }
      
    }//loop on genparticles

    if (!Z && (!lepplus || !lepminus)) {
      std::cout << " -- Warning, found no Z and not 2 leptons to calculate pT !! " << std::endl;
      return 1;
    }

    double mass = -1;
    double pt = -1;
    if (Z) {
      mass = (Z->vector()).M();
      pt = (Z->vector()).pt();
    }
    else if (lepplus && lepminus) {
      mass = (lepplus->vector()+lepminus->vector()).M();
      pt = (lepplus->vector()+lepminus->vector()).pt();
      //std::cout << " -- Found two leptons with mass = " << mass << " pt " << pt << std::endl;
      //Diagnostic plots if you want them
      //std::string modname=this->ModuleName();
      //TFileDirectory const& dir = fs_->mkdir(("Zmass_"+modname).c_str());
      //zmass = dir.make<TH1F>("zmass","zmass",1000,0.,1000.);
    }
    if (pt<=0){
      n0pt_++;
      /*for (unsigned i = 0; i < parts.size(); ++i) {
	std::vector<bool> flags;
	if (!is2012_) flags = parts[i]->statusFlags();
	
	parts[i]->Print();
	std::cout << " -- Flags: "; 
	for (unsigned j(0);j<flags.size();j++){
	  std::cout << flags[j] << " " ;
	}
	std::cout << std::endl;
      }
      exit(1);*/
    }

    if(variable_ == cutVar::mass){
      if (mass >= min_ && mass < max_){
	return 0;
      }
    }
    else if (variable_ == cutVar::pt){
      if (pt < max_) countStatus3_++;
      if (pt >= min_ && pt < max_){
	return 0;
      }
    }
    else if (variable_ == cutVar::ht){
      if (gen_ht >= min_ && gen_ht < max_){
	return 0;
      }
    }

    return 1;
  }

  int HinvZDecay::PostAnalysis() {

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PostAnalysis Info for HinvZDecay" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    if (variable_ == cutVar::pt){
      std::cout << " -- Number of Z->ll pT<" << max_ << ": " << countStatus3_ << std::endl;
      std::cout << " -- Number of Z->ll pT=0 " << n0pt_ << std::endl;
    }

    return 0;
  }

  void HinvZDecay::PrintInfo() {
    ;
  }

}//namespace
