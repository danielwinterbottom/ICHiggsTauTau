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
    input_label_ = "pfJetsPFlow";
    jesupordown_ = true; //true is up false is down
    uesupordown_ = true;
    dosmear_=false;
    dojetresmeasurement_ = false;
    dojessyst_ = false;
    dojersyst_ = false;
    douessyst_ = false;
    dodatajessyst_= false;
    dogaus_=false;
    dospring10gaus_=false;
    doetsmear_=false;
    doaltmatch_=false;
    dojerdebug_=false;
    randomno = new TRandom3(randomseed_);
  }

  JetMETModifier::~JetMETModifier() {
    ;
  }

  int JetMETModifier::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for JetMETModifier" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    l1JetPar_data_  = new JetCorrectorParameters(jec_data_files_[0]);
    l2JetPar_data_  = new JetCorrectorParameters(jec_data_files_[1]);
    l3JetPar_data_  = new JetCorrectorParameters(jec_data_files_[2]);
    resJetPar_data_ = new JetCorrectorParameters(jec_data_files_[3]); 
    //  Load the JetCorrectorParameter objects into a vector, IMPORTANT: THE ORDER MATTERS HERE !!!! 
    std::vector<JetCorrectorParameters> vPar_data;
    vPar_data.push_back(*l1JetPar_data_);
    vPar_data.push_back(*l2JetPar_data_);
    vPar_data.push_back(*l3JetPar_data_);
    vPar_data.push_back(*resJetPar_data_);
    jetCorrector_data_ = new FactorizedJetCorrector(vPar_data);

    if(is_data_){
      if (reapplyJEC_) {
	std::cout << "-- Reapplying JEC using corrections from files:" << std::endl;
	for (unsigned i(0); i<jec_data_files_.size();++i){
	  std::cout << jec_data_files_[i] << std::endl;
	}
      }
      else if (!dodatajessyst_) {
	std::cout<<"-- Sample is data, no corrections will be made."<<std::endl;
      }
    }
    else{
      if(dosmear_){
	std::cout << "Doing jet central value smearing. "<<std::endl;
	std::cout<<"Random seed for jet smearing is: "<<randomseed_<<std::endl;
      }
      else if((!dosmear_)&&dojersyst_){
	std::cout << "Error: doing jersyst but not doing smearing won't work!"<<std::endl;
      }
      else{
	std::cout << "Not doing jet central value smearing. "<<std::endl;
      }

      if(dojessyst_&&dojersyst_){
	std::cout << "Warning trying to do JES and JER at the same time."<<std::endl;
      }
      
      if((!dojessyst_)&&(!dojersyst_)){
	std::cout << "Doing central value"<<std::endl;
      }
      else if(dodatajessyst_&&dojessyst_&&jesupordown_){
	std::cout<< "Doing Data JESUP"<<std::endl;
      }
      else if(dodatajessyst_&&dojessyst_&&!jesupordown_){
	std::cout<< "Doing Data JESDOWN"<<std::endl;
      }
      else if(dojessyst_&&jesupordown_){
	std::cout << "Doing JESUP"<<std::endl;
      }
      else if(dojessyst_&&(!jesupordown_)){
	std::cout << "Doing JESDOWN"<<std::endl;
      }
      else if(dojersyst_&&jerbetterorworse_){
	std::cout << "Doing JERBETTER"<<std::endl;
      }
      else if(dojersyst_&&(!jerbetterorworse_)){
	std::cout << "Doing JERWORSE"<<std::endl;
      }
      else if(douessyst_&&uesupordown_){
	std::cout << "Doing UESUP"<<std::endl;
      }
      else if(douessyst_&&(!uesupordown_)){
	std::cout << "Doing UESDOWN"<<std::endl;
      }
    }
    jetCorUnc_ = new JetCorrectionUncertainty(*(new JetCorrectorParameters(jesuncfile_)));
    
    std::cout<<"Got parameters successfully"<<std::endl;
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
    if(is_data_ && !reapplyJEC_ && !dodatajessyst_) return 0;

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

    ROOT::Math::PxPyPzEVector  oldmet = ROOT::Math::PxPyPzEVector(met->vector());
    ROOT::Math::PxPyPzEVector  newmet = oldmet;

    if(is_data_){
      //std::cout << " - MET before: ";
      //met->Print();
      double sumetdiff = 0;
      for (int i = 0; unsigned(i) < jetvec.size(); ++i) {//LOOP OVER JET COLLECTION
	//std::cout << " - Before : Jet " << i ;
	//jetvec[i]->Print();

	//Get jet information
	ROOT::Math::PxPyPzEVector  oldjet = ROOT::Math::PxPyPzEVector(jetvec[i]->vector());
	ROOT::Math::PxPyPzEVector  newjet;

	double oldcor = oldjet.E()/jetvec[i]->uncorrected_energy();
	//remove old correction using corrector
	//double oldcor = jetvec[i]->GetJecFactor("L1FastJet")*
	//jetvec[i]->GetJecFactor("L2Relative")*
	//jetvec[i]->GetJecFactor("L3Absolute")*
	//jetvec[i]->GetJecFactor("L2L3Residual");

	ROOT::Math::PxPyPzEVector rawjet = 1./oldcor*oldjet;
	
	//std::cout << " -- Check of removal of corrections: uncorE = "<< jetvec[i]->uncorrected_energy() << " rawjet E = " << rawjet.E() << std::endl;

	//apply new correction
	jetCorrector_data_->setJetEta(rawjet.eta());
	jetCorrector_data_->setJetPt(rawjet.pt());
	jetCorrector_data_->setJetA(jetvec[i]->jet_area());
	jetCorrector_data_->setRho(eventInfo->jet_rho()); 
      
	//Step5 (Get the correction factor or a vector of the individual correction factors) 
      
	double newcor = jetCorrector_data_->getCorrection();
      //vector<double> factors = JetCorrector_data_->getSubCorrections();
      //IMPORTANT: the getSubCorrections member function returns the vector of the subcorrections UP to the given level. For example in the example above, factors[0] is the L1 correction and factors[3] is the L1+L2+L3+Residual correction. 

	newjet = newcor/oldcor*oldjet;
	sumetdiff += (newcor/oldcor-1)*oldjet.pt();
	//update jet and correspondingly the met.
	jetvec[i]->set_vector(ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >(newjet));

	//std::cout << " - After : Jet " << i ;
	//jetvec[i]->Print();

	double dpx = newjet.px()-oldjet.px();
	double dpy = newjet.py()-oldjet.py();
	newmet.SetPx(newmet.px()-dpx);
	newmet.SetPy(newmet.py()-dpy);
	newmet.SetE(newmet.pt());
      }//end of loop over jets

      met->set_vector(ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >(newmet));
      met->set_sum_et(met->sum_et()+sumetdiff);
      //std::cout << " - MET after: ";
      //met->Print();

      return 0;
    }
    else{
      double wt_ = eventInfo->total_weight();
      //GET MET AND JET COLLECTIONS
      std::vector<GenJet *> & genvec = event->GetPtrVec<GenJet>("genJets");//GenJet collection, note: could make this a parameter but we only have one collection at the moment in the ntuples
      //std::vector<GenParticle*> const& taus = event->GetPtrVec<GenParticle>("genParticlesTaus");//Tau Collection could be used in future but commented out to remove compiler warning


      //GET RUNMETUNCS COLLECTIONS FOR COMPARISON
      std::vector<ic::Candidate *> runmetuncvec;
      std::vector< std::pair<PFJet*, ic::Candidate*> > jet_runmetjet_pairs;
      if(dojerdebug_){
	runmetuncvec = event->GetPtrVec<ic::Candidate>("jetsmearedcentralJets");//Main jet collection
	for (int i = 0; unsigned(i) < runmetuncvec.size(); ++i) {//loop over the runmetjet collection
	  runmetjetpt->Fill(runmetuncvec[i]->vector().pt());
	}
	//MATCH RUNMETUNCS JETS	
	jet_runmetjet_pairs = MatchByDR(jetvec,runmetuncvec,0.5,true,true);
      }
      
      //MATCH GEN JETS
      std::vector< std::pair<PFJet*, GenJet*> > jet_genjet_pairs;

      if(!doaltmatch_)jet_genjet_pairs = MatchByDR(jetvec,genvec,0.5,true,true);//TWIKI METHOD
      else{//RUNMETUNCERTAINTIES METHOD
	std::vector< std::pair<PFJet*,GenJet*> > pairVec = MakePairs(jetvec,genvec);//Make all possible pairs
	std::vector< std::pair<PFJet*,GenJet*> > filteredpairVec;
	for(int g =0;unsigned(g)<pairVec.size();g++){//Filter pairs with too large a dr separation
	  double dr=sqrt( (pairVec[g].first->eta()-pairVec[g].second->eta())*(pairVec[g].first->eta()-pairVec[g].second->eta()) + (pairVec[g].first->phi()-pairVec[g].second->phi())*(pairVec[g].first->phi()-pairVec[g].second->phi()) );
	  if(dr<std::min(0.5,0.3+0.1*exp(-0.05*pairVec[g].second->pt()))) filteredpairVec.push_back(pairVec[g]);
	}
	std::sort(filteredpairVec.begin(), filteredpairVec.end(), DRCompare<PFJet*,GenJet*>);//sort the pairs by dr
	//get the vector of unique pairs choosing the pairs with the smallest dr separation
	std::vector<PFJet*> fVec;
	std::vector<GenJet*> sVec;
	std::pair<PFJet*,GenJet*> aPair;
        BOOST_FOREACH(aPair, pairVec) {
          bool inFVec = std::count(fVec.begin(),fVec.end(),aPair.first);
          bool inSVec = std::count(sVec.begin(),sVec.end(),aPair.second);
          if (!inFVec && !inSVec) {
            jet_genjet_pairs.push_back(aPair);
            fVec.push_back(aPair.first);
            sVec.push_back(aPair.second);
          }
        }
      }

      //Find closest gen jet to each jet
      for (int i = 0; unsigned(i) < jetvec.size(); ++i) {//loop over the jet collection
	double jeteta=jetvec[i]->vector().eta();
	double jetphi=jetvec[i]->vector().phi();
	double mindr=999;
	for(int j = 0;unsigned(j)<genvec.size();j++){
	  double dr = sqrt((jeteta-genvec[j]->vector().eta())*(jeteta-genvec[j]->vector().eta())+(jetphi-genvec[j]->vector().phi())*(jetphi-genvec[j]->vector().phi()));
	  if(dr<mindr){
	    mindr=dr;
	  }
	}
	Smeargenmindr->Fill(mindr);
      }

      //initialise variables for finding highest two pt jets      
      double oldjet1pt = -1.;
      double oldjet2pt = -1.;
      int oldjet1index = -1;
      int oldjet2index = -1;
      double newjet1pt = -1.;
      double newjet2pt = -1.;
      int newjet1index = -1;
      int newjet2index = -1;
      
      for (int i = 0; unsigned(i) < jetvec.size(); ++i) {//LOOP OVER JET COLLECTION
	//Get jet information
	ROOT::Math::PxPyPzEVector  oldjet = ROOT::Math::PxPyPzEVector(jetvec[i]->vector());
	ROOT::Math::PxPyPzEVector  newjet;

	int index = -1;
	//Check for matches between reco and gen jets
	for(int j = 0;unsigned(j)<jet_genjet_pairs.size();j++){
	  if(jet_genjet_pairs[j].first->id()==jetvec[i]->id()){
	    index = j;
	    break;
	  }
	}

	
	//MAKE PLOTS FOR RESOLUTION MEASUREMENT
	if(dojetresmeasurement_){
	if(index!=-1){
	  int ipt=-1;
	  int ieta=-1;
	  double pt=oldjet.pt();
	  double eta=oldjet.eta();

	  //GET PT BIN
	  double binedges[70][2];
	  for(int m=0;m<70;m++){
	    std::string s=pts[m];
	    std::string delimiter = "-";
	    size_t pos = 0;
	    std::string token;
	    pos = s.find(delimiter);
	    token = s.substr(0, pos);
	    if(token=="inf"){
	      token="999999";//SET INF BEHAVIOUR                                                                                                
	    }
	    binedges[m][0]=atof(token.c_str());
	    s.erase(0, pos + delimiter.length());
	    if(s=="inf"){
	      s="999999";//SET INF BIN BEHAVIOUR
	    }
	    binedges[m][1]=atof(s.c_str());
	  }
	  for(int m=0;m<70;m++){
	    if(pt>=binedges[m][0]){
	      if(pt<binedges[m][1]){
		ipt=m;
		break;
	      }
	    }
	  }
	  if(ipt==-1)std::cout<<"problem with jet pt value "<<pt<<std::endl;
	  
// 	  if     (pt<20)           ipt=0;
// 	  else if((20<=pt)&&(pt<40))   ipt=1;
// 	  else if((40<=pt)&&(pt<60))   ipt=2;
// 	  else if((60<=pt)&&(pt<80))   ipt=3;
// 	  else if((80<=pt)&&(pt<100))  ipt=4;
// 	  else if((100<=pt)&&(pt<120)) ipt=5;
// 	  else if((120<=pt)&&(pt<140)) ipt=6;
// 	  else if((140<=pt)&&(pt<160)) ipt=7;
// 	  else if((160<=pt)&&(pt<180)) ipt=8;
// 	  else if((180<=pt)&&(pt<200)) ipt=9;
// 	  else if((200<=pt)&&(pt<250)) ipt=10;
// 	  else if((250<=pt)&&(pt<300)) ipt=11;
// 	  else if(300<=pt)         ipt=12;
// 	  else std::cout<<"problem with jet pt value"<<std::endl;

	  //GET ETA BIN
 	  if     ((2.3<fabs(eta))&&(fabs(eta)<=5))   ieta=4;
 	  else if((1.7<fabs(eta))&&(fabs(eta)<=2.3)) ieta=3;
 	  else if((1.1<fabs(eta))&&(fabs(eta)<=1.7)) ieta=2;
 	  else if((0.5<fabs(eta))&&(fabs(eta)<=1.1)) ieta=1;
 	  else if((0.<fabs(eta))&&(fabs(eta)<=0.5))  ieta=0;
	  else std::cout<<"problem with jet eta value "<<eta<<std::endl;

// 	  if     ((4.5<fabs(eta))&&(fabs(eta)<=9.9)) ieta=9;
// 	  else if((4.0<fabs(eta))&&(fabs(eta)<=4.5)) ieta=8;
// 	  else if((3.5<fabs(eta))&&(fabs(eta)<=4.0)) ieta=7;
// 	  else if((3.0<fabs(eta))&&(fabs(eta)<=3.5)) ieta=6;
// 	  else if((2.5<fabs(eta))&&(fabs(eta)<=3.0)) ieta=5;
// 	  else if((2.0<fabs(eta))&&(fabs(eta)<=2.5)) ieta=4;
// 	  else if((1.5<fabs(eta))&&(fabs(eta)<=2.0)) ieta=3;
// 	  else if((1.0<fabs(eta))&&(fabs(eta)<=1.5)) ieta=2;
// 	  else if((0.5<fabs(eta))&&(fabs(eta)<=1.0)) ieta=1;
// 	  else if((0.0<fabs(eta))&&(fabs(eta)<=0.5)) ieta=0;
	  if(ieta!=-1&&ipt!=-1){
	    recogenjetptratio[ieta][ipt]->Fill(pt/jet_genjet_pairs[index].second->pt(),wt_);
	  }
	
	}
	}
      
	//SMEARING
	if(dosmear_){
	  double JERscalefac=1.;//if no match leave jet alone
	  double JERcencorrfac=1.;
	  if(!dojersyst_){//doing central value
	    if(fabs(oldjet.eta())<0.5)JERcencorrfac=1.052;
	    else if(fabs(oldjet.eta())<1.1)JERcencorrfac=1.057;
	    else if(fabs(oldjet.eta())<1.7)JERcencorrfac=1.096;
	    else if(fabs(oldjet.eta())<2.3)JERcencorrfac=1.134;
	    else if(fabs(oldjet.eta())<5.0)JERcencorrfac=1.288;
	  }
	  else if(dojersyst_&&jerbetterorworse_){//doing JERBETTER
	    if(fabs(oldjet.eta())<0.5)JERcencorrfac=0.990;
	    else if(fabs(oldjet.eta())<1.1)JERcencorrfac=1.001;
	    else if(fabs(oldjet.eta())<1.7)JERcencorrfac=1.032;
	    else if(fabs(oldjet.eta())<2.3)JERcencorrfac=1.042;
	    else if(fabs(oldjet.eta())<5.0)JERcencorrfac=1.089;
	  }
	  else if(dojersyst_&&(!jerbetterorworse_)){//doing JERWORSE
	    if(fabs(oldjet.eta())<0.5)JERcencorrfac=1.115;
	    else if(fabs(oldjet.eta())<1.1)JERcencorrfac=1.114;
	    else if(fabs(oldjet.eta())<1.7)JERcencorrfac=1.161;
	    else if(fabs(oldjet.eta())<2.3)JERcencorrfac=1.228;
	    else if(fabs(oldjet.eta())<5.0)JERcencorrfac=1.488;	      
	  }
	  if(index!=-1){//if gen jet match calculate correction factor for pt
	    if(oldjet.pt()>50.) Smear50miss->Fill(-1.);
	    //calculate 4 vector correction factor
	    if(!doetsmear_){//do twiki smearing
	      double ptcorrected = jet_genjet_pairs[index].second->pt()+JERcencorrfac*(oldjet.pt()-jet_genjet_pairs[index].second->pt());
	      if(ptcorrected<0.)ptcorrected=0.;
	      JERscalefac = ptcorrected/oldjet.pt();
	      Smearjetgenjetptdiff->Fill(oldjet.pt()-jet_genjet_pairs[index].second->pt());
	      Smearjetgenjetptratio->Fill(oldjet.pt()/jet_genjet_pairs[index].second->pt());
	      Smearjetgenjetptratioetabin->Fill(oldjet.pt()/jet_genjet_pairs[index].second->pt(),oldjet.eta());
	    }
	    else{//do runmetuncertainties smearing
	      double et = oldjet.energy()/cosh(oldjet.eta());
	      double etraw = jetvec[i]->uncorrected_energy()/cosh(oldjet.eta());
	      double etgen = jet_genjet_pairs[index].second->energy()/cosh(oldjet.eta());
	      double etcorrected=et*(1+(JERcencorrfac-1)*(et-etgen)/std::max(et,etraw));
	      if(etcorrected<0.)etcorrected=0.;
	      JERscalefac=etcorrected/et;
	      Smearjetgenjetptdiff->Fill(et-etgen);
	      Smearjetgenjetptratio->Fill(et/etgen);
	      Smearjetgenjetptratioetabin->Fill(et/etgen,oldjet.eta());
	    }
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
	  //correct jet 4 vector
	  newjet = oldjet*JERscalefac;
	  	  
	  //correct met
	  newmet.SetPx(newmet.px()-(newjet.px()-oldjet.px()));
	  newmet.SetPy(newmet.py()-(newjet.py()-oldjet.py()));
	  newmet.SetE(sqrt(newmet.px()*newmet.px()+newmet.py()*newmet.py()));  
	}
	else{//If not doing any smearing use normal jet
	  newjet = oldjet;
	}
	Smearptdiff->Fill(newjet.pt()-oldjet.pt());

	//Get runmetuncjet matching icjet
	int runmetindex = -1;
	if(dojerdebug_){
	  //Check for matches between ic and runmetunc jets
	  for(int j = 0;unsigned(j)<jet_runmetjet_pairs.size();j++){
	    if(jet_runmetjet_pairs[j].first->id()==jetvec[i]->id()){
	      runmetindex = j;
	      break;
	    }
	  }
	  icjetpt->Fill(newjet.pt());
	  nojerjetpt->Fill(oldjet.pt());
	  if(runmetindex!=-1){
	    matchedicjetpt->Fill(newjet.pt());
	    matchednojerjetpt->Fill(oldjet.pt());
	    matchedrunmetjetpt->Fill(jet_runmetjet_pairs[runmetindex].second->pt());
	    if(index!=-1){
	      icjetrunmetjetptdiff->Fill(jet_runmetjet_pairs[runmetindex].second->pt()-newjet.pt());
	      icjetrunmetjetptratio->Fill(jet_runmetjet_pairs[runmetindex].second->pt()/newjet.pt());
	      icjetnosmearptratio->Fill(newjet.pt()/oldjet.pt());
	      runmetjetnosmearptratio->Fill(jet_runmetjet_pairs[runmetindex].second->pt()/oldjet.pt());
	      icjetgenjetptratio->Fill(newjet.pt()/jet_genjet_pairs[index].second->pt());
	      runmetjetgenjetptratio->Fill((jet_runmetjet_pairs[runmetindex].second->pt())/jet_genjet_pairs[index].second->pt());
	    }
	  }
	  else{
	    icjetrunmetjetptdiff->Fill(10);
	    icjetrunmetjetptratio->Fill(0);
	  }
	}
	
	//Get initial jet order
	if(newjet.pt() > oldjet1pt){
	  oldjet2index=oldjet1index;
	  oldjet2pt=oldjet1pt;
	  oldjet1index=i;
	  oldjet1pt=newjet.pt();
	}
	else if(newjet.pt() > oldjet2pt) {
	  oldjet2index=i;
	  oldjet2pt=newjet.pt();
	}
	
	//JES SYSTEMATICS
	if(!dojessyst_){//Central value
	  newjet1index=oldjet1index;
	  newjet2index=oldjet2index;
	  newjet1pt=oldjet1pt;
	  newjet2pt=newjet2pt;
	}
	//else if((is_data_&&dodatajessyst_)||(!is_data_&&!dodatajessyst_)){//if not central value correct by the JES uncertainty
	else if(!is_data_){
	  //Get JES uncertainty
	  jetCorUnc_->setJetPt(newjet.pt());
	  // Catch the few events with |Eta| > 5.4 and apply the extremal uncertainty
	  if (newjet.eta() > 5.4 || newjet.eta() < -5.4) {
	    newjet.eta() > 0 ? jetCorUnc_->setJetEta(5.39) : jetCorUnc_->setJetEta(-5.4);
	  }
	  else {
	    jetCorUnc_->setJetEta(newjet.eta());
	  }
	  double uncert = jetCorUnc_->getUncertainty(jesupordown_);
	  JEScorrfac->Fill(newjet.pt(),uncert); //Fill histogram of uncertainty against pt	
	  float jesupordownmult;
	  if(jesupordown_==true) jesupordownmult=1.; //upper uncertainty
	  else jesupordownmult=-1.; //lower uncertainty
	  
	  //Correct jet and met
	  double deltapx = newjet.px()*jesupordownmult*uncert;
	  double deltapy = newjet.py()*jesupordownmult*uncert;
	  newjet=newjet*(1+jesupordownmult*uncert);
	  newmet.SetPx(newmet.px()-deltapx);
	  newmet.SetPy(newmet.py()-deltapy);
	  newmet.SetE(newmet.px()*newmet.px()+newmet.py()*newmet.py());
	  
	  JESjetphidiff->Fill(newjet.phi()-oldjet.phi());
	  JESjetetadiff->Fill(newjet.eta()-oldjet.eta());
	
	  //check if order of jets is same
	  if(newjet.pt() > newjet1pt){
	    newjet2index=newjet1index;
	    newjet2pt=newjet1pt;
	    newjet1index=i;
	    newjet1pt=newjet.pt();
	  }
	  else if(newjet.pt() > newjet2pt) {
	    newjet2index=i;
            newjet2pt=newjet.pt();
	  }  

	}
	//Set jet in event to corrected jet
	jetvec[i]->set_vector(ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >(newjet));
      }//end of loop over jets
      //Get Unclustered MET for UES Systematic
      if (!is_data_ && douessyst_){
	std::string ues_label_;
	uesupordown_ ? ues_label_ = "pfMetUnclusteredEnUp" : ues_label_ = "pfMetUnclusteredEnDown";
	ic::Candidate* uesCorrected = event->GetPtr<ic::Candidate>(ues_label_);
	newmet.SetPx(uesCorrected->pt() * cos(uesCorrected->phi()));
	newmet.SetPy(uesCorrected->pt() * sin(uesCorrected->phi()));
	newmet.SetE(uesCorrected->pt());
	// !! Scale met significance using Ues Up/Down met
	double met_sig = met->et_sig();
	double new_met_sig = met_sig / met->pt() * newmet.Pt();
	met->set_et_sig(new_met_sig);
      }
      met->set_vector(ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >(newmet));
      //Set met in event to corrected met
      JESmetdiff->Fill(newmet.energy()-oldmet.energy());
      
      //Check if first two jets have changed
      if((oldjet1index==-1)||(newjet1index==-1)||(oldjet2index==-1)||(newjet2index==-1)){
	if(jetvec.size()>1){
	  JESisordersame->Fill(-2.);//ERROR there are two or more jets but no second highest pt jet has been found
	}
      }
      if(oldjet1index==newjet1index){
	if(oldjet2index==newjet2index){
	  JESisordersame->Fill(1.);//Jets are the same
	  return 0;
	}
      }
      if(oldjet1index==newjet2index){
	if(oldjet2index==newjet1index){
	  JESisordersame->Fill(-1.);//Jet 1 and 2 have swapped order
	  //std::cout<<"JETS SWAPPED"<<std::endl;
	  return 0;
	}
      }
      if(oldjet1index!=newjet2index){
	if(oldjet2index!=newjet1index){
	  JESisordersame->Fill(2.);//Different jets are the top two after JES correction
	  return 0;
	}
      }
      return 0;
    }
  }


  int JetMETModifier::PostAnalysis() {
    return 0;
  }

  void JetMETModifier::PrintInfo() {
    ;
  }


}
