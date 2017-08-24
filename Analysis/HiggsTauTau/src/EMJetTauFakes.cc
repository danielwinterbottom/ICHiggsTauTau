#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/EMJetTauFakes.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

#include "TMVA/Reader.h"
#include "TVector3.h"
#include "boost/format.hpp"
#include "TMath.h"
#include "TLorentzVector.h"

namespace ic {

  EMJetTauFakes::EMJetTauFakes(std::string const& name) : ModuleBase(name){ 
      muon_label_ = "sel_muons";
      jets_label_ = "pfJetsPFlow";
      fs_ = NULL;
      write_tree_ = true;
      is_data_=false;
  }

  EMJetTauFakes::~EMJetTauFakes() {
    ;
  }

  int EMJetTauFakes::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "EMJetTauFakes" << std::endl;
    std::cout << "-------------------------------------" << std::endl;    
      std::cout << boost::format(param_fmt()) % "muon_label"      % muon_label_;
      std::cout << boost::format(param_fmt()) % "jets_label"      % jets_label_;
      std::cout << boost::format(param_fmt()) % "write_tree"      % write_tree_;

    if (fs_ && write_tree_) {
      outtree_ = fs_->make<TTree>("jetfakes","jetfakes");
      outtree_->Branch("tau_iso_vloose",    &tau_iso_vloose_);
      outtree_->Branch("tau_iso_loose",    &tau_iso_loose_);
      outtree_->Branch("tau_iso_medium",    &tau_iso_medium_);
      outtree_->Branch("tau_iso_tight",     &tau_iso_tight_);
      outtree_->Branch("tau_pt",            &tau_pt_);
      outtree_->Branch("tau_eta",           &tau_eta_);
      outtree_->Branch("jet_pt",            &jet_pt_);
      outtree_->Branch("jet_eta",            &jet_eta_);
      outtree_->Branch("wt",               &wt_);
    }

    return 0;
  }

  int EMJetTauFakes::Execute(TreeEvent *event) {

  //First do the event selection. Trigger etc already done, so just isolation/lepton vetos/topological
  //0) Get trigger bool:
  if(!event->Get<bool>("trg_muonelectron")) return 1;
  //1) Isolation
    std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>("ditau");
    CompositeCandidate const* ditau = ditau_vec.at(0);
    Candidate const* lep1 = ditau->GetCandidate("lepton1");
    Candidate const* lep2 = ditau->GetCandidate("lepton2");
    Electron const* elec = dynamic_cast<Electron const*>(lep1);
    Muon const* muon = dynamic_cast<Muon const*>(lep2);

    
    if(PF03IsolationVal(elec,0.5,0) > 0.2) return 1;
    if(PF04IsolationVal(muon,0.5,0) > 0.25) return 1;

 //2) Lepton vetoes
    bool extraelec_veto_ = event->Get<bool>("extra_elec_veto");
    bool extramuon_veto_ = event->Get<bool>("extra_muon_veto");
    if(extraelec_veto_ || extramuon_veto_) return 1;

 //3) MET and Dzeta
 Met const* pfmet = NULL;
 std::vector<Met*> pfMet_vec = event->GetPtrVec<Met>("pfMetFromSlimmed");
 pfmet = pfMet_vec.at(0);  
 double pzeta = PZeta(ditau,pfmet, 0.85);
 double met = pfmet->pt();
 
 if (pzeta>-50||met<80) return 1;

 //After selection, build numerator and denominator by looping through jets and taus.
 //First define event weights 
 wt_=1.0;
 if(!is_data_){
      EventInfo *eventInfo = event->GetPtr<EventInfo>("eventInfo");
      double wt_pu = eventInfo->weight("pileup");
      int wt_mcsign = eventInfo->weight("wt_mc_sign");
      double trk_wt = eventInfo->weight("wt_tracking_eff"); 
      double lepton_wt = eventInfo->weight("lepton");
      double wt_stitching = eventInfo->weight("dysoup")*eventInfo->weight("wsoup");
      //double wt_dy_pt = eventInfo->weight("wt_zpt");
      double ttshape_wt = eventInfo->weight("topquark_weight");
      
      wt_ = wt_pu*wt_mcsign*trk_wt*lepton_wt*wt_stitching*ttshape_wt;
 }
 
 std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
 ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 20.0, 4.7));

  std::vector<Tau *> taus = event->GetPtrVec<Tau>("taus");
  ic::erase_if(taus,!boost::bind(MinPtMaxEta,_1,18,2.3));
  ic::erase_if_not(taus,[=](ic::Tau* t){return fabs(t->lead_dz_vertex()) < 0.2 && fabs(t->charge()) ==1 && t->GetTauID("decayModeFinding") > 0.5;});

  std::vector<std::pair<PFJet*, Tau*> > jet_tau_matches = MatchByDR(jets,taus,0.2,true,true);

  std::map<PFJet*,Tau*> jet_tau_map;

  for (auto const& pair: jet_tau_matches){
    jet_tau_map[pair.first] = pair.second;
  }

 for (unsigned i = 0; i<jets.size();++i){
    tau_pt_ = -999;
    tau_eta_ = -999;
    tau_iso_vloose_ = 0;
    tau_iso_loose_ = 0;
    tau_iso_medium_ = 0;
    tau_iso_tight_ = 0;
    jet_pt_ = jets[i]->pt();
    jet_eta_ = jets[i]->eta();
    if(jet_tau_map.count(jets[i]) >0 ){
     tau_pt_ = jet_tau_map.at(jets[i])->pt();  
     tau_eta_ = jet_tau_map.at(jets[i])->eta();  
     tau_iso_vloose_ = jet_tau_map.at(jets[i])->GetTauID("byVLooseIsolationMVArun2v1DBoldDMwLT");
     tau_iso_loose_ = jet_tau_map.at(jets[i])->GetTauID("byLooseIsolationMVArun2v1DBoldDMwLT");
     tau_iso_medium_ = jet_tau_map.at(jets[i])->GetTauID("byMediumIsolationMVArun2v1DBoldDMwLT");
     tau_iso_tight_ = jet_tau_map.at(jets[i])->GetTauID("byTightIsolationMVArun2v1DBoldDMwLT");
   }
    if (write_tree_ && fs_) outtree_->Fill();
  }
   

    return 0;
  }

  int EMJetTauFakes::PostAnalysis() {
    return 0;
  }

  void EMJetTauFakes::PrintInfo() {
    ;
  }

}

