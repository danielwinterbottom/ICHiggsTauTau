#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvWJetsPlots.h"

#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

#include "TMVA/Reader.h"
#include "TVector3.h"


namespace ic {



  HinvWJetsControlPlots::HinvWJetsControlPlots(TFileDirectory const& dir) {
    TH1F::SetDefaultSumw2();
    n_electrons = dir.make<TH1F>("n_electrons","n_electrons", 10, 0, 10); 
    n_muons = dir.make<TH1F>("n_muons","n_muons", 10, 0, 10); 
    mt_enu = dir.make<TH1F>("mt_enu","mt_enu", 1000, 0, 500); 
    mt_munu = dir.make<TH1F>("mt_munu","mt_munu", 1000, 0, 500); 
    ept_1 = dir.make<TH1F>("ept_1","ept_1", 1000, 0, 1000); 
    ept_2 = dir.make<TH1F>("ept_2","ept_2", 1000, 0, 1000); 
    eeta_1 = dir.make<TH1F>("eeta_1","eeta_1", 100, -5, 5); 
    eeta_2 = dir.make<TH1F>("eeta_2","eeta_2", 100, -5, 5); 
    mupt_1 = dir.make<TH1F>("mupt_1","mupt_1", 1000, 0, 1000); 
    mupt_2 = dir.make<TH1F>("mupt_2","mupt_2", 1000, 0, 1000); 
    mueta_1 = dir.make<TH1F>("mueta_1","mueta_1", 100, -5, 5); 
    mueta_2 = dir.make<TH1F>("mueta_2","mueta_2", 100, -5, 5); 
    met_noelectrons = dir.make<TH1F>("met_noelectrons","met_noelectrons", 1000, 0, 1000); 
    met_nomuons = dir.make<TH1F>("met_nomuons","met_nomuons", 1000, 0, 1000); 
 
  };


  HinvWJetsPlots::HinvWJetsPlots(std::string const& name): ModuleBase(name){
    fs_ = NULL;
    met_label_ = "pfMet";
    electrons_label_ = "electrons";
    muons_label_ = "muonsPFlow";
    sel_label_ = "JetPair";
  }

  HinvWJetsPlots::~HinvWJetsPlots(){;}


  int  HinvWJetsPlots::PreAnalysis(){
    std::cout << "** PreAnalysis Info for HinvWJetsPlots **" << std::endl;
    if (fs_) {
      std::cout << "MET Label: " << met_label_ << std::endl;
      std::cout << "electrons Label: " << electrons_label_ << std::endl;
      std::cout << "muons Label: " << muons_label_ << std::endl;
      std::cout << "Selection Label: " << sel_label_ << std::endl;
    }

    misc_plots_ = new DynamicHistoSet(fs_->mkdir("misc_plots"));
    misc_2dplots_ = new Dynamic2DHistoSet(fs_->mkdir("misc_2dplots"));
    InitPlots();
    
    yields_ = 0;

    return 0;
  }

  int  HinvWJetsPlots::Execute(TreeEvent *event){

    // Get the objects we need from the event
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    wt_ = eventInfo->total_weight();

    Met const* met = event->GetPtr<Met>(met_label_);

    std::vector<Electron*> electrons = event->GetPtrVec<Electron>("electrons");
    std::sort(electrons.begin(), electrons.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));

    std::vector<Muon*> muons = event->GetPtrVec<Muon>("muonsPFlow");
    std::sort(muons.begin(), muons.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    
    // Define event properties
    // IMPORTANT: Make sure each property is re-set
    // for each new event
    
    
    n_electrons_ = electrons.size();
    n_muons_ = muons.size();
    
    ept_1_ = -1;
    eeta_1_ = -10;
    mt_enu_ = -1;
   if (n_electrons_ > 0) {
      ept_1_ = electrons[0]->pt();
      eeta_1_ = electrons[0]->eta();
      mt_enu_ = MT(electrons[0],met);
    }
    ept_2_ = -1;
    eeta_2_ = -10;
    if (n_electrons_ > 1){
      ept_2_ = electrons[1]->pt();
      eeta_2_ = electrons[1]->eta();
    }

    mupt_1_ = -1;
    mueta_1_ = -10;
    mt_munu_ = -1;
    if (n_muons_ > 0) {
      mupt_1_ = muons[0]->pt();
      mueta_1_ = muons[0]->eta();
      mt_munu_ = MT(muons[0],met);
    }
    mupt_2_ = -1;
    mueta_2_ = -10;
    if (n_muons_ > 1){
      mupt_2_ = muons[1]->pt();
      mueta_2_ = muons[1]->eta();
    }
    
    met_noelectrons_ = met->pt();
    for (unsigned iL(0); iL<n_electrons_; ++iL){
      met_noelectrons_ += electrons[iL]->pt();
    }

    met_nomuons_ = met->pt();
    for (unsigned iL(0); iL<n_muons_; ++iL){
      met_nomuons_ += muons[iL]->pt();
    }


    FillPlots();

    return 0;
  }

  int  HinvWJetsPlots::PostAnalysis(){
    return 0;
  }

  void  HinvWJetsPlots::PrintInfo(){
    ;
  }

  void HinvWJetsPlots::InitPlots() {
    wjetsplots_ = new HinvWJetsControlPlots(fs_->mkdir(sel_label_));
  }


 void HinvWJetsPlots::FillYields() {
   yields_ = yields_ + wt_;
 }


  void HinvWJetsPlots::FillPlots() {
    wjetsplots_->n_electrons->Fill(n_electrons_, wt_);
    wjetsplots_->n_muons->Fill(n_muons_, wt_);
    wjetsplots_->mt_enu->Fill(mt_enu_, wt_);
    wjetsplots_->mt_munu->Fill(mt_munu_, wt_);
    wjetsplots_->ept_1->Fill(ept_1_, wt_);
    wjetsplots_->eeta_1->Fill(eeta_1_, wt_);
    wjetsplots_->ept_2->Fill(ept_2_, wt_);
    wjetsplots_->eeta_2->Fill(eeta_2_, wt_);            
    wjetsplots_->mupt_1->Fill(mupt_1_, wt_);
    wjetsplots_->mueta_1->Fill(mueta_1_, wt_);
    wjetsplots_->mupt_2->Fill(mupt_2_, wt_);
    wjetsplots_->mueta_2->Fill(mueta_2_, wt_);            
    wjetsplots_->met_noelectrons->Fill(met_noelectrons_, wt_);
    wjetsplots_->met_nomuons->Fill(met_nomuons_, wt_);
  }


}//namespace
