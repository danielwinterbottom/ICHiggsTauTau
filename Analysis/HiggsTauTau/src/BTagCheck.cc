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
      hists_->Create("csv_orig_embed_flag", 2, -0.5, 1.5, 2, -0.5, 1.5);
      hists1d_ = new DynamicHistoSet(fs_->mkdir("BTagCheck1D"));
      hists1d_->Create("btag_den", 4, 20, 180);
      hists1d_->Create("btag_num", 4, 20, 180);
      hists1d_->Create("btag_eff", 4, 20, 180);
      hists1d_->Create("real_btag_den", 4, 20, 180);
      hists1d_->Create("real_btag_num", 4, 20, 180);
      hists1d_->Create("real_btag_eff", 4, 20, 180);
      hists1d_->Create("orig_btag_den", 4, 20, 180);
      hists1d_->Create("orig_btag_num", 4, 20, 180);
      hists1d_->Create("orig_btag_eff", 4, 20, 180);
    }
    return 0;
  }

  int BTagCheck::Execute(TreeEvent *event) {
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    double wt = eventInfo->total_weight();
    std::vector<PFJet*> embed_jets = event->GetPtrVec<PFJet>("pfJetsPFlow");
    ic::erase_if(embed_jets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
    for (unsigned i = 0; i < embed_jets.size(); ++i) {
      hists1d_->Fill("btag_den", embed_jets[i]->pt(), wt);
      if (abs(embed_jets[i]->parton_flavour()) == 5) hists1d_->Fill("real_btag_den", embed_jets[i]->pt(), wt);
      if (embed_jets[i]->GetBDiscriminator("combinedSecondaryVertexBJetTags") > 0.679) {
        hists1d_->Fill("btag_num", embed_jets[i]->pt(), wt);
        if (abs(embed_jets[i]->parton_flavour()) == 5) hists1d_->Fill("real_btag_num", embed_jets[i]->pt(), wt);
      }
    }
    
    std::vector<PFJet*> orig_jets = event->GetPtrVec<PFJet>("pfJetsReco");
    auto pairs = MatchByDR(embed_jets, orig_jets, 0.5, true, true);
    for (unsigned i = 0; i < pairs.size(); ++i) {
      hists_->Fill("csv_orig_embed", 
        pairs[i].second->GetBDiscriminator("combinedSecondaryVertexBJetTags"), 
        pairs[i].first->GetBDiscriminator("combinedSecondaryVertexBJetTags"));
      hists_->Fill("csv_orig_embed_flag", 
        pairs[i].second->GetBDiscriminator("combinedSecondaryVertexBJetTags") > 0.679, 
        pairs[i].first->GetBDiscriminator("combinedSecondaryVertexBJetTags") > 0.679);
    }
    orig_jets = ExtractSecond(pairs);
    for (unsigned i = 0; i < orig_jets.size(); ++i) {
      hists1d_->Fill("orig_btag_den", orig_jets[i]->pt(), wt);
      if (orig_jets[i]->GetBDiscriminator("combinedSecondaryVertexBJetTags") > 0.679) {
        hists1d_->Fill("orig_btag_num", orig_jets[i]->pt(), wt);
      }
    }
    
    return 0;
  }
  int BTagCheck::PostAnalysis() {
    double tot = hists_->Get_Histo("csv_orig_embed_flag")->Integral();
    hists_->Get_Histo("csv_orig_embed_flag")->Scale(1./tot);
    hists1d_->Get_Histo("btag_eff")->Divide(hists1d_->Get_Histo("btag_num"),hists1d_->Get_Histo("btag_den"),1,1,"B");
    hists1d_->Get_Histo("real_btag_eff")->Divide(hists1d_->Get_Histo("real_btag_num"),hists1d_->Get_Histo("real_btag_den"),1,1,"B");
    hists1d_->Get_Histo("orig_btag_eff")->Divide(hists1d_->Get_Histo("orig_btag_num"),hists1d_->Get_Histo("orig_btag_den"),1,1,"B");
    return 0;
  }

  void BTagCheck::PrintInfo() {
    ;
  }
}
