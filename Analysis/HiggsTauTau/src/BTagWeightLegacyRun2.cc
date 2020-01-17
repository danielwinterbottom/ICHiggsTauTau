#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/BTagWeightLegacyRun2.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include <cmath>
#include <algorithm>

namespace ic {
  BTagWeightLegacyRun2::BTagWeightLegacyRun2(std::string const& name) : ModuleBase(name), channel_(channel::et), era_(era::data_2015), strategy_(strategy::mssmsummer16) {
    jet_label_ = "pfJetsPFlow";
    bbtag_eff_ = nullptr;
    cbtag_eff_ = nullptr;
    othbtag_eff_ = nullptr;
    bbtag_eff_alt_ = nullptr;
    cbtag_eff_alt_ = nullptr;
    othbtag_eff_alt_ = nullptr;
    btag_mode_ = 0;
    add_name_="";
  }
  BTagWeightLegacyRun2::~BTagWeightLegacyRun2() {
   ;
  }

  int BTagWeightLegacyRun2::PreAnalysis() {

    std::string name = "btag_calib";
    if(ProductExists(name) && ProductExists(name+"_reader_comb_tight") && ProductExists(name+"_reader_comb_loose")){
      std::cout << "Getting BTagCalibration and BTagCalibrationReader objects from products." << std::endl;
      calib = GetProduct<const BTagCalibration *>(name);
      reader_comb_tight = GetProduct<BTagCalibrationReader*>(name+"_reader_comb_tight");
      reader_comb_loose = GetProduct<BTagCalibrationReader*>(name+"_reader_comb_loose");
    } else {
      std::string csv_file_path = "";
      if (era_==era::data_2016) csv_file_path = "./input/btag_sf/DeepCSV_2016LegacySF_V1.csv";
      else if (era_==era::data_2017) csv_file_path = "./input/btag_sf/DeepCSV_94XSF_V4_B_F.csv";
      else if (era_==era::data_2018) csv_file_path = "./input/btag_sf/DeepCSV_102XSF_V1.csv";
  
      calib  = new const BTagCalibration("deepcsv",csv_file_path);
  
      reader_comb_tight = new BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central",{"up","down"}); 
      reader_comb_tight->load(*calib,BTagEntry::FLAV_B,"comb");
      reader_comb_tight->load(*calib,BTagEntry::FLAV_C,"comb");
      reader_comb_tight->load(*calib,BTagEntry::FLAV_UDSG,"comb");

      reader_comb_loose = new BTagCalibrationReader(BTagEntry::OP_LOOSE, "central",{"up","down"}); 
      reader_comb_loose->load(*calib,BTagEntry::FLAV_B,"comb");
      reader_comb_loose->load(*calib,BTagEntry::FLAV_C,"comb");
      reader_comb_loose->load(*calib,BTagEntry::FLAV_UDSG,"comb");
  
      std::cout << "Adding BTagCalibration and BTagCalibrationReader objects to products." << std::endl;
      AddToProducts(name, calib);
      AddToProducts(name+"_reader_comb_tight", reader_comb_tight);
      AddToProducts(name+"_reader_comb_loose", reader_comb_loose);
    }
    return 0;
  }

  int BTagWeightLegacyRun2::Execute(TreeEvent *event) {
    std::vector<PFJet*> embed_jets = event->GetPtrVec<PFJet>(jet_label_);
    ic::erase_if(embed_jets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
    double btag_evt_weight = EventReweighting(embed_jets, btag_mode_);
    event->Add("btag_evt_weight"+add_name_, btag_evt_weight);
    return 0;
  }


  int BTagWeightLegacyRun2::PostAnalysis() {
    return 0;
  }

  void BTagWeightLegacyRun2::PrintInfo() {
    ;
  }

  double BTagWeightLegacyRun2::EventReweighting(std::vector<PFJet *> const& jets, unsigned btag_mode) const {
    double result = 1;
    // std::vector<PFJet*> loose_btag;
    // std::vector<PFJet*> tight_btag;
    // std::vector<PFJet*> no_btag;
    double p_mc = 1.;
    double p_data = 1.;

    double pt = 0.;
    double eta = 0.;
    unsigned jet_flavour = 0;
    double sf_loose = 1.;
    double sf_tight = 1.;
    // First decide whether a jet is tight btagged, loose-only btagged, or untagged
    // Then calculate the probablities for MC and data
    for (unsigned i = 0; i < jets.size(); ++i) {
      eta = fabs(jets[i]->eta());
      pt = jets[i]->pt();
      jet_flavour = jets[i]->hadron_flavour();
      double eff_loose = GetEff(jet_flavour,pt, fabs(eta), "loose");
      double eff_tight = GetEff(jet_flavour,pt, fabs(eta), "tight");

      if(jet_flavour == 5){
        if(btag_mode == 2){ 
         sf_tight = reader_comb_tight->eval_auto_bounds("up",BTagEntry::FLAV_B, eta, pt);
         sf_loose = reader_comb_loose->eval_auto_bounds("up",BTagEntry::FLAV_B, eta, pt);
        } else if(btag_mode == 1){
         sf_tight = reader_comb_tight->eval_auto_bounds("down",BTagEntry::FLAV_B, eta, pt);
         sf_loose = reader_comb_loose->eval_auto_bounds("down",BTagEntry::FLAV_B, eta, pt);
        } else {
         sf_tight = reader_comb_tight->eval_auto_bounds("central",BTagEntry::FLAV_B, eta, pt);
         sf_loose = reader_comb_loose->eval_auto_bounds("central",BTagEntry::FLAV_B, eta, pt);
        }
      } else if(jet_flavour == 4){
        if(btag_mode == 2){ 
         sf_tight = reader_comb_tight->eval_auto_bounds("up",BTagEntry::FLAV_C, eta, pt);
         sf_loose = reader_comb_loose->eval_auto_bounds("up",BTagEntry::FLAV_C, eta, pt);
        } else if(btag_mode == 1){
         sf_tight = reader_comb_tight->eval_auto_bounds("down",BTagEntry::FLAV_C, eta, pt);
         sf_loose = reader_comb_loose->eval_auto_bounds("down",BTagEntry::FLAV_C, eta, pt);
        } else {
         sf_tight = reader_comb_tight->eval_auto_bounds("central",BTagEntry::FLAV_C, eta, pt);
         sf_loose = reader_comb_loose->eval_auto_bounds("central",BTagEntry::FLAV_C, eta, pt);
        }
      }

      double csv = jets[i]->GetBDiscriminator("pfDeepCSVJetTags:probb") +
        jets[i]->GetBDiscriminator("pfDeepCSVJetTags:probbb");

      double tight_wp = 0.5;
      double loose_wp = 0.1;
      if(era_ == era::data_2016){
        tight_wp = 0.6321; // medium deepCSV wp
        loose_wp = 0.2217; // loose deepCSV wp
      }
      else if(era_ == era::data_2017) {
        tight_wp = 0.4941; // medium deepCSV wp
        loose_wp = 0.1522; // loose deepCSV wp
      }
      else if (era_ == era::data_2018){
        tight_wp = 0.4184; // medium deepCSV wp
        loose_wp = 0.1241; // loose deepCSV wp
      }
      if (csv > tight_wp) {
        // tight_btag.push_back(jets[i]);
        p_mc *= eff_tight;
        p_data *= eff_tight;
      }
      else if (csv > loose_wp && csv < tight_wp){
        // loose_btag.push_back(jets[i]);
        p_mc *= (eff_loose - eff_tight);
        p_data *= (sf_loose*eff_loose - sf_tight*eff_tight);
      }
      else if (csv < loose_wp){
        // no_btag.push_back(jets[i]);
        p_mc *= (1 - eff_loose);
        p_data *= (1 - sf_loose*eff_loose);
      }
    }

    result = p_data/p_mc;
    return result;
  }

  double BTagWeightLegacyRun2::GetEff(unsigned flav, double pt, double eta, std::string wp) const {
    double res=0;
    wp = "tight";
    if (wp == "tight") {
      if(flav == 5){
        if(pt > bbtag_eff_->GetXaxis()->GetBinLowEdge(bbtag_eff_->GetNbinsX()+1))
          res = bbtag_eff_->GetBinContent(bbtag_eff_->GetNbinsX(),bbtag_eff_->GetYaxis()->FindBin(eta));
        else 
          res = bbtag_eff_->GetBinContent(bbtag_eff_->GetXaxis()->FindBin(pt),bbtag_eff_->GetYaxis()->FindBin(eta));

      } else if(flav == 4){
        if(pt > cbtag_eff_->GetXaxis()->GetBinLowEdge(cbtag_eff_->GetNbinsX()+1))
          res = cbtag_eff_->GetBinContent(cbtag_eff_->GetNbinsX(),cbtag_eff_->GetYaxis()->FindBin(eta));
        else 
          res = cbtag_eff_->GetBinContent(cbtag_eff_->GetXaxis()->FindBin(pt),cbtag_eff_->GetYaxis()->FindBin(eta));

      } else {
        if(pt > othbtag_eff_->GetXaxis()->GetBinLowEdge(othbtag_eff_->GetNbinsX()+1))
          res = othbtag_eff_->GetBinContent(othbtag_eff_->GetNbinsX(),othbtag_eff_->GetYaxis()->FindBin(eta));
        else 
          res = othbtag_eff_->GetBinContent(othbtag_eff_->GetXaxis()->FindBin(pt),othbtag_eff_->GetYaxis()->FindBin(eta));
      } 
    }
    else if (wp == "loose") {
      if(flav == 5){
        if(pt > bbtag_eff_alt_->GetXaxis()->GetBinLowEdge(bbtag_eff_alt_->GetNbinsX()+1))
          res = bbtag_eff_alt_->GetBinContent(bbtag_eff_alt_->GetNbinsX(),bbtag_eff_alt_->GetYaxis()->FindBin(eta));
        else 
          res = bbtag_eff_alt_->GetBinContent(bbtag_eff_alt_->GetXaxis()->FindBin(pt),bbtag_eff_alt_->GetYaxis()->FindBin(eta));

      } else if(flav == 4){
        if(pt > cbtag_eff_alt_->GetXaxis()->GetBinLowEdge(cbtag_eff_alt_->GetNbinsX()+1))
          res = cbtag_eff_alt_->GetBinContent(cbtag_eff_alt_->GetNbinsX(),cbtag_eff_alt_->GetYaxis()->FindBin(eta));
        else 
          res = cbtag_eff_alt_->GetBinContent(cbtag_eff_alt_->GetXaxis()->FindBin(pt),cbtag_eff_alt_->GetYaxis()->FindBin(eta));

      } else {
        if(pt > othbtag_eff_alt_->GetXaxis()->GetBinLowEdge(othbtag_eff_alt_->GetNbinsX()+1))
          res = othbtag_eff_alt_->GetBinContent(othbtag_eff_alt_->GetNbinsX(),othbtag_eff_alt_->GetYaxis()->FindBin(eta));
        else 
          res = othbtag_eff_alt_->GetBinContent(othbtag_eff_alt_->GetXaxis()->FindBin(pt),othbtag_eff_alt_->GetYaxis()->FindBin(eta));
      } 
    }
   return res;

  }
 

}
