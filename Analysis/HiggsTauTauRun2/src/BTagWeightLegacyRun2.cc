#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/BTagWeightLegacyRun2.h"
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
    add_name_="";
    do_cp5_2016_=false;
    bbtag_eff_cp5_ = nullptr;
    cbtag_eff_cp5_ = nullptr;
    othbtag_eff_cp5_ = nullptr;
    bbtag_eff_cp5_alt_ = nullptr;
    cbtag_eff_cp5_alt_ = nullptr;
    othbtag_eff_cp5_alt_ = nullptr;
  }
  BTagWeightLegacyRun2::~BTagWeightLegacyRun2() {
   ;
  }

  int BTagWeightLegacyRun2::PreAnalysis() {
    if(do_cp5_2016_) std::cout << "Using CP5 tune fix for btag SFs in 2016" << std::endl;
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
    std::vector<double> btag_evt_weight = EventReweighting(embed_jets);
    event->ForceAdd("btag_evt_weight"+add_name_, btag_evt_weight[0]);
    event->ForceAdd("btag_evt_weight_down"+add_name_, btag_evt_weight[1]);
    event->ForceAdd("btag_evt_weight_up"+add_name_, btag_evt_weight[2]);

    event->ForceAdd("btag_evt_weight_realbtag_down"+add_name_, btag_evt_weight[3]);
    event->ForceAdd("btag_evt_weight_realbtag_up"+add_name_, btag_evt_weight[4]);
    event->ForceAdd("btag_evt_weight_fakebtag_down"+add_name_, btag_evt_weight[5]);
    event->ForceAdd("btag_evt_weight_fakebtag_up"+add_name_, btag_evt_weight[6]);
    return 0;
  }


  int BTagWeightLegacyRun2::PostAnalysis() {
    return 0;
  }

  void BTagWeightLegacyRun2::PrintInfo() {
    ;
  }

  std::vector<double> BTagWeightLegacyRun2::EventReweighting(std::vector<PFJet *> const& jets) const {
    std::vector<double> result = {1.,1.,1.,1.,1.,1.,1.};
    bool verbose = false;
    double pt = 0.;
    double eta = 0.;
    unsigned jet_flavour = 0;
    std::vector<double> sf_loose = {1.,1.,1.,1.,1.,1.,1.};
    std::vector<double> sf_tight = {1.,1.,1.,1.,1.,1.,1.};
    /*double sf_loose = 1.;
    double sf_tight = 1.;*/
    double eff_loose = 1.;
    double eff_tight = 1.;
    double sf_extra_loose=1., sf_extra_tight=1.;
    // First decide whether a jet is tight btagged, loose-only btagged, or untagged
    // Then calculate the probablities for MC and data
    for (unsigned i = 0; i < jets.size(); ++i) {
      eta = fabs(jets[i]->eta());
      pt = jets[i]->pt();
      jet_flavour = jets[i]->hadron_flavour();
      eff_loose = GetEff(jet_flavour, pt, fabs(eta), "loose");
      eff_tight = GetEff(jet_flavour, pt, fabs(eta), "tight");
      if(do_cp5_2016_) {
        eff_loose = GetEffCP5(jet_flavour, pt, fabs(eta), "loose");
        eff_tight = GetEffCP5(jet_flavour, pt, fabs(eta), "tight");
        sf_extra_loose = eff_loose >0. ? GetEff(jet_flavour, pt, fabs(eta), "loose")/eff_loose : 1.;
        sf_extra_tight = eff_tight >0. ? GetEff(jet_flavour, pt, fabs(eta), "tight")/eff_tight : 1.;
      }

      if(jet_flavour == 5){
        sf_tight[0] = reader_comb_tight->eval_auto_bounds("central",BTagEntry::FLAV_B, eta, pt);
        sf_loose[0] = reader_comb_loose->eval_auto_bounds("central",BTagEntry::FLAV_B, eta, pt);
        sf_tight[1] = reader_comb_tight->eval_auto_bounds("down",BTagEntry::FLAV_B, eta, pt);
        sf_loose[1] = reader_comb_loose->eval_auto_bounds("down",BTagEntry::FLAV_B, eta, pt);
        sf_tight[2] = reader_comb_tight->eval_auto_bounds("up",BTagEntry::FLAV_B, eta, pt);
        sf_loose[2] = reader_comb_loose->eval_auto_bounds("up",BTagEntry::FLAV_B, eta, pt);
        // now do b separate
        sf_tight[3] = sf_tight[1]; // tight down
        sf_loose[3] = sf_loose[1]; // loose down
        sf_tight[4] = sf_tight[2]; // tight up
        sf_loose[4] = sf_loose[2]; // tight down
        // now do c and udsg separate
        sf_tight[5] = sf_tight[0]; // tight central
        sf_loose[5] = sf_loose[0]; // loose central
        sf_tight[6] = sf_tight[0]; // tight central
        sf_loose[6] = sf_loose[0]; // loose central
      } else if(jet_flavour == 4){
        sf_tight[0] = reader_comb_tight->eval_auto_bounds("central",BTagEntry::FLAV_C, eta, pt);
        sf_loose[0] = reader_comb_loose->eval_auto_bounds("central",BTagEntry::FLAV_C, eta, pt);
        sf_tight[1] = reader_comb_tight->eval_auto_bounds("down",BTagEntry::FLAV_C, eta, pt);
        sf_loose[1] = reader_comb_loose->eval_auto_bounds("down",BTagEntry::FLAV_C, eta, pt);
        sf_tight[2] = reader_comb_tight->eval_auto_bounds("up",BTagEntry::FLAV_C, eta, pt);
        sf_loose[2] = reader_comb_loose->eval_auto_bounds("up",BTagEntry::FLAV_C, eta, pt);
        // do b separate
        sf_tight[3] = sf_tight[0]; // tight central
        sf_loose[3] = sf_loose[0]; // loose central
        sf_tight[4] = sf_tight[0]; // tight central
        sf_loose[4] = sf_loose[0]; // loose central
        // do c and udsg separate
        sf_tight[5] = sf_tight[1]; // tight down
        sf_loose[5] = sf_loose[1]; // loose down
        sf_tight[6] = sf_tight[2]; // tight up
        sf_loose[6] = sf_loose[2]; // loose up
      } else {
        sf_tight[0] = reader_comb_tight->eval_auto_bounds("central",BTagEntry::FLAV_UDSG, eta, pt);
        sf_loose[0] = reader_comb_loose->eval_auto_bounds("central",BTagEntry::FLAV_UDSG, eta, pt);
        sf_tight[1] = reader_comb_tight->eval_auto_bounds("down",BTagEntry::FLAV_UDSG, eta, pt);
        sf_loose[1] = reader_comb_loose->eval_auto_bounds("down",BTagEntry::FLAV_UDSG, eta, pt);
        sf_tight[2] = reader_comb_tight->eval_auto_bounds("up",BTagEntry::FLAV_UDSG, eta, pt);
        sf_loose[2] = reader_comb_loose->eval_auto_bounds("up",BTagEntry::FLAV_UDSG, eta, pt);
        // do b separate
        sf_tight[3] = sf_tight[0]; // tight central
        sf_loose[3] = sf_loose[0]; // loose central
        sf_tight[4] = sf_tight[0]; // tight central
        sf_loose[4] = sf_loose[0]; // loose central
        // do c and udsg separate
        sf_tight[5] = sf_tight[1]; // tight down
        sf_loose[5] = sf_loose[1]; // loose down
        sf_tight[6] = sf_tight[2]; // tight up
        sf_loose[6] = sf_loose[2]; // loose up
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
      for (unsigned j = 0; j < sf_tight.size(); j++) {
        double p_mc = 1.;
        double p_data = 1.;

        double sf_l = sf_loose[j];
        double sf_t = sf_tight[j];
      
        if(do_cp5_2016_) {
          sf_l*=sf_extra_loose;
          sf_t*=sf_extra_tight;
        }
    
        if (csv > tight_wp) {
          p_data *= sf_t;
        }
        if (csv > loose_wp && csv < tight_wp){
          p_mc *= (eff_loose - eff_tight);
          p_data *= (sf_l*eff_loose - sf_t*eff_tight);
        }
        else if (csv < loose_wp){
          p_mc *= (1 - eff_loose);
          p_data *= (1 - sf_l*eff_loose);
        }

        if (p_mc != 0.) result[j] = p_data/p_mc;
        else result[j] = 1.;
        if (result[j] < 0) result[j] = 1.;

      }
      if (verbose) {
        std::cout << "Jet " << i << " " << jets[i]->vector() << " deep csv: " << 
            jets[i]->GetBDiscriminator("pfDeepCSVJetTags:probb") + 
            jets[i]->GetBDiscriminator("pfDeepCSVJetTags:probbb") << 
            "  hadron flavour: " << jets[i]->hadron_flavour() << std::endl;
        std::cout << "-- efficiency loose: " << eff_loose << std::endl;
        std::cout << "-- efficiency tight: " << eff_tight << std::endl;
        std::cout << "-- scale factor central loose: " << sf_loose[0] << std::endl;
        std::cout << "-- scale factor central tight: " << sf_tight[0] << std::endl;
      }
    }

    return result;
  }

  double BTagWeightLegacyRun2::GetEff(unsigned flav, double pt, double eta, std::string wp) const {
    double res = 1.;
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

  double BTagWeightLegacyRun2::GetEffCP5(unsigned flav, double pt, double eta, std::string wp) const {
    double res = 1.;
    if (wp == "tight") {
      if(flav == 5){
        if(pt > bbtag_eff_cp5_->GetXaxis()->GetBinLowEdge(bbtag_eff_cp5_->GetNbinsX()+1))
          res = bbtag_eff_cp5_->GetBinContent(bbtag_eff_cp5_->GetNbinsX(),bbtag_eff_cp5_->GetYaxis()->FindBin(eta));
        else
          res = bbtag_eff_cp5_->GetBinContent(bbtag_eff_cp5_->GetXaxis()->FindBin(pt),bbtag_eff_cp5_->GetYaxis()->FindBin(eta));

      } else if(flav == 4){
        if(pt > cbtag_eff_cp5_->GetXaxis()->GetBinLowEdge(cbtag_eff_cp5_->GetNbinsX()+1))
          res = cbtag_eff_cp5_->GetBinContent(cbtag_eff_cp5_->GetNbinsX(),cbtag_eff_cp5_->GetYaxis()->FindBin(eta));
        else
          res = cbtag_eff_cp5_->GetBinContent(cbtag_eff_cp5_->GetXaxis()->FindBin(pt),cbtag_eff_cp5_->GetYaxis()->FindBin(eta));

      } else {
        if(pt > othbtag_eff_cp5_->GetXaxis()->GetBinLowEdge(othbtag_eff_cp5_->GetNbinsX()+1))
          res = othbtag_eff_cp5_->GetBinContent(othbtag_eff_cp5_->GetNbinsX(),othbtag_eff_cp5_->GetYaxis()->FindBin(eta));
        else
          res = othbtag_eff_cp5_->GetBinContent(othbtag_eff_cp5_->GetXaxis()->FindBin(pt),othbtag_eff_cp5_->GetYaxis()->FindBin(eta));
      }
    }
    else if (wp == "loose") {
      if(flav == 5){
        if(pt > bbtag_eff_cp5_alt_->GetXaxis()->GetBinLowEdge(bbtag_eff_cp5_alt_->GetNbinsX()+1))
          res = bbtag_eff_cp5_alt_->GetBinContent(bbtag_eff_cp5_alt_->GetNbinsX(),bbtag_eff_cp5_alt_->GetYaxis()->FindBin(eta));
        else
          res = bbtag_eff_cp5_alt_->GetBinContent(bbtag_eff_cp5_alt_->GetXaxis()->FindBin(pt),bbtag_eff_cp5_alt_->GetYaxis()->FindBin(eta));

      } else if(flav == 4){
        if(pt > cbtag_eff_cp5_alt_->GetXaxis()->GetBinLowEdge(cbtag_eff_cp5_alt_->GetNbinsX()+1))
          res = cbtag_eff_cp5_alt_->GetBinContent(cbtag_eff_cp5_alt_->GetNbinsX(),cbtag_eff_cp5_alt_->GetYaxis()->FindBin(eta));
        else
          res = cbtag_eff_cp5_alt_->GetBinContent(cbtag_eff_cp5_alt_->GetXaxis()->FindBin(pt),cbtag_eff_cp5_alt_->GetYaxis()->FindBin(eta));

      } else {
        if(pt > othbtag_eff_cp5_alt_->GetXaxis()->GetBinLowEdge(othbtag_eff_cp5_alt_->GetNbinsX()+1))
          res = othbtag_eff_cp5_alt_->GetBinContent(othbtag_eff_cp5_alt_->GetNbinsX(),othbtag_eff_cp5_alt_->GetYaxis()->FindBin(eta));
        else
          res = othbtag_eff_cp5_alt_->GetBinContent(othbtag_eff_cp5_alt_->GetXaxis()->FindBin(pt),othbtag_eff_cp5_alt_->GetYaxis()->FindBin(eta));
      }
    }
   return res;

  }

}
