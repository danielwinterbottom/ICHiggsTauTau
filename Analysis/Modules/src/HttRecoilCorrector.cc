#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttRecoilCorrector.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

#include "TMVA/Reader.h"
#include "TVector3.h"

namespace ic {

  HttRecoilCorrector::HttRecoilCorrector(std::string const& name) : ModuleBase(name) {
    dilepton_label_ = "emtauCandidates";
    met_label_ = "pfMVAMet";
    jets_label_ = "pfJetsPFlow";
    sample_ = "";
    mode_ = 0;
    disable = true;
    is_2012_ = true;
    is_wjets_ = false;
    era_ = 2;
  }

  HttRecoilCorrector::~HttRecoilCorrector() {
    ;
  }

  int HttRecoilCorrector::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for HTT Recoil Corrector" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Using MET: " << met_label_ << std::endl;
    std::cout << "Era: " << era_ << std::endl;
    std::string process_file;
    std::string data_file;
    std::string mc_file;
    if (is_2012_) { // (strategy == "HCP_2012" || strategy == "MORIOND_2013") && (data_era == "ICHEP_2012_53X || HCP_2012_53X "
      data_file = "data/recoilfits/recoilfit_datamm53X_20pv_njet.root";
      mc_file = "data/recoilfits/recoilfit_zmm53X_20pv_njet.root";
    } else {
      data_file = "data/recoilfits/recoilfit_datamm42X_20pv_njet.root";
      mc_file = "data/recoilfits/recoilfit_zmm42X_20pv_njet.root";
      if (era_ == 1) {
        data_file = "data/recoilfits/recoilfit_datamm_njet.root";
        mc_file = "data/recoilfits/recoilfit_zmm42X_njet.root";
      }
    }


    if ( (sample_.find("WJetsToLNu") != sample_.npos) ) {
      disable = false;
      is_wjets_ = true;
      if (is_2012_) {
        process_file = "data/recoilfits/recoilfit_wjets53X_20pv_njet.root";
      } else {
        process_file = "data/recoilfits/recoilfit_wjets42X_20pv_njet.root";
        if (era_ == 1) {
          process_file = "data/recoilfits/recoilfit_wjets_njet.root";
        }
      }
      std::cout << "W+Jets sample detected, using process file: " << process_file << std::endl;
      boson_id_.push_back(24);
    }

    if ( (sample_.find("GluGluToHToTauTau") != sample_.npos) 
      || (sample_.find("SUSYGluGluToHToTauTau") != sample_.npos)
      || (sample_.find("SUSYBBHToTauTau") != sample_.npos) 
      || (sample_.find("VBF_HToTauTau") != sample_.npos)  
      || (sample_.find("WH_ZH_TTH_HToTauTau") != sample_.npos) ) {
      disable = false;
      if (is_2012_) {
        process_file = "data/recoilfits/recoilfit_higgs53X_20pv_njet.root";
      } else {
        process_file = "data/recoilfits/recoilfit_higgs42X_20pv_njet.root";
        if (mode_ == 2) process_file = "data/recoilfits/recoilfit_higgsem42X_20pv_njet.root";
        if (era_ == 1) {
          process_file = "data/recoilfits/recoilfit_higgs_njet.root";
        }
      }
      std::cout << "Signal sample detected, using process file: " << process_file << std::endl;
      boson_id_.push_back(25);
      boson_id_.push_back(35);
      boson_id_.push_back(36);
    }
    if ( sample_.find("DYJetsToLL") != sample_.npos ) {
      disable = false;
      if (is_2012_) {
        process_file = "data/recoilfits/recoilfit_zmm53X_20pv_njet.root";
      } else {
        process_file = "data/recoilfits/recoilfit_zmm42X_20pv_njet.root";
        if (era_ == 1) {
          process_file = "data/recoilfits/recoilfit_zmm42X_njet.root";
        }
      }
      std::cout << "DYJetsToLL sample detected, using process file: " << process_file << std::endl;
      boson_id_.push_back(23);
    }

    if (disable) {
      std::cout << "This sample does not require a recoil correction, module disabled!" << std::endl;
      return 0;
    }

    std::cout << "Using data file: " << data_file << std::endl;
    std::cout << "Using MC file: " << mc_file << std::endl;

    corrector_ = new RecoilCorrector(process_file);
    corrector_->addMCFile(mc_file);
    corrector_->addDataFile(data_file);
    return 0;
  }

  int HttRecoilCorrector::Execute(TreeEvent *event) {

    if (disable) return 0;

    // Get the stuff we need from the event
    std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>(dilepton_label_);
    Met * pfMet = event->GetPtr<Met>(met_label_);
    std::vector<GenParticle *> parts = event->GetPtrVec<GenParticle>("genParticles");
    GenParticle const* boson = NULL;
    for (unsigned i = 0; i < parts.size(); ++i) {
      unsigned id = abs(parts[i]->pdgid());
      if (parts[i]->status() == 3) {
        for (unsigned j = 0; j < boson_id_.size(); ++j) {
          if (id == boson_id_[j]) {
            boson = parts[i];
          }
        }
      }
    }
    if (!boson) {
      std::cerr << "Recoil Corrector could not find gen boson!" << std::endl;
      throw;
    }


    //Met const* genMet = event->GetPtr<Met>("pfMetGen");
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_); // Make a copy of the jet collection
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
    unsigned njets = jets.size();
    double pfmet = pfMet->pt();
    double pfmetphi = pfMet->phi();
    double genpt = boson->pt();
    double genphi = boson->phi();
    double lep_pt = dilepton.at(0)->pt();
    double lep_phi = dilepton.at(0)->phi();
    if (is_wjets_ && (mode_ == 0 || mode_ == 1)) { // Use e or mu for e-tau and mu-tau
      lep_pt = dilepton.at(0)->GetCandidate("lepton1")->pt();
      lep_phi = dilepton.at(0)->GetCandidate("lepton1")->phi();
    } else if (is_wjets_ && (mode_ == 2) ) { // Use mu for e-mu
      lep_pt = dilepton.at(0)->GetCandidate("lepton2")->pt();
      lep_phi = dilepton.at(0)->GetCandidate("lepton2")->phi();
    }
    double U1 = 0;
    double U2 = 0;
    double iFluc = 0;
    double iScale = 0;
    //iFluc 1, iScale 1
    //iFluc -1, iScale -1
    if (is_2012_) {
      corrector_->CorrectType2(pfmet, pfmetphi, genpt, genphi, lep_pt, lep_phi, U1, U2, iFluc, iScale, njets);
    } else {
      if (era_ != 1) {
        corrector_->CorrectType1(pfmet, pfmetphi, genpt, genphi, lep_pt, lep_phi, U1, U2, iFluc, iScale, njets);            
      } else {
        corrector_->CorrectType2(pfmet, pfmetphi, genpt, genphi, lep_pt, lep_phi, U1, U2, iFluc, iScale, njets);
      }
    }

    pfMet->set_pt(pfmet);
    pfMet->set_energy(pfmet);
    pfMet->set_phi(pfmetphi);

    return 0;
  }
  int HttRecoilCorrector::PostAnalysis() {

    return 0;
  }

  void HttRecoilCorrector::PrintInfo() {
    ;
  }
}
