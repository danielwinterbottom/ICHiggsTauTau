#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/BTagWeightRun2.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include <cmath>
#include <algorithm>

namespace ic {
  BTagWeightRun2::BTagWeightRun2(std::string const& name) : ModuleBase(name) {
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
    calib  = new BTagCalibration("csvv2","./input/btag_sf/CSVv2.csv");
    reader_incl = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "incl","central");
    reader_mujets = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "mujets","central");
    reader_iterativefit = new BTagCalibrationReader(calib, BTagEntry::OP_RESHAPING, "iterativefit","central");
    reader_inclup = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "incl","up");
    reader_incldown = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "incl","down");
    reader_mujetsup = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "mujets","up");
    reader_mujetsdown = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "mujets","down");
    reader_jesup = new BTagCalibrationReader(calib, BTagEntry::OP_RESHAPING, "iterativefit","up_jes");
    reader_jesdown= new BTagCalibrationReader(calib, BTagEntry::OP_RESHAPING, "iterativefit","down_jes");
    reader_lfup = new BTagCalibrationReader(calib, BTagEntry::OP_RESHAPING, "iterativefit","up_lf");
    reader_lfdown= new BTagCalibrationReader(calib, BTagEntry::OP_RESHAPING, "iterativefit","down_lf");
    reader_hfup = new BTagCalibrationReader(calib, BTagEntry::OP_RESHAPING, "iterativefit","up_hf");
    reader_hfdown= new BTagCalibrationReader(calib, BTagEntry::OP_RESHAPING, "iterativefit","down_hf");
    reader_hfstats1up = new BTagCalibrationReader(calib, BTagEntry::OP_RESHAPING, "iterativefit","up_hfstats1");
    reader_hfstats1down= new BTagCalibrationReader(calib, BTagEntry::OP_RESHAPING, "iterativefit","down_hfstats1");
    reader_hfstats2up = new BTagCalibrationReader(calib, BTagEntry::OP_RESHAPING, "iterativefit","up_hfstats2");
    reader_hfstats2down= new BTagCalibrationReader(calib, BTagEntry::OP_RESHAPING, "iterativefit","down_hfstats2");
    reader_lfstats1up = new BTagCalibrationReader(calib, BTagEntry::OP_RESHAPING, "iterativefit","up_lfstats1");
    reader_lfstats1down= new BTagCalibrationReader(calib, BTagEntry::OP_RESHAPING, "iterativefit","down_lfstats1");
    reader_lfstats2up = new BTagCalibrationReader(calib, BTagEntry::OP_RESHAPING, "iterativefit","up_lfstats2");
    reader_lfstats2down= new BTagCalibrationReader(calib, BTagEntry::OP_RESHAPING, "iterativefit","down_lfstats2");
    reader_cferr1up = new BTagCalibrationReader(calib, BTagEntry::OP_RESHAPING, "iterativefit","up_cferr1");
    reader_cferr1down= new BTagCalibrationReader(calib, BTagEntry::OP_RESHAPING, "iterativefit","down_cferr1");
    reader_cferr2up = new BTagCalibrationReader(calib, BTagEntry::OP_RESHAPING, "iterativefit","up_cferr2");
    reader_cferr2down= new BTagCalibrationReader(calib, BTagEntry::OP_RESHAPING, "iterativefit","down_cferr2");

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
           sf = reader_jesup->eval(BTagEntry::FLAV_B, fabs(eta), pt, csv);
          } else if(btag_mode == 3){
           sf = reader_jesdown->eval(BTagEntry::FLAV_B, fabs(eta), pt, csv);
          } else if (btag_mode == 6){
           sf = reader_lfup->eval(BTagEntry::FLAV_B, fabs(eta), pt, csv);
          } else if (btag_mode == 5){
           sf = reader_lfdown->eval(BTagEntry::FLAV_B, fabs(eta), pt, csv);
          } else if (btag_mode == 10){
           sf = reader_hfstats1up->eval(BTagEntry::FLAV_B, fabs(eta), pt, csv);
          } else if (btag_mode == 9 ){
           sf = reader_hfstats1down->eval(BTagEntry::FLAV_B, fabs(eta), pt, csv);
          } else if (btag_mode == 12){
           sf = reader_hfstats2up->eval(BTagEntry::FLAV_B, fabs(eta), pt, csv);
          } else if (btag_mode == 11){
           sf = reader_hfstats2down->eval(BTagEntry::FLAV_B, fabs(eta), pt, csv);
          } else {
           sf = reader_iterativefit->eval(BTagEntry::FLAV_B, fabs(eta), pt, csv);
          }
        } else if(jet_flavour == 4){
          if(btag_mode == 18){
           sf = reader_cferr1up->eval(BTagEntry::FLAV_C, fabs(eta), pt, csv);
          } else if(btag_mode == 17){
           sf = reader_cferr1down->eval(BTagEntry::FLAV_C, fabs(eta), pt, csv);
          } else if (btag_mode == 20){
           sf = reader_cferr2up->eval(BTagEntry::FLAV_C, fabs(eta), pt, csv);
          } else if (btag_mode == 19){
           sf = reader_cferr2down->eval(BTagEntry::FLAV_C, fabs(eta), pt, csv);
          } else {
            sf = 1; 
          }
        } else {
          if(btag_mode == 4){
           sf = reader_jesup->eval(BTagEntry::FLAV_UDSG, fabs(eta), pt, csv);
          } else if (btag_mode == 3 ){
           sf = reader_jesdown->eval(BTagEntry::FLAV_UDSG, fabs(eta), pt, csv);
          } else if (btag_mode == 8){
           sf = reader_hfup->eval(BTagEntry::FLAV_UDSG, fabs(eta), pt, csv);
          } else if (btag_mode == 7){
           sf = reader_hfdown->eval(BTagEntry::FLAV_UDSG, fabs(eta), pt, csv);
          } else if (btag_mode == 14){
           sf = reader_lfstats1up->eval(BTagEntry::FLAV_UDSG, fabs(eta), pt, csv);
          } else if (btag_mode == 13){
           sf = reader_lfstats1down->eval(BTagEntry::FLAV_UDSG, fabs(eta), pt, csv);
          } else if (btag_mode == 16){
           sf = reader_lfstats2up->eval(BTagEntry::FLAV_UDSG, fabs(eta), pt, csv);
          } else if (btag_mode == 15){
           sf = reader_lfstats2down->eval(BTagEntry::FLAV_UDSG, fabs(eta), pt, csv);
          } else{
           sf = reader_iterativefit->eval(BTagEntry::FLAV_UDSG, fabs(eta), pt, csv);
          }
        }
       result *=sf;
      }
    return result;
  }

  std::map<std::size_t, bool> BTagWeightRun2::ReTag(std::vector<PFJet *> const& jets, unsigned btag_mode, unsigned bfake_mode) const {
    bool verbose = false;
    std::map<std::size_t, bool> pass_result;
    double pt = 0/* = embed_jets[i]->pt()*/;
    double eta = 0/* embed_jets[i]->eta()*/;
    unsigned jet_flavour = 0;
    double sf=0;
    double sub_sf=0;
    for (unsigned i = 0; i < jets.size(); ++i) {
      rand->SetSeed((int)((jets[i]->eta()+5)*100000));
      eta = jets[i]->eta();
      pt = jets[i]->pt();
      jet_flavour = jets[i]->hadron_flavour();
      double eff = GetEff(jet_flavour,pt, fabs(eta));
      //Stupid implementation, need to fix
      if(jet_flavour == 5){
        if(pt > 670){
          if(btag_mode == 2){ //Need to double the uncertainty
           sf = reader_mujetsup->eval(BTagEntry::FLAV_B, eta, 670);
           sub_sf = reader_mujets->eval(BTagEntry::FLAV_B, eta, 670);
           sf = 2*(sf-sub_sf)+sub_sf;
          } else if(btag_mode == 1){
           sf = reader_mujetsdown->eval(BTagEntry::FLAV_B, eta, 670);
           sub_sf = reader_mujets->eval(BTagEntry::FLAV_B, eta, 670);
           sf = 2*(sf-sub_sf)+sub_sf;
          } else {
            sf = reader_mujets->eval(BTagEntry::FLAV_B, eta, 670);
          }
         } else if (pt < 30){
          if(btag_mode == 2){ //Need to double the uncertainty
           sf = reader_mujetsup->eval(BTagEntry::FLAV_B, eta, 30);
           sub_sf = reader_mujets->eval(BTagEntry::FLAV_B, eta, 30);
           sf = 2*(sf-sub_sf)+sub_sf;
          } else if(btag_mode == 1){
           sf = reader_mujetsdown->eval(BTagEntry::FLAV_B, eta, 30);
           sub_sf = reader_mujets->eval(BTagEntry::FLAV_B, eta, 30);
           sf = 2*(sf-sub_sf)+sub_sf;
          } else {
           sf = reader_mujets->eval(BTagEntry::FLAV_B, eta, 30);
          }
         } else{
          if(btag_mode == 2){ 
           sf = reader_mujetsup->eval(BTagEntry::FLAV_B, eta, pt);
          } else if(btag_mode == 1){
           sf = reader_mujetsdown->eval(BTagEntry::FLAV_B, eta, pt);
          } else {
           sf = reader_mujets->eval(BTagEntry::FLAV_B, eta, pt);
          }
         }
        } else if(jet_flavour == 4){
         if (pt > 670){
          if(btag_mode == 2){ //Need to double the uncertainty
           sf = reader_mujetsup->eval(BTagEntry::FLAV_C, eta, 670);
           sub_sf = reader_mujets->eval(BTagEntry::FLAV_C, eta, 670);
           sf = 2*(sf-sub_sf)+sub_sf;
          } else if(btag_mode == 1){
           sf = reader_mujetsdown->eval(BTagEntry::FLAV_C, eta, 670);
           sub_sf = reader_mujets->eval(BTagEntry::FLAV_C, eta, 670);
           sf = 2*(sf-sub_sf)+sub_sf;
          } else {
            sf = reader_mujets->eval(BTagEntry::FLAV_C, eta, 670);
          }
         } else if(pt<30){
          if(btag_mode == 2){ //Need to double the uncertainty
           sf = reader_mujetsup->eval(BTagEntry::FLAV_C, eta, 30);
           sub_sf = reader_mujets->eval(BTagEntry::FLAV_C, eta, 30);
           sf = 2*(sf-sub_sf)+sub_sf;
          } else if(btag_mode == 1){
           sf = reader_mujetsdown->eval(BTagEntry::FLAV_C, eta, 30);
           sub_sf = reader_mujets->eval(BTagEntry::FLAV_C, eta, 30);
           sf = 2*(sf-sub_sf)+sub_sf;
          } else {
            sf = reader_mujets->eval(BTagEntry::FLAV_C, eta, 30);
          }
        } else{
          if(btag_mode == 2){ 
           sf = reader_mujetsup->eval(BTagEntry::FLAV_C, eta, pt);
          } else if(btag_mode == 1){
           sf = reader_mujetsdown->eval(BTagEntry::FLAV_C, eta, pt);
          } else {
           sf = reader_mujets->eval(BTagEntry::FLAV_C, eta, pt);
         }
       }
      } else {
         if (pt > 1000){
           if(bfake_mode == 2){
             sf = reader_inclup->eval(BTagEntry::FLAV_UDSG, eta, 1000);
             sub_sf = reader_incl->eval(BTagEntry::FLAV_UDSG, eta, 1000);
             sf = 2*(sf-sub_sf)+sub_sf;
           } else if(bfake_mode ==1 ){
             sf = reader_incldown->eval(BTagEntry::FLAV_UDSG, eta, 1000);
             sub_sf = reader_incl->eval(BTagEntry::FLAV_UDSG, eta, 1000);
             sf = 2*(sf-sub_sf)+sub_sf;
           }else{
            sf = reader_incl->eval(BTagEntry::FLAV_UDSG, eta, 1000);
           }
          } else {
           if(bfake_mode == 2){
            sf = reader_inclup->eval(BTagEntry::FLAV_UDSG, eta, pt);
           } else if(bfake_mode == 1){
            sf = reader_incldown->eval(BTagEntry::FLAV_UDSG, eta, pt);
           } else{
             sf = reader_incl->eval(BTagEntry::FLAV_UDSG, eta, pt);
           }
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
      bool passtag = jets[i]->GetBDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags") > 0.8;
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
