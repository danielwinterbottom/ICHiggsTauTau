#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTFakeFactorWeights.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "boost/format.hpp"
#include "TMVA/Reader.h"
#include "TVector3.h"
#include "TFile.h"
#include "TString.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp> 

namespace ic {

  HTTFakeFactorWeights::HTTFakeFactorWeights(std::string const& name) : ModuleBase(name),
    channel_(channel::et), strategy_(strategy::mssmsummer16){ 
    met_label_ = "pfMET";
    jets_label_ = "ak4PFJetsCHS";
    ditau_label_ = "ditau";
    categories_ = "inclusive";
    do_systematics_= false;
    ff_file_= "20170330_tight";
  }

  HTTFakeFactorWeights::~HTTFakeFactorWeights() {
    ;
  }

  int HTTFakeFactorWeights::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HTTFakeFactorWeights" << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    std::cout << boost::format(param_fmt()) % "channel"         % Channel2String(channel_);
    std::cout << boost::format(param_fmt()) % "met_label"       % met_label_;
    std::cout << boost::format(param_fmt()) % "jets_label"      % jets_label_;
    std::cout << boost::format(param_fmt()) % "ditau_label"     % ditau_label_;
    
    boost::split(category_names_, categories_, boost::is_any_of(","), boost::token_compress_on);
    std::string baseDir = (std::string)getenv("CMSSW_BASE") + "/src/";
    
    std::string channel = Channel2String(channel_);
    for(unsigned i=0; i<category_names_.size(); ++i){
      std::string ff_file_name = "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/fake_factors/Jet2TauFakesFiles/"+ff_file_+"/"+channel+"/"+category_names_[i]+"/fakeFactors_"+ff_file_+".root";
      ff_file_name = baseDir + ff_file_name;
      TFile* ff_file = new TFile(ff_file_name.c_str());
      FakeFactor* ff = (FakeFactor*)ff_file->Get("ff_comb");
      std::string map_key = category_names_[i];
      fake_factors_[map_key]  = std::shared_ptr<FakeFactor>(ff);
      ff_file->Close();
      delete ff_file;
    }

    return 0;
  }

  int HTTFakeFactorWeights::Execute(TreeEvent *event) {
      
    if(channel_ != channel::et && channel_ != channel::mt && channel_ != channel::tt) return 0;

    Met * met = event->GetPtr<Met>(met_label_);
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
    std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>(ditau_label_);
    CompositeCandidate const* ditau = ditau_vec.at(0);
    Candidate const* lep1 = ditau->GetCandidate("lepton1");
    Candidate const* lep2 = ditau->GetCandidate("lepton2");
    
    // Get all inputs needed by FF 
    double pt_1_ = lep1->pt();  
    double pt_2_ = lep2->pt();
    double m_vis_ = ditau->M();
    double mt_1_ = MT(lep1, met);
    
    double iso_1_ = 0;
    if (channel_ == channel::et) {
      Electron const* elec = dynamic_cast<Electron const*>(lep1);
      iso_1_ = PF03IsolationVal(elec, 0.5, 0);
    } else if (channel_ == channel::mt){
      Muon const* muon = dynamic_cast<Muon const*>(lep1);
      iso_1_ = PF04IsolationVal(muon, 0.5, 0);
    }
    
    double tau_decaymode_1_=0;
    double tau_decaymode_2_=0;
    if(channel_ == channel::et || channel_ == channel::mt){
      Tau const* tau = dynamic_cast<Tau const*>(lep2);
      tau_decaymode_2_ = tau->decay_mode();
    } else if (channel_ == channel::tt){
      Tau const* tau1 = dynamic_cast<Tau const*>(lep1);
      Tau const* tau2 = dynamic_cast<Tau const*>(lep2);
      tau_decaymode_1_ = tau1->decay_mode();
      tau_decaymode_2_ = tau2->decay_mode();  
    }
    
    double n_jets_ = jets.size();
    
    std::vector<double> inputs(6);
    std::vector<double> tt_inputs_1(6);
    std::vector<double> tt_inputs_2(6);
    if(channel_ == channel::et || channel_ == channel::mt){
      inputs[0] = pt_2_; inputs[1] = tau_decaymode_2_; inputs[2] = n_jets_; inputs[3] = m_vis_; inputs[4] = mt_1_; inputs[5] = iso_1_;    
    } else if (channel_ == channel::tt){
      double mt_tot_ = sqrt(pow(MT(lep1, met),2) + pow(MT(lep2, met),2) + pow(MT(lep1, lep2),2));  
      tt_inputs_1[0] = pt_1_; tt_inputs_1[1] = pt_2_; tt_inputs_1[2] = tau_decaymode_1_; tt_inputs_1[3] = n_jets_; tt_inputs_1[4] = m_vis_; tt_inputs_1[5] = mt_tot_;
      tt_inputs_2[0] = pt_2_; tt_inputs_2[1] = pt_1_; tt_inputs_2[2] = tau_decaymode_2_; tt_inputs_2[3] = n_jets_; tt_inputs_2[4] = m_vis_; tt_inputs_2[5] = mt_tot_;
    }
    
    // Need to loop over all categories and add a ff weight to the event for each
    for(unsigned i=0; i<category_names_.size(); ++i){
      std::string map_key = category_names_[i];
      
      // Retrieve fake factors and add to event as weights
      if(channel_ == channel::et || channel_ == channel::mt){
        double ff_nom = fake_factors_[map_key]->value(inputs);
        event->Add("wt_ff_"+map_key, ff_nom);
        
        if(do_systematics_){
          std::vector<std::string> systematics = {"ff_qcd_syst_up","ff_qcd_syst_down","ff_qcd_dm0_njet0_stat_up","ff_qcd_dm0_njet0_stat_down","ff_qcd_dm0_njet1_stat_up","ff_qcd_dm0_njet1_stat_down","ff_qcd_dm1_njet0_stat_up","ff_qcd_dm1_njet0_stat_down","ff_qcd_dm1_njet1_stat_up","ff_qcd_dm1_njet1_stat_down","ff_w_syst_up","ff_w_syst_down","ff_w_dm0_njet0_stat_up","ff_w_dm0_njet0_stat_down","ff_w_dm0_njet1_stat_up","ff_w_dm0_njet1_stat_down","ff_w_dm1_njet0_stat_up","ff_w_dm1_njet0_stat_down","ff_w_dm1_njet1_stat_up","ff_w_dm1_njet1_stat_down","ff_tt_syst_up","ff_tt_syst_down","ff_tt_dm0_njet0_stat_up","ff_tt_dm0_njet0_stat_down","ff_tt_dm0_njet1_stat_up","ff_tt_dm0_njet1_stat_down","ff_tt_dm1_njet0_stat_up","ff_tt_dm1_njet0_stat_down" ,"ff_tt_dm1_njet1_stat_up","ff_tt_dm1_njet1_stat_down"};
          for(unsigned j=0; j<systematics.size(); ++j){
            std::string syst = systematics[j];
            double ff_syst = fake_factors_[map_key]->value(inputs,syst);
            std::string syst_name = "wt_"+map_key+"_"+syst;
            event->Add(syst_name, ff_syst);
          } 
        }
      } else if(channel_ == channel::tt){
        double ff_nom_1 = fake_factors_[map_key]->value(tt_inputs_1)*0.5;
        double ff_nom_2 = fake_factors_[map_key]->value(tt_inputs_2)*0.5;
        event->Add("wt_ff_"+map_key, ff_nom_1);
        event->Add("wt_ff_"+map_key+"_2", ff_nom_2);
        
        if(do_systematics_){
          std::vector<std::string> systematics = {"ff_qcd_syst_up","ff_qcd_syst_down","ff_qcd_dm0_njet0_stat_up","ff_qcd_dm0_njet0_stat_down","ff_qcd_dm0_njet1_stat_up","ff_qcd_dm0_njet1_stat_down","ff_qcd_dm1_njet0_stat_up","ff_qcd_dm1_njet0_stat_down","ff_qcd_dm1_njet1_stat_up","ff_qcd_dm1_njet1_stat_down","ff_w_syst_up","ff_w_syst_down","ff_tt_syst_up","ff_tt_syst_down", "ff_w_frac_syst_up", "ff_w_frac_syst_down", "ff_tt_frac_syst_up", "ff_tt_frac_syst_down", "ff_dy_frac_syst_up", "ff_dy_frac_syst_down"};
          
          for(unsigned j=0; j<systematics.size(); ++j){
            std::string syst = systematics[j];
            double ff_syst_1 = fake_factors_[map_key]->value(tt_inputs_1,syst)*0.5;
            double ff_syst_2 = fake_factors_[map_key]->value(tt_inputs_2,syst)*0.5;
            std::string syst_name = "wt_"+map_key+"_"+syst;
            event->Add(syst_name+"_1", ff_syst_1);
            event->Add(syst_name+"_2", ff_syst_2);
          } 
        }
      }
    }

    return 0;
  }
  
  int HTTFakeFactorWeights::PostAnalysis() {
    return 0;
  }

  void HTTFakeFactorWeights::PrintInfo() {
    ;
  }
}
