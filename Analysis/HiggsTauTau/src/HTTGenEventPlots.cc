#include "HiggsTauTau/interface/HTTGenEventPlots.h"
#include "Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnPairs.h"
#include "HiggsTauTau/interface/HTTGenEvent.h"
#include "UserCode/ICHiggsTauTau/interface/Track.hh"

namespace ic {

HTTGenEventPlots::HTTGenEventPlots(std::string const& name) : ModuleBase(name) {

}

HTTGenEventPlots::~HTTGenEventPlots() { ; }

int HTTGenEventPlots::PreAnalysis() {
  TH1::SetDefaultSumw2(true);
  using ROOT::Math::Pi;
  if (!fs_) return 0;
  dir_ = new TFileDirectory(fs_->mkdir("HTTGenEventPlots"));
  h_gen_h_pt = dir_->make<TH1F>("gen_h_pt", "",     100, 0, 500);
  h_gen_h_eta = dir_->make<TH1F>("gen_h_eta", "",   50, -5, 5);
  h_gen_h_phi = dir_->make<TH1F>("gen_h_phi", "",   50, -Pi(), Pi());
  h_gen_h_mass = dir_->make<TH1F>("gen_h_mass", "", 200, 0, 2000);

  h_gen_th_pt = dir_->make<TH1F>("gen_th_pt", "", 50, 0, 200);
  h_gen_th_eta = dir_->make<TH1F>("gen_th_eta", "", 30, -5, 5);
  h_gen_th_mode = dir_->make<TH1F>("gen_th_mode", "", 20, -0.5, 19.5);

  return 0;
}


int HTTGenEventPlots::Execute(TreeEvent* event) {
  auto const& gevt = event->Get<GenEvent_XToTauTau>("genEvent_XToTauTau");
  if (gevt.boson) {
    h_gen_h_pt->Fill(gevt.boson->pt());
    h_gen_h_eta->Fill(gevt.boson->eta());
    h_gen_h_phi->Fill(gevt.boson->phi());
    h_gen_h_mass->Fill(gevt.boson->M());
  }

  // A vector of the GenEvent infos for any hadronic tau decays
  std::vector<GenEvent_Tau const*> gen_th_vec;
  // A vector of the visible hadronic tau jets (only really needed as input to
  // the matching algorithm)
  std::vector<GenJet const*> gen_th_vis_vec;
  if (gevt.tau_0.hadronic_mode >= 0) gen_th_vec.push_back(&(gevt.tau_0));
  if (gevt.tau_1.hadronic_mode >= 0) gen_th_vec.push_back(&(gevt.tau_1));
  for (auto th : gen_th_vec) gen_th_vis_vec.push_back(th->vis_jet);

  // Now loop through the hadronic taus
  for (auto gen_th : gen_th_vec) {
    // Get the visible jet
    GenJet const* gen_th_vis = gen_th->vis_jet;
    h_gen_th_pt->Fill(gen_th_vis->pt());
    h_gen_th_eta->Fill(gen_th_vis->eta());
    h_gen_th_mode->Fill(gen_th->hadronic_mode);
  }
  return 0;
}


int HTTGenEventPlots::PostAnalysis() { return 0; }

void HTTGenEventPlots::PrintInfo() { ; }
}
