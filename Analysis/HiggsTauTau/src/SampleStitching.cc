#include "HiggsTauTau/interface/SampleStitching.h"
#include "Objects/interface/EventInfo.hh"
#include "Objects/interface/GenParticle.hh"
#include "Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnPairs.h"
#include "TFile.h"

namespace ic {

DYJetsStitching::DYJetsStitching(std::string const& name)
    : ModuleBase(name),
      lumi_DYJetsToLL_(0),
      lumi_DY1JetsToLL_(0),
      lumi_DY2JetsToLL_(0),
      lumi_DY3JetsToLL_(0),
      lumi_DY4JetsToLL_(0),
      lumi_DYJetsToTauTau_M150_(0) {
  fs_ = nullptr;
}

DYJetsStitching::~DYJetsStitching() { ; }

int DYJetsStitching::PreAnalysis() {
  PrintHeader("DYJetsStitching");
  PrintArg("lumi_DYJetsToLL", lumi_DYJetsToLL_);
  PrintArg("lumi_DY1JetsToLL", lumi_DY1JetsToLL_);
  PrintArg("lumi_DY2JetsToLL", lumi_DY2JetsToLL_);
  PrintArg("lumi_DY3JetsToLL", lumi_DY3JetsToLL_);
  PrintArg("lumi_DY4JetsToLL", lumi_DY4JetsToLL_);
  PrintArg("lumi_DYJetsToTauTau_M150", lumi_DYJetsToTauTau_M150_);

  if (fs_) {
    t_gen_info_ = fs_->make<TTree>("dyjets_stitching", "dyjets_stitching");
    t_gen_info_->Branch("decay", &decay_);
    t_gen_info_->Branch("mll", &mll_);
    t_gen_info_->Branch("njets", &njets_);
    t_gen_info_->Branch("wt", &wt_);
  }
  return 0;
}

int DYJetsStitching::Execute(TreeEvent* event) {
  EventInfo* eventInfo = event->GetPtr<EventInfo>("eventInfo");

  njets_ = 0;
  decay_ = 0;
  mll_ = 0.;
  wt_ = 0.;

  std::vector<GenParticle*> const& lhe_parts =
      event->GetPtrVec<GenParticle>("lheParticles");

  std::vector<GenParticle*> zll_cands;
  for (unsigned i = 0; i < lhe_parts.size(); ++i) {
    if (lhe_parts[i]->status() != 1) continue;
    unsigned id = abs(lhe_parts[i]->pdgid());
    if ((id >= 1 && id <= 6) || id == 21) ++njets_;
    if (id == 11 || id == 13 || id == 15) zll_cands.push_back(lhe_parts[i]);
  }

  if (njets_ > 4) {
    std::cerr << "Error: event has " << njets_ << " partons\n";
    throw std::runtime_error("DYJetsStitching error");
  }

  if (zll_cands.size() == 2) {
    mll_ = (zll_cands[0]->vector() + zll_cands[1]->vector()).M();
  } else {
    std::cerr << "Error: event has " << zll_cands.size()
              << " Z->ll candidates, 2 expected\n";
    throw std::runtime_error("DYJetsStitching error");
  }
  if (std::abs(zll_cands[0]->pdgid()) == 11 &&
      std::abs(zll_cands[1]->pdgid()) == 11) {
    decay_ = 0;
  } else if (std::abs(zll_cands[0]->pdgid()) == 13 &&
             std::abs(zll_cands[1]->pdgid()) == 13) {
    decay_ = 1;
  } else if (std::abs(zll_cands[0]->pdgid()) == 15 &&
             std::abs(zll_cands[1]->pdgid()) == 15) {
    decay_ = 2;
  } else {
    std::cerr << "Error: incorrect flavour of Z->ll leptons\n";
    throw std::runtime_error("DYJetsStitching error");
  }

  double target_lumi = lumi_DYJetsToLL_;
  double stitched_lumi = target_lumi;

  if (njets_ == 1) stitched_lumi += lumi_DY1JetsToLL_;
  if (njets_ == 2) stitched_lumi += lumi_DY2JetsToLL_;
  if (njets_ == 3) stitched_lumi += lumi_DY3JetsToLL_;
  if (njets_ == 4) stitched_lumi += lumi_DY4JetsToLL_;
  if (decay_ == 2 && mll_ > 150.) stitched_lumi += lumi_DYJetsToTauTau_M150_;

  wt_ = target_lumi / stitched_lumi;

  if (fs_) {
    t_gen_info_->Fill();
  }

  eventInfo->set_weight("dysoup", wt_);

  return 0;
  }

  int DYJetsStitching::PostAnalysis() {
    return 0;
  }

  void DYJetsStitching::PrintInfo() {
    ;
  }


  WJetsStitching::WJetsStitching(std::string const& name)
      : ModuleBase(name),
        lumi_WJetsToLNu_(0),
        lumi_W1JetsToLNu_(0),
        lumi_W2JetsToLNu_(0),
        lumi_W3JetsToLNu_(0),
        lumi_W4JetsToLNu_(0) {
    fs_ = nullptr;
  }

  WJetsStitching::~WJetsStitching() { ; }

  int WJetsStitching::PreAnalysis() {
    PrintHeader("WJetsStitching");
    PrintArg("lumi_WJetsToLNu", lumi_WJetsToLNu_);
    PrintArg("lumi_W1JetsToLNu", lumi_W1JetsToLNu_);
    PrintArg("lumi_W2JetsToLNu", lumi_W2JetsToLNu_);
    PrintArg("lumi_W3JetsToLNu", lumi_W3JetsToLNu_);
    PrintArg("lumi_W4JetsToLNu", lumi_W4JetsToLNu_);

    if (fs_) {
      t_gen_info_ = fs_->make<TTree>("dyjets_stitching", "dyjets_stitching");
      t_gen_info_->Branch("njets", &njets_);
      t_gen_info_->Branch("wt", &wt_);
    }
    return 0;
  }

  int WJetsStitching::Execute(TreeEvent* event) {
    EventInfo* eventInfo = event->GetPtr<EventInfo>("eventInfo");

    njets_ = 0;
    wt_ = 0.;

    std::vector<GenParticle*> const& lhe_parts =
        event->GetPtrVec<GenParticle>("lheParticles");

    for (unsigned i = 0; i < lhe_parts.size(); ++i) {
      if (lhe_parts[i]->status() != 1) continue;
      unsigned id = abs(lhe_parts[i]->pdgid());
      if ((id >= 1 && id <= 6) || id == 21) ++njets_;
    }

    if (njets_ > 4) {
      std::cerr << "Error: event has " << njets_ << " partons\n";
      throw std::runtime_error("WJetsStitching error");
    }

    double target_lumi = lumi_WJetsToLNu_;
    double stitched_lumi = target_lumi;

    if (njets_ == 1) stitched_lumi += lumi_W1JetsToLNu_;
    if (njets_ == 2) stitched_lumi += lumi_W2JetsToLNu_;
    if (njets_ == 3) stitched_lumi += lumi_W3JetsToLNu_;
    if (njets_ == 4) stitched_lumi += lumi_W4JetsToLNu_;

    wt_ = target_lumi / stitched_lumi;

    if (fs_) {
      t_gen_info_->Fill();
    }

    eventInfo->set_weight("wsoup", wt_);

    return 0;
    }

    int WJetsStitching::PostAnalysis() {
      return 0;
    }

    void WJetsStitching::PrintInfo() {
      ;
    }
}
