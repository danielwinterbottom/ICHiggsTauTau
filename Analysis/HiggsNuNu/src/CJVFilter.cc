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
    jetsinput_label_ = "cjvpfJetsPFlow";
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
    std::cout << "doing CJV"<<std::endl;
    return 0;
  }

  int CJVFilter::Execute(TreeEvent *event) {
    //GET JET COLLECTIONS
    std::vector<PFJet *> & jetvec = event->GetPtrVec<PFJet>(jetsinput_label_);//Main jet collection
    std::vector<CompositeCandidate *> & pairvec = event->GetPtrVec<CompositeCandidate>(pairinput_label_);//Jet Pair collection
    if(pairvec.size()<1){
      std::cout<<"Error: no leading pair skipping event"<<std::endl;
      return 1;
    }
    if(pairvec.size()>1){
      std::cout<<"Warning: more than one pair"<<std::endl;
    }
    double oneleadingjeteta = pairvec[0]->At(0)->eta();
    double otherleadingjeteta = pairvec[0]->At(1)->eta();
    double etahigh=0.;
    double etalow=0.;
    if(oneleadingjeteta>otherleadingjeteta){
      etahigh=oneleadingjeteta;
      etalow=otherleadingjeteta;
    }
    else{
      etahigh=otherleadingjeteta;
      etalow=oneleadingjeteta;
    }
    int njetsingap=0;
    for (int i = 0; unsigned(i) < jetvec.size(); ++i) {//loop over the jet collection 
      double jeteta=jetvec[i]->vector().eta();
      double jetpt=jetvec[i]->vector().pt();
      if(jetpt>ptcut_&&(jeteta>etalow)&&(jeteta<etahigh)){//if jet is above pt threshold increase jets in gap cound
	njetsingap++;
	return 1;
      }
      
    }
    
    if(njetsingap==0){//if no central jets pass event
      return 0;
    }
    else{//otherwise reject event
      return 1;
    }
  }


  int CJVFilter::PostAnalysis() {
    return 0;
  }

  void CJVFilter::PrintInfo() {
    ;
  }


}
