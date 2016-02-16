#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/JetMETModifier.h"
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

  JetMETModifier::JetMETModifier(std::string const& name) : ModuleBase(name) {
    is_data_ = true;

    syst_ = jetmetSyst::none;

    input_label_ = "pfJetsPFlow";
    dojetresmeasurement_ = false;

    dogaus_=false;
    dospring10gaus_=false;

    randomseed_ = 0;

    reapplyJecData_ = false;
    reapplyJecMC_ = false;
    smear_ = false;
  }

  JetMETModifier::~JetMETModifier() {
    ;
  }

  int JetMETModifier::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for JetMETModifier" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    randomno = new TRandom3(randomseed_);

    std::cout << " --random seed: " << randomno->GetSeed() << std::endl;
    std::cout << " -- Applying jetmetCor: " << std::endl;
    for (unsigned ic(0); ic< corVec_.size(); ++ic){
      //std::cout <<  corVec_[ic] << " ";
      if (corVec_[ic]==jetmetCor::jecData){
	reapplyJecData_ = true;
	std::cout << " -- Reapplying JEC on data using corrections from files:" << std::endl;
	for (unsigned i(0); i<jec_data_files_.size();++i){
	  std::cout << jec_data_files_[i] << std::endl;
	}
      }
      else if (corVec_[ic]==jetmetCor::jecMC){
	reapplyJecMC_ = true;
	std::cout << " -- Reapplying JEC on MC using corrections from files:" << std::endl;
	for (unsigned i(0); i<jec_mc_files_.size();++i){
	  std::cout << jec_mc_files_[i] << std::endl;
	}
      }
      if (corVec_[ic]==jetmetCor::smearMC){
	std::cout << " -- smearing MC jets"  << std::endl;
	smear_ = true;
      }
    }

    std::cout << " -- Applying jetmetSyst: " << syst_ << std::endl;
    

    //to reapply JEC on data
    if (is_data_ && reapplyJecData_){
      if (jec_data_files_.size() != 4) {
	std::cout << " -- Check JEC data filename vec, wrong size:" << jec_data_files_.size() << std::endl;
	return 1;
      }
      l1JetPar_  = new JetCorrectorParameters(jec_data_files_[0]);
      l2JetPar_  = new JetCorrectorParameters(jec_data_files_[1]);
      l3JetPar_  = new JetCorrectorParameters(jec_data_files_[2]);
      resJetPar_ = new JetCorrectorParameters(jec_data_files_[3]); 
    }
    else if (reapplyJecMC_) {
      if (jec_mc_files_.size() != 3){
	std::cout << " -- Check JEC MC filename vec, wrong size: " << jec_mc_files_.size() << std::endl;
	return 1;
      }
      l1JetPar_  = new JetCorrectorParameters(jec_mc_files_[0]);
      l2JetPar_  = new JetCorrectorParameters(jec_mc_files_[1]);
      l3JetPar_  = new JetCorrectorParameters(jec_mc_files_[2]);
    }
    if (reapplyJecData_ || reapplyJecMC_){
      //  Load the JetCorrectorParameter objects into a vector, IMPORTANT: THE ORDER MATTERS HERE !!!! 
      std::vector<JetCorrectorParameters> vPar;
      vPar.push_back(*l1JetPar_);
      vPar.push_back(*l2JetPar_);
      vPar.push_back(*l3JetPar_);
      if (is_data_) vPar.push_back(*resJetPar_);
      jetCorrector_ = new FactorizedJetCorrector(vPar);
    }

    jetCorUnc_ = new JetCorrectionUncertainty(*(new JetCorrectorParameters(jesuncfile_)));
    
    std::cout<<" -- Got parameters successfully"<<std::endl;
    TFileDirectory const& dir = fs_->mkdir("JES");
    TFileDirectory const& dir2 = fs_->mkdir("Smear");
    TFileDirectory const& dir3 = fs_->mkdir("RunMetComparison");
    TFileDirectory const& dir4 = fs_->mkdir("ResMeasurement");
    std::cout<<"Made plot dir"<<std::endl;
    JEScorrfac = dir.make<TH2F>("JEScorrfac","JEScorrfac",1000,0.,1000.,1000,-0.3,0.3);
    JESmetdiff = dir.make<TH1F>("JESmetdiff","JESmetdiff",1000,-10.,10.);
    JESjetphidiff = dir.make<TH1F>("JESjetphidiff","JESjetphidiff",1000,-10.,10.);
    JESjetetadiff = dir.make<TH1F>("JESjetetadiff","JESjetetadiff",1000,-10.,10.);
    JESisordersame = dir.make<TH1F>("JESisordersame","JESisordersame",40,-10.,10.);
    Smearptdiff = dir2.make<TH1F>("Smearptdiff","Smearptdiff",2000,-10.,10.);
    Smear50miss = dir2.make<TH1F>("Smear50miss","Smear50miss",20,-10.,10.);
    Smearjetgenjetptdiff = dir2.make<TH1F>("Smearjetgenjetptdiff","Smearjetgenjetptdiff",600,-30.,30.);
    Smeargenmindr = dir2.make<TH1F>("Smeargenmindr","Smeargenmindr",1000,0.,10.);
    Smearjetgenjetptratio = dir2.make<TH1F>("Smearjetgenjetptratio","Smearjetgenjetptratio",100,0.,10.);
    Smearjetgenjetptratioetabin = dir2.make<TH2F>("Smearjetgenjetptratioetabin","Smearjetgenjetptratioetabin",100,0.,10.,100,-5.,5.);
    
    //Runmetunccomparisons
    icjetrunmetjetptdiff = dir3.make<TH1F>("icjetrunmetjetptdiff","icjetrunmetjetptdiff",600,-30.,30.);
    icjetrunmetjetptratio = dir3.make<TH1F>("icjetrunmetjetptratio","icjetrunmetjetptratio",10100,0.,10.1);
    icjetnosmearptratio = dir3.make<TH1F>("icjetnosmearptratio","icjetnosmearptratio",10100,0.,10.1);
    runmetjetnosmearptratio = dir3.make<TH1F>("runmetjetnosmearptratio","runmetjetnosmearptratio",10100,0.,10.1);
    runmetjetgenjetptratio = dir3.make<TH1F>("runmetjetgenjetptratio","runmetjetgenjetptratio",10100,0.,10.1);
    icjetgenjetptratio = dir3.make<TH1F>("icjetgenjetptratio","icjetgenjetptratio",10100,0.,10.1);
    icjetpt = dir3.make<TH1F>("icjetpt","icjetpt",10000,0.,1000.);
    runmetjetpt = dir3.make<TH1F>("runmetjetpt","runmetjetpt",10000,0.,1000.);
    nojerjetpt = dir3.make<TH1F>("nojerjetpt","nojerjetpt",10000,0.,1000.);
    matchedicjetpt = dir3.make<TH1F>("matchedicjetpt","matchedicjetpt",10000,0.,1000.);
    matchedrunmetjetpt = dir3.make<TH1F>("matchedrunmetjetpt","matchedrunmetjetpt",10000,0.,1000.);
    matchednojerjetpt = dir3.make<TH1F>("matchednojerjetpt","matchednojerjetpt",10000,0.,1000.);

    
    //Jet resolution measurements
    if(dojetresmeasurement_){
    int npts = 40;
    int netas = 5;
    std::string etas[5]={"0p0-0p5","0p5-1p1","1p1-1p7","1p7-2p3","2p3-5p0"};
    //    int netas=10;
    //std::string etas[10]={"0p0-0p5","0p5-1p0","1p0-1p5","1p5-2p0","2p0-2p5","2p5-3p0","3p0-3p5","3p5-4p0","4p0-4p5","4p5-9p9"};
    //std::string pts[13]={"0-20","20-40","40-60","60-80","80-100","100-120","120-140","140-160","160-180","180-200","200-250","250-300","300-inf"};

    //Set pts
    for(int i=0;i<30;i++){
      std::ostringstream convert;
      convert << 2*i;
      std::string binlow=convert.str();
      std::ostringstream convert2;
      convert2 << 2*(i+1);
      std::string binhigh=convert2.str();
      pts[i]=(binlow+"-"+binhigh);
    } 
    for(int i=30;i<37;i++){
      std::ostringstream convert;
      convert << ((i-30)*20)+60;
      std::string binlow=convert.str();
      std::ostringstream convert2;
      convert2 << ((i-29)*20)+60;
      std::string binhigh=convert2.str();
      pts[i]=(binlow+"-"+binhigh);
    }
    pts[37]="200-250";
    pts[38]="250-300";
    pts[39]="300-inf";

    for(int i =0;i<netas;i++){
      for(int j=0;j<npts;j++){
	recogenjetptratio[i][j] = dir4.make<TH1F>(("recogenjetptratio_"+etas[i]+"_"+pts[j]).c_str(),("recogenjetptratio_"+etas[i]+"_"+pts[j]).c_str(),300,0.,3.);
      }
    }
    TFile *resin=new TFile("input/MCres/MCresolutions.root","read");
    for(int i=0;i<netas;i++){
      resin->GetObject(("resforeta"+etas[i]).c_str(),res[i]);
      resin->GetObject(("resfuncforeta"+etas[i]).c_str(),resfunc[i]);
      resin->GetObject(("spring10resforeta"+etas[i]).c_str(),spring10resfunc[i]);
    }
    }
    return 0;
  }

  int JetMETModifier::Execute(TreeEvent *event) {

    if (is_data_ && !reapplyJecData_) return 0;

    std::vector<PFJet *> & jetvec = event->GetPtrVec<PFJet>(input_label_);//Main jet collection
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    Met *met = 0;
    try {
      std::vector<Met*> metCol = event->GetPtrVec<Met>(met_label_);
      met = metCol[0];
    } catch (...){
      //std::cout << " Met vec not found..." << std::endl;
      met = event->GetPtr<Met>(met_label_);
      if (!met) {
	std::cerr << " -- Found no MET " << met_label_ << " in event! Exiting..." << std::endl;
	exit(1);
      }
    }
    //GET MET AND JET COLLECTIONS
    std::vector<GenJet *> genvec;
    std::vector<std::pair<unsigned,bool> >  recotogenmatch;
    
    if (!is_data_){
      genvec = event->GetPtrVec<GenJet>("genJets");
      getGenRecoMatches<PFJet,GenJet>(jetvec,genvec,recotogenmatch,0.4);
    }

    ROOT::Math::PxPyPzEVector  oldmet = ROOT::Math::PxPyPzEVector(met->vector());
    ROOT::Math::PxPyPzEVector  newmet = oldmet;

    double sumetdiff = 0;
    for (unsigned i = 0; i < jetvec.size(); ++i) {//LOOP OVER JET COLLECTION
      //std::cout << " - Before : Jet " << i ;
      //jetvec[i]->Print();
      
      //Get jet information
      ROOT::Math::PxPyPzEVector  oldjet = ROOT::Math::PxPyPzEVector(jetvec[i]->vector());
      ROOT::Math::PxPyPzEVector  newjet;
      ROOT::Math::PxPyPzEVector  prevjet;
      
      double oldcor = oldjet.E()/jetvec[i]->uncorrected_energy();
      //remove old correction using corrector
      //double oldcor = jetvec[i]->GetJecFactor("L1FastJet")*
      //jetvec[i]->GetJecFactor("L2Relative")*
      //jetvec[i]->GetJecFactor("L3Absolute")*
      //jetvec[i]->GetJecFactor("L2L3Residual");
      
      ROOT::Math::PxPyPzEVector rawjet = 1./oldcor*oldjet;
	
      //std::cout << " -- Check of removal of corrections: uncorE = "<< jetvec[i]->uncorrected_energy() << " rawjet E = " << rawjet.E() << std::endl;
      
      //apply new correction
      double newcor = oldcor;
      if ( (is_data_ && reapplyJecData_) || 
	   (!is_data_ && reapplyJecMC_) ) {
	newcor = applyCorrection(rawjet,jetvec[i]->jet_area(),eventInfo->jet_rho());
      }
      prevjet = oldjet;
      newjet = newcor/oldcor*oldjet;

      double smearFact = 1;
      //-1 = jerbetter, 0 = smear, +1 = jerworse
      if (!is_data_){
	int index = recotogenmatch[i].second ? recotogenmatch[i].first : -1;
	GenJet* match = 0;
	if (index != -1) match = genvec[index];
	if (smear_) smearFact = applySmearing(0,match,newjet);
	if (syst_ == jetmetSyst::jerBetter)  smearFact = applySmearing(-1,match,newjet);
	else if (syst_ == jetmetSyst::jerWorse) smearFact = applySmearing(1,match,newjet);
      }

      prevjet = newjet;
      newjet = newjet*smearFact;

      //JES SYSTEMATICS
      double jesVal = 0;
      if (syst_ == jetmetSyst::jesUp || syst_ == jetmetSyst::jesDown) jesVal = applyJESuncertainty(syst_ == jetmetSyst::jesUp?true:false,newjet);
 
      prevjet = newjet;
      newjet = newjet*(1+jesVal);

      JESjetphidiff->Fill(newjet.phi()-prevjet.phi());
      JESjetetadiff->Fill(newjet.eta()-prevjet.eta());

      //update jet and correspondingly the met.
      jetvec[i]->set_vector(ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >(newjet));
      sumetdiff += newjet.pt()-oldjet.pt();
      
      //std::cout << " - After : Jet " << i ;
      //jetvec[i]->Print();
      
      double dpx = newjet.px()-oldjet.px();
      double dpy = newjet.py()-oldjet.py();
      newmet.SetPx(newmet.px()-dpx);
      newmet.SetPy(newmet.py()-dpy);
      newmet.SetE(newmet.pt());
    }//end of loop over jets
    //Get Unclustered MET for UES Systematic
    if (syst_ == jetmetSyst::uesUp || syst_ == jetmetSyst::uesDown){
      if (!run2_) {
	std::string ues_label_ = syst_ == jetmetSyst::uesUp ? "pfMetUnclusteredEnUp" : "pfMetUnclusteredEnDown";
	ic::Candidate* uesCorrected = event->GetPtr<ic::Candidate>(ues_label_);
	applyUESuncertainty(uesCorrected,newmet,sumetdiff);
      }
      else {
	std::string ues_label_ = syst_ == jetmetSyst::uesUp ? "UnclusteredEnUp" : "UnclusteredEnDown";
	const Met::BasicMet & shiftedMet = met->GetShiftedMet(ues_label_);
	const Met::BasicMet & centralMet =  met->GetShiftedMet("NoShift");
	applyUESuncertainty(shiftedMet,centralMet,newmet,sumetdiff);
      }
    }

    met->set_vector(ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >(newmet));
    met->set_sum_et(met->sum_et()+sumetdiff);
    // !! Scale met significance using Ues Up/Down met
    double met_sig = met->et_sig();
    double new_met_sig = met_sig / met->pt() * newmet.Pt();
    met->set_et_sig(new_met_sig);
    //std::cout << " - MET after: ";
    //met->Print();
    JESmetdiff->Fill(newmet.energy()-oldmet.energy());
    
    return 0;
  }

  double JetMETModifier::applyCorrection(const ROOT::Math::PxPyPzEVector & rawjet,
					 const double & jetarea,
					 const double & rho){
    jetCorrector_->setJetEta(rawjet.eta());
    jetCorrector_->setJetPt(rawjet.pt());
    jetCorrector_->setJetA(jetarea);
    jetCorrector_->setRho(rho); 
    
    //Step5 (Get the correction factor or a vector of the individual correction factors) 
    
    return jetCorrector_->getCorrection();
    //vector<double> factors = JetCorrector_->getSubCorrections();
    //IMPORTANT: the getSubCorrections member function returns the vector of the subcorrections UP to the given level. For example in the example above, factors[0] is the L1 correction and factors[3] is the L1+L2+L3+Residual correction. 
  }

  double JetMETModifier::applySmearing(const int error, 
				       const GenJet* match,
				       const ROOT::Math::PxPyPzEVector & oldjet){
    
    double JERscalefac=1.;//if no match leave jet alone
    double JERcencorrfac=getJERcorrfac(fabs(oldjet.eta()),error,run2_);
    if(match!=-0){//if gen jet match calculate correction factor for pt
      if(oldjet.pt()>50.) Smear50miss->Fill(-1.);
      //calculate 4 vector correction factor
      double ptcorrected = match->pt()+JERcencorrfac*(oldjet.pt()-match->pt());
      if(ptcorrected<0.)ptcorrected=0.;
      JERscalefac = ptcorrected/oldjet.pt();
      Smearjetgenjetptdiff->Fill(oldjet.pt()-match->pt());
      Smearjetgenjetptratio->Fill(oldjet.pt()/match->pt());
      Smearjetgenjetptratioetabin->Fill(oldjet.pt()/match->pt(),oldjet.eta());
    }
    else{//Jets with no gen match
      if(oldjet.pt()>50.){
	Smear50miss->Fill(1.);
      }
      if(dogaus_){//Do Gaussian smearing for JERWORSE
	int etabin=-1;
	if(fabs(oldjet.eta())<0.5)etabin=0;
	else if(fabs(oldjet.eta())<1.1)etabin=1;
	else if(fabs(oldjet.eta())<1.7)etabin=2;
	else if(fabs(oldjet.eta())<2.3)etabin=3;
	else if(fabs(oldjet.eta())<5.0)etabin=4;
	//std::cout<<"etabin is: "<<etabin<<std::endl;
	double mcrespt=oldjet.pt();
	if(mcrespt<20.)mcrespt=20.;
	double sigmamc=(resfunc[etabin]->Eval(mcrespt)*oldjet.pt());
	double spring10sigmamc=(spring10resfunc[etabin]->Eval(mcrespt)*oldjet.pt());
	double gauscorr;
	//std::cout<<"Jet pt and eta are: "<<oldjet.pt()<<" "<<oldjet.eta()<<"Sigma MC is: "<<sigmamc<<" "<<spring10sigmamc<<" Gaus corr is: "<<gauscorr<<std::endl;
	if(!dospring10gaus_) gauscorr=randomno->Gaus(0,(sqrt((JERcencorrfac*JERcencorrfac)-1)*sigmamc));
	else gauscorr=randomno->Gaus(0,(sqrt((JERcencorrfac*JERcencorrfac)-1)*spring10sigmamc));
	double ptcorrected=oldjet.pt()+gauscorr;
	JERscalefac=ptcorrected/oldjet.pt();
	
      }
    }
    return JERscalefac;
  
  }

  double JetMETModifier::applyJESuncertainty(const bool doUp,
					     const ROOT::Math::PxPyPzEVector & newjet){
    //Get JES uncertainty
    jetCorUnc_->setJetPt(newjet.pt());
    // Catch the few events with |Eta| > 5.4 and apply the extremal uncertainty
    if (fabs(newjet.eta()) > 5.4) {
      newjet.eta() > 0 ? jetCorUnc_->setJetEta(5.39) : jetCorUnc_->setJetEta(-5.4);
    }
    else {
      jetCorUnc_->setJetEta(newjet.eta());
    }
    double uncert = jetCorUnc_->getUncertainty(doUp);
    JEScorrfac->Fill(newjet.pt(),uncert); //Fill histogram of uncertainty against pt	

    if (doUp) return uncert;
    else return -1.*uncert;
  }



  void JetMETModifier::applyUESuncertainty(ic::Candidate* uesCorrected,
					   ROOT::Math::PxPyPzEVector & newmet,
					   double & sumetdiff){
    
    sumetdiff += uesCorrected->pt()-newmet.pt();
    newmet.SetPx(uesCorrected->pt() * cos(uesCorrected->phi()));
    newmet.SetPy(uesCorrected->pt() * sin(uesCorrected->phi()));
    newmet.SetE(uesCorrected->pt());
  }

  void JetMETModifier::applyUESuncertainty(const Met::BasicMet & shiftedMet,
					   const Met::BasicMet & centralMet,
					   ROOT::Math::PxPyPzEVector & newmet,
					   double & sumetdiff){

    sumetdiff += shiftedMet.pt()-centralMet.pt();
    newmet.SetPx(newmet.px()+shiftedMet.px-centralMet.px);
    newmet.SetPy(newmet.py()+shiftedMet.py-centralMet.py);
    newmet.SetE(newmet.pt());

  }

  double JetMETModifier::getJERcorrfac(const double & abseta,
				       const int error,
				       const bool run2){
    double JERcencorrfac=1;
    if (!run2){
      if (error==0) {
	if(abseta< 0.5)JERcencorrfac=1.052;
	else if(abseta<1.1)JERcencorrfac=1.057;
	else if(abseta<1.7)JERcencorrfac=1.096;
	else if(abseta<2.3)JERcencorrfac=1.134;
	else if(abseta<5.0)JERcencorrfac=1.288;
      }
      else if (error==-1){
	if(abseta<0.5)JERcencorrfac=0.990;
	else if(abseta<1.1)JERcencorrfac=1.001;
	else if(abseta<1.7)JERcencorrfac=1.032;
	else if(abseta<2.3)JERcencorrfac=1.042;
	else if(abseta<5.0)JERcencorrfac=1.089;
      }
      else if (error==1){
	if(abseta<0.5)JERcencorrfac=1.115;
	else if(abseta<1.1)JERcencorrfac=1.114;
	else if(abseta<1.7)JERcencorrfac=1.161;
	else if(abseta<2.3)JERcencorrfac=1.228;
	else if(abseta<5.0)JERcencorrfac=1.488;	      
      }
    }//run1
    else {
      double val[7] = {1.061,1.088,1.106,1.126,1.343,1.303,1.320};
      double err[7] = {0.023,0.029,0.030,0.094,0.123,0.111,0.286}; 
      double etabounds[8] = {0,0.8,1.3,1.9,2.5,3,3.2,5};
      for (unsigned id(0); id<7;++id){
	if (abseta>=etabounds[id] && abseta<etabounds[id+1]) JERcencorrfac=val[id]+error*err[id];
      }

    }
    return JERcencorrfac;
  }


  int JetMETModifier::PostAnalysis() {
    return 0;
  }
  
  void JetMETModifier::PrintInfo() {
    ;
  }
  
  
}
