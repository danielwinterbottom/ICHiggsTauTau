#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/WMuNuJetTauFakes.h"
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

  WMuNuJetTauFakes::WMuNuJetTauFakes(std::string const& name) : ModuleBase(name){ 
      muon_label_ = "sel_muons";
      jets_label_ = "pfJetsPFlow";
      fs_ = NULL;
      write_tree_ = true;
      is_data_=false;
  }

  WMuNuJetTauFakes::~WMuNuJetTauFakes() {
    ;
  }

  int WMuNuJetTauFakes::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "WMuNuJetTauFakes" << std::endl;
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
      outtree_->Branch("wt",                &wt_);
      outtree_->Branch("met",                &met_);
      outtree_->Branch("mt",                &mt_);
      outtree_->Branch("muon_pt",                &muon_pt_);
    }
      TFile f("input/scale_factors/htt_scalefactors_v16_5.root");
      w_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));;
      f.Close();
      fns_["m_trk_ratio"] = std::shared_ptr<RooFunctor>(
      w_->function("m_trk_ratio")->functor(w_->argSet("m_eta")));
      fns_["m_trgOR4_binned_ratio"] = std::shared_ptr<RooFunctor>(
      w_->function("m_trgOR4_binned_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));
      fns_["m_id_ratio"] = std::shared_ptr<RooFunctor>(
      w_->function("m_id_ratio")->functor(w_->argSet("m_pt,m_eta")));
      fns_["m_iso_binned_ratio"] = std::shared_ptr<RooFunctor>(
      w_->function("m_iso_binned_ratio")->functor(w_->argSet("m_pt,m_eta,m_iso")));


    return 0;
  }

  int WMuNuJetTauFakes::Execute(TreeEvent *event) {

  //First do the event selection
  //1) Trigger and pT/eta/iso
  std::string alt_trig_obj_label = "triggerObjectsIsoMu22";
  std::string alt_leg1_filter =  "hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09"; 
  std::string alt_trk_trig_obj_label = "triggerObjectsIsoTkMu22";
  std::string alt_trk_leg1_filter =  "hltL3fL1sMu20L1f0Tkf22QL3trkIsoFiltered0p09";
  
  std::string alt_er_trig_obj_label = "triggerObjectsIsoMu22Eta2p1";
  std::string alt_er_leg1_filter =  "hltL3crIsoL1sSingleMu20erL1f0L2f10QL3f22QL3trkIsoFiltered0p09"; 
  std::string alt_er_trk_leg1_filter = "hltL3fL1sMu20erL1f0Tkf22QL3trkIsoFiltered0p09";
  std::string alt_er_trk_trig_obj_label = "triggerObjectsIsoTkMu22Eta2p1";


  std::vector<Muon *> & muons_vec = event->GetPtrVec<Muon>(muon_label_);
  ic::erase_if(muons_vec, !boost::bind(MinPtMaxEta, _1, 23.0, 2.1));
  ic::erase_if_not(muons_vec,[=](ic::Muon* m){return PF04IsolationVal(m,0.5) < 0.15;});

  std::vector<TriggerObject*> const& objs = event->GetPtrVec<TriggerObject>(alt_trig_obj_label);
  std::vector<TriggerObject*> const& objs_trk = event->GetPtrVec<TriggerObject>(alt_trk_trig_obj_label);
  
  std::vector<TriggerObject*> const& objs_er = event->GetPtrVec<TriggerObject>(alt_er_trig_obj_label);
  std::vector<TriggerObject*> const& objs_er_trk = event->GetPtrVec<TriggerObject>(alt_er_trk_trig_obj_label);

  std::vector<Muon *> leptons_pass;

  for ( unsigned i = 0; i < muons_vec.size(); ++i) {
     bool leg1_match = IsFilterMatched(muons_vec[i], objs, alt_leg1_filter, 0.5) || IsFilterMatched(muons_vec[i], objs_er, alt_er_leg1_filter, 0.5);
     bool leg1_match_alt = IsFilterMatched(muons_vec[i], objs_trk, alt_trk_leg1_filter, 0.5) || IsFilterMatched(muons_vec[i], objs_er_trk, alt_er_trk_leg1_filter, 0.5);
     if(leg1_match || leg1_match_alt) leptons_pass.push_back(muons_vec[i]);
  }

  if (!(leptons_pass.size()>=1) ) return 1;
  
 //2) Lepton vetoes
 std::vector<Electron * > &electrons_ = event->GetPtrVec<Electron>("electrons");
 std::vector<Muon * > &muons_ = event->GetPtrVec<Muon>("muons");
  
 ic::erase_if(electrons_, !boost::bind(MinPtMaxEta,_1,10,2.5));
 ic::erase_if_not(electrons_, [=](ic::Electron* e){return PF03IsolationVal(e,0.5)<0.3;});
 ic::erase_if(electrons_, !boost::bind(ElectronHTTIdSpring16,_1,true));
 ic::erase_if_not(electrons_,[=](ic::Electron* e){return fabs(e->dxy_vertex())<0.045 && fabs(e->dz_vertex())<0.2 ;});

 std::function<bool(Muon const*)> ExtraMuonID;
 ExtraMuonID = [](Muon const* m) {return MuonMediumHIPsafe(m); };

 ic::erase_if(muons_, !boost::bind(MinPtMaxEta,_1,10,2.5));
 ic::erase_if_not(muons_, [=](ic::Muon* m){return PF04IsolationVal(m,0.5)<0.3;});
 ic::erase_if_not(muons_, [=](ic::Muon* m){return ExtraMuonID(m) && fabs(m->dxy_vertex())<0.045&&fabs(m->dz_vertex())<0.2;});

 if(muons_.size()>1 || electrons_.size()>0) return 1;

 //3) mT
 Met const* pfmet = NULL;
 std::vector<Met*> pfMet_vec = event->GetPtrVec<Met>("pfMetFromSlimmed");
 pfmet = pfMet_vec.at(0);  
 Muon const* muon = leptons_pass.at(0);
 
 double mt = MT(muon,pfmet);
 if (mt<60) return 1;

//4) b-jets
 std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
 ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 20.0, 4.7));
 std::vector<PFJet*> prebjets = jets;
 ic::erase_if(prebjets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
 std::vector<PFJet*> bjets = prebjets;

 std::string btag_label = "pfCombinedInclusiveSecondaryVertexV2BJetTags";
 double btag_wp = 0.8484;
 if (event->Exists("retag_result")) {
      auto const& retag_result = event->Get<std::map<std::size_t,bool>>("retag_result"); 
      ic::erase_if(bjets, !boost::bind(IsReBTagged, _1, retag_result));
  } else{ 
     ic::erase_if(bjets, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label) < btag_wp);
  } 
 
  if ( bjets.size() > 0) return 1;

 //After selection, build numerator and denominator by looping through jets and taus.
 //First define event weights
  wt_=1.0;
  if(!is_data_){

      double pt = muon->pt();
      double m_signed_eta = muon->eta();
      double m_iso = 0.15;

      auto args_id = std::vector<double>{pt,m_signed_eta};
      auto args_iso = std::vector<double>{pt,m_signed_eta,m_iso};
      auto args_trk = std::vector<double>{m_signed_eta};

      double tracking_wt = fns_["m_trk_ratio"]->eval(args_trk.data());
      double id_wt = fns_["m_id_ratio"]->eval(args_id.data());
      double iso_wt = fns_["m_iso_binned_ratio"]->eval(args_iso.data());
      double trg_wt = fns_["m_trgOR4_binned_ratio"]->eval(args_iso.data());

      EventInfo *eventInfo = event->GetPtr<EventInfo>("eventInfo");
      double wt_pu = eventInfo->weight("pileup");
      int wt_mcsign = eventInfo->weight("wt_mc_sign");
      double wt_stitching = eventInfo->weight("dysoup")*eventInfo->weight("wsoup");

     wt_ = tracking_wt*id_wt*iso_wt*trg_wt*wt_pu*wt_mcsign*wt_stitching;
  }

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
    met_=pfmet->vector().pt();
    mt_=mt;
    muon_pt_=muon->pt();;
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

  int WMuNuJetTauFakes::PostAnalysis() {
    return 0;
  }

  void WMuNuJetTauFakes::PrintInfo() {
    ;
  }

}

