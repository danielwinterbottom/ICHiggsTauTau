#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttBTagCategory.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

namespace ic {

  HttBTagCategory::HttBTagCategory(std::string const& name, std::string const& jets_label) : ModuleBase(name) {
    jet_pt_ = 30.0;
    jet_eta_ = 4.7;
    btag_jet_pt_ = 20.0;
    btag_jet_eta_ = 2.4;
    mode_ = 0;
    apply_special_mode_ = false;
    special_min_ = 1;
    special_max_ = 1;
    special_pt_ = 20.0;
    special_eta_ = 2.4;

    jets_label_ = jets_label;
    met_label_ = "pfMVAMet";


    fs_ = NULL;

    btag_plots_.resize(4);
    btag_yields_.resize(4);
    btag_loose_plots_.resize(4);
    btag_loose_yields_.resize(4);

    total_btagged_ = 0.0;
    pu_id_btagged_ = 0.0;
    total_btagged_real_ = 0.0;
    pu_id_btagged_real_ = 0.0;
    do_btag_weight_ = false;

  }

  HttBTagCategory::~HttBTagCategory() {
    ;
  }

  int HttBTagCategory::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for HTT btag Category" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Jets: [" << jets_label_ << "]" << std::endl;
    if (!apply_special_mode_) {
      std::cout << "b-Jet: [Pt > " << btag_jet_pt_ << "] [|eta| < " << btag_jet_eta_ << "]" << std::endl;
      std::cout << "Jet Veto: [Pt > " << jet_pt_ << "] [|Eta| < " << jet_eta_ << "]" << std::endl;
    } else {
      std::cout << "SPECIAL MODE ENABLED: Require [" << special_min_ << "," << special_max_ << "] jets" << std::endl;
      std::cout << "Jets: [Pt > " << special_pt_ << "] [|Eta| < " << special_eta_ << "]" << std::endl;
    }

    std::string pt_cat_string = "btag";
    btag_plots_[0] = new HttPlots(fs_->mkdir(pt_cat_string+"_os_sel"));
    btag_plots_[1] = new HttPlots(fs_->mkdir(pt_cat_string+"_ss_sel"));
    btag_plots_[2] = new HttPlots(fs_->mkdir(pt_cat_string+"_os_con"));
    btag_plots_[3] = new HttPlots(fs_->mkdir(pt_cat_string+"_ss_con"));

    btag_loose_plots_[0] = new HttPlots(fs_->mkdir(pt_cat_string+"_loose_os_sel"));
    btag_loose_plots_[1] = new HttPlots(fs_->mkdir(pt_cat_string+"_loose_ss_sel"));
    btag_loose_plots_[2] = new HttPlots(fs_->mkdir(pt_cat_string+"_loose_os_con"));
    btag_loose_plots_[3] = new HttPlots(fs_->mkdir(pt_cat_string+"_loose_ss_con"));

    return 0;
  }

  int HttBTagCategory::Execute(TreeEvent *event) {
    std::map<std::string, bool> cat_status;
    if (event->Exists("cat_status")) {
      cat_status = event->Get< std::map<std::string, bool> >("cat_status");
    }
    std::string cat_label = "btag";
    cat_status[cat_label] = false;

    std::vector<CompositeCandidate*> const& dilepton = event->GetPtrVec<CompositeCandidate>("emtauCandidates");


    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    double wt = eventInfo->total_weight();
    unsigned sel_mode = event->Get<unsigned>("sel_mode");
    Met const* pfMVAMet = event->GetPtr<Met>(met_label_);
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_); // Make a copy of the jet collection

    std::vector<PFJet*> main_jets = jets;
    std::vector<PFJet*> btag_jets = jets;
    // Always veto > 1 main jets
    ic::erase_if(main_jets,!boost::bind(MinPtMaxEta, _1, jet_pt_, jet_eta_));
    if (main_jets.size() > 1) {
      event->ForceAdd("cat_status", cat_status);
      return 2;
    }

    if (apply_special_mode_) {
      ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, special_pt_, special_eta_));
      if (jets.size() >= special_min_ && jets.size() <= special_max_) {
        if (fs_ && sel_mode < 4) {
          btag_yields_[sel_mode] += wt;
          btag_plots_[sel_mode]->FillLeptonMetPlots(*(dilepton[0]), *pfMVAMet, wt);
          btag_plots_[sel_mode]->FillPt20JetPlots(jets, wt, 1.0);
          if (event->Exists("svfitMass")) btag_plots_[sel_mode]->FillSVFitMassPlot(event->Get<double>("svfitMass"),wt);
        }
        if (fs_ && sel_mode%2 == 0) btag_plots_[0]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
        if (fs_ && sel_mode%2 == 1) btag_plots_[1]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
        if (fs_ && sel_mode == 2) btag_plots_[2]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
        if (fs_ && sel_mode == 3) btag_plots_[3]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
        cat_status[cat_label] = true;
        event->ForceAdd("cat_status", cat_status);
        return 0;
      } else {
        event->ForceAdd("cat_status", cat_status);
        return 2;
      }
    }


    ic::erase_if(btag_jets,!boost::bind(MinPtMaxEta, _1, btag_jet_pt_, btag_jet_eta_));

    ic::erase_if(btag_jets, boost::bind(&PFJet::GetBDiscriminator, _1, "combinedSecondaryVertexBJetTags") < 0.244);
    if (btag_jets.size() > 0) {
      if (fs_ && sel_mode < 4) {
        btag_loose_yields_[sel_mode] += wt;
        std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_); // Make a copy of the jet collection
        btag_loose_plots_[sel_mode]->FillLeptonMetPlots(*(dilepton[0]), *pfMVAMet, wt);
        btag_loose_plots_[sel_mode]->FillJetPlots(jets, wt);
        btag_loose_plots_[sel_mode]->FillPt20JetPlots(btag_jets, wt, 1.0);
        if (event->Exists("svfitMass")) btag_loose_plots_[sel_mode]->FillSVFitMassPlot(event->Get<double>("svfitMass"),wt);
      }
      if (fs_ && sel_mode%2 == 0) btag_loose_plots_[0]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
      if (fs_ && sel_mode%2 == 1) btag_loose_plots_[1]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
      if (fs_ && sel_mode == 2) btag_loose_plots_[2]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
      if (fs_ && sel_mode == 3) btag_loose_plots_[3]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
    }


    ic::erase_if(btag_jets, boost::bind(&PFJet::GetBDiscriminator, _1, "combinedSecondaryVertexBJetTags") < 0.679);
    if (btag_jets.size() == 0) {
      event->ForceAdd("cat_status", cat_status);
      return 2;
    } else {
      if (do_btag_weight_) wt *= event->Get<double>("inclusive_btag_weight");
    }



    // if (sel_mode == 0 && btag_jets.size() == 1) {
    //   total_btagged_ += (double(btag_jets.size()) * wt);
    //   std::vector<GenJet *> & genjets = event->GetPtrVec<GenJet>("genJets");
    //   ic::erase_if(genjets,!boost::bind(MinPtMaxEta, _1, 10.0, 5.0));
    //   std::vector<std::pair<PFJet*, GenJet*> > matches = MatchByDR(btag_jets, genjets, 0.5, true, true);
    //   if (matches.size() > 0) total_btagged_real_ += (double(btag_jets.size()) * wt);
    //   ic::erase_if(btag_jets, !boost::bind(&PFJet::pu_id_mva_loose, _1));
    //   pu_id_btagged_ += (double(btag_jets.size()) * wt);
    //   if (matches.size() > 0) pu_id_btagged_real_ += (double(btag_jets.size()) * wt);
    // }
    //total_btagged_ += btag_jets.size();
    //pu_id_btagged_ += btag_jets.size();


    if (fs_ && sel_mode < 4) {
      btag_yields_[sel_mode] += wt;
      std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_); // Make a copy of the jet collection
      btag_plots_[sel_mode]->FillLeptonMetPlots(*(dilepton[0]), *pfMVAMet, wt);
      btag_plots_[sel_mode]->FillJetPlots(jets, wt);
      btag_plots_[sel_mode]->FillPt20JetPlots(jets, wt, 1.0);
      if (event->Exists("svfitMass")) btag_plots_[sel_mode]->FillSVFitMassPlot(event->Get<double>("svfitMass"),wt);
    }
    if (fs_ && sel_mode%2 == 0) btag_plots_[0]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
    if (fs_ && sel_mode%2 == 1) btag_plots_[1]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
    if (fs_ && sel_mode == 2) btag_plots_[2]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
    if (fs_ && sel_mode == 3) btag_plots_[3]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);


    cat_status[cat_label] = true;
    event->ForceAdd("cat_status", cat_status);
    return 0;
  }
  int HttBTagCategory::PostAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Post-Analysis Info for HTT btag Category" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << boost::format("%-15s %-10s %-10s %-10s %-10s\n") 
      % "Sel Mode:" % "OS-Sel" % "SS-Sel" % "OS-Con" % "SS-Con";
    std::cout << boost::format("%-15s %-10s %-10s %-10s %-10s\n") 
      % (std::string("btag")) % btag_yields_[0] % btag_yields_[1] % btag_yields_[2] % btag_yields_[3];
    // PrintEff("Total Eff", pu_id_btagged_, total_btagged_);
    // PrintEff("Real Eff", pu_id_btagged_real_, total_btagged_real_);
    // PrintEff("PU Eff", pu_id_btagged_ - pu_id_btagged_real_, total_btagged_ - total_btagged_real_);
    // PrintEff("Purity Before", total_btagged_real_, total_btagged_);
    // PrintEff("Purity After", pu_id_btagged_real_, pu_id_btagged_);
    return 0;
  }

  void HttBTagCategory::PrintInfo() {
    ;
  }
}
