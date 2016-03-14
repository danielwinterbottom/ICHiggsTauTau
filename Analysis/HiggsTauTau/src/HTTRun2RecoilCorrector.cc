#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTRun2RecoilCorrector.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "boost/format.hpp"
#include "TMVA/Reader.h"
#include "TVector3.h"

namespace ic {

  HTTRun2RecoilCorrector::HTTRun2RecoilCorrector(std::string const& name) : ModuleBase(name), 
    channel_(channel::et),
    strategy_(strategy::paper2013),
    mc_(mc::summer12_53X),
    era_(era::data_2012_rereco) {
    met_label_ = "pfMVAMet";
    jets_label_ = "pfJetsPFlow";
    sample_ = "";
    disable = true;
    is_ztt = false;
    is_htt = false;
    is_wjets = false;
  }

  HTTRun2RecoilCorrector::~HTTRun2RecoilCorrector() {
    ;
  }

  int HTTRun2RecoilCorrector::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HTTRun2RecoilCorrector" << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    std::cout << boost::format(param_fmt()) % "channel"         % Channel2String(channel_);
    std::cout << boost::format(param_fmt()) % "strategy"        % Strategy2String(strategy_);
    std::cout << boost::format(param_fmt()) % "era"             % Era2String(era_);
    std::cout << boost::format(param_fmt()) % "mc"              % MC2String(mc_);
    std::cout << boost::format(param_fmt()) % "met_label"       % met_label_;
    std::cout << boost::format(param_fmt()) % "jets_label"      % jets_label_;

    std::string process_file;
    if (strategy_ ==strategy::fall15){
      process_file = "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/recoilfits/recoilMvaMEt_76X_newTraining_MG5.root";
    }else{
      std::cerr << "Strategy: " << Strategy2String(strategy_) << " not recognised, an exception will be thrown." << std::endl;
      throw;
    }
   
    if ( (sample_.find("WJetsToLNu") != sample_.npos) || (sample_.find("W1JetsToLNu") != sample_.npos) || (sample_.find("W2JetsToLNu")!=sample_.npos) || (sample_.find("W3JetsToLNu")!=sample_.npos) || (sample_.find("W4JetsToLNu")!=sample_.npos) ){
      disable = false;
      is_wjets = true;
    }

    if ( (sample_.find("DY")!=sample_.npos && sample_.find("JetsToLL")!=sample_.npos) ) {
      disable = false;
      is_ztt = true;
    }

    if (sample_.find("HToTauTau")!=sample_.npos){
     disable = false;
     is_htt = true;
    }
    
    if (disable) {
      std::cout << boost::format(param_fmt()) % "enabled"      % false;
      return 0;
    } else {
      corrector_ = new RecoilCorrectorRun2(process_file);
    }

    return 0;
  }

  int HTTRun2RecoilCorrector::Execute(TreeEvent *event) {

    if (disable) return 0;

    // Get the stuff we need from the event
//    std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>(dilepton_label_);
    Met * mvaMet = event->GetPtr<Met>(met_label_);
    std::vector<GenParticle *> parts = event->GetPtrVec<GenParticle>("genParticles");
    std::vector<GenParticle *> lhe_parts;
    if(!is_htt){//Only have lheParticles for DY and W
      lhe_parts = event->GetPtrVec<GenParticle>("lheParticles");
    }
    std::vector<GenParticle *> sel_lhe_parts;
    std::vector<GenParticle *> sel_gen_parts;
    double genpX=0;
    double genpY=0;
    double vispX=0;
    double vispY=0;
    bool has_tau = false;

    //Start with the generator Z/W pT, which we'll take from the lhe particles:
    if(!is_htt){
      for(unsigned i = 0; i < lhe_parts.size() ; ++i){
       if(lhe_parts[i]->status() != 1) continue;
       unsigned id = abs(lhe_parts[i]->pdgid());
       if(is_ztt){//For DY, we want es,mus and taus:
         if (id == 11|| id ==13 || id ==15) sel_lhe_parts.push_back(lhe_parts[i]);
         if (id == 15) has_tau = true;
       } else if(is_wjets){//For WJets, we want all leptons:
         if (id >= 11 && id <= 16) sel_lhe_parts.push_back(lhe_parts[i]);
         if (id == 15) has_tau = true;
       }
     }
    } else {//We don't have the lhe parts for H->tautau signal, so need to do something else here
      has_tau=true;
      for(unsigned i=0 ; i< parts.size(); ++i){
        unsigned id = abs(parts[i]->pdgid());
        if(id == 25 || id == 35 || id == 36){
          if(parts[i]->pt()!=0) sel_lhe_parts.push_back(parts[i]);
        }
      }
      if(sel_lhe_parts.size()!=1){
        std::cerr<<"Warning: there should be exactly one h,H or A in the evt record"<<std::endl;
        throw;
      }
    }

   //Now loop through our selected particles and calculate genpX and genpY:
   for( unsigned i = 0; i < sel_lhe_parts.size() ; ++i){
     genpX+= sel_lhe_parts[i]->vector().px();
     genpY+= sel_lhe_parts[i]->vector().py();
   }

  //Now we need the visible gen pX and pY:
  if(is_ztt || is_htt){
   if(!has_tau){ //for DY that doesn't have a tau, gen pX,pY is the same as visible pX,pY
    vispX = genpX;
    vispY = genpY;
  } else {//If there is a tau we need the gen particle collection:
  for( unsigned i = 0 ;i <parts.size() ; ++i){
    std::vector<bool> status_flags = parts[i]->statusFlags();
    if ( ((abs(parts[i]->pdgid()) == 11 )||(abs(parts[i]->pdgid()) == 13 ))&& status_flags[IsDirectPromptTauDecayProduct] ){
      sel_gen_parts.push_back(parts[i]);
    }
  }
  //Also build tau jets out of the gen particles, in case we have a hadronically decaying tau
  std::vector<GenJet> gen_taus = BuildTauJets(parts, false,true);

  if((gen_taus.size() + sel_gen_parts.size()) != 2) std::cout<<"More than two particles!"<<std::endl;
  for(unsigned i = 0; i< gen_taus.size(); ++i){
    vispX += gen_taus[i].vector().px();
    vispY += gen_taus[i].vector().py();
  } 
  for(unsigned i = 0 ; i < sel_gen_parts.size() ; ++i){
    vispX += sel_gen_parts[i]->vector().px();
    vispY += sel_gen_parts[i]->vector().py();
  }
 }
}else if(is_wjets){
  if(!has_tau){//for W that doesn't have a tau, can take vispX,pY from lheParticle collection:
   for(unsigned i = 0 ; i < lhe_parts.size() ; ++i){
     if(lhe_parts[i]->status() != 1) continue;
     unsigned id = abs(lhe_parts[i]->pdgid());
     if(id == 11 || id == 13){
       vispX = lhe_parts[i]->vector().px();
       vispY = lhe_parts[i]->vector().py();
     }
    }
  } else { //if there is a tau we need the gen particle collection again:
  for( unsigned i = 0 ;i <parts.size() ; ++i){
    std::vector<bool> status_flags = parts[i]->statusFlags();
    if ( ((abs(parts[i]->pdgid()) == 11 )||(abs(parts[i]->pdgid()) == 13 ))&& status_flags[IsDirectPromptTauDecayProduct] ){
      sel_gen_parts.push_back(parts[i]);
    }
  }
  //Also build tau jets out of the gen particles, in case we have a hadronically decaying tau
  std::vector<GenJet> gen_taus = BuildTauJets(parts, false,true);

  if((gen_taus.size() + sel_gen_parts.size()) != 1) std::cout<<"More than one particle!"<<std::endl;

  for(unsigned i = 0; i< gen_taus.size(); ++i){
    vispX += gen_taus[i].vector().px();
    vispY += gen_taus[i].vector().py();
  } 
  for(unsigned i = 0 ; i < sel_gen_parts.size() ; ++i){
    vispX += sel_gen_parts[i]->vector().px();
    vispY += sel_gen_parts[i]->vector().py();
  }
 }
 }

  //Now we can finally start correcting
  std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_); // Make a copy of the jet collection
  ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
  unsigned njets = jets.size();
  double mvaMetx = mvaMet->vector().px();
  double mvaMety = mvaMet->vector().py();
  double mvaMete = mvaMet->energy();
  double met_res_e = sqrt(mvaMete*mvaMete-mvaMetx*mvaMetx-mvaMety*mvaMety);//As we can only set pt/eta/phi/energy, not px or py
  float correctedMetx, correctedMety;
  corrector_->Correct(mvaMetx,mvaMety,genpX,genpY,vispX,vispY,njets,correctedMetx,correctedMety);
  //Now stick this back into our met object:
  mvaMet->set_pt(sqrt(correctedMetx*correctedMetx+correctedMety*correctedMety));
  mvaMet->set_phi(atan2(correctedMety,correctedMetx));
  mvaMet->set_energy(sqrt(met_res_e*met_res_e+correctedMetx*correctedMetx+correctedMety*correctedMety));


    return 0;
  }
  int HTTRun2RecoilCorrector::PostAnalysis() {

    return 0;
  }

  void HTTRun2RecoilCorrector::PrintInfo() {
    ;
  }
}
