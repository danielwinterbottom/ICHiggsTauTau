#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/ZJetsControlPlots.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

namespace ic {

  ZJetsControlPlots::ZJetsControlPlots(std::string const& name) : ModuleBase(name), channel_(channel::et) {
    met_label_ = "pfMVAMet";
    fs_ = NULL;
}

  ZJetsControlPlots::~ZJetsControlPlots() {
    ;
  }

  int ZJetsControlPlots::PreAnalysis() {
    std::cout << "**PreAnalysis Info for ZJetsControlPlots**" << std::endl;
    if (fs_) hset = new DynamicHistoSet(fs_->mkdir("inclusive"));
    if (fs_) h2dset = new Dynamic2DHistoSet(fs_->mkdir("inclusive_2d"));
    hset->Create("m_z", 120, 60, 120);


    return 0;
  }

  int ZJetsControlPlots::Execute(TreeEvent *event) {
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    double wt = eventInfo->total_weight();
    std::vector<CompositeCandidate *> const& dileptons = event->GetPtrVec<CompositeCandidate>("dileptons");
    hset->Fill("m_z", dileptons[0]->M(), wt);
   
    return 0;
  }
  int ZJetsControlPlots::PostAnalysis() {
    return 0;
  }

  void ZJetsControlPlots::PrintInfo() {
    ;
  }
}
