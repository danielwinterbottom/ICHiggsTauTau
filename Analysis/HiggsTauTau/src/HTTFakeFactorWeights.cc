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
    fracs_file_ = "";
    is_embedded_ = "";
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
    if(strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16) category_names_ = {"inclusive"};
    
    std::string channel = Channel2String(channel_);
    for(unsigned i=0; i<category_names_.size(); ++i){
      if(strategy_==strategy::cpsummer17) continue;  
      std::string ff_file_name;
      if(strategy_ == strategy::mssmsummer16) ff_file_name = "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/fake_factors/Jet2TauFakesFiles/"+ff_file_+"/"+channel+"/"+category_names_[i]+"/fakeFactors_"+ff_file_+".root";
      if(strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16){
        ff_file_name = "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/fake_factors/Jet2TauFakesFiles/"+ff_file_+"/"+channel+"/fakeFactors_"+ff_file_+".root";
      }
      ff_file_name = baseDir + ff_file_name;
      TFile* ff_file = new TFile(ff_file_name.c_str());
      FakeFactor* ff = (FakeFactor*)ff_file->Get("ff_comb");
      std::string map_key = category_names_[i];
      fake_factors_[map_key]  = std::shared_ptr<FakeFactor>(ff);
      ff_file->Close();
      delete ff_file;
    }
    
    if(fracs_file_!="" && strategy_!=strategy::cpsummer17) {
      TFile f(fracs_file_.c_str());
      w_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));
      f.Close();
      
      fns_["w_et_fracs"] = std::shared_ptr<RooFunctor>(
            w_->function("w_et_fracs")->functor(w_->argSet("mvis,pt_tt,njets,mjj,nbjets")));
      fns_["qcd_et_fracs"] = std::shared_ptr<RooFunctor>(
            w_->function("qcd_et_fracs")->functor(w_->argSet("mvis,pt_tt,njets,mjj,nbjets")));
      fns_["ttbar_et_fracs"] = std::shared_ptr<RooFunctor>(
            w_->function("ttbar_et_fracs")->functor(w_->argSet("mvis,pt_tt,njets,mjj,nbjets")));
      fns_["realtau_et_sf"] = std::shared_ptr<RooFunctor>(
            w_->function("realtau_et_sf")->functor(w_->argSet("mvis,pt_tt,njets,mjj,nbjets")));
      fns_["w_mt_fracs"] = std::shared_ptr<RooFunctor>(
            w_->function("w_mt_fracs")->functor(w_->argSet("mvis,pt_tt,njets,mjj,nbjets")));
      fns_["qcd_mt_fracs"] = std::shared_ptr<RooFunctor>(
            w_->function("qcd_mt_fracs")->functor(w_->argSet("mvis,pt_tt,njets,mjj,nbjets")));
      fns_["ttbar_mt_fracs"] = std::shared_ptr<RooFunctor>(
            w_->function("ttbar_mt_fracs")->functor(w_->argSet("mvis,pt_tt,njets,mjj,nbjets")));
      fns_["realtau_mt_sf"] = std::shared_ptr<RooFunctor>(
            w_->function("realtau_mt_sf")->functor(w_->argSet("mvis,pt_tt,njets,mjj,nbjets")));
      fns_["w_tt_fracs_1"] = std::shared_ptr<RooFunctor>(
            w_->function("w_tt_fracs_1")->functor(w_->argSet("mvis,pt_tt,njets,mjj")));
      fns_["qcd_tt_fracs_1"] = std::shared_ptr<RooFunctor>(
            w_->function("qcd_tt_fracs_1")->functor(w_->argSet("mvis,pt_tt,njets,mjj")));
      fns_["ttbar_tt_fracs_1"] = std::shared_ptr<RooFunctor>(
            w_->function("ttbar_tt_fracs_1")->functor(w_->argSet("mvis,pt_tt,njets,mjj")));
      fns_["dy_tt_fracs_1"] = std::shared_ptr<RooFunctor>(
            w_->function("dy_tt_fracs_1")->functor(w_->argSet("mvis,pt_tt,njets,mjj")));
      fns_["realtau_tt_sf_1"] = std::shared_ptr<RooFunctor>(
            w_->function("realtau_tt_sf_1")->functor(w_->argSet("mvis,pt_tt,njets,mjj")));
      fns_["w_tt_fracs_2"] = std::shared_ptr<RooFunctor>(
            w_->function("w_tt_fracs_2")->functor(w_->argSet("mvis,pt_tt,njets,mjj")));
      fns_["qcd_tt_fracs_2"] = std::shared_ptr<RooFunctor>(
            w_->function("qcd_tt_fracs_2")->functor(w_->argSet("mvis,pt_tt,njets,mjj")));
      fns_["ttbar_tt_fracs_2"] = std::shared_ptr<RooFunctor>(
            w_->function("ttbar_tt_fracs_2")->functor(w_->argSet("mvis,pt_tt,njets,mjj")));
      fns_["dy_tt_fracs_2"] = std::shared_ptr<RooFunctor>(
            w_->function("dy_tt_fracs_2")->functor(w_->argSet("mvis,pt_tt,njets,mjj")));
      fns_["realtau_tt_sf_2"] = std::shared_ptr<RooFunctor>(
            w_->function("realtau_tt_sf_2")->functor(w_->argSet("mvis,pt_tt,njets,mjj")));
      
    }
    if(strategy_==strategy::cpsummer17){
      TFile *f = new TFile("input/fake_factors/zmm_taufakes_2017.root");
      ff_hist_dm0_ = (TH1D*)f->Get("ff_dm0")->Clone();
      ff_hist_dm1_ = (TH1D*)f->Get("ff_dm1")->Clone();
      ff_hist_dm10_ = (TH1D*)f->Get("ff_dm10")->Clone();
      ff_hist_dm0_->SetDirectory(0);
      ff_hist_dm1_->SetDirectory(0);
      ff_hist_dm10_->SetDirectory(0);
      f->Close();    
    }

    return 0;
  }

  int HTTFakeFactorWeights::Execute(TreeEvent *event) {
      
    if(channel_ != channel::et && channel_ != channel::mt && channel_ != channel::tt) return 0;

    Met * met = event->GetPtr<Met>(met_label_);
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
    std::vector<PFJet*> bjets = jets;
    ic::erase_if(bjets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
    
    std::string btag_label = "pfCombinedInclusiveSecondaryVertexV2BJetTags";
    double btag_wp;
    if(strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16) btag_wp = 0.8484;
    if(strategy_ == strategy::cpsummer17) btag_wp = 0.8838;
    if (event->Exists("retag_result")) {
      auto const& retag_result = event->Get<std::map<std::size_t,bool>>("retag_result"); 
      ic::erase_if(bjets, !boost::bind(IsReBTagged, _1, retag_result));
    } else{ 
      ic::erase_if(bjets, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label) < btag_wp);
    } 
    
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
    double n_bjets_ = bjets.size();
    
    std::vector<double> inputs;
    std::vector<double> tt_inputs_1;
    std::vector<double> tt_inputs_2;
    if(strategy_ == strategy::mssmsummer16) {
      inputs.resize(6);
      tt_inputs_1.resize(6);
      tt_inputs_2.resize(6);
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
    } else if(strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16) {
      double pt_tt = ditau->pt();
      double mjj=0.;
      if(jets.size()>=2) mjj=(jets[0]->vector()+jets[1]->vector()).M();
      inputs.resize(9);
      tt_inputs_1.resize(9);
      tt_inputs_2.resize(9);
      if(channel_ == channel::et || channel_ == channel::mt){
        auto args = std::vector<double>{m_vis_,pt_tt,n_jets_,mjj,n_bjets_};  
        double qcd_frac=0.5, w_frac=0.5, tt_frac=0., realtau_sf=1.0;
        if(channel_ == channel::et){
          qcd_frac = fns_["qcd_et_fracs"]->eval(args.data());  
          w_frac = fns_["w_et_fracs"]->eval(args.data());
          tt_frac = fns_["ttbar_et_fracs"]->eval(args.data());
          realtau_sf = fns_["realtau_et_sf"]->eval(args.data()); 
        }
        if(channel_ == channel::mt){
          auto args = std::vector<double>{m_vis_,pt_tt,n_jets_,mjj,n_bjets_};    
          qcd_frac = fns_["qcd_mt_fracs"]->eval(args.data());  
          w_frac = fns_["w_mt_fracs"]->eval(args.data());
          tt_frac = fns_["ttbar_mt_fracs"]->eval(args.data());
          realtau_sf = fns_["realtau_mt_sf"]->eval(args.data());
        }
        inputs[0] = pt_2_; inputs[1] = tau_decaymode_2_; inputs[2] = n_jets_; inputs[3] = m_vis_; inputs[4] = mt_1_; inputs[5] = iso_1_; inputs[6] = qcd_frac; inputs[7] = w_frac; inputs[8] = tt_frac;
        event->Add("wt_ff_realtau_1", realtau_sf);
      } else if (channel_ == channel::tt){
        auto args = std::vector<double>{m_vis_,pt_tt,n_jets_,mjj};    
        double qcd_frac_1 = fns_["qcd_tt_fracs_1"]->eval(args.data());  
        double w_frac_1 = fns_["w_tt_fracs_1"]->eval(args.data());
        double tt_frac_1 = fns_["ttbar_tt_fracs_1"]->eval(args.data());
        double dy_frac_1 = fns_["dy_tt_fracs_1"]->eval(args.data());
        double realtau_sf_1 = fns_["realtau_tt_sf_1"]->eval(args.data()); 
        double qcd_frac_2 = fns_["qcd_tt_fracs_2"]->eval(args.data());  
        double w_frac_2 = fns_["w_tt_fracs_2"]->eval(args.data());
        double tt_frac_2 = fns_["ttbar_tt_fracs_2"]->eval(args.data());
        double dy_frac_2 = fns_["dy_tt_fracs_2"]->eval(args.data());
        double realtau_sf_2 = fns_["realtau_tt_sf_2"]->eval(args.data());
        event->Add("wt_ff_realtau_1", realtau_sf_1);
        event->Add("wt_ff_realtau_2", realtau_sf_2);

        tt_inputs_1[0] = pt_1_; tt_inputs_1[1] = pt_2_; tt_inputs_1[2] = tau_decaymode_1_; tt_inputs_1[3] = n_jets_; tt_inputs_1[4] = m_vis_; tt_inputs_1[5] = qcd_frac_1; tt_inputs_1[6] = w_frac_1; tt_inputs_1[7] = tt_frac_1; tt_inputs_1[8] = dy_frac_1;
        tt_inputs_2[0] = pt_2_; tt_inputs_2[1] = pt_1_; tt_inputs_2[2] = tau_decaymode_2_; tt_inputs_2[3] = n_jets_; tt_inputs_2[4] = m_vis_; tt_inputs_2[5] = qcd_frac_2; tt_inputs_2[6] = w_frac_2; tt_inputs_2[7] = tt_frac_2; tt_inputs_2[8] = dy_frac_2;
      }
      
      std::string map_key = "inclusive";
      // Retrieve fake factors and add to event as weights
      if(channel_ == channel::et || channel_ == channel::mt){
        double ff_nom = fake_factors_[map_key]->value(inputs);
        event->Add("wt_ff_1", ff_nom);
        
        if(do_systematics_){
          std::vector<std::string> systematics = {"ff_qcd_syst_up","ff_qcd_syst_down","ff_qcd_dm0_njet0_stat_up","ff_qcd_dm0_njet0_stat_down","ff_qcd_dm0_njet1_stat_up","ff_qcd_dm0_njet1_stat_down","ff_qcd_dm1_njet0_stat_up","ff_qcd_dm1_njet0_stat_down","ff_qcd_dm1_njet1_stat_up","ff_qcd_dm1_njet1_stat_down","ff_w_syst_up","ff_w_syst_down","ff_w_dm0_njet0_stat_up","ff_w_dm0_njet0_stat_down","ff_w_dm0_njet1_stat_up","ff_w_dm0_njet1_stat_down","ff_w_dm1_njet0_stat_up","ff_w_dm1_njet0_stat_down","ff_w_dm1_njet1_stat_up","ff_w_dm1_njet1_stat_down","ff_tt_syst_up","ff_tt_syst_down","ff_tt_dm0_njet0_stat_up","ff_tt_dm0_njet0_stat_down","ff_tt_dm0_njet1_stat_up","ff_tt_dm0_njet1_stat_down","ff_tt_dm1_njet0_stat_up","ff_tt_dm1_njet0_stat_down" ,"ff_tt_dm1_njet1_stat_up","ff_tt_dm1_njet1_stat_down"};
          for(unsigned j=0; j<systematics.size(); ++j){
            std::string syst = systematics[j];
            double ff_syst = fake_factors_[map_key]->value(inputs,syst);
            std::string syst_name = "wt_"+syst+"_1";
            event->Add(syst_name, ff_syst);
            if(std::isinf(ff_syst) || std::isnan(ff_syst)){
              ff_syst = 0.0;
            }
          } 
        }
      } else if(channel_ == channel::tt){
        double ff_nom_1 = fake_factors_[map_key]->value(tt_inputs_1)*0.5;
        double ff_nom_2 = fake_factors_[map_key]->value(tt_inputs_2)*0.5;
        event->Add("wt_ff_1",  ff_nom_1);
        event->Add("wt_ff_2",  ff_nom_2);
        
        if(do_systematics_){
          std::vector<std::string> systematics = {"ff_qcd_syst_up","ff_qcd_syst_down","ff_qcd_dm0_njet0_stat_up","ff_qcd_dm0_njet0_stat_down","ff_qcd_dm0_njet1_stat_up","ff_qcd_dm0_njet1_stat_down","ff_qcd_dm1_njet0_stat_up","ff_qcd_dm1_njet0_stat_down","ff_qcd_dm1_njet1_stat_up","ff_qcd_dm1_njet1_stat_down","ff_w_syst_up","ff_w_syst_down","ff_tt_syst_up","ff_tt_syst_down","ff_w_frac_syst_up", "ff_w_frac_syst_down", "ff_tt_frac_syst_up", "ff_tt_frac_syst_down", "ff_dy_frac_syst_up", "ff_dy_frac_syst_down"};
          
          for(unsigned j=0; j<systematics.size(); ++j){
            std::string syst = systematics[j];
            double ff_syst_1 = fake_factors_[map_key]->value(tt_inputs_1,syst)*0.5;
            double ff_syst_2 = fake_factors_[map_key]->value(tt_inputs_2,syst)*0.5;

            std::string syst_name = "wt_"+syst;
            event->Add(syst_name+"_1", ff_syst_1);
            event->Add(syst_name+"_2", ff_syst_2);
          } 
        }
      }
    } else if(strategy_ == strategy::cpsummer17) {
      double ff_nom;
      if(tau_decaymode_2_==0)  ff_nom = ff_hist_dm0_->GetBinContent(ff_hist_dm0_->FindBin(pt_2_));
      if(tau_decaymode_2_==1)  ff_nom = ff_hist_dm1_->GetBinContent(ff_hist_dm1_->FindBin(pt_2_));
      if(tau_decaymode_2_==10) ff_nom = ff_hist_dm10_->GetBinContent(ff_hist_dm10_->FindBin(pt_2_));
      event->Add("wt_ff_1", ff_nom);
      
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
