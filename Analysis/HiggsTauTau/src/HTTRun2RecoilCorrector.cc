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
    }

    if ( (sample_.find("DY")!=sample_.npos && sample_.find("JetsToLL")!=sample_.npos) ) {
      disable = false;
    }

    if (sample_.find("HToTauTau")!=sample_.npos){
     disable = false;
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
    Met * mvaMet = event->GetPtr<Met>(met_label_);
    std::vector<GenParticle *> parts = event->GetPtrVec<GenParticle>("genParticles");
    std::vector<GenParticle *> sel_gen_parts;
    std::vector<GenParticle *> sel_vis_parts;
    double genpX=0;
    double genpY=0;
    double vispX=0;
    double vispY=0;
    
    for(unsigned i = 0; i < parts.size(); ++i){
      std::vector<bool> status_flags = parts[i]->statusFlags();
      unsigned id = abs(parts[i]->pdgid());
      unsigned status = abs(parts[i]->status());
      if ( (id >= 11 && id <= 16 && status_flags[FromHardProcess] && status==1) || status_flags[IsDirectHardProcessTauDecayProduct]) sel_gen_parts.push_back(parts[i]);
      if ( ( (id == 11 || id == 13 || id == 15) && status_flags[FromHardProcess] && status==1) || (status_flags[IsDirectHardProcessTauDecayProduct] && !(id==12||id==14||id==16))) sel_vis_parts.push_back(parts[i]);
   }

   for( unsigned i = 0; i < sel_gen_parts.size() ; ++i){
     genpX+= sel_gen_parts[i]->vector().px();
     genpY+= sel_gen_parts[i]->vector().py();
   }


   for( unsigned i = 0; i < sel_vis_parts.size() ; ++i){
     vispX+= sel_vis_parts[i]->vector().px();
     vispY+= sel_vis_parts[i]->vector().py();
   }


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
