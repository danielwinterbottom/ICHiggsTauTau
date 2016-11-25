#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/JECStudy.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include <utility>
#include <algorithm>
#include "TRandom3.h"
#include <sstream>
#include <string>
#include <iostream>

namespace ic {

  JECStudy::JECStudy(std::string const& name) : ModuleBase(name) {
    is_data_ = true;
    reapplyJEC_ = true;
    input_label_ = "pfJetsPFlow";
  }

  JECStudy::~JECStudy() {
    ;
  }

  int JECStudy::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for JECStudy" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    l1JetPar_old_  = new JetCorrectorParameters(jec_old_files_[0]);
    l2JetPar_old_  = new JetCorrectorParameters(jec_old_files_[1]);
    l3JetPar_old_  = new JetCorrectorParameters(jec_old_files_[2]);
    resJetPar_old_ = new JetCorrectorParameters(jec_old_files_[3]); 
    //  Load the JetCorrectorParameter objects into a vector, IMPORTANT: THE ORDER MATTERS HERE !!!! 
    std::vector<JetCorrectorParameters> vPar_old;
    vPar_old.push_back(*l1JetPar_old_);
    vPar_old.push_back(*l2JetPar_old_);
    vPar_old.push_back(*l3JetPar_old_);
    vPar_old.push_back(*resJetPar_old_);
    jetCorrector_old_ = new FactorizedJetCorrector(vPar_old);

    l1JetPar_new_  = new JetCorrectorParameters(jec_new_files_[0]);
    l2JetPar_new_  = new JetCorrectorParameters(jec_new_files_[1]);
    l3JetPar_new_  = new JetCorrectorParameters(jec_new_files_[2]);
    resJetPar_new_ = new JetCorrectorParameters(jec_new_files_[3]); 
    //  Load the JetCorrectorParameter objects into a vector, IMPORTANT: THE ORDER MATTERS HERE !!!! 
    std::vector<JetCorrectorParameters> vPar_new;
    vPar_new.push_back(*l1JetPar_new_);
    vPar_new.push_back(*l2JetPar_new_);
    vPar_new.push_back(*l3JetPar_new_);
    vPar_new.push_back(*resJetPar_new_);
    jetCorrector_new_ = new FactorizedJetCorrector(vPar_new);

    if(is_data_){
      if (reapplyJEC_) {
	std::cout << "-- Reapplying JEC using corrections from old files:" << std::endl;
	for (unsigned i(0); i<jec_old_files_.size();++i){
	  std::cout << jec_old_files_[i] << std::endl;
	}

	std::cout << "-- Reapplying JEC using corrections from new files:" << std::endl;
	for (unsigned i(0); i<jec_new_files_.size();++i){
	  std::cout << jec_new_files_[i] << std::endl;
	}
      }
    }

    TFileDirectory const& dir = fs_->mkdir("JES");
    dpTvspTeta = dir.make<TH2F>("dpTvspTeta",";p_{T} (GeV);#eta;#Delta p_{T}",100,0,100,100,-5,5);

    return 0;
  }

  int JECStudy::Execute(TreeEvent *event) {
    if(is_data_ && !reapplyJEC_) return 0;

    std::vector<PFJet *> & jetvec = event->GetPtrVec<PFJet>(input_label_);//Main jet collection
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");

    if(is_data_){
      //std::cout << " - MET before: ";
      //met->Print();
      for (int i = 0; unsigned(i) < jetvec.size(); ++i) {//LOOP OVER JET COLLECTION
	//std::cout << " - Before : Jet " << i ;
	//jetvec[i]->Print();

	//Get jet information
	ROOT::Math::PxPyPzEVector  oldjet = ROOT::Math::PxPyPzEVector(jetvec[i]->vector());

	double oldcor = oldjet.E()/jetvec[i]->uncorrected_energy();
	//remove old correction using corrector
	//double oldcor = jetvec[i]->GetJecFactor("L1FastJet")*
	//jetvec[i]->GetJecFactor("L2Relative")*
	//jetvec[i]->GetJecFactor("L3Absolute")*
	//jetvec[i]->GetJecFactor("L2L3Residual");

	ROOT::Math::PxPyPzEVector rawjet = 1./oldcor*oldjet;
	ROOT::Math::PxPyPzEVector newjet_old;
	ROOT::Math::PxPyPzEVector newjet_new;
	//std::cout << " -- Check of removal of corrections: uncorE = "<< jetvec[i]->uncorrected_energy() << " rawjet E = " << rawjet.E() << std::endl;

	//apply new correction
	jetCorrector_old_->setJetEta(rawjet.eta());
	jetCorrector_old_->setJetPt(rawjet.pt());
	jetCorrector_old_->setJetA(jetvec[i]->jet_area());
	jetCorrector_old_->setRho(eventInfo->jet_rho()); 
      
	jetCorrector_new_->setJetEta(rawjet.eta());
	jetCorrector_new_->setJetPt(rawjet.pt());
	jetCorrector_new_->setJetA(jetvec[i]->jet_area());
	jetCorrector_new_->setRho(eventInfo->jet_rho()); 
	//Step5 (Get the correction factor or a vector of the individual correction factors) 
      
	double cor_old = jetCorrector_old_->getCorrection();
	double cor_new = jetCorrector_new_->getCorrection();
      //vector<double> factors = JetCorrector_old_->getSubCorrections();
      //IMPORTANT: the getSubCorrections member function returns the vector of the subcorrections UP to the given level. For example in the example above, factors[0] is the L1 correction and factors[3] is the L1+L2+L3+Residual correction. 
	newjet_old = cor_old/oldcor*oldjet;
	newjet_new = cor_new/oldcor*oldjet;

	double dpt = (newjet_new.pt()-newjet_old.pt())/rawjet.pt();
	dpTvspTeta->Fill(rawjet.pt(),rawjet.eta(),dpt);
      }//end of loop over jets

      return 0;
    }
    return 0;
  }


  int JECStudy::PostAnalysis() {
    return 0;
  }

  void JECStudy::PrintInfo() {
    ;
  }


}
