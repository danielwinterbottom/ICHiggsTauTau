#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/ProducePileup.h"
#include "UserCode/ICHiggsTauTau/interface/PileupInfo.hh"


namespace ic {

  ProducePileup::ProducePileup(std::string const& name) : ModuleBase(name) {
    file_ = new TFile("true_interactions.root","RECREATE");


  }

  ProducePileup::~ProducePileup() {
    ;
  }

  int ProducePileup::PreAnalysis() {
    hist_ = new TH1D("true_interactions","true_interactions",bins_, 0, max_);
    return 0;
  }


  int ProducePileup::Execute(TreeEvent *event) {

    std::vector<PileupInfo *> const& puInfo = event->GetPtrVec<PileupInfo>("pileupInfo");
    for (unsigned i = 0; i < puInfo.size(); ++i) {
      if (puInfo[i]->bunch_crossing() == 0) hist_->Fill(puInfo[i]->true_num_interactions());
    }
  return 0;
  }

  int ProducePileup::PostAnalysis() {
    file_->cd();
    hist_->Write();
    file_->Close();
    return 0;
  }

  void ProducePileup::PrintInfo() {
    ;
  }
}
