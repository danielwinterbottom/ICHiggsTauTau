#ifndef ICHiggsTauTau_HiggsNuNu_HinvJESUncertainty_h
#define ICHiggsTauTau_HiggsNuNu_HinvJESUncertainty_h

#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include <cmath>
#include "TH2F.h"
#include <vector>
#include <string>
#include "boost/bind.hpp"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Math/GenVector/VectorUtil.h"
#include "Math/Vector4Dfwd.h"

namespace ic {
  
  template <class T,class M>
    class HinvJESUncertainty : public ModuleBase {
  private:
    JetCorrectionUncertainty *total;
    JetCorrectionUncertainty *total2;
    CLASS_MEMBER(HinvJESUncertainty,fwlite::TFileService*, fs);
    CLASS_MEMBER(HinvJESUncertainty, bool, is_data)
    CLASS_MEMBER(HinvJESUncertainty, bool, dojessyst)
    CLASS_MEMBER(HinvJESUncertainty, bool, upordown)
    CLASS_MEMBER(HinvJESUncertainty, std::string, input_label)
    CLASS_MEMBER(HinvJESUncertainty, std::string, met_label)
    TH2F* JESupcorrfac;
    TH2F* JESdowncorrfac;
    TH2F* JESusedcorrfac;
    TH1F* JESmetdiff;
    TH1F* JESwhichran;
    

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
    dojessyst_ = false;
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
      std::cout << "Doing JES? "<<dojessyst_<<std::endl;
      std::cout << "If doing JES are we doing up error? "<<upordown_<<std::endl;
      
      
      std::cout<<"Getting JES uncertainty parameters from file."<<std::endl;
      //getting the correct uncertainty, note currently module is only being run on mc
      if(is_data_){
	total = new JetCorrectionUncertainty(*(new JetCorrectorParameters("data/jec/Fall12_V7_DATA_Uncertainty_AK5PF.txt")));
      }
      else{
	total = new JetCorrectionUncertainty(*(new JetCorrectorParameters("data/jec/Fall12_V7_MC_Uncertainty_AK5PF.txt")));
      }
      std::cout<<"Got parameters successfully"<<std::endl;
      TFileDirectory const& dir = fs_->mkdir("JES");
      JESupcorrfac = dir.make<TH2F>("JESupcorrfac","JESupcorrfac",1000,0.,1000.,1000,-0.3,0.3);
      JESdowncorrfac = dir.make<TH2F>("JESdowncorrfac","JESdowncorrfac",1000,0.,1000.,1000,-0.3,0.3);
      JESusedcorrfac = dir.make<TH2F>("JESusedcorrfac","JESusedcorrfac",1000,0.,1000.,1000,-0.3,0.3);
      JESmetdiff = dir.make<TH1F>("JESmetdiff","JESmetdiff",1000,-10.,10.);
      JESwhichran = dir.make<TH1F>("JESwhichran","JESwhichran",1000,-10.,10.);
      std::cout<<"Made plot dir"<<std::endl;
      return 0;
    }
    
    template <class T,class M>
      int HinvJESUncertainty<T,M>::Execute(TreeEvent *event) {
      if(is_data_){
	return 0;
      }
      std::vector<T *> & vec = event->GetPtrVec<T>(input_label_);//get the collection (should be a jet collection)
      M *met = event->GetPtr<M>(met_label_);//get the met
      
      //Get MET information and create variables to be updated
      double metpx = met->vector().px();
      double metpy = met->vector().py();
      double metet = met->vector().energy();
      double newmetpx = metpx;
      double newmetpy = metpy;
      double newmetet = metet;
      
      for (unsigned i = 0; i < vec.size(); ++i) {//loop over the collection
	//Get jet information
	double jetpx = vec[i]->vector().px();
	double jetpy = vec[i]->vector().py();
	double jetpz = vec[i]->vector().pz();
	double jete = vec[i]->vector().E();
	double jetpt = vec[i]->pt();
	double jeteta = vec[i]->eta();
	
	//Get JES uncertainty
	total->setJetPt(jetpt);
	total->setJetEta(jeteta);
	double upuncert = total->getUncertainty(true);
	total->setJetPt(jetpt);
	total->setJetEta(jeteta);
	double downuncert = total->getUncertainty(false);
	
	//std::cout<<uncert<<std::endl;
	JESupcorrfac->Fill(jetpt,upuncert); //Fill histogram of uncertainty against pt
	JESdowncorrfac->Fill(jetpt,downuncert); //Fill histogram of uncertainty against pt
	
	if(dojessyst_){//if not central value correct by the JES uncertainty
	  if(upordown_==true){//upper uncertainty
	    JESusedcorrfac->Fill(jetpt,upuncert); //Fill histogram of uncertainty against pt
	    JESwhichran->Fill(1.);
	    //Correct jet
	    double newjetpx = jetpx*(1+upuncert);
	    double newjetpy = jetpy*(1+upuncert);
	    double newjetpz = jetpz*(1+upuncert);
	    double newjete = jete*(1+upuncert);
	    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >  newjet(newjetpx,newjetpy,newjetpz,newjete);
	    vec[i]->set_vector(ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >(newjet));
	    
	    //Correct met
	    newmetpx = metpx - (jetpx*upuncert);
	    newmetpy = metpy - (jetpy*upuncert);
	    
	  }
	  else if(upordown_==false){//lower uncertainty
	    JESusedcorrfac->Fill(jetpt,downuncert); //Fill histogram of uncertainty against pt
	    JESwhichran->Fill(-1.);
	    //Correct jet
	    double newjetpx = jetpx*(1-downuncert);
	    double newjetpy = jetpy*(1-downuncert);
	    double newjetpz = jetpz*(1-downuncert);
	    double newjete = jete*(1-downuncert);
	    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > newjet(newjetpx,newjetpy,newjetpz,newjete);
	    vec[i]->set_vector(ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >(newjet));
	    
	    //Correct met
	    newmetpx = newmetpx + (jetpx*downuncert);
	    newmetpy = newmetpy + (jetpy*downuncert);	    
	  }
	}
	else JESwhichran->Fill(0.);
      }
      newmetet=sqrt(newmetpx*newmetpx+newmetpy*newmetpy);
      JESmetdiff->Fill(newmetet-metet);
      
      ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > new_met(newmetpx,newmetpy,0,newmetet);
      met->set_vector(ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >(new_met));
      
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
