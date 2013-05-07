#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/JetMETModifier.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"

namespace ic {

  JetMETModifier::JetMETModifier(std::string const& name) : ModuleBase(name) {
    is_data_ = true;
    input_label_ = "pfJetsPFlow";
    upordown_ = true; //true is up false is down                                                                                                                        
    dojessyst_ = false;
  }

  JetMETModifier::~JetMETModifier() {
    ;
  }

  int JetMETModifier::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for JetMETModifier" << std::endl;
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
    std::cout<<"Made plot dir"<<std::endl;
    JEScorrfac = dir.make<TH2F>("JEScorrfac","JEScorrfac",1000,0.,1000.,1000,-0.3,0.3);
    JESmetdiff = dir.make<TH1F>("JESmetdiff","JESmetdiff",1000,-10.,10.);
    JESjetphidiff = dir.make<TH1F>("JESjetphidiff","JESjetphidiff",1000,-10.,10.);
    JESjetetadiff = dir.make<TH1F>("JESjetetadiff","JESjetetadiff",1000,-10.,10.);
    JESisordersame = dir.make<TH1F>("JESisordersame","JESisordersame",40,-10.,10.);

    return 0;
  }

  int JetMETModifier::Execute(TreeEvent *event) {
    if(is_data_){
      return 0;
    }
    std::vector<PFJet *> & vec = event->GetPtrVec<PFJet>(input_label_);//get the collection (should be a jet collection)                                                
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

    for (unsigned i = 0; i < vec.size(); ++i) {//loop over the collection                                                                                               
      //Get jet information                                                                                                                                             
      double jetpx = vec[i]->vector().px();
      double jetpy = vec[i]->vector().py();
      double jetpz = vec[i]->vector().pz();
      double jete = vec[i]->vector().E();
      double jetpt = vec[i]->pt();
      double jeteta = vec[i]->eta();
      double jetphi = vec[i]->phi();
      if(jetpt > oldjet1pt){
        oldjet2index=oldjet1index;
        oldjet2pt=oldjet1pt;
        oldjet1index=i;
        oldjet1pt=jetpt;
      }
      else if(jetpt > oldjet2pt) {
        oldjet2index=i;
        oldjet2pt=jetpt;
      }

      //Get JES uncertainty                                                                                                                                             
      total->setJetPt(jetpt);
      total->setJetEta(jeteta);
      double upuncert = total->getUncertainty(true);
      total->setJetPt(jetpt);
      total->setJetEta(jeteta);
      double downuncert = total->getUncertainty(false);

      //std::cout<<uncert<<std::endl;                                                                                                                                   

      if(dojessyst_){//if not central value correct by the JES uncertainty                                                                                              
        if(upordown_==true){//upper uncertainty                                                                                                                         
          JEScorrfac->Fill(jetpt,upuncert); //Fill histogram of uncertainty against pt                                                                                  
          //Correct jet                                                                                                                                                 
          double newjetpx = jetpx*(1+upuncert);
          double newjetpy = jetpy*(1+upuncert);
          double newjetpz = jetpz*(1+upuncert);
          double newjete = jete*(1+upuncert);
	  ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >  newjet(newjetpx,newjetpy,newjetpz,newjete);
          vec[i]->set_vector(ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >(newjet));

          //Check eta doesn't change                                                                                                                                    
          double newjeteta = vec[i]->eta();
          double newjetphi = vec[i]->phi();
          JESjetphidiff->Fill(newjetphi-jetphi);
          JESjetetadiff->Fill(newjeteta-jeteta);

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
          newmetpx = metpx - (jetpx*upuncert);
          newmetpy = metpy - (jetpy*upuncert);

        }
        else if(upordown_==false){//lower uncertainty                                                                                                                   
          JEScorrfac->Fill(jetpt,downuncert); //Fill histogram of uncertainty against pt                                                                                
          //Correct jet                                                                                                                                                 
          double newjetpx = jetpx*(1-downuncert);
          double newjetpy = jetpy*(1-downuncert);
          double newjetpz = jetpz*(1-downuncert);
          double newjete = jete*(1-downuncert);
	  ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > newjet(newjetpx,newjetpy,newjetpz,newjete);
          vec[i]->set_vector(ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >(newjet));

          //check eta doesn't change                                                                                                                                    
          double newjeteta = vec[i]->eta();
          double newjetphi = vec[i]->phi();
          JESjetphidiff->Fill(newjetphi-jetphi);
          JESjetetadiff->Fill(newjeteta-jeteta);

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
          newmetpx = newmetpx + (jetpx*downuncert);
          newmetpy = newmetpy + (jetpy*downuncert);
        }
      }
      else{//Central value                                                                                                                                              
        newjet1index=oldjet1index;
        newjet2index=oldjet2index;
        newjet1pt=oldjet1pt;
        newjet2pt=newjet2pt;
      }
    }

    //Set met in event to corrected met                                                                                                                                 
    newmetet=sqrt(newmetpx*newmetpx+newmetpy*newmetpy);
    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > new_met(newmetpx,newmetpy,0,newmetet);
    met->set_vector(ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >(new_met));

    JESmetdiff->Fill(newmetet-metet);

    //Check if first two jets have changed                                                                                                                              
    if((oldjet1index==-1)||(newjet1index==-1)||(oldjet2index==-1)||(newjet2index==-1)){
      if(vec.size()>1){
        JESisordersame->Fill(-2.);
	std::cout<<"JET SWAP METHOD ERROR"<<std::endl;
      }
    }
    if(oldjet1index==newjet1index){
      if(oldjet2index==newjet2index){
        JESisordersame->Fill(1.);
        return 0;
      }
    }
    if(oldjet1index==newjet2index){
      if(oldjet2index==newjet1index){
        JESisordersame->Fill(-1.);
	std::cout<<"JETS SWAPPED"<<std::endl;
        return 0;
      }
    }
    if(oldjet1index!=newjet2index){
      if(oldjet2index!=newjet1index){
        JESisordersame->Fill(2.);
	std::cout<<"JETS DIFFERENT"<<std::endl;
        return 0;
      }
    }
    return 0;
  }



  int JetMETModifier::PostAnalysis() {
    return 0;
  }

  void JetMETModifier::PrintInfo() {
    ;
  }


}
