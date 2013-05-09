#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/JetMETModifier.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include <utility>

namespace ic {

  JetMETModifier::JetMETModifier(std::string const& name) : ModuleBase(name) {
    is_data_ = true;
    input_label_ = "pfJetsPFlow";
    upordown_ = true; //true is up false is down
    dosmear_ = false;
    dojessyst_ = false;
  }

  JetMETModifier::~JetMETModifier() {
    ;
  }

  int JetMETModifier::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for JetMETModifier" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    if(is_data_){
      std::cout<<"Sample is data, no corrections will be made."<<std::endl;
    }
    else{
      std::cout << "Doing Smearing? "<<dosmear_<<std::endl;
      if(!dojessyst_){
	std::cout << "Doing central value"<<upordown_<<std::endl;
      }
      else if(dojessyst_&&upordown_){
	std::cout << "Doing JESUP"<<upordown_<<std::endl;
      }
      else if(dojessyst_&&(!upordown_)){
	std::cout << "Doing JESDOWN"<<upordown_<<std::endl;
      }
    }
    std::cout<<"Getting JES uncertainty parameters from file: "<<jesuncfile_<<std::endl;
    total = new JetCorrectionUncertainty(*(new JetCorrectorParameters(jesuncfile_)));
    
    std::cout<<"Got parameters successfully"<<std::endl;
    TFileDirectory const& dir = fs_->mkdir("JES");
    TFileDirectory const& dir2 = fs_->mkdir("Smear");
    std::cout<<"Made plot dir"<<std::endl;
    JEScorrfac = dir.make<TH2F>("JEScorrfac","JEScorrfac",1000,0.,1000.,1000,-0.3,0.3);
    JESmetdiff = dir.make<TH1F>("JESmetdiff","JESmetdiff",1000,-10.,10.);
    JESjetphidiff = dir.make<TH1F>("JESjetphidiff","JESjetphidiff",1000,-10.,10.);
    JESjetetadiff = dir.make<TH1F>("JESjetetadiff","JESjetetadiff",1000,-10.,10.);
    JESisordersame = dir.make<TH1F>("JESisordersame","JESisordersame",40,-10.,10.);
    Smearptdiff = dir2.make<TH1F>("Smearptdiff","Smearptdiff",200,-10.,10.);
    Smear50miss = dir2.make<TH1F>("Smear50miss","Smear50miss",20,-10.,10.);
    return 0;
  }

  int JetMETModifier::Execute(TreeEvent *event) {
    if(is_data_){
      return 0;
    }
    else{
      std::vector<PFJet *> & vec = event->GetPtrVec<PFJet>(input_label_);//get the collection (should be a jet collection)
      std::vector<Candidate *> & smearvec = event->GetPtrVec<Candidate>(smear_label_);
      std::vector< std::pair<PFJet*, Candidate*> > jet_smeared_jet_pairs = MatchByDR(vec,smearvec,0.001, true, true);
      if(dosmear_){
	if(jet_smeared_jet_pairs.size()!=vec.size()){
	  vec.clear();
	  for(int i = 0;unsigned(i) <jet_smeared_jet_pairs.size();i++){
	    vec.push_back(jet_smeared_jet_pairs[i].first);
	  }
	  //std::cout<<"hmm no match for some of the jets"<<std::endl;
	}
      }
      
      
      Met *met = event->GetPtr<Met>(met_label_);//get the met
      
      //Get MET information and create variables to be updated
      double metpx = met->vector().px();
      double metpy = met->vector().py();
      double metet = met->vector().energy();
      double newmetpx = metpx;
      double newmetpy = metpy;
      double newmetet = metet;
      
      double oldjet1pt = -1.;
      double oldjet2pt = -1.;
      int oldjet1index = -1;
      int oldjet2index = -1;
      double newjet1pt = -1.;
      double newjet2pt = -1.;
      int newjet1index = -1;
      int newjet2index = -1;
      
      for (int i = 0; unsigned(i) < vec.size(); ++i) {//loop over the collection
	//Get jet information
	double oldjetpx = vec[i]->vector().px();
	double oldjetpy = vec[i]->vector().py();
	double oldjetpz = vec[i]->vector().pz();
	double oldjete = vec[i]->vector().E();
	double oldjetpt = vec[i]->pt();
	double oldjeteta = vec[i]->eta();
	double oldjetphi = vec[i]->phi();
	double newjetpx;
	double newjetpy;
	double newjetpz;
	double newjete;
	double newjetpt;
	double newjeteta;
	double newjetphi;

	if(dosmear_){//Smearing section
	int index = -1;
	  //Check for matches between smeared and unsmeared jets
	  for(int j = 0;unsigned(j)<jet_smeared_jet_pairs.size();j++){
	    if(jet_smeared_jet_pairs[j].first->id()==vec[i]->id()){
	      index = j;
	      break;
	    }
	  }
	  if(index == -1){//If no match use unsmeared jet
	    //std::cout<<"No match for jet with pt: "<<oldjetpt<<std::endl;
	    newjetpx = oldjetpx;
	    newjetpy = oldjetpy;
	    newjetpz = oldjetpz;
	    newjete = oldjete;
	    newjetpt = oldjetpt;
	    newjeteta = oldjeteta;
	    newjetphi = oldjetphi;
	  }
	  else{//If match use smeared 4 vector for jet
	    newjetpx = smearvec[index]->vector().px();
	    newjetpy= smearvec[index]->vector().py();
	    newjetpz= smearvec[index]->vector().pz();
	    newjete= smearvec[index]->vector().E();
	    newjetpt = smearvec[index]->pt();
	    newjeteta = smearvec[index]->eta();
	    newjetphi = smearvec[index]->phi();
	  }
	  
	  //Study how many jets above 50GeV have no match
	  if(oldjetpt>50.){
	    if(index==-1){
	      Smear50miss->Fill(-1.);
	      std::cout<<"Old 4 vectors"<<std::endl;
	      for(unsigned l =0;l<vec.size();l++){
		std::cout<<"pt: "<<vec[l]->pt()<<" eta: "<<vec[l]->eta()<<" phi: "<<vec[l]->phi()<<std::endl;            
	      }
	      std::cout<<"Smeared 4 vectors"<<std::endl;
	      for(unsigned l =0;l<smearvec.size();l++){
		std::cout<<"pt: "<<smearvec[l]->pt()<<" eta: "<<smearvec[l]->eta()<<" phi: "<<smearvec[l]->phi()<<std::endl;            
	      }
	    }
	    else Smear50miss->Fill(1.);
	  }
	  Smearptdiff->Fill(newjetpt-oldjetpt);
	}
	
	else{//If not doing smear use normal jet
	  newjetpx = oldjetpx;
	  newjetpy = oldjetpy;
	  newjetpz = oldjetpz;
	  newjete = oldjete;
	  newjetpt = oldjetpt;
	  newjeteta = oldjeteta;
	  newjetphi = oldjetphi;
	}
	
	//Get jet order
	if(newjetpt > oldjet1pt){
	  oldjet2index=oldjet1index;
	  oldjet2pt=oldjet1pt;
	  oldjet1index=i;
	  oldjet1pt=newjetpt;
	}
	else if(newjetpt > oldjet2pt) {
	  oldjet2index=i;
	  oldjet2pt=newjetpt;
	}
	
	if(!dojessyst_){//Central value                                                                                                                                              
	  newjet1index=oldjet1index;
	  newjet2index=oldjet2index;
	  newjet1pt=oldjet1pt;
	  newjet2pt=newjet2pt;
	}
	
	else if(dojessyst_){//if not central value correct by the JES uncertainty
	  //Get JES uncertainty
	  total->setJetPt(newjetpt);
	  total->setJetEta(newjeteta);
	  double uncert = total->getUncertainty(upordown_);
	  JEScorrfac->Fill(newjetpt,uncert); //Fill histogram of uncertainty against pt	
	  float upordownmult;
	  if(upordown_==true) upordownmult=1.; //upper uncertainty
	  else upordownmult=-1.; //lower uncertainty
	  
	  //Correct jet
	  newjetpx = newjetpx*(1+upordownmult*uncert);
	  newjetpy = newjetpy*(1+upordownmult*uncert);
	  newjetpz = newjetpz*(1+upordownmult*uncert);
	  newjete = newjete*(1+upordownmult*uncert);
	  
	  ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >  jetforcomp(newjetpx,newjetpy,newjetpz,newjete);
	  
	  //Check eta doesn't change
	  double newjeteta = jetforcomp.eta();
	  double newjetphi = jetforcomp.phi();
	  JESjetphidiff->Fill(newjetphi-oldjetphi);
	  JESjetetadiff->Fill(newjeteta-oldjeteta);
	
	  //check if order of jets is same
	  double newjetpt=vec[i]->pt();
	  if(newjetpt > newjet1pt){
	    newjet2index=newjet1index;
	    newjet2pt=newjet1pt;
	    newjet1index=i;
	    newjet1pt=newjetpt;
	  }
	  else if(newjetpt > newjet2pt) {
	    newjet2index=i;
            newjet2pt=newjetpt;
	  }
	  
	  //Correct met
	  newmetpx = metpx - (upordownmult*newjetpx*uncert);
	  newmetpy = metpy - (upordownmult*newjetpy*uncert);
	}
	//Set jet in event to corrected jet
	ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >  newjet(newjetpx,newjetpy,newjetpz,newjete);
	vec[i]->set_vector(ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >(newjet));
      }
      
      //Set met in event to corrected met
      newmetet=sqrt(newmetpx*newmetpx+newmetpy*newmetpy);
      ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > new_met(newmetpx,newmetpy,0,newmetet);
      met->set_vector(ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >(new_met));
      
      JESmetdiff->Fill(newmetet-metet);
      
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
