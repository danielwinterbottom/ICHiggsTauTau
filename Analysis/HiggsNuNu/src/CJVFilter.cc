#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/CJVFilter.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include <utility>
#include <algorithm>

namespace ic {

  CJVFilter::CJVFilter(std::string const& name) : ModuleBase(name) {
    is_data_ = true;
    jetsinput_label_ = "pfJetsPFlow";
    pairinput_label_ = "jjLeadingCandidates";
    ptcut_ = 30.;
  }

  CJVFilter::~CJVFilter() {
    ;
  }

  int CJVFilter::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for CJVFilter" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "doing CJV with pT cut : " << ptcut_ << std::endl;
    return 0;
  }

  int CJVFilter::Execute(TreeEvent *event) {
      

    //GET JET COLLECTIONS
    std::vector<PFJet *> & jetvec = event->GetPtrVec<PFJet>(jetsinput_label_);//CJV jet collection
    std::vector<CompositeCandidate *> & pairvec = event->GetPtrVec<CompositeCandidate>(pairinput_label_);//Jet Pair collection
    if(pairvec.size()<1){
      std::cout<<"Error: no leading pair, doing nothing ... "<<std::endl;
      return 0;
    }
    if(pairvec.size()>1){
      std::cout<<"Warning: more than one pair"<<std::endl;
    }

    Candidate const* jet1 = pairvec[0]->GetCandidate("jet1");
    Candidate const* jet2 = pairvec[0]->GetCandidate("jet2");

    double etahigh = (jet1->eta() > jet2->eta()) ? jet1->eta() : jet2->eta();
    double etalow = (jet1->eta() > jet2->eta()) ? jet2->eta() : jet1->eta();
    
    unsigned njetsingap=0;
    //no need to loop if only two jets in the event....
    if (jetvec.size()>2){
      for (unsigned i = 0; i < jetvec.size(); ++i) {//loop over the jet collection 
	double jeteta=jetvec[i]->eta();
	double jetpt=jetvec[i]->pt();
	if(jetpt>ptcut_&&(jeteta>etalow)&&(jeteta<etahigh)){//if jet is above pt threshold increase jets in gap cound
	  njetsingap++;
	  //return 1;
	}
      }
    }

    //do not filter anymore, just add to the event

    event->Add("nJetsInGap",njetsingap);
    return 0;

    //if(njetsingap==0){//if no central jets pass event
      //return 0;
    //}
    //else{//otherwise reject event
      //return 1;
    //}
  }


  int CJVFilter::PostAnalysis() {
    return 0;
  }

  void CJVFilter::PrintInfo() {
    ;
  }


}
