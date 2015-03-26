#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HhhEMuMVA.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/Muon.hh"
#include "TMVA/Reader.h"
#include "Math/VectorUtil.h"
#include "boost/bind.hpp"
#include "boost/format.hpp"
#include "Core/interface/TreeEvent.h"
#include <vector>

namespace ic {

  HhhEMuMVA::HhhEMuMVA(std::string const& name) : ModuleBase(name) {
    ditau_label_ = "emtauCandidates";
    met_label_ = "pfMVAMet";
    gf_mva_file_ = "data/Hhh_mva/SingleBDT_BDTG.weights.xml";
    // vbf_mva_file_ = "data/vbf_mva/HttEmu_vbf_v1.weights.xml";
    gf_reader_ = nullptr;
    // vbf_reader_ = nullptr;
  }

  HhhEMuMVA::~HhhEMuMVA() {
    ;
  }

  int HhhEMuMVA::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HhhEMuMVA" << std::endl;
    std::cout << "-------------------------------------" << std::endl;    
    std::cout << boost::format(param_fmt()) % "dilepton_label"  % ditau_label_;
    std::cout << boost::format(param_fmt()) % "met_label"       % met_label_;
    std::cout << boost::format(param_fmt()) % "gf_mva_file"     % gf_mva_file_;
    // std::cout << boost::format(param_fmt()) % "vbf_mva_file"    % vbf_mva_file_;
    gf_reader_ = new TMVA::Reader("!Color:!Silent:Error");
    // vbf_reader_ = new TMVA::Reader("!Color:!Silent:Error");
    std::vector<TMVA::Reader *> readers = {gf_reader_};
    for (auto & r : readers) {
      r->AddVariable("pt_1", &fpt_1_);
      r->AddVariable("met", &fmet_);
      r->AddVariable("mt_ll", &fmt_ll_);
      r->AddVariable("pzetamiss", &fpzetamiss_);
      r->AddVariable("pt_2", &fpt_2_);
			r->AddVariable("emu_dphi",&femu_dphi_);
      r->AddVariable("pzeta", &fpzeta_);
      // r->AddVariable("d02", &mu_dxy_);
    }
    // vbf_reader_->BookMVA("BDTG", vbf_mva_file_);
    gf_reader_->BookMVA("BDTG", gf_mva_file_);
    return 0;
  }
  
  int HhhEMuMVA::Execute(TreeEvent *event) {
    std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>(ditau_label_);
    CompositeCandidate const* ditau = ditau_vec.at(0);
		Candidate const* lep1 = ditau->GetCandidate("lepton1");
		Candidate const* lep2 = ditau->GetCandidate("lepton2");
    Met const* met = event->GetPtr<Met>(met_label_);
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("pfJetsPFlow"); // Take a copy of the jets
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
    std::sort(jets.begin(), jets.end(), boost::bind(&Candidate::pt, _1) > boost::bind(&Candidate::pt, _2));

    fpzeta_ = (float) PZeta(ditau,met,0.85);
    fpzetamiss_ = (float) PZeta(ditau, met, 0.0);
    femu_dphi_ = (float) std::fabs(ROOT::Math::VectorUtil::DeltaPhi(lep1->vector(), lep2->vector()));
		fpt_1_ = (float) lep1->pt();
		fpt_2_ = (float) lep2->pt();
    fmt_ll_ = (float) MT(ditau, met);
		fmet_ = (float) met->pt();
    event->Add("em_gf_mva", gf_reader_->EvaluateMVA("BDTG"));
    // event->Add("em_vbf_mva", vbf_reader_->EvaluateMVA("BDTG"));
    return 0;
  }

  int HhhEMuMVA::PostAnalysis() {
    if (gf_reader_) delete gf_reader_;
    // if (vbf_reader_) delete vbf_reader_;
    return 0;
  }

  void HhhEMuMVA::PrintInfo() {
    ;
  }

}
