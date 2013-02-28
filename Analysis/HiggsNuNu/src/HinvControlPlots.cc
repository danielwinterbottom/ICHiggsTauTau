#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvControlPlots.h"

#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

#include "TMVA/Reader.h"
#include "TVector3.h"


namespace ic {



  HinvCoreControlPlots::HinvCoreControlPlots(TFileDirectory const& dir) {
    TH1F::SetDefaultSumw2();
    n_vtx = dir.make<TH1F>("n_vtx","n_vtx", 40, 0, 40); 
    met = dir.make<TH1F>("met","met", 1000, 0, 1000); 
    met_phi = dir.make<TH1F>("met_phi","met_phi", 63, -3.15, 3.15); 
    n_jets = dir.make<TH1F>("n_jets","n_jets", 50, 0, 50); 
    n_jetsingap = dir.make<TH1F>("n_jetsingap","n_jetsingap", 50, 0, 50); 
    jpt_1 = dir.make<TH1F>("jpt_1","jpt_1", 1000, 0, 1000); 
    jpt_2 = dir.make<TH1F>("jpt_2","jpt_2", 1000, 0, 1000); 
    jeta_1 = dir.make<TH1F>("jeta_1","jeta_1", 100, -5, 5); 
    jeta_2 = dir.make<TH1F>("jeta_2","jeta_2", 100, -5, 5); 
    mjj = dir.make<TH1F>("mjj","mjj", 5000, 0, 5000); 
    detajj = dir.make<TH1F>("detajj","detajj", 100, 0, 10);
    etaprodjj = dir.make<TH1F>("etaprodjj","etaprodjj", 100, -25, 25);
    drjj = dir.make<TH1F>("drjj","drjj", 100, 0, 10);
    dphijj = dir.make<TH1F>("dphijj","dphijj", 100, 0, 7);

 
  };



  HinvControlPlots::HinvControlPlots(std::string const& name): ModuleBase(name){
    fs_ = NULL;
    met_label_ = "pfMet";
    dijet_label_ = "jjCandidates";
    sel_label_ = "JetPair";
  }

  HinvControlPlots::~HinvControlPlots(){;}


  int  HinvControlPlots::PreAnalysis(){
    std::cout << "** PreAnalysis Info for HinvControlPlots **" << std::endl;
    if (fs_) {
      std::cout << "MET Label: " << met_label_ << std::endl;
      std::cout << "dijet Label: " << dijet_label_ << std::endl;
      std::cout << "Selection Label: " << sel_label_ << std::endl;
    }

    misc_plots_ = new DynamicHistoSet(fs_->mkdir("misc_plots"));
    misc_2dplots_ = new Dynamic2DHistoSet(fs_->mkdir("misc_2dplots"));
    InitCoreControlPlots();
    
    yields_ = 0;

    return 0;
  }

  int  HinvControlPlots::Execute(TreeEvent *event){

    // Get the objects we need from the event
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    wt_ = eventInfo->total_weight();
    std::vector<CompositeCandidate *> const& dijet_vec = event->GetPtrVec<CompositeCandidate>(dijet_label_);

    if (dijet_vec.size() == 0) {
      std::cerr << " #### ERROR : require at least one jet pair..." << std::endl;
      //no point in making plots, go to the next module.... 
      return 0;
    }

    CompositeCandidate const* dijet = dijet_vec.at(0);
    Candidate const* jet1 = dijet->GetCandidate("jet1");
    Candidate const* jet2 = dijet->GetCandidate("jet2");
    Met const* met = event->GetPtr<Met>(met_label_);
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("pfJetsPFlow");
    std::sort(jets.begin(), jets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    
    // Define event properties
    // IMPORTANT: Make sure each property is re-set
    // for each new event
    
    n_vtx_ = eventInfo->good_vertices();

    met_ = met->pt();
    met_phi_ = met->phi();

    n_jets_ = jets.size();
    
    jpt_1_ = jet1->pt();
    jeta_1_ = jet1->eta();
    jpt_2_ = jet2->pt();
    jeta_2_ = jet2->eta();

    mjj_ = dijet->M();
    detajj_ = fabs(jet1->eta() - jet2->eta());
    etaprodjj_ = jet1->eta() * jet2->eta();
    drjj_ = ROOT::Math::VectorUtil::DeltaR(jet1->vector(),jet2->vector());
    dphijj_ = ROOT::Math::VectorUtil::DeltaPhi(jet1->vector(),jet2->vector());

    double eta_high = (jet1->eta() > jet2->eta()) ? jet1->eta() : jet2->eta();
    double eta_low = (jet1->eta() > jet2->eta()) ? jet2->eta() : jet1->eta();
    n_jetsingap_ = 0;
    if (n_jets_ > 2) {
      for (unsigned i = 2; i < jets.size(); ++i) {
	if (jets[i]->pt() > 30.0 &&  jets[i]->eta() > eta_low && jets[i]->eta() < eta_high) ++n_jetsingap_;
      }
    }

    FillCoreControlPlots();

    return 0;
  }

  int  HinvControlPlots::PostAnalysis(){
    return 0;
  }

  void  HinvControlPlots::PrintInfo(){
    ;
  }

  void HinvControlPlots::InitCoreControlPlots() {
    controlplots_ = new HinvCoreControlPlots(fs_->mkdir(sel_label_));
  }


 void HinvControlPlots::FillYields() {
   yields_ = yields_ + wt_;
 }


  void HinvControlPlots::FillCoreControlPlots() {
    controlplots_->n_vtx->Fill(n_vtx_, wt_);
    controlplots_->met->Fill(met_, wt_);
    controlplots_->met_phi->Fill(met_phi_, wt_);
    controlplots_->n_jets->Fill(n_jets_, wt_);
    controlplots_->n_jetsingap->Fill(n_jetsingap_, wt_);
    controlplots_->jpt_1->Fill(jpt_1_, wt_);
    controlplots_->jeta_1->Fill(jeta_1_, wt_);
    controlplots_->jpt_2->Fill(jpt_2_, wt_);
    controlplots_->jeta_2->Fill(jeta_2_, wt_);            
    controlplots_->mjj->Fill(mjj_, wt_);
    controlplots_->detajj->Fill(detajj_, wt_);
    controlplots_->etaprodjj->Fill(etaprodjj_, wt_);
    controlplots_->drjj->Fill(drjj_, wt_);
    controlplots_->dphijj->Fill(dphijj_, wt_);
  }


}//namespace
