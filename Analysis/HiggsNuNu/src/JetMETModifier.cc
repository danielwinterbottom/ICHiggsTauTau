#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/JetMETModifier.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include <utility>
#include <algorithm>

namespace ic {

  JetMETModifier::JetMETModifier(std::string const& name) : ModuleBase(name) {
    is_data_ = true;
    input_label_ = "pfJetsPFlow";
    jesupordown_ = true; //true is up false is down
    dosmear_=false;
    dojessyst_ = false;
    dojersyst_ = false;
    dodatajessyst_= false;
    dogaus_=false;
    doetsmear_=false;
    doaltmatch_=false;
    dojerdebug_=false;
  }

  JetMETModifier::~JetMETModifier() {
    ;
  }

  int JetMETModifier::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for JetMETModifier" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    if(is_data_&&!dodatajessyst_){
      std::cout<<"Sample is data, no corrections will be made."<<std::endl;
    }
    else{
      if(dosmear_){
	std::cout << "Doing jet central value smearing. "<<std::endl;
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
    }
    std::cout<<"Getting JES uncertainty parameters from file: "<<jesuncfile_<<std::endl;
    total = new JetCorrectionUncertainty(*(new JetCorrectorParameters(jesuncfile_)));
    
    std::cout<<"Got parameters successfully"<<std::endl;
    TFileDirectory const& dir = fs_->mkdir("JES");
    TFileDirectory const& dir2 = fs_->mkdir("Smear");
    TFileDirectory const& dir3 = fs_->mkdir("RunMetComparison");
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
    icjetpt = dir3.make<TH1F>("icjetpt","icjetpt",10000,0.,1000.);
    runmetjetpt = dir3.make<TH1F>("runmetjetpt","runmetjetpt",10000,0.,1000.);
    nojerjetpt = dir3.make<TH1F>("nojerjetpt","nojerjetpt",10000,0.,1000.);
    matchedicjetpt = dir3.make<TH1F>("matchedicjetpt","matchedicjetpt",10000,0.,1000.);
    matchedrunmetjetpt = dir3.make<TH1F>("matchedrunmetjetpt","matchedrunmetjetpt",10000,0.,1000.);
    matchednojerjetpt = dir3.make<TH1F>("matchednojerjetpt","matchednojerjetpt",10000,0.,1000.);
    return 0;
  }

  int JetMETModifier::Execute(TreeEvent *event) {
    if(is_data_){
      return 0;
    }
    else{
      //GET MET AND JET COLLECTIONS
      std::vector<PFJet *> & vec = event->GetPtrVec<PFJet>(input_label_);//Main jet collection
      std::vector<GenJet *> & genvec = event->GetPtrVec<GenJet>("genJets");//GenJet collection, note: could make this a parameter but we only have one collection at the moment in the ntuples
      Met *met = event->GetPtr<Met>(met_label_);//MET collection
      ROOT::Math::PxPyPzEVector  oldmet = ROOT::Math::PxPyPzEVector(met->vector());
      ROOT::Math::PxPyPzEVector  newmet = oldmet;

      //GET RUNMETUNCS COLLECTIONS FOR COMPARISON
      std::vector<ic::Candidate *> runmetuncvec;
      std::vector< std::pair<PFJet*, ic::Candidate*> > jet_runmetjet_pairs;
      if(dojerdebug_){
	runmetuncvec = event->GetPtrVec<ic::Candidate>("jetsmearedcentralJets");//Main jet collection
	for (int i = 0; unsigned(i) < runmetuncvec.size(); ++i) {//loop over the runmetjet collection
	  runmetjetpt->Fill(runmetuncvec[i]->vector().pt());
	}
	//MATCH RUNMETUNCS JETS	
	jet_runmetjet_pairs = MatchByDR(vec,runmetuncvec,0.5,true,true);
      }
      
      //MATCH GEN JETS
      std::vector< std::pair<PFJet*, GenJet*> > jet_genjet_pairs;

      if(!doaltmatch_)jet_genjet_pairs = MatchByDR(vec,genvec,0.5,true,true);//TWIKI METHOD
      else{//RUNMETUNCERTAINTIES METHOD
	std::vector< std::pair<PFJet*,GenJet*> > pairVec = MakePairs(vec,genvec);//Make all possible pairs
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
      for (int i = 0; unsigned(i) < vec.size(); ++i) {//loop over the jet collection
	double jeteta=vec[i]->vector().eta();
	double jetphi=vec[i]->vector().phi();
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
      
      for (int i = 0; unsigned(i) < vec.size(); ++i) {//LOOP OVER JET COLLECTION
	//Get jet information
	ROOT::Math::PxPyPzEVector  oldjet = ROOT::Math::PxPyPzEVector(vec[i]->vector());
	ROOT::Math::PxPyPzEVector  newjet;

	int index = -1;
	//Check for matches between smeared and unsmeared jets
	for(int j = 0;unsigned(j)<jet_genjet_pairs.size();j++){
	  if(jet_genjet_pairs[j].first->id()==vec[i]->id()){
	    index = j;
	    break;
	  }
	}
	

	//SMEARING
	if(dosmear_){
	  double JERscalefac=1.;//if no match leave jet alone
	  if(index!=-1){//if gen jet match calculate correction factor for pt
	    if(oldjet.pt()>50.) Smear50miss->Fill(-1.);
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
	      double etraw = vec[i]->uncorrected_energy()/cosh(oldjet.eta());
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
	    if(jet_runmetjet_pairs[j].first->id()==vec[i]->id()){
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
	    
	    icjetrunmetjetptdiff->Fill(jet_runmetjet_pairs[runmetindex].second->pt()-newjet.pt());
	    icjetrunmetjetptratio->Fill(jet_runmetjet_pairs[runmetindex].second->pt()/newjet.pt());
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
	  total->setJetPt(newjet.pt());
	  total->setJetEta(newjet.eta());
	  double uncert = total->getUncertainty(jesupordown_);
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
	vec[i]->set_vector(ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >(newjet));
      }//end of loop over jets
      
      //Set met in event to corrected met
      met->set_vector(ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >(newmet));
      
      JESmetdiff->Fill(newmet.energy()-oldmet.energy());
      
      //Check if first two jets have changed
      if((oldjet1index==-1)||(newjet1index==-1)||(oldjet2index==-1)||(newjet2index==-1)){
	if(vec.size()>1){
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
