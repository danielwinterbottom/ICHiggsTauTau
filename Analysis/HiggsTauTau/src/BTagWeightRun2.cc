#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/BTagWeightRun2.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include <cmath>
#include <algorithm>

namespace ic {
  BTagWeightRun2::BTagWeightRun2(std::string const& name) : ModuleBase(name), channel_(channel::et), era_(era::data_2015), strategy_(strategy::mssmsummer16) {
    jet_label_ = "pfJetsPFlow";
    bbtag_eff_ = nullptr;
    cbtag_eff_ = nullptr;
    othbtag_eff_ = nullptr;
    do_reshape_ = false;
    btag_mode_ = 0;
    bfake_mode_ = 0;
  }
  BTagWeightRun2::~BTagWeightRun2() {
   ;
  }

  int BTagWeightRun2::PreAnalysis() {
    std::string csv_file_path = "./input/btag_sf/CSVv2.csv";
    if(strategy_ == strategy::mssmsummer16) csv_file_path = "./input/btag_sf/CSVv2Moriond17_2017_1_26_BtoH.csv";
    else if (strategy_ == strategy::mssmspring16 || strategy_ == strategy::smspring16) csv_file_path = "./input/btag_sf/CSVv2_ichep.csv";
    calib = new const BTagCalibration("csvv2",csv_file_path);
    if(era_ == era::data_2016){
      reader_comb = new BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central",{"up","down"});
    }
    if(channel_ != channel::tt || era_==era::data_2016){
      reader_incl = new BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central",{"up","down"});
      reader_mujets = new BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central",{"up","down"});
    } else {
      reader_incl = new BTagCalibrationReader(BTagEntry::OP_LOOSE, "central",{"up","down"});
      reader_mujets = new BTagCalibrationReader(BTagEntry::OP_LOOSE, "central",{"up","down"});
    }
    reader_iterativefit = new BTagCalibrationReader(BTagEntry::OP_RESHAPING, "central",{"up_jes","down_jes","up_lf","down_lf","up_hf","down_hf","up_hfstats1","down_hfstats1","up_hfstats2","down_hfstats2","up_lfstats1","down_lfstats1","up_lfstats2","down_lfstats2","up_cferr1","down_cferr1","up_cferr2","down_cferr2"});
    reader_incl->load(*calib,BTagEntry::FLAV_B,"incl");
    reader_incl->load(*calib,BTagEntry::FLAV_C,"incl");
    reader_incl->load(*calib,BTagEntry::FLAV_UDSG,"incl");
    reader_mujets->load(*calib,BTagEntry::FLAV_B,"mujets");
    reader_mujets->load(*calib,BTagEntry::FLAV_C,"mujets");
    reader_mujets->load(*calib,BTagEntry::FLAV_UDSG,"mujets");
    if(era_ == era::data_2016){
      reader_comb->load(*calib,BTagEntry::FLAV_B,"comb");
      reader_comb->load(*calib,BTagEntry::FLAV_C,"comb");
      reader_comb->load(*calib,BTagEntry::FLAV_UDSG,"comb");
    }
    reader_iterativefit->load(*calib,BTagEntry::FLAV_B,"iterativefit");
    reader_iterativefit->load(*calib,BTagEntry::FLAV_C,"iterativefit");
    reader_iterativefit->load(*calib,BTagEntry::FLAV_UDSG,"iterativefit");




    rand = new TRandom3(0);
    return 0;
  }

  int BTagWeightRun2::Execute(TreeEvent *event) {
    std::vector<PFJet*> embed_jets = event->GetPtrVec<PFJet>(jet_label_);
    ic::erase_if(embed_jets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
    if (!do_reshape_){
      std::map<std::size_t, bool> retag_result = ReTag(embed_jets, btag_mode_, bfake_mode_);
      event->Add("retag_result", retag_result);
    } else {
      double btag_evt_weight = SFCSVShape(embed_jets, btag_mode_);
      event->Add("btag_evt_weight", btag_evt_weight);
    }
    return 0;
  }


  int BTagWeightRun2::PostAnalysis() {
    return 0;
  }

  void BTagWeightRun2::PrintInfo() {
    ;
  }

  double BTagWeightRun2::SFCSVShape(std::vector<PFJet *> const& jets, unsigned btag_mode) const {
    double result = 1;
    for (unsigned i = 0; i < jets.size(); ++i) {
      double sf=0;
      double csv = jets[i]->GetBDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");
      if (csv < 0) csv = -0.05;
      if (csv > 1) csv = 1;
      double eta = jets[i]->eta();
      unsigned jet_flavour = jets[i]->hadron_flavour();
      double pt = jets[i]->pt();
      if(jet_flavour == 5){
          if(btag_mode == 4){
           sf = reader_iterativefit->eval_auto_bounds("up_jes",BTagEntry::FLAV_B, fabs(eta), pt, csv);
          } else if(btag_mode == 3){
           sf = reader_iterativefit->eval_auto_bounds("down_jes",BTagEntry::FLAV_B, fabs(eta), pt, csv);
          } else if (btag_mode == 6){
           sf = reader_iterativefit->eval_auto_bounds("up_lf",BTagEntry::FLAV_B, fabs(eta), pt, csv);
          } else if (btag_mode == 5){
           sf = reader_iterativefit->eval_auto_bounds("down_lf",BTagEntry::FLAV_B, fabs(eta), pt, csv);
          } else if (btag_mode == 10){
           sf = reader_iterativefit->eval_auto_bounds("up_hfstats1",BTagEntry::FLAV_B, fabs(eta), pt, csv);
          } else if (btag_mode == 9 ){
           sf = reader_iterativefit->eval_auto_bounds("down_hfstats1",BTagEntry::FLAV_B, fabs(eta), pt, csv);
          } else if (btag_mode == 12){
           sf = reader_iterativefit->eval_auto_bounds("up_hfstats2",BTagEntry::FLAV_B, fabs(eta), pt, csv);
          } else if (btag_mode == 11){
           sf = reader_iterativefit->eval_auto_bounds("down_hfstats2",BTagEntry::FLAV_B, fabs(eta), pt, csv);
          } else {
           sf = reader_iterativefit->eval_auto_bounds("central",BTagEntry::FLAV_B, fabs(eta), pt, csv);
          }
        } else if(jet_flavour == 4){
          if(btag_mode == 18){
           sf = reader_iterativefit->eval_auto_bounds("up_cferr1",BTagEntry::FLAV_C, fabs(eta), pt, csv);
          } else if(btag_mode == 17){
           sf = reader_iterativefit->eval_auto_bounds("down_cferr1",BTagEntry::FLAV_C, fabs(eta), pt, csv);
          } else if (btag_mode == 20){
           sf = reader_iterativefit->eval_auto_bounds("up_cferr2",BTagEntry::FLAV_C, fabs(eta), pt, csv);
          } else if (btag_mode == 19){
           sf = reader_iterativefit->eval_auto_bounds("down_cferr2",BTagEntry::FLAV_C, fabs(eta), pt, csv);
          } else {
            sf = 1; 
          }
        } else {
          if(btag_mode == 4){
           sf = reader_iterativefit->eval_auto_bounds("up_jes",BTagEntry::FLAV_UDSG, fabs(eta), pt, csv);
          } else if (btag_mode == 3 ){
           sf = reader_iterativefit->eval_auto_bounds("down_jes",BTagEntry::FLAV_UDSG, fabs(eta), pt, csv);
          } else if (btag_mode == 8){
           sf = reader_iterativefit->eval_auto_bounds("up_hf",BTagEntry::FLAV_UDSG, fabs(eta), pt, csv);
          } else if (btag_mode == 7){
           sf = reader_iterativefit->eval_auto_bounds("down_hf",BTagEntry::FLAV_UDSG, fabs(eta), pt, csv);
          } else if (btag_mode == 14){
           sf = reader_iterativefit->eval_auto_bounds("up_lfstats1",BTagEntry::FLAV_UDSG, fabs(eta), pt, csv);
          } else if (btag_mode == 13){
           sf = reader_iterativefit->eval_auto_bounds("down_lfstats1",BTagEntry::FLAV_UDSG, fabs(eta), pt, csv);
          } else if (btag_mode == 16){
           sf = reader_iterativefit->eval_auto_bounds("up_lfstats2",BTagEntry::FLAV_UDSG, fabs(eta), pt, csv);
          } else if (btag_mode == 15){
           sf = reader_iterativefit->eval_auto_bounds("down_lfstats2",BTagEntry::FLAV_UDSG, fabs(eta), pt, csv);
          } else{
           sf = reader_iterativefit->eval_auto_bounds("central",BTagEntry::FLAV_UDSG, fabs(eta), pt, csv);
          }
        }
       result *=sf;
      }
    return result;
  }

  std::map<std::size_t, bool> BTagWeightRun2::ReTag(std::vector<PFJet *> const& jets, unsigned btag_mode, unsigned bfake_mode) const {
    bool verbose = true;
    std::map<std::size_t, bool> pass_result;
    float pt = 0/* = embed_jets[i]->pt()*/;
    float eta = 0/* embed_jets[i]->eta()*/;
    unsigned jet_flavour = 0;
    double sf=0;
    for (unsigned i = 0; i < jets.size(); ++i) {
      rand->SetSeed((int)(((float)jets[i]->eta()+5)*100000));
      eta = fabs(jets[i]->eta());
      pt = jets[i]->pt();
      jet_flavour = jets[i]->hadron_flavour();
      double eff = GetEff(jet_flavour,pt, fabs(eta));
      //Stupid implementation, need to fix
      if(jet_flavour == 5 && era_==era::data_2015){
        if(btag_mode == 2){ 
         sf = reader_mujets->eval_auto_bounds("up",BTagEntry::FLAV_B, eta, pt);
        } else if(btag_mode == 1){
         sf = reader_mujets->eval_auto_bounds("down",BTagEntry::FLAV_B, eta, pt);
        } else {
         sf = reader_mujets->eval_auto_bounds("central",BTagEntry::FLAV_B, eta, pt);
        }
      } else if(jet_flavour == 5 && era_ == era::data_2016){
        if(btag_mode == 2){ 
         sf = reader_comb->eval_auto_bounds("up",BTagEntry::FLAV_B, eta, pt);
        } else if(btag_mode == 1){
         sf = reader_comb->eval_auto_bounds("down",BTagEntry::FLAV_B, eta, pt);
        } else {
         sf = reader_comb->eval_auto_bounds("central",BTagEntry::FLAV_B, eta, pt);
        }
      } else if(jet_flavour == 4 && era_==era::data_2015){
        if(btag_mode == 2){ 
         sf = reader_mujets->eval_auto_bounds("up",BTagEntry::FLAV_C, eta, pt);
        } else if(btag_mode == 1){
         sf = reader_mujets->eval_auto_bounds("down",BTagEntry::FLAV_C, eta, pt);
        } else {
         sf = reader_mujets->eval_auto_bounds("central",BTagEntry::FLAV_C, eta, pt);
       }
      } else if(jet_flavour == 4 && era_==era::data_2016){
        if(btag_mode == 2){ 
         sf = reader_comb->eval_auto_bounds("up",BTagEntry::FLAV_C, eta, pt);
        } else if(btag_mode == 1){
         sf = reader_comb->eval_auto_bounds("down",BTagEntry::FLAV_C, eta, pt);
        } else {
         sf = reader_comb->eval_auto_bounds("central",BTagEntry::FLAV_C, eta, pt);
       }
     } else {
         if(bfake_mode == 2){
          sf = reader_incl->eval_auto_bounds("up",BTagEntry::FLAV_UDSG, eta, pt);
         } else if(bfake_mode == 1){
          sf = reader_incl->eval_auto_bounds("down",BTagEntry::FLAV_UDSG, eta, pt);
         } else{
           sf = reader_incl->eval_auto_bounds("central",BTagEntry::FLAV_UDSG, eta, pt);
         }
      }
      double demoteProb_btag = 0;
      double promoteProb_btag = 0;
      if(sf < 1) {
        demoteProb_btag = fabs(1.0 - sf);
      } else {
        promoteProb_btag = fabs(sf - 1.0)/((1./eff) - 1.0);
      }
      if (verbose) {
        std::cout << "Jet " << i << " " << jets[i]->vector() << "  csv: " << jets[i]->GetBDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags") << "  hadron flavour: " << jets[i]->hadron_flavour() << std::endl;
        std::cout << "-- random seed: " << ((int)((jets[i]->eta()+5)*100000)) << std::endl;
        std::cout << "-- efficiency: " << eff << std::endl;
        std::cout << "-- scale factor: " << sf << std::endl;
      }
      bool passtag;
      if(channel_ != channel::tt || era_==era::data_2016){
        double tight_wp = 0.8;
        if(strategy_ == strategy::mssmsummer16) tight_wp = 0.8484;
        passtag  = jets[i]->GetBDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags") > tight_wp;
      } else {
        passtag  = jets[i]->GetBDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags") > 0.46;
      }
      double randVal = rand->Uniform();
      if(passtag) {                       // if tagged
        if(demoteProb_btag > 0. && randVal < demoteProb_btag) {
          // jets[i]->SetBDiscriminator("combinedSecondaryVertexBJetTags", 0.60);
          pass_result[jets[i]->id()] = false;
          if (verbose) std::cout << "-- demote prob is " << demoteProb_btag << ", rand is " << randVal << ". Jet is demoted" << std::endl;
        } else {
          pass_result[jets[i]->id()] = true;            
          if (verbose) std::cout << "-- demote prob is " << demoteProb_btag << ", rand is " << randVal << ". Jet remains tagged" << std::endl;
        }
      } else {
        if(promoteProb_btag > 0. && randVal < promoteProb_btag) {
          // jets[i]->SetBDiscriminator("combinedSecondaryVertexBJetTags", 0.70);
          pass_result[jets[i]->id()] = true;
          if (verbose) std::cout << "-- promote prob is " << promoteProb_btag << ", rand is " << randVal << ". Jet is promoted" << std::endl;
        } else {
          pass_result[jets[i]->id()] = false;  
          if (verbose) std::cout << "-- promote prob is " << promoteProb_btag << ", rand is " << randVal << ". Jet remains untagged" << std::endl;
        }
      }
    }
    return pass_result;
  }


  double BTagWeightRun2::GetEff(unsigned flav, double pt, double eta) const {
    double res=0;
    if(flav == 5){
     if(pt > bbtag_eff_->GetXaxis()->GetBinLowEdge(bbtag_eff_->GetNbinsX()+1)){
       res = bbtag_eff_->GetBinContent(bbtag_eff_->GetNbinsX(),bbtag_eff_->GetYaxis()->FindBin(eta));
   }  else{
       res = bbtag_eff_->GetBinContent(bbtag_eff_->GetXaxis()->FindBin(pt),bbtag_eff_->GetYaxis()->FindBin(eta));
     }
   } else if(flav == 4){
     if(pt > cbtag_eff_->GetXaxis()->GetBinLowEdge(cbtag_eff_->GetNbinsX()+1)){
       res = cbtag_eff_->GetBinContent(cbtag_eff_->GetNbinsX(),cbtag_eff_->GetYaxis()->FindBin(eta));
   }  else{
       res = cbtag_eff_->GetBinContent(cbtag_eff_->GetXaxis()->FindBin(pt),cbtag_eff_->GetYaxis()->FindBin(eta));
     }
   } else {
      if(pt > othbtag_eff_->GetXaxis()->GetBinLowEdge(othbtag_eff_->GetNbinsX()+1)){
       res = othbtag_eff_->GetBinContent(othbtag_eff_->GetNbinsX(),othbtag_eff_->GetYaxis()->FindBin(eta));
    } else{
       res = othbtag_eff_->GetBinContent(othbtag_eff_->GetXaxis()->FindBin(pt),othbtag_eff_->GetYaxis()->FindBin(eta));
     }
   } 
   return res;

  }
 

}
