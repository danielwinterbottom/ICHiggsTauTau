#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/BTagCheck.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

namespace ic {

  BTagCheck::BTagCheck(std::string const& name) : ModuleBase(name) {
    fs_ = NULL;
    jet_label_ = "pfJetsPFlow";
    do_legacy_ = true;
  }

  BTagCheck::~BTagCheck() {
    ;
  }

  int BTagCheck::PreAnalysis() {
    if (fs_ && do_legacy_) {
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
    if (fs_ && !do_legacy_){
      double pt_range[16] = {20,30,40,50,60,70,80,100,120,160,210,260,320,400,500,600};
      double eta_range[5] = {0.,0.9,1.2,2.1,2.4};
      hists_ = new Dynamic2DHistoSet(fs_->mkdir("BTagCheck"));
      hists_->Create("NBtag_bflav",15,pt_range,4,eta_range);
      hists_->Create("NTot_bflav",15,pt_range,4,eta_range);
      hists_->Create("NBtag_cflav",15,pt_range,4,eta_range);
      hists_->Create("NTot_cflav",15,pt_range,4,eta_range);
      hists_->Create("NBtag_otherflav",15,pt_range,4,eta_range);
      hists_->Create("NTot_otherflav",15,pt_range,4,eta_range);
      hists_->Create("NBtag_bflav_genmatch",15,pt_range,4,eta_range);
      hists_->Create("NTot_bflav_genmatch",15,pt_range,4,eta_range);
      hists_->Create("NBtag_cflav_genmatch",15,pt_range,4,eta_range);
      hists_->Create("NTot_cflav_genmatch",15,pt_range,4,eta_range);
      hists_->Create("NBtag_otherflav_genmatch",15,pt_range,4,eta_range);
      hists_->Create("NTot_otherflav_genmatch",15,pt_range,4,eta_range);
      outtree_ = fs_->make<TTree>("btageff","btageff");
      outtree_->Branch("wt",&wt); 
      outtree_->Branch("pt",&pt);
      outtree_->Branch("eta",&eta);
      outtree_->Branch("csv_value",&csv);
      outtree_->Branch("jet_flavour",&jet_flavour);
      outtree_->Branch("gen_match",&gen_match);
    }
    return 0;
  }

  int BTagCheck::Execute(TreeEvent *event) {
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    double wt = eventInfo->total_weight();
    std::vector<PFJet*> embed_jets = event->GetPtrVec<PFJet>(jet_label_);
    ic::erase_if(embed_jets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
    std::vector<GenJet*> gen_jets;
    if(!do_legacy_){
      gen_jets = event->GetPtrVec<GenJet>("genJets");
      ic::erase_if(gen_jets, !boost::bind(MinPtMaxEta,_1,8.0,999));
    }
    if(do_legacy_){
      for (unsigned i = 0; i < embed_jets.size(); ++i) {
        hists1d_->Fill("btag_den", embed_jets[i]->pt(), wt);
        if (abs(embed_jets[i]->parton_flavour()) == 5) hists1d_->Fill("real_btag_den", embed_jets[i]->pt(), wt);
        if (embed_jets[i]->GetBDiscriminator("combinedSecondaryVertexBJetTags") > 0.679) {
          hists1d_->Fill("btag_num", embed_jets[i]->pt(), wt);
          if (abs(embed_jets[i]->parton_flavour()) == 5) hists1d_->Fill("real_btag_num", embed_jets[i]->pt(), wt);
        }
      }
    } else {
      for (unsigned i = 0; i<embed_jets.size(); ++i){
        pt = embed_jets[i]->pt();
        eta = embed_jets[i]->eta();
        csv = embed_jets[i]->GetBDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");
        jet_flavour = abs(embed_jets[i]->parton_flavour());
        std::vector<PFJet*> current_jet;
        current_jet.push_back(embed_jets[i]);
        std::vector<std::pair<PFJet*, GenJet*> > gen_jet_match = MatchByDR(current_jet,gen_jets,0.5,true,true);
        if(gen_jet_match.size()>0) gen_match = true; else gen_match = false;
        if(jet_flavour == 5){
          hists_->Fill("NTot_bflav",pt,eta,wt);
          if(gen_match) hists_->Fill("NTot_bflav_genmatch",pt,eta,wt);
          if(csv>0.89){
            hists_->Fill("NBtag_bflav",pt,eta,wt);
            if(gen_match) hists_->Fill("NBtag_bflav_genmatch",pt,eta,wt);
          }
        } else if(jet_flavour == 4){
          hists_->Fill("NTot_cflav",pt,eta,wt);
          if(gen_match) hists_->Fill("NTot_cflav_genmatch",pt,eta,wt);
          if(csv>0.89){
            hists_->Fill("NBtag_cflav",pt,eta,wt);
            if(gen_match) hists_->Fill("NBtag_cflav_genmatch",pt,eta,wt);
          }
        } else {
          hists_->Fill("NTot_otherflav",pt,eta,wt);
          if(gen_match) hists_->Fill("NTot_otherflav_genmatch",pt,eta,wt);
          if(csv>0.89){
            hists_->Fill("NBtag_otherflav",pt,eta,wt);
            if(gen_match) hists_->Fill("NBtag_otherflav_genmatch",pt,eta,wt);
          }
        }
        outtree_->Fill();
      }         
    } 
    
    if(do_legacy_){
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
    }
    
    return 0;
  }
  int BTagCheck::PostAnalysis() {
    if(do_legacy_){
      double tot = hists_->Get_Histo("csv_orig_embed_flag")->Integral();
      hists_->Get_Histo("csv_orig_embed_flag")->Scale(1./tot);
      hists1d_->Get_Histo("btag_eff")->Divide(hists1d_->Get_Histo("btag_num"),hists1d_->Get_Histo("btag_den"),1,1,"B");
      hists1d_->Get_Histo("real_btag_eff")->Divide(hists1d_->Get_Histo("real_btag_num"),hists1d_->Get_Histo("real_btag_den"),1,1,"B");
      hists1d_->Get_Histo("orig_btag_eff")->Divide(hists1d_->Get_Histo("orig_btag_num"),hists1d_->Get_Histo("orig_btag_den"),1,1,"B");
    }
    return 0;
  }

  void BTagCheck::PrintInfo() {
    ;
  }
}
