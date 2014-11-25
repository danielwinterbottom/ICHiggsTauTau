#include "UserCode/ICHiggsTauTau/Analysis/HiggsHTohh/interface/HhhRecoilCorrector.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "boost/format.hpp"
#include "TMVA/Reader.h"
#include "TVector3.h"

namespace ic {

  HhhRecoilCorrector::HhhRecoilCorrector(std::string const& name) : ModuleBase(name), 
    channel_(channel::et),
    strategy_(strategy::moriond2013),
    mc_(mc::summer12_53X),
    era_(era::data_2012_moriond) {
    dilepton_label_ = "emtauCandidates";
    met_label_ = "pfMVAMet";
    jets_label_ = "pfJetsPFlow";
    sample_ = "";
    disable = true;
    is_wjets_ = false;
    is_ttbar_ = false;
    met_scale_mode_ = 0;
    w_hack_ = false;
  }

  HhhRecoilCorrector::~HhhRecoilCorrector() {
    ;
  }

  int HhhRecoilCorrector::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HhhRecoilCorrector" << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    std::cout << boost::format(param_fmt()) % "channel"         % Channel2String(channel_);
    std::cout << boost::format(param_fmt()) % "strategy"        % Strategy2String(strategy_);
    std::cout << boost::format(param_fmt()) % "era"             % Era2String(era_);
    std::cout << boost::format(param_fmt()) % "mc"              % MC2String(mc_);
    std::cout << boost::format(param_fmt()) % "dilepton_label"  % dilepton_label_;
    std::cout << boost::format(param_fmt()) % "met_label"       % met_label_;
    std::cout << boost::format(param_fmt()) % "jets_label"      % jets_label_;

    std::string process_file;
    std::string data_file;
    std::string mc_file;
    if (strategy_ == strategy::hcp2012) {
      if (mc_ == mc::summer12_53X) {
        data_file = "data/recoilfits/recoilfit_datamm53X_20pv_njet.root";
        mc_file = "data/recoilfits/recoilfit_zmm53X_20pv_njet.root";
      } else if (mc_ == mc::fall11_42X) {
        data_file = "data/recoilfits/recoilfit_datamm42X_20pv_njet.root";
        mc_file = "data/recoilfits/recoilfit_zmm42X_20pv_njet.root";
      } else {
        return 0;
      }
    } else if (strategy_ == strategy::moriond2013) {
      if (mc_ == mc::summer12_53X) { 
        data_file = "data/recoilfits/recoilfit_datamm53X_2012_njet.root";
        mc_file = "data/recoilfits/recoilfit_zmm53X_2012_njet.root";
      } else if (mc_ == mc::fall11_42X) {
        data_file = "data/recoilfits/recoilfit_datamm42X_20pv_njet.root";
        mc_file = "data/recoilfits/recoilfit_zmm42X_20pv_njet.root";
      } else {
        return 0;
      }
    } else if (strategy_ == strategy::paper2013) {
      if (mc_ == mc::summer12_53X) { 
        data_file = "data/recoilfits/recoilfit_datamm53XRR_2012_njet.root";
        mc_file = "data/recoilfits/recoilfit_zmm53XRR_2012_njet.root";
      } else if (mc_ == mc::fall11_42X) {
        data_file = "data/recoilfits/recoilfit_datamm42X_20pv_njet.root";
        mc_file = "data/recoilfits/recoilfit_zmm42X_20pv_njet.root";
      } else {
        return 0;
      }
    } else {
      std::cerr << "Strategy: " << Strategy2String(strategy_) << " not recognised, an exception will be thrown." << std::endl;
      throw;
    }
   
    if ( (sample_.find("WJetsToLNu") != sample_.npos) ) {
      disable = false;
      is_wjets_ = true;
      if (mc_ == mc::summer12_53X) process_file = "data/recoilfits/recoilfit_wjets53X_20pv_njet.root";
      if (mc_ == mc::fall11_42X) process_file = "data/recoilfits/recoilfit_wjets42X_20pv_njet.root";
      std::cout << boost::format(param_fmt()) % "enabled"       % true;
      std::cout << boost::format(param_fmt()) % "type"          % "W+Jets";
      std::cout << boost::format(param_fmt()) % "process_file"  % process_file;
      boson_id_.push_back(24);
    }
    
    /*
    if ( (sample_.find("TTJets") != sample_.npos) ) {
      disable = false;
      is_ttbar_ = true;
      if (mc_ == mc::summer12_53X) process_file = "data/recoilfits/recoilfit_wjets53X_20pv_njet.root";
      if (mc_ == mc::fall11_42X) process_file = "data/recoilfits/recoilfit_wjets42X_20pv_njet.root";
      std::cout << boost::format(param_fmt()) % "enabled"       % true;
      std::cout << boost::format(param_fmt()) % "type"          % "TTJets";
      std::cout << boost::format(param_fmt()) % "process_file"  % process_file;
      boson_id_.push_back(24);
    }
    */

    if (sample_.find("HToTauTau") != sample_.npos) {
      disable = false;
      if (mc_ == mc::summer12_53X) process_file = "data/recoilfits/recoilfit_higgs53X_20pv_njet.root";
      if (mc_ == mc::summer12_53X && channel_ == channel::em) process_file = "data/recoilfits/recoilfit_higgsem53X_20pv_njet.root";
      if (mc_ == mc::fall11_42X) process_file = "data/recoilfits/recoilfit_higgs42X_20pv_njet.root";
      if (mc_ == mc::fall11_42X && channel_ == channel::em) process_file = "data/recoilfits/recoilfit_higgsem42X_20pv_njet.root";
      std::cout << boost::format(param_fmt()) % "enabled"       % true;
      std::cout << boost::format(param_fmt()) % "type"          % "signal";
      std::cout << boost::format(param_fmt()) % "process_file"  % process_file;      
      boson_id_.push_back(25);
      boson_id_.push_back(35);
      boson_id_.push_back(36);
    }
    //Some special treatment is needed for the H->hh signal. The gen object should be the h->tautau.
    if (sample_.find("HTohh") != sample_.npos) {
      disable = false;
      if (mc_ == mc::summer12_53X) process_file = "data/recoilfits/recoilfit_higgs53X_20pv_njet.root";
      if (mc_ == mc::summer12_53X && channel_ == channel::em) process_file = "data/recoilfits/recoilfit_higgsem53X_20pv_njet.root";
      if (mc_ == mc::fall11_42X) process_file = "data/recoilfits/recoilfit_higgs42X_20pv_njet.root";
      if (mc_ == mc::fall11_42X && channel_ == channel::em) process_file = "data/recoilfits/recoilfit_higgsem42X_20pv_njet.root";
      std::cout << boost::format(param_fmt()) % "enabled"       % true;
      std::cout << boost::format(param_fmt()) % "type"          % "signal";
      std::cout << boost::format(param_fmt()) % "process_file"  % process_file; 
      boson_id_.push_back(25);
    }
    //For A->Zh, we should pick up the h instead of the A.
    if (sample_.find("AToZh") != sample_.npos) {
      disable = false;
      if (mc_ == mc::summer12_53X) process_file = "data/recoilfits/recoilfit_higgs53X_20pv_njet.root";
      if (mc_ == mc::summer12_53X && channel_ == channel::em) process_file = "data/recoilfits/recoilfit_higgsem53X_20pv_njet.root";
      if (mc_ == mc::fall11_42X) process_file = "data/recoilfits/recoilfit_higgs42X_20pv_njet.root";
      if (mc_ == mc::fall11_42X && channel_ == channel::em) process_file = "data/recoilfits/recoilfit_higgsem42X_20pv_njet.root";
      std::cout << boost::format(param_fmt()) % "enabled"       % true;
      std::cout << boost::format(param_fmt()) % "type"          % "signal";
      std::cout << boost::format(param_fmt()) % "process_file"  % process_file;      
      boson_id_.push_back(25);
    }
    //Some special treatment is needed for the H->hh signal. The gen object should be the h->tautau.

    if ( sample_.find("DYJetsToLL") != sample_.npos 
      || sample_.find("DYJetsToTauTau") != sample_.npos ) {
      disable = false;
      if (mc_ == mc::summer12_53X) {
        if (strategy_ == strategy::hcp2012) process_file = "data/recoilfits/recoilfit_zmm53X_20pv_njet.root";
        if (strategy_ == strategy::moriond2013) process_file = "data/recoilfits/recoilfit_zmm53X_2012_njet.root";
        if (strategy_ == strategy::paper2013)   process_file = "data/recoilfits/recoilfit_zmm53XRR_2012_njet.root";
      }
      if (mc_ == mc::fall11_42X) process_file = "data/recoilfits/recoilfit_zmm42X_20pv_njet.root";
      std::cout << boost::format(param_fmt()) % "enabled"       % true;
      std::cout << boost::format(param_fmt()) % "type"          % "Z+jets";
      std::cout << boost::format(param_fmt()) % "process_file"  % process_file; 
      boson_id_.push_back(23);
    }

    if (disable) {
      std::cout << boost::format(param_fmt()) % "enabled"      % false;
      return 0;
    } else {
      std::cout << boost::format(param_fmt()) % "data_file"  % data_file;        
      std::cout << boost::format(param_fmt()) % "mc_file"  % mc_file;        
      corrector_ = new RecoilCorrector(process_file);
      corrector_->addMCFile(mc_file);
      corrector_->addDataFile(data_file);
    }

    return 0;
  }


  int HhhRecoilCorrector::Execute(TreeEvent *event) {

    if (disable) return 0;

    // Get the stuff we need from the event
    std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>(dilepton_label_);
    Met * pfMet = event->GetPtr<Met>(met_label_);
    std::vector<GenParticle *> parts = event->GetPtrVec<GenParticle>("genParticles");
    GenParticle const* boson = NULL;
    GenParticle const* boson2 = NULL;
    GenParticle const* tau1 = NULL;
    GenParticle const* tau2 = NULL;
    //loop once to find the daughter taus needed for the H->hh signal
    if (sample_.find("HTohh") != sample_.npos) {
      for (unsigned i = 0; i < parts.size(); ++i) {
        int sign_id = parts[i]->pdgid();
        if (sign_id == 15 ) tau1 = parts[i];
        if (sign_id == -15 ) tau2 = parts[i];
      }
    }
    for (unsigned i = 0; i < parts.size(); ++i) {
      unsigned id = abs(parts[i]->pdgid());
      if (parts[i]->status() == 3) {
        for (unsigned j = 0; j < boson_id_.size(); ++j) {
          if (id == boson_id_[j]) {
            //For the H->hh signal, select the h with the smallest difference between its pt and that of the tau pair (not exact due to double precision)
            double min = 0.1;
            if (!is_ttbar_ && !(sample_.find("HTohh") != sample_.npos) ) {
              boson = parts[i];
            }
            else if(sample_.find("HTohh") != sample_.npos) {
              double pt_diff = fabs((tau1->vector()+tau2->vector()).pt() - parts[i]->pt());
              if (pt_diff < min ) {
                min = pt_diff;
                boson = parts[i];
              } 
            }
            else {
              if (!boson) boson = parts[i];
              if (boson && !boson2) boson2 = parts[i];
            }
          }
        }
      }
    }
 
    
    if (!boson) {
      std::cerr << "Error in <HhhRecoilCorrector>: Boson GenParticle not found, an exception will be thrown" << std::endl;
      throw;
    }
    if (is_ttbar_ && !boson && !boson2) {
      std::cerr << "Error in <HhhRecoilCorrector>: Boson GenParticles not found, an exception will be thrown" << std::endl;
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
    if (is_ttbar_) {
      genpt = (boson->vector()+boson2->vector()).pt();
      genphi = (boson->vector()+boson2->vector()).phi();
    }
    double lep_pt = dilepton.at(0)->pt();
    double lep_phi = dilepton.at(0)->phi();
    if (is_wjets_ && (channel_ == channel::et || channel_ == channel::mt || channel_ == channel::mtmet)) { // Use e or mu for et, mt and mtmet
      lep_pt = dilepton.at(0)->GetCandidate("lepton1")->pt();
      lep_phi = dilepton.at(0)->GetCandidate("lepton1")->phi();
      if (w_hack_) {
        std::vector<Candidate *> mu_cand;
        mu_cand.push_back(dilepton.at(0)->GetCandidate("lepton1"));
        std::vector<PFJet *> filtered_jets = event->GetPtrVec<PFJet>("pfJetsPFlowFiltered");
        ic::erase_if(filtered_jets,! boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
        ic::erase_if(filtered_jets,! boost::bind(MinDRToCollection<Candidate *>, _1, mu_cand, 0.5));
        njets = filtered_jets.size();
      }
    } else if (is_wjets_ && (channel_ == channel::em) ) { // Use mu for em
      lep_pt = dilepton.at(0)->GetCandidate("lepton2")->pt();
      lep_phi = dilepton.at(0)->GetCandidate("lepton2")->phi();
    }
    double U1 = 0;
    double U2 = 0;
    double iFluc = 0;
    double iScale = 0;
    if(met_scale_mode_ == 1) { 
        iFluc=-1; 
        iScale=-1;
    }
    if(met_scale_mode_ == 2) { 
        iFluc=1; 
        iScale=1;
    }
    
    if (mc_ == mc::summer12_53X) {

      if (strategy_ == strategy::hcp2012) corrector_->CorrectType2(pfmet, pfmetphi, genpt, genphi, lep_pt, lep_phi, U1, U2, iFluc, iScale, njets);
      if (strategy_ == strategy::moriond2013) corrector_->CorrectType1(pfmet, pfmetphi, genpt, genphi, lep_pt, lep_phi, U1, U2, iFluc, iScale, njets);
      if (strategy_ == strategy::paper2013)   corrector_->CorrectType1(pfmet, pfmetphi, genpt, genphi, lep_pt, lep_phi, U1, U2, iFluc, iScale, njets);

      if (strategy_ == strategy::hcp2012) corrector_->CorrectType2(pfmet, pfmetphi, genpt, genphi, lep_pt, lep_phi, U1, U2, iFluc, iScale, njets);
    } else if (mc_ == mc::fall11_42X) {
      corrector_->CorrectType1(pfmet, pfmetphi, genpt, genphi, lep_pt, lep_phi, U1, U2, iFluc, iScale, njets);
    }
    pfMet->set_pt(pfmet);
    pfMet->set_energy(pfmet);
    pfMet->set_phi(pfmetphi);

    return 0;
  }
  int HhhRecoilCorrector::PostAnalysis() {

    return 0;
  }

  void HhhRecoilCorrector::PrintInfo() {
    ;
  }
}
