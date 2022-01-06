#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/PreselectionFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"

namespace ic {

  PreselectionFilter::PreselectionFilter(std::string const& name) : ModuleBase(name), channel_(channel::et) {
    do_preselection_ = false;
  }

  PreselectionFilter::~PreselectionFilter() {
    ;
  }

  int PreselectionFilter::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "PreselectionFilter" << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    return 0;
  }

int PreselectionFilter::Execute(TreeEvent *event) {

  if (!do_preselection_) return 0;

  EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
  std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>(dilepton_label_);
  Candidate c1 = *(dilepton.at(0)->GetCandidate("lepton1"));
  Candidate c2 = *(dilepton.at(0)->GetCandidate("lepton2"));
  Candidate const* lep1 = dilepton.at(0)->GetCandidate("lepton1");
  Candidate const* lep2 = dilepton.at(0)->GetCandidate("lepton2");

  //bool dilepton_veto_ = false;
  bool extraelec_veto_ = false;
  bool extramuon_veto_ = false;
  double iso_1_ = 0;
  double iso_2_ = 0; 
  bool pass_presel = false;

  if(channel_ == channel::et) {
      //if(event->Exists("dielec_veto"))  dilepton_veto_ = event->Get<bool>("dielec_veto");
      if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
      if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
      Electron const* elec = dynamic_cast<Electron const*>(lep1);
      Tau const* tau = dynamic_cast<Tau const*>(lep2);
      iso_1_ = PF03EAIsolationVal(elec, eventInfo->jet_rho());
      pass_presel = (tau->GetTauID("byVVVLooseDeepTau2017v2p1VSe") && tau->GetTauID("byVLooseDeepTau2017v2p1VSmu") && tau->GetTauID("byVVVLooseDeepTau2017v2p1VSjet") && iso_1_<0.5); 
  }
  if(channel_ == channel::mt) { 
      //if(event->Exists("dimuon_veto")) dilepton_veto_ = event->Get<bool>("dimuon_veto");
      if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
      if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
      Muon const* muon  = dynamic_cast<Muon const*>(lep1);
      Tau const* tau = dynamic_cast<Tau const*>(lep2);
      iso_1_ = PF04IsolationVal(muon, 0.5, 0);
      pass_presel = (tau->GetTauID("byVVVLooseDeepTau2017v2p1VSe") && tau->GetTauID("byVLooseDeepTau2017v2p1VSmu") && tau->GetTauID("byVVVLooseDeepTau2017v2p1VSjet") && iso_1_<0.5); 
  }
  if(channel_ == channel::em) { 
      if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
      if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
      Electron  const* elec  = dynamic_cast<Electron const*>(lep1);
      Muon const* muon = dynamic_cast<Muon const*>(lep2);
      iso_1_ = PF03EAIsolationVal(elec, eventInfo->jet_rho());
      iso_2_ = PF04IsolationVal(muon, 0.5, 0);
      if(iso_2_<0.5 && iso_1_<0.5) pass_presel = true;
  } 
  if(channel_ == channel::tt) {
      if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
      if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
      Tau  const* tau1  = dynamic_cast<Tau const*>(lep1);
      Tau const* tau2 = dynamic_cast<Tau const*>(lep2);
      pass_presel = (tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSe") && tau1->GetTauID("byVLooseDeepTau2017v2p1VSmu") && tau1->GetTauID("byVVVLooseDeepTau2017v2p1VSjet") && tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSe") && tau2->GetTauID("byVLooseDeepTau2017v2p1VSmu") && tau2->GetTauID("byVVVLooseDeepTau2017v2p1VSjet")) ; 
  }
  if(channel_ == channel::zmm || channel_ == channel::tpzmm) { 
      if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
      if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
      Muon const* muon1  = dynamic_cast<Muon const*>(lep1);
      Muon const* muon2 = dynamic_cast<Muon const*>(lep2);
      iso_1_ = PF04IsolationVal(muon1, 0.5, 0);
      iso_2_ = PF04IsolationVal(muon2, 0.5, 0);
      if(iso_2_<0.2 && iso_1_<0.2) pass_presel = true;
  }
  if(channel_ == channel::zee || channel_ == channel::tpzee) { 
      if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
      if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
      Electron  const* elec1  = dynamic_cast<Electron const*>(lep1);
      Electron const* elec2 = dynamic_cast<Electron const*>(lep2);
      iso_1_ = PF03EAIsolationVal(elec1, eventInfo->jet_rho());
      iso_2_ = PF03EAIsolationVal(elec2, eventInfo->jet_rho());
      if(iso_2_<0.2 && iso_1_<0.2) pass_presel = true;
  }
    
  if((channel_ == channel::zmm || channel_ == channel::zee) && do_preselection_) pass_presel = true;
    
  bool lepton_veto_ = extraelec_veto_ || extramuon_veto_;
  if ( channel_ == channel::zmm || channel_ == channel::zee ) lepton_veto_ = false;

  bool preselect_evt = pass_presel&&!lepton_veto_;

  if (preselect_evt) return 0;
  else return 1;
}
int PreselectionFilter::PostAnalysis() {
    return 0;
}

void PreselectionFilter::PrintInfo() {
    ;
}

}
