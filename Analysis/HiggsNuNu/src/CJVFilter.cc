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
    jetsjetsinput_label_ = "cjvpfJetsPFlow";
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
    
    TFileDirectory const& dir = fs_->mkdir("CJV");
    std::cout<<"Made plot dir"<<std::endl;
    genericplot = dir.make<TH1F>("genericplot","genericplot",100,0.,10.,100,-5.,5.);
    return 0;
  }

  int CJVFilter::Execute(TreeEvent *event) {
    //GET JET COLLECTIONS
    std::vector<PFJet *> & jetvec = event->GetPtrVec<PFJet>(jetsinput_label_);//Main jet collection
    std::vector<CompositeCandidate *> & pairvec = event->GetPtrVec<CompositeCandidate>(pairinput_label_);//Jet Pair collection
    if(pairvec.size()>1){
      std::cout<<"Error: More than one leading pair skipping event"<<std::endl;
      return 1;
    }
    PFJet oneleadingjet = pairvec[0]->first;
    PFJet otherleadingjet = pairvec[0]->second;
    double etahigh=0.;
    double etalow=0.;
    if(oneleadingjet.eta()>otherleadingjet.eta()){
      etahigh=oneleadingjet.eta();
      etalow=otherleadingjet.eta();
    }
    else{
      etahigh=otherleadingjet.eta();
      etalow=oneleadingjet.eta();
    }
    int njetsingap=0;
    for (int i = 0; unsigned(i) < jetvec.size(); ++i) {//loop over the jet collection 
      double jeteta=vec[i]->vector().eta();
      double jetpt=vec[i]->vector().pt();
      if(jetpt>30.&&jeteta>etalow&&jeteta<etahigh){
	njetsingap++;
      }
      
    }
    
    if(njetsingap==0){//set to if no central jets return 0
      return 0;
    }
    else{
      return 1;
  }


  int CJVFilter::PostAnalysis() {
    return 0;
  }

  void CJVFilter::PrintInfo() {
    ;
  }


}
