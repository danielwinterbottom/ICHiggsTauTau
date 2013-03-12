#ifndef ICHiggsTauTau_HiggsNuNu_HinvJESUncertainty_h
#define ICHiggsTauTau_HiggsNuNu_HinvJESUncertainty_h

#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include <cmath>
#include <vector>
#include <string>
#include "boost/bind.hpp"

namespace ic {
  
  template <class T,class M>
    class HinvJESUncertainty : public ModuleBase {
  private:
    JetCorrectionUncertainty *total;
    CLASS_MEMBER(HinvJESUncertainty, bool, is_data)
    CLASS_MEMBER(HinvJESUncertainty, bool, upordown)
    CLASS_MEMBER(HinvJESUncertainty, std::string, input_label)
    CLASS_MEMBER(HinvJESUncertainty, std::string, met_label)
      
      

  public:
    HinvJESUncertainty(std::string const& name);
    virtual ~HinvJESUncertainty();
    
    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
  };

  template <class T,class M>
    HinvJESUncertainty<T,M>::HinvJESUncertainty(std::string const& name) : ModuleBase(name) {
    is_data_ = true;
    input_label_ = "pfJetsPFlow";
    upordown_ = true; //true is up false is down                                                                                                                        
  }
    
    template <class T,class M>
      HinvJESUncertainty<T,M>::~HinvJESUncertainty() {
    ;
  }
    
    template <class T,class M>
      int HinvJESUncertainty<T,M>::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for HinvJESUncertainty" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Doing up error? "<<upordown_<<std::endl;

    std::cout<<"Getting JES uncertainty parameters from file."<<std::endl;
    if(is_data_){
      total = new JetCorrectionUncertainty(*(new JetCorrectorParameters("data/jec/Fall12_V7_DATA_Uncertainty_AK5PF.txt")));
    }
    else{
      total = new JetCorrectionUncertainty(*(new JetCorrectorParameters("data/jec/Fall12_V7_MC_Uncertainty_AK5PF.txt")));
    }
    std::cout<<"Got parameters successfully"<<std::endl;
    return 0;
  }

    template <class T,class M>
      int HinvJESUncertainty<T,M>::Execute(TreeEvent *event) {

    std::vector<T *> & vec = event->GetPtrVec<T>(input_label_);
    M *met = event->GetPtr<M>(met_label_);
    double metpt = met->pt();
    double metenergy = met->energy();
    double metphi = met->phi();
    double metpx = metpt*cos(metphi);
    double metpy = metpt*sin(metphi);
    //std::vector<M *> & vecmet = event->GetPtrVec<M>(met_label_);
    for (unsigned i = 0; i < vec.size(); ++i) {
      double jetpt = vec[i]->pt();
      double jeteta = vec[i]->eta();
      double jetphi = vec[i]->phi();
      double jetenergy = vec[i]->energy();
      total->setJetPt(jetpt);
      total->setJetEta(jeteta);
      double uncert = total->getUncertainty(upordown_);
      //std::cout<<uncert<<std::endl;
      if(upordown_==true){//upper uncertainty
        vec[i]->set_pt(jetpt*(1+uncert));
        vec[i]->set_energy(jetenergy*(1+uncert));
	metpx = metpx-((jetpt*uncert)*cos(jetphi));
	metpy = metpy-((jetpt*uncert)*sin(jetphi));
	metenergy = metenergy-jetenergy*uncert;
      }
      else if(upordown_==false){//lower uncertainty
        vec[i]->set_pt(jetpt*(1-uncert));
        vec[i]->set_energy(jetenergy*(1-uncert));
	metpx = metpx+((jetpt*uncert)*cos(jetphi));
	metpy = metpy+((jetpt*uncert)*sin(jetphi));
	metenergy = metenergy+jetenergy*uncert;
      }
    }
    metpt=sqrt(metpx*metpx+metpy*metpy);
    if(metpx>0){
      metphi=atan(metpy/metpx);
    }
    if(metpx<0){
      metphi=(3.14159265358979-atan(metpy/(-1*metpx)));
    }
    met->set_pt(metpt);
    met->set_phi(metphi);
    met->set_energy(metenergy);
    return 0;
  }

    template <class T,class M>
      int HinvJESUncertainty<T,M>::PostAnalysis() {
    return 0;
  }

    template <class T,class M>
      void HinvJESUncertainty<T,M>::PrintInfo() {
    ;
  } 
    
}

#endif
