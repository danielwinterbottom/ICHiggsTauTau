#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTEMuMVA.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "TMVA/Reader.h"
#include "Math/VectorUtil.h"
#include "boost/bind.hpp"
#include <vector>

namespace ic {

  HTTEMuMVA::HTTEMuMVA(std::string const& name) : ModuleBase(name) {
    ditau_label_ = "emtauCandidates";
    met_label_ = "pfMVAMet";
    gf_mva_file_ = "data/vbf_mva/HttEmu_gf_v0.weights.xml";
    vbf_mva_file_ = "data/vbf_mva/HttEmu_vbf_v0.weights.xml";
    gf_reader_ = nullptr;
    vbf_reader_ = nullptr;
  }

  HTTEMuMVA::~HTTEMuMVA() {
    ;
  }

  int HTTEMuMVA::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HTTEMuMVA" << std::endl;
    std::cout << "-------------------------------------" << std::endl;    
    std::cout << boost::format(param_fmt()) % "dilepton_label"  % ditau_label_;
    std::cout << boost::format(param_fmt()) % "met_label"       % met_label_;
    std::cout << boost::format(param_fmt()) % "gf_mva_file"     % gf_mva_file_;
    std::cout << boost::format(param_fmt()) % "vbf_mva_file"    % vbf_mva_file_;
    gf_reader_ = new TMVA::Reader("!Color:!Silent:Error");
    vbf_reader_ = new TMVA::Reader("!Color:!Silent:Error");
    std::vector<TMVA::Reader *> readers = {gf_reader_, vbf_reader_};
    for (auto & r : readers) {
      r->AddVariable("pzetavis", &pzetavis_);
      r->AddVariable("pzetamiss", &pzetamiss_);
      r->AddVariable("dphi", &dphi_);
      r->AddVariable("mvamet", &mvamet_);
      r->AddVariable("csv", &csv_);
      r->AddVariable("d01", &el_dxy_);
      r->AddVariable("d02", &mu_dxy_);
    }
    vbf_reader_->BookMVA("BDTG", vbf_mva_file_);
    gf_reader_->BookMVA("BDTG", gf_mva_file_);
    return 0;
  }
  
  int HTTEMuMVA::Execute(TreeEvent *event) {
    std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>(ditau_label_);
    CompositeCandidate const* ditau = ditau_vec.at(0);
    Met const* met = event->GetPtr<Met>(met_label_);
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("pfJetsPFlow"); // Take a copy of the jets
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
    std::sort(jets.begin(), jets.end(), boost::bind(&Candidate::pt, _1) > boost::bind(&Candidate::pt, _2));

    pzetavis_ = PZetaVis(ditau);
    pzetamiss_ = PZeta(ditau, met, 0.0);
    dphi_ = std::fabs(ROOT::Math::VectorUtil::DeltaPhi(ditau->At(0)->vector(), ditau->At(1)->vector()));
    mvamet_ = met->pt();
    csv_ = (jets.size() > 0) ? jets[0]->GetBDiscriminator("combinedSecondaryVertexBJetTags") : -1.0;
    el_dxy_ = -1. * dynamic_cast<Electron const*>(ditau->GetCandidate("lepton1"))->dxy_vertex();
    mu_dxy_ = -1. * dynamic_cast<Muon const*>(ditau->GetCandidate("lepton2"))->dxy_vertex();
    event->Add("em_gf_mva", gf_reader_->EvaluateMVA("BDTG"));
    event->Add("em_vbf_mva", vbf_reader_->EvaluateMVA("BDTG"));
    return 0;
  }

  int HTTEMuMVA::PostAnalysis() {
    if (gf_reader_) delete gf_reader_;
    if (vbf_reader_) delete vbf_reader_;
    return 0;
  }

  void HTTEMuMVA::PrintInfo() {
    ;
  }

}
