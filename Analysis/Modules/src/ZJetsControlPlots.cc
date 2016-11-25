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
    std::cout << "** PreAnalysis Info for ZJetsControlPlots **" << std::endl;
    if (fs_) hset = new DynamicHistoSet(fs_->mkdir("inclusive"));
    if (fs_) h2dset = new Dynamic2DHistoSet(fs_->mkdir("inclusive_2d"));
    hset->Create("m_z", 120, 60, 120);

    hset->Create("jpt_all", 100, 0, 200);
    hset->Create("jpt_1", 100, 0, 200);
    hset->Create("jpt_2", 100, 0, 200);

    hset->Create("jeta_all", 100, -5, 5);
    hset->Create("jeta_1", 100, -5, 5);
    hset->Create("jeta_2", 100, -5, 5);

    return 0;
  }

  int ZJetsControlPlots::Execute(TreeEvent *event) {
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    double wt = eventInfo->total_weight();
    std::vector<CompositeCandidate *> const& dileptons = event->GetPtrVec<CompositeCandidate>("dileptons");
    hset->Fill("m_z", dileptons[0]->M(), wt);
   

   std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("pfJetsPFlow"); // Make a copy of the jet collection
   ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 5.0));

   for (unsigned i = 0; i < jets.size(); ++i) {
    hset->Fill("jpt_all", jets[i]->pt(), wt);
    hset->Fill("jeta_all", jets[i]->eta(), wt);
   }
   if (jets.size() >= 1) {
    hset->Fill("jpt_1", jets[0]->pt(), wt);
    hset->Fill("jeta_1", jets[0]->eta(), wt);
   }
   if (jets.size() >= 2) {
    hset->Fill("jpt_2", jets[1]->pt(), wt);
    hset->Fill("jeta_2", jets[1]->eta(), wt);
   }

    return 0;
  }
  int ZJetsControlPlots::PostAnalysis() {
    return 0;
  }

  void ZJetsControlPlots::PrintInfo() {
    ;
  }
}
