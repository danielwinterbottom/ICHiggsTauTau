#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/BTagCheck.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

namespace ic {

  BTagCheck::BTagCheck(std::string const& name) : ModuleBase(name) {
    fs_ = NULL;
  }

  BTagCheck::~BTagCheck() {
    ;
  }

  int BTagCheck::PreAnalysis() {
    if (fs_) {
      hists_ = new Dynamic2DHistoSet(fs_->mkdir("BTagCheck"));
      hists_->Create("csv_orig_embed", 51, -1, 1.04, 51, -1, 1.04);
    }
    return 0;
  }

  int BTagCheck::Execute(TreeEvent *event) {
    std::vector<PFJet*> embed_jets = event->GetPtrVec<PFJet>("pfJetsPFlow");
    ic::erase_if(embed_jets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
    std::vector<PFJet*> orig_jets = event->GetPtrVec<PFJet>("pfJetsRECO");
    auto pairs = MatchByDR(embed_jets, orig_jets, 0.5, true, true);
    for (unsigned i = 0; i < pairs.size(); ++i) {
      hists_->Fill("csv_orig_embed", 
        pairs[i].second->GetBDiscriminator("combinedSecondaryVertexBJetTags"), 
        pairs[i].first->GetBDiscriminator("combinedSecondaryVertexBJetTags"));
    }
    return 0;
  }
  int BTagCheck::PostAnalysis() {
    return 0;
  }

  void BTagCheck::PrintInfo() {
    ;
  }
}
