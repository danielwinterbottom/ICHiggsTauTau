#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttSelection.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

#include "TMVA/Reader.h"
#include "TVector3.h"

namespace ic {

  HttSelection::HttSelection(std::string const& name) : ModuleBase(name) {
    dilepton_label_ = "emtauCandidates";
    met_label_ = "pfMVAMet";
    mt_max_selection_ = 20.0;
    mt_min_control_ = 70.0;
    mt_max_control_ = 9999999.;
    pzeta_min_selection_ = -20.0;
    pzeta_max_control_ = -40.0;
    pzeta_alpha_ = 0.85;
    mode_ = 0;
    distinguish_os_ = true;
    fs_ = NULL;
    mssm_mode_ = 0;

    dilepton_plots_.resize(4);
    dilepton_yields_.resize(6);

    inclusive_plots_.resize(4);
    inclusive_yields_.resize(6);
  }

  HttSelection::~HttSelection() {
    ;
  }

  int HttSelection::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for HTT Selection" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    if (fs_) {
      std::cout << "Mode: " << mode_ << std::endl;
      std::cout << "Dilepton Label: " << dilepton_label_ << std::endl;
      std::cout << "MET Label: " << met_label_ << std::endl;
      if (mssm_mode_ == 0) {
        if (mode_ == 0 || mode_ == 1) {
          std::cout << "- Selection region requires mT < " << mt_max_selection_ << std::endl;
          std::cout << "- Control region requires " << mt_min_control_ << " < mT < " << mt_max_control_ << std::endl;
        } else if (mode_ == 2) {
          std::cout << "- Selection region requires PZeta > " << pzeta_min_selection_ << std::endl;
          std::cout << "- Using alpha = " << pzeta_alpha_ << std::endl;
        }
      } else {
        if (mode_ == 0 || mode_ == 1) {
          std::cout << "- Selection region requires PZeta > " << pzeta_min_selection_ << std::endl;
          std::cout << "- Control region requires PZeta < " << pzeta_max_control_ << std::endl;
          std::cout << "- Using alpha = " << pzeta_alpha_ << std::endl;
        } else if (mode_ == 2) {
          std::cout << "- Selection region requires PZeta > " << pzeta_min_selection_ << std::endl;
          std::cout << "- Using alpha = " << pzeta_alpha_ << std::endl;
        }
      }
      if (!distinguish_os_) {
        std::cout << "- WARNING, Not distinguishing OS/SS" << std::endl;
      }
      dilepton_plots_[0] = new HttPlots(fs_->mkdir("dilepton_os_sel"));
      dilepton_plots_[1] = new HttPlots(fs_->mkdir("dilepton_ss_sel"));
      dilepton_plots_[2] = new HttPlots(fs_->mkdir("dilepton_os_con"));
      dilepton_plots_[3] = new HttPlots(fs_->mkdir("dilepton_ss_con"));

      inclusive_plots_[0] = new HttPlots(fs_->mkdir("inclusive_os_sel"));
      inclusive_plots_[1] = new HttPlots(fs_->mkdir("inclusive_ss_sel"));
      inclusive_plots_[2] = new HttPlots(fs_->mkdir("inclusive_os_con"));
      inclusive_plots_[3] = new HttPlots(fs_->mkdir("inclusive_ss_con"));
    }
    return 0;
  }

  int HttSelection::Execute(TreeEvent *event) {

    // Get the stuff we need from the event
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    double wt = eventInfo->total_weight();
    std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>(dilepton_label_);
    Met const* pfMetMVA = event->GetPtr<Met>(met_label_);
    
    // If we are distinguishing OS/SS, set the sel_mode to 0 for OS, 1 for SS.
    // Otherwise leave as 0
    unsigned sel_mode = 0; 
    if (distinguish_os_ && (!PairOppSign(dilepton[0]))) sel_mode = 1;

    // Determine if event is in control region for ETau or MuTau channels
    // No control region for EMu
    double mt = MT(dilepton[0]->GetCandidate("lepton1"),pfMetMVA);
    double pzeta = PZeta(dilepton.at(0), pfMetMVA, pzeta_alpha_);
    bool in_control = false;
    if (mssm_mode_ == 0) {
      if (mode_ == 0 || mode_ == 1) {
        if (mt > mt_min_control_ && mt < mt_max_control_) in_control = true;
      } else if (mode_ == 2) {
        in_control = false;
      }
    } else if (mssm_mode_ > 0) {
      // if (mode_ == 0 || mode_ == 1) {
      //   if (pzeta < pzeta_max_control_) in_control = true;
      if (mode_ == 0 || mode_ == 1) {
        if (mt > mt_min_control_ && mt < mt_max_control_) in_control = true;
      } else if (mode_ == 2) {
        in_control = false;
      }
    }

      std::vector<PFJet*> const& jets = event->GetPtrVec<PFJet>("pfJetsPFlow");
    // Special case, for the dilepton selection the selection region is inclusive 
    // in mt
    dilepton_yields_[sel_mode] += wt;
    if (fs_) {
      dilepton_plots_[sel_mode]->FillVertexPlots(eventInfo->good_vertices(), wt);
      dilepton_plots_[sel_mode]->FillLeptonMetPlots(*(dilepton[0]), *pfMetMVA, wt);
      dilepton_plots_[sel_mode]->FillJetPlotsDummy(jets, wt);
      dilepton_plots_[sel_mode]->FillPt20JetPlotsDummy(jets, wt);
      dilepton_plots_[sel_mode]->FillVbfMvaPlotsDummy(wt);
      dilepton_plots_[sel_mode]->FillBEfficiency(jets, wt);
      if (event->Exists("genParticles")) {
        std::vector<GenParticle *> parts = event->GetPtrVec<GenParticle>("genParticles");
        dilepton_plots_[sel_mode]->FillNPartons(parts, wt);
      } 
      if (sel_mode%2 == 0) dilepton_plots_[0]->FillIncMT(*(dilepton[0]), *pfMetMVA, wt);
      if (sel_mode%2 == 1) dilepton_plots_[1]->FillIncMT(*(dilepton[0]), *pfMetMVA, wt);

      if (event->Exists("svfitMass")) {
        dilepton_plots_[sel_mode]->FillSVFitMassModePlots(*(dilepton[0]), (event->Get<double>("svfitMass")), wt);
        dilepton_plots_[sel_mode]->FillSVFitMassPlot(event->Get<double>("svfitMass"),wt);
      }
    }
    if (in_control) {
      dilepton_yields_[sel_mode + 2] += wt;
      dilepton_plots_[sel_mode + 2]->FillVertexPlots(eventInfo->good_vertices(), wt);
      dilepton_plots_[sel_mode + 2]->FillLeptonMetPlots(*(dilepton[0]), *pfMetMVA, wt);
      dilepton_plots_[sel_mode + 2]->FillJetPlotsDummy(jets, wt);
      dilepton_plots_[sel_mode + 2]->FillPt20JetPlotsDummy(jets, wt);
      dilepton_plots_[sel_mode + 2]->FillVbfMvaPlotsDummy(wt);
      if (sel_mode%2 == 0) dilepton_plots_[sel_mode + 2]->FillIncMT(*(dilepton[0]), *pfMetMVA, wt);
      if (sel_mode%2 == 1) dilepton_plots_[sel_mode + 2]->FillIncMT(*(dilepton[0]), *pfMetMVA, wt);

      if (event->Exists("svfitMass")) {
        dilepton_plots_[sel_mode + 2]->FillSVFitMassPlot(event->Get<double>("svfitMass"),wt);
      }
    }
    

    // Now define the actual selection mode
    // 0 = OS, Selection
    // 1 = SS, Selection
    // 2 = OS, Control
    // 3 = SS, Control
    // if (mssm_mode_ == 0) {
    //   if ( (mode_ == 0 || mode_ == 1) && mt >= mt_max_selection_ && (mt <= mt_min_control_ || mt >= mt_max_control_)) return 1;
    //   if (  mode_ == 2 && pzeta <= pzeta_min_selection_ ) return 1;
    // } else {
    //   //if ( (mode_ == 0 || mode_ == 1) && pzeta >= pzeta_max_control_ && pzeta <= pzeta_min_selection_) return 1;
    //   if ( (mode_ == 0 || mode_ == 1) && mt >= mt_max_selection_ && (mt <= mt_min_control_ || mt >= mt_max_control_)) return 1;
    //   if (  mode_ == 2 && pzeta <= pzeta_min_selection_ ) return 1;
    // }
    bool in_signal = false;
    if (mssm_mode_ == 0) {
      if ( (mode_ == 0 || mode_ == 1) && mt < mt_max_selection_) in_signal = true;
      if (  mode_ == 2 && pzeta > pzeta_min_selection_ ) in_signal = true;
    } else {
      if ( (mode_ == 0 || mode_ == 1) && mt < mt_max_selection_) in_signal = true;;
      if (  mode_ == 2 && pzeta > pzeta_min_selection_ ) in_signal = true;
    }

    if (in_control) sel_mode = sel_mode + 2;
    if (!in_control && !in_signal) sel_mode = sel_mode + 4;
    inclusive_yields_[sel_mode] += wt;
    if (fs_ && sel_mode < 4) {
      inclusive_plots_[sel_mode]->FillVertexPlots(eventInfo->good_vertices(), wt);
      inclusive_plots_[sel_mode]->FillLeptonMetPlots(*(dilepton[0]), *pfMetMVA, wt);
      std::vector<PFJet*> const& jets = event->GetPtrVec<PFJet>("pfJetsPFlow");
      inclusive_plots_[sel_mode]->FillJetPlots(jets, wt);
      double btag_weight = 1.0;
      if (event->Exists("inclusive_btag_weight")) btag_weight = event->Get<double>("inclusive_btag_weight");
      inclusive_plots_[sel_mode]->FillPt20JetPlots(jets, wt, btag_weight);
      inclusive_plots_[sel_mode]->FillVbfMvaPlotsDummy(wt);
      if (event->Exists("svfitMass")) {
        inclusive_plots_[sel_mode]->FillSVFitMassModePlots(*(dilepton[0]), (event->Get<double>("svfitMass")), wt);
        inclusive_plots_[sel_mode]->FillSVFitMassPlot(event->Get<double>("svfitMass"),wt);
      }
    }
    if (fs_ && sel_mode%2 == 0) inclusive_plots_[0]->FillIncMT(*(dilepton[0]), *pfMetMVA, wt);
    if (fs_ && sel_mode%2 == 1) inclusive_plots_[1]->FillIncMT(*(dilepton[0]), *pfMetMVA, wt);
    if (fs_ && sel_mode == 2) inclusive_plots_[2]->FillIncMT(*(dilepton[0]), *pfMetMVA, wt);
    if (fs_ && sel_mode == 3) inclusive_plots_[3]->FillIncMT(*(dilepton[0]), *pfMetMVA, wt);

    event->Add("sel_mode", sel_mode);
    
    return 0;
  }
  int HttSelection::PostAnalysis() {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Post-Analysis Info for HTT Selection" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << boost::format("%-15s %-10s %-10s %-10s %-10s %-10s %-10s\n") 
          % "Sel Mode:" % "OS-Sel" % "SS-Sel" % "OS-Con" % "SS-Con" % "OS-Out" % "SS-Out";
        std::cout << boost::format("%-15s %-10s %-10s %-10s %-10s %-10s %-10s\n") 
          % "Dilepton:" % dilepton_yields_[0] % dilepton_yields_[1] % dilepton_yields_[2] % dilepton_yields_[3] % dilepton_yields_[4] % dilepton_yields_[5];
        std::cout << boost::format("%-15s %-10s %-10s %-10s %-10s %-10s %-10s\n") 
          % "Inclusive:" % inclusive_yields_[0] % inclusive_yields_[1] % inclusive_yields_[2] % inclusive_yields_[3] % inclusive_yields_[4] % inclusive_yields_[5];

    return 0;
  }

  void HttSelection::PrintInfo() {
    ;
  }
}
