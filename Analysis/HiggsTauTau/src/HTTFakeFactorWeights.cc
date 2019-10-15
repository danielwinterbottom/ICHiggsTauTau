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
    channel_(channel::et), strategy_(strategy::mssmsummer16), era_(era::data_2012_rereco){ 
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

    std::cout << boost::format(param_fmt()) % "channel"     % Channel2String(channel_);
    std::cout << boost::format(param_fmt()) % "era"         % Era2String(era_);
    std::cout << boost::format(param_fmt()) % "met_label"   % met_label_;
    std::cout << boost::format(param_fmt()) % "jets_label"  % jets_label_;
    std::cout << boost::format(param_fmt()) % "ditau_label" % ditau_label_;
    std::cout << boost::format(param_fmt()) % "ff_file"     % ff_file_;
    std::cout << boost::format(param_fmt()) % "fracs_file"  % fracs_file_;
     
    boost::split(category_names_, categories_, boost::is_any_of(","), boost::token_compress_on);
    std::string baseDir = (std::string)getenv("CMSSW_BASE") + "/src/";
    if(strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) category_names_ = {"inclusive"};

    if((strategy_ == strategy::cpdecays18 || strategy_ == strategy::cpdecays16 || strategy_ == strategy::legacy16) && channel_==channel::tt) {
      TFile f((baseDir+"UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/"+ff_file_).c_str());
      ff_ws_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));
      f.Close();

      std::vector<std::string> systs = {"","_qcd_syst_up","_qcd_syst_down","_wjets_syst_up","_wjets_syst_down","_ttbar_syst_up","_ttbar_syst_down","_qcd_stat_njet0_mvadm0_sig_lt3_up","_qcd_stat_njet0_mvadm0_sig_lt3_down","_qcd_stat_njet0_mvadm0_sig_gt3_up","_qcd_stat_njet0_mvadm0_sig_gt3_down","_qcd_stat_njet0_mvadm1_up","_qcd_stat_njet0_mvadm1_down","_qcd_stat_njet0_mvadm2_up","_qcd_stat_njet0_mvadm2_down","_qcd_stat_njet0_mvadm10_up","_qcd_stat_njet0_mvadm10_down","_qcd_stat_njet0_mvadm11_up","_qcd_stat_njet0_mvadm11_down","_qcd_stat_njet1_mvadm0_sig_lt3_up","_qcd_stat_njet1_mvadm0_sig_lt3_down","_qcd_stat_njet1_mvadm0_sig_gt3_up","_qcd_stat_njet1_mvadm0_sig_gt3_down","_qcd_stat_njet1_mvadm1_up","_qcd_stat_njet1_mvadm1_down","_qcd_stat_njet1_mvadm2_up","_qcd_stat_njet1_mvadm2_down","_qcd_stat_njet1_mvadm10_up","_qcd_stat_njet1_mvadm10_down","_qcd_stat_njet1_mvadm11_up","_qcd_stat_njet1_mvadm11_down","_qcd_stat_njet2_mvadm0_sig_lt3_up","_qcd_stat_njet2_mvadm0_sig_lt3_down","_qcd_stat_njet2_mvadm0_sig_gt3_up","_qcd_stat_njet2_mvadm0_sig_gt3_down","_qcd_stat_njet2_mvadm1_up","_qcd_stat_njet2_mvadm1_down","_qcd_stat_njet2_mvadm2_up","_qcd_stat_njet2_mvadm2_down","_qcd_stat_njet2_mvadm10_up","_qcd_stat_njet2_mvadm10_down","_qcd_stat_njet2_mvadm11_up","_qcd_stat_njet2_mvadm11_down","_qcd_stat_pt2_up","_qcd_stat_pt2_down","_qcd_stat_met_up","_qcd_stat_met_down"  
      };
      for(auto s : systs) {
        fns_["ff_tt_tight_mvadmbins"+s] = std::shared_ptr<RooFunctor>(
              ff_ws_->function(("ff_tt_tight_mvadmbins"+s).c_str())->functor(ff_ws_->argSet("pt,mvadm,ipsig,njets,pt_2,os,met")));
      }
    }

    
    std::string channel = Channel2String(channel_);
    for(unsigned i=0; i<category_names_.size(); ++i){
      std::string ff_file_name;
      if(strategy_ == strategy::mssmsummer16) ff_file_name = "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/fake_factors/Jet2TauFakesFiles/"+ff_file_+"/"+channel+"/"+category_names_[i]+"/fakeFactors_"+ff_file_+".root";
      if(strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16){
        ff_file_name = "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/fake_factors/Jet2TauFakesFiles2016/"+ff_file_;
      }
      if(strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17){
        ff_file_name = "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/fake_factors/Jet2TauFakesFilesNew/"+ff_file_;
      }
      if(strategy_ == strategy::cpdecays18){
        ff_file_name = "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/input/fake_factors/Jet2TauFakesFiles2018New/"+ff_file_;
      }

      ff_file_name = baseDir + ff_file_name;
      
      std::string map_key = category_names_[i];
      std::string map_name = "ff_map_"+channel+"_"+map_key;
      if(strategy_ != strategy::cpdecays18 && strategy_ != strategy::cpdecays16 && strategy_ != strategy::legacy16){
        if(ProductExists(map_name)){
          fake_factors_[map_key] = GetProduct<std::shared_ptr<FakeFactor>>(map_name);
          std::cout << "Getting " << fake_factors_[map_key] << std::endl;
        } else {
          TFile* ff_file = new TFile(ff_file_name.c_str());
          FakeFactor* ff = (FakeFactor*)ff_file->Get("ff_comb");
          fake_factors_[map_key]  = std::shared_ptr<FakeFactor>(ff);
          AddToProducts(map_name, fake_factors_[map_key]);
          std::cout << "Adding " << fake_factors_[map_key] << std::endl;
          ff_file->Close();
          delete ff_file;
        }
      }
    }
    
    if(fracs_file_!="") {
      TFile f(fracs_file_.c_str());
      w_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));
      f.Close();
       
      if(false){      
        fns_["w_et_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("w_et_fracs")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["qcd_et_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("qcd_et_fracs")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["ttbar_et_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("ttbar_et_fracs")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["w_et_ss_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("w_et_ss_fracs")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["qcd_et_ss_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("qcd_et_ss_fracs")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["ttbar_et_ss_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("ttbar_et_ss_fracs")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["w_et_highmt_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("w_et_highmt_fracs")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["qcd_et_highmt_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("qcd_et_highmt_fracs")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["ttbar_et_highmt_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("ttbar_et_highmt_fracs")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["w_mt_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("w_mt_fracs")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["qcd_mt_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("qcd_mt_fracs")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["ttbar_mt_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("ttbar_mt_fracs")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["w_mt_ss_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("w_mt_ss_fracs")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["qcd_mt_ss_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("qcd_mt_ss_fracs")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["ttbar_mt_ss_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("ttbar_mt_ss_fracs")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["w_mt_highmt_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("w_mt_highmt_fracs")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["qcd_mt_highmt_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("qcd_mt_highmt_fracs")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["ttbar_mt_highmt_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("ttbar_mt_highmt_fracs")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["w_tt_fracs_1"] = std::shared_ptr<RooFunctor>(
              w_->function("w_tt_fracs_1")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["qcd_tt_fracs_1"] = std::shared_ptr<RooFunctor>(
              w_->function("qcd_tt_fracs_1")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["ttbar_tt_fracs_1"] = std::shared_ptr<RooFunctor>(
              w_->function("ttbar_tt_fracs_1")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["dy_tt_fracs_1"] = std::shared_ptr<RooFunctor>(
              w_->function("dy_tt_fracs_1")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["w_tt_fracs_2"] = std::shared_ptr<RooFunctor>(
              w_->function("w_tt_fracs_2")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["qcd_tt_fracs_2"] = std::shared_ptr<RooFunctor>(
              w_->function("qcd_tt_fracs_2")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["ttbar_tt_fracs_2"] = std::shared_ptr<RooFunctor>(
              w_->function("ttbar_tt_fracs_2")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["dy_tt_fracs_2"] = std::shared_ptr<RooFunctor>(
              w_->function("dy_tt_fracs_2")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["w_tt_ss_fracs_1"] = std::shared_ptr<RooFunctor>(
              w_->function("w_tt_ss_fracs_1")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["qcd_tt_ss_fracs_1"] = std::shared_ptr<RooFunctor>(
              w_->function("qcd_tt_ss_fracs_1")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["ttbar_tt_ss_fracs_1"] = std::shared_ptr<RooFunctor>(
              w_->function("ttbar_tt_ss_fracs_1")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["dy_tt_ss_fracs_1"] = std::shared_ptr<RooFunctor>(
              w_->function("dy_tt_ss_fracs_1")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["w_tt_ss_fracs_2"] = std::shared_ptr<RooFunctor>(
              w_->function("w_tt_ss_fracs_2")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["qcd_tt_ss_fracs_2"] = std::shared_ptr<RooFunctor>(
              w_->function("qcd_tt_ss_fracs_2")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["ttbar_tt_ss_fracs_2"] = std::shared_ptr<RooFunctor>(
              w_->function("ttbar_tt_ss_fracs_2")->functor(w_->argSet("pt,njets,nbjets")));
        fns_["dy_tt_ss_fracs_2"] = std::shared_ptr<RooFunctor>(
              w_->function("dy_tt_ss_fracs_2")->functor(w_->argSet("pt,njets,nbjets")));
      } else {
        fns_["w_et_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("w_et_fracs")->functor(w_->argSet("m_sv,pt_tt,njets,mjj,sjdphi")));
        fns_["qcd_et_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("qcd_et_fracs")->functor(w_->argSet("m_sv,pt_tt,njets,mjj,sjdphi")));
        fns_["ttbar_et_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("ttbar_et_fracs")->functor(w_->argSet("m_sv,pt_tt,njets,mjj,sjdphi")));

        fns_["w_mt_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("w_mt_fracs")->functor(w_->argSet("m_sv,pt_tt,njets,mjj,sjdphi")));
        fns_["qcd_mt_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("qcd_mt_fracs")->functor(w_->argSet("m_sv,pt_tt,njets,mjj,sjdphi")));
        fns_["ttbar_mt_fracs"] = std::shared_ptr<RooFunctor>(
              w_->function("ttbar_mt_fracs")->functor(w_->argSet("m_sv,pt_tt,njets,mjj,sjdphi")));

        fns_["w_tt_fracs_1"] = std::shared_ptr<RooFunctor>(
              w_->function("w_tt_fracs_1")->functor(w_->argSet("m_sv,pt_tt,njets,mjj,sjdphi")));
        fns_["qcd_tt_fracs_1"] = std::shared_ptr<RooFunctor>(
              w_->function("qcd_tt_fracs_1")->functor(w_->argSet("m_sv,pt_tt,njets,mjj,sjdphi")));
        fns_["ttbar_tt_fracs_1"] = std::shared_ptr<RooFunctor>(
              w_->function("ttbar_tt_fracs_1")->functor(w_->argSet("m_sv,pt_tt,njets,mjj,sjdphi")));
        fns_["dy_tt_fracs_1"] = std::shared_ptr<RooFunctor>(
              w_->function("dy_tt_fracs_1")->functor(w_->argSet("m_sv,pt_tt,njets,mjj,sjdphi")));
        fns_["w_tt_fracs_2"] = std::shared_ptr<RooFunctor>(
              w_->function("w_tt_fracs_2")->functor(w_->argSet("m_sv,pt_tt,njets,mjj,sjdphi")));
        fns_["qcd_tt_fracs_2"] = std::shared_ptr<RooFunctor>(
              w_->function("qcd_tt_fracs_2")->functor(w_->argSet("m_sv,pt_tt,njets,mjj,sjdphi")));
        fns_["ttbar_tt_fracs_2"] = std::shared_ptr<RooFunctor>(
              w_->function("ttbar_tt_fracs_2")->functor(w_->argSet("m_sv,pt_tt,njets,mjj,sjdphi")));
        fns_["dy_tt_fracs_2"] = std::shared_ptr<RooFunctor>(
              w_->function("dy_tt_fracs_2")->functor(w_->argSet("m_sv,pt_tt,njets,mjj,sjdphi")));
      } 
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
    
    std::string btag_label       = "pfCombinedInclusiveSecondaryVertexV2BJetTags";
    std::string btag_label_extra = "";
    double btag_wp (1.0);
    auto filterBTagSumTight = [btag_label, btag_label_extra, btag_wp] (PFJet* s1) -> bool {
      return s1->GetBDiscriminator(btag_label) + s1->GetBDiscriminator(btag_label_extra) > btag_wp;
    };
    if(strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16) btag_wp = 0.8484;
    // if(strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) btag_wp = 0.8838;
    if (era_ == era::data_2017) {
      btag_wp          = 0.4941;
      btag_label       = "pfDeepCSVJetTags:probb";
      btag_label_extra = "pfDeepCSVJetTags:probbb";
    }
    if (era_ == era::data_2018) {
      btag_wp          = 0.4184;
      btag_label       = "pfDeepCSVJetTags:probb";
      btag_label_extra = "pfDeepCSVJetTags:probbb";
    } 
    if (era_ == era::data_2017 || era_ == era::data_2018) {
      if (event->Exists("retag_result")) {
        auto const& retag_result = event->Get<std::map<std::size_t,bool>>("retag_result"); 
        ic::erase_if(bjets, !boost::bind(IsReBTagged, _1, retag_result));
      } else{ 
        ic::erase_if_not(bjets, filterBTagSumTight);
      } 
    } 
    else{
      if (event->Exists("retag_result")) {
        auto const& retag_result = event->Get<std::map<std::size_t,bool>>("retag_result"); 
        ic::erase_if(bjets, !boost::bind(IsReBTagged, _1, retag_result));
      } else{ 
        ic::erase_if(bjets, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label) < btag_wp);
      } 
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

    double m_sv_=-9999;
    if (event->Exists("svfitMass")) {
      m_sv_ = event->Get<double>("svfitMass");
    } else {
      //m_sv_ = -9999;
      m_sv_ = m_vis_*1.4; // not intended for use in a propper analysis but quick fix for cases when sv fit mas has not been calculated yet. 1.4 factor is roughly the ratio of m_sv/m_vis for QCD/W jets events
    }
    double pt_tt_ = (ditau->vector() + met->vector()).pt();   
 
    double iso_1_ = 0;
    if (channel_ == channel::et) {
      Electron const* elec = dynamic_cast<Electron const*>(lep1);
      if(strategy_ == strategy::legacy16){
        EventInfo *eventInfo = event->GetPtr<EventInfo>("eventInfo");
        iso_1_ = PF03EAIsolationVal(elec, eventInfo->jet_rho());
      }
      else
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
    double mjj_ = 0;
    if(n_jets_>1) mjj_ = (jets[0]->vector()+jets[1]->vector()).M();    

    double sjdphi_ = -9999;
    if(n_jets_>1) sjdphi_ =  std::fabs(ROOT::Math::VectorUtil::DeltaPhi(jets[0]->vector(), jets[1]->vector()));
 
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
    } else if(strategy_ == strategy::smsummer16 || strategy_ == strategy::cpsummer16 || strategy_ == strategy::legacy16 || strategy_ == strategy::cpdecays16 || strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) {
      double real_frac=0;
      double real_frac_2=0;
      inputs.resize(9);
      if(true||(strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18)){
        tt_inputs_1.resize(8);
        tt_inputs_2.resize(8); 
      } else {
        tt_inputs_1.resize(9);
        tt_inputs_2.resize(9);
      }
      if(channel_ == channel::et || channel_ == channel::mt){
        std::vector<double> args = {m_sv_,pt_tt_,n_jets_,mjj_,sjdphi_};
        double qcd_frac=0.5, w_frac=0.5, tt_frac=0.0;
        if(channel_ == channel::et){
          qcd_frac = fns_["qcd_et_fracs"]->eval(args.data());  
          w_frac = fns_["w_et_fracs"]->eval(args.data());
          tt_frac = fns_["ttbar_et_fracs"]->eval(args.data());
          real_frac = 1-qcd_frac-w_frac-tt_frac;
        }
        if(channel_ == channel::mt){
          std::vector<double> args = {m_sv_,pt_tt_,n_jets_,mjj_,sjdphi_};
          qcd_frac = fns_["qcd_mt_fracs"]->eval(args.data());  
          w_frac = fns_["w_mt_fracs"]->eval(args.data());
          tt_frac = fns_["ttbar_mt_fracs"]->eval(args.data());
          real_frac = 1-qcd_frac-w_frac-tt_frac;
        }
        // for some bins the fractions can be 0 (if all background is accounted for by real taus) in this case take W fraction as 1
        if(qcd_frac+w_frac+tt_frac<=0) {
          qcd_frac = 0.0;
          w_frac = 1.0;
          tt_frac = 0.0;
        }

        // make sure fractions always sum to 1
        double tot_frac = qcd_frac+w_frac+tt_frac;
        qcd_frac /= tot_frac;
        w_frac /= tot_frac;
        tt_frac /= tot_frac;

        inputs[0] = pt_2_; inputs[1] = tau_decaymode_2_; inputs[2] = n_jets_; inputs[3] = m_vis_; inputs[4] = mt_1_; inputs[5] = iso_1_; inputs[6] = qcd_frac; inputs[7] = w_frac; inputs[8] = tt_frac;
      } else if (channel_ == channel::tt){
        std::vector<double> args_1 = {m_sv_,pt_tt_,n_jets_,mjj_,sjdphi_};
        std::vector<double> args_2 = {m_sv_,pt_tt_,n_jets_,mjj_,sjdphi_};
        double qcd_frac_1=1.0, w_frac_1=0.0, tt_frac_1=0.0, dy_frac_1=0.0, qcd_frac_2=1.0, w_frac_2=0.0, tt_frac_2=0.0, dy_frac_2=0.0;

        if(fracs_file_!="") {
          qcd_frac_1 = fns_["qcd_tt_fracs_1"]->eval(args_1.data());  
          w_frac_1 = fns_["w_tt_fracs_1"]->eval(args_1.data());
          tt_frac_1 = fns_["ttbar_tt_fracs_1"]->eval(args_1.data());
          dy_frac_1 = fns_["dy_tt_fracs_1"]->eval(args_1.data());
          qcd_frac_2 = fns_["qcd_tt_fracs_2"]->eval(args_2.data());  
          w_frac_2 = fns_["w_tt_fracs_2"]->eval(args_2.data());
          tt_frac_2 = fns_["ttbar_tt_fracs_2"]->eval(args_2.data());
          dy_frac_2 = fns_["dy_tt_fracs_2"]->eval(args_2.data());
          real_frac = 1-qcd_frac_1-w_frac_1-tt_frac_1-dy_frac_1;
          real_frac_2 = 1-qcd_frac_2-w_frac_2-tt_frac_2-dy_frac_2;
        }
        // for some bins the fractions can be 0 (if all background is accounted for by real taus) in this case take QCD fraction as 1
        if(qcd_frac_1+w_frac_1+tt_frac_1<=0) {
          qcd_frac_1 = 1.0;
          w_frac_1 = 0.0;
          tt_frac_1 = 0.0;
        }
        if(qcd_frac_2+w_frac_2+tt_frac_2<=0) {
          qcd_frac_2 = 1.0;
          w_frac_2 = 0.0;
          tt_frac_2 = 0.0;
        }

        // make sure fractions always sum to 1
        double tot_frac_1 = qcd_frac_1+w_frac_1+tt_frac_1;
        double tot_frac_2 = qcd_frac_2+w_frac_2+tt_frac_2;
        qcd_frac_1 /= tot_frac_1;
        w_frac_1 /= tot_frac_1;
        tt_frac_1 /= tot_frac_1;
        qcd_frac_2 /= tot_frac_2;
        w_frac_2 /= tot_frac_2;
        tt_frac_2 /= tot_frac_2;

        if(true || (strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18)) {
          tt_inputs_1[0] = pt_1_; tt_inputs_1[1] = pt_2_; tt_inputs_1[2] = tau_decaymode_1_; tt_inputs_1[3] = n_jets_; tt_inputs_1[4] = m_vis_; tt_inputs_1[5] = qcd_frac_1; tt_inputs_1[6] = w_frac_1+dy_frac_1; tt_inputs_1[7] = tt_frac_1; 
          tt_inputs_2[0] = pt_2_; tt_inputs_2[1] = pt_1_; tt_inputs_2[2] = tau_decaymode_2_; tt_inputs_2[3] = n_jets_; tt_inputs_2[4] = m_vis_; tt_inputs_2[5] = qcd_frac_2; tt_inputs_2[6] = w_frac_2+dy_frac_2; tt_inputs_2[7] = tt_frac_2; 
        } else {
          tt_inputs_1[0] = pt_1_; tt_inputs_1[1] = pt_2_; tt_inputs_1[2] = tau_decaymode_1_; tt_inputs_1[3] = n_jets_; tt_inputs_1[4] = m_vis_; tt_inputs_1[5] = qcd_frac_1; tt_inputs_1[6] = w_frac_1; tt_inputs_1[7] = tt_frac_1; tt_inputs_1[8] = dy_frac_1;
          tt_inputs_2[0] = pt_2_; tt_inputs_2[1] = pt_1_; tt_inputs_2[2] = tau_decaymode_2_; tt_inputs_2[3] = n_jets_; tt_inputs_2[4] = m_vis_; tt_inputs_2[5] = qcd_frac_2; tt_inputs_2[6] = w_frac_2; tt_inputs_2[7] = tt_frac_2; tt_inputs_2[8] = dy_frac_2;
        }
      }
      std::string map_key = "inclusive";
      // Retrieve fake factors and add to event as weights
      if(channel_ == channel::et || channel_ == channel::mt){
        double ff_nom = fake_factors_[map_key]->value(inputs);
        event->Add("wt_ff_1",  ff_nom);
        double ff_real_up = ff_nom*(1.-real_frac*1.1)/(1.-real_frac);
        double ff_real_down = ff_nom*(1.-real_frac*0.9)/(1.-real_frac);
        event->Add("wt_ff_realtau_up_1",  ff_real_up);
        event->Add("wt_ff_realtau_down_1",  ff_real_down);

        if(do_systematics_){
          std::vector<std::string> systematics = {"ff_qcd_syst_up","ff_qcd_syst_down","ff_qcd_dm0_njet0_stat_up","ff_qcd_dm0_njet0_stat_down","ff_qcd_dm0_njet1_stat_up","ff_qcd_dm0_njet1_stat_down","ff_qcd_dm1_njet0_stat_up","ff_qcd_dm1_njet0_stat_down","ff_qcd_dm1_njet1_stat_up","ff_qcd_dm1_njet1_stat_down","ff_w_syst_up","ff_w_syst_down","ff_w_dm0_njet0_stat_up","ff_w_dm0_njet0_stat_down","ff_w_dm0_njet1_stat_up","ff_w_dm0_njet1_stat_down","ff_w_dm1_njet0_stat_up","ff_w_dm1_njet0_stat_down","ff_w_dm1_njet1_stat_up","ff_w_dm1_njet1_stat_down","ff_tt_syst_up","ff_tt_syst_down","ff_tt_dm0_njet0_stat_up","ff_tt_dm0_njet0_stat_down","ff_tt_dm0_njet1_stat_up","ff_tt_dm0_njet1_stat_down","ff_tt_dm1_njet0_stat_up","ff_tt_dm1_njet0_stat_down" ,"ff_tt_dm1_njet1_stat_up","ff_tt_dm1_njet1_stat_down"};
          for(unsigned j=0; j<systematics.size(); ++j){
            std::string syst = systematics[j];
            double ff_syst = fake_factors_[map_key]->value(inputs,syst);
            if(std::isinf(ff_syst) || std::isnan(ff_syst)) ff_syst = ff_nom; 
            std::string syst_name = "wt_"+syst+"_1";
            event->Add(syst_name, ff_syst);

          } 
        }
      } else if(channel_ == channel::tt){

        if(strategy_ == strategy::cpdecays18 || strategy_ == strategy::legacy16) { 
          // FF from workspace for cp in decay analysis

          Tau const* tau1 = dynamic_cast<Tau const*>(lep1);
          Tau const* tau2 = dynamic_cast<Tau const*>(lep2);

          std::vector<ic::PFCandidate*> pfcands =  event->GetPtrVec<ic::PFCandidate>("pfCandidates");
          std::vector<ic::Vertex*> & vertex_vec = event->GetPtrVec<ic::Vertex>("vertices");
          std::vector<ic::Vertex*> & refit_vertex_vec = event->GetPtrVec<ic::Vertex>("refittedVertices");
          ic::Vertex* refit_vertex = vertex_vec[0];
          for(auto v : refit_vertex_vec) {
            if(v->id() == tau1->id()+tau2->id()) refit_vertex = v;
          }

          double ipsig = IPAndSignificance(tau1, refit_vertex, pfcands).second;

          double mva_dm_1=tau1->HasTauID("MVADM2017v1") ? tau1->GetTauID("MVADM2017v1") : -1.;
          double os = PairOppSign(ditau);

          auto args = std::vector<double>{pt_1_,mva_dm_1,ipsig,n_jets_,pt_2_,os,met->pt()};
          //std::cout << pt_1_ << "    " << mva_dm_1 << "    " << ipsig << "    " << n_jets_ << "    " << m_vis_ << "    " << os << std::endl;
          double ff_nom = fns_["ff_tt_tight_mvadmbins"]->eval(args.data()); 
          event->Add("wt_ff_1",  ff_nom);
          //std::cout << ff_nom << std::endl;
          //
          //wt_ff_2
          double mva_dm_2=tau2->HasTauID("MVADM2017v1") ? tau2->GetTauID("MVADM2017v1") : -1.;
          double ipsig2 = IPAndSignificance(tau2, refit_vertex, pfcands).second;
          auto args2 = std::vector<double>{pt_2_,mva_dm_2,ipsig2,n_jets_,pt_1_,false,met->pt()}; // we are using this FF only for W and ttbar contributions so we set this to false as we want to take the same-sign value in this case 
          double ff_nom_2 = fns_["ff_tt_tight_mvadmbins"]->eval(args2.data());
          event->Add("wt_ff_2",  ff_nom_2);

          if(do_systematics_){
            std::vector<std::string> systematics = {"_qcd_syst_up","_qcd_syst_down","_wjets_syst_up","_wjets_syst_down","_ttbar_syst_up","_ttbar_syst_down","_qcd_stat_njet0_mvadm0_sig_lt3_up","_qcd_stat_njet0_mvadm0_sig_lt3_down","_qcd_stat_njet0_mvadm0_sig_gt3_up","_qcd_stat_njet0_mvadm0_sig_gt3_down","_qcd_stat_njet0_mvadm1_up","_qcd_stat_njet0_mvadm1_down","_qcd_stat_njet0_mvadm2_up","_qcd_stat_njet0_mvadm2_down","_qcd_stat_njet0_mvadm10_up","_qcd_stat_njet0_mvadm10_down","_qcd_stat_njet0_mvadm11_up","_qcd_stat_njet0_mvadm11_down","_qcd_stat_njet1_mvadm0_sig_lt3_up","_qcd_stat_njet1_mvadm0_sig_lt3_down","_qcd_stat_njet1_mvadm0_sig_gt3_up","_qcd_stat_njet1_mvadm0_sig_gt3_down","_qcd_stat_njet1_mvadm1_up","_qcd_stat_njet1_mvadm1_down","_qcd_stat_njet1_mvadm2_up","_qcd_stat_njet1_mvadm2_down","_qcd_stat_njet1_mvadm10_up","_qcd_stat_njet1_mvadm10_down","_qcd_stat_njet1_mvadm11_up","_qcd_stat_njet1_mvadm11_down","_qcd_stat_njet2_mvadm0_sig_lt3_up","_qcd_stat_njet2_mvadm0_sig_lt3_down","_qcd_stat_njet2_mvadm0_sig_gt3_up","_qcd_stat_njet2_mvadm0_sig_gt3_down","_qcd_stat_njet2_mvadm1_up","_qcd_stat_njet2_mvadm1_down","_qcd_stat_njet2_mvadm2_up","_qcd_stat_njet2_mvadm2_down","_qcd_stat_njet2_mvadm10_up","_qcd_stat_njet2_mvadm10_down","_qcd_stat_njet2_mvadm11_up","_qcd_stat_njet2_mvadm11_down","_qcd_stat_pt2_up","_qcd_stat_pt2_down","_qcd_stat_pt2_up","_qcd_stat_pt2_down"};

            for(auto s : systematics){
              double ff_syst = fns_["ff_tt_tight_mvadmbins"+s]->eval(args.data()); 
              std::string syst_name = "wt_ff"+s;
              //std::cout << syst_name << "    " << ff_syst << "    " << ff_nom << std::endl;
              event->Add(syst_name+"_1", ff_syst);
            }
          }
          return 0;
        }
        double ff_nom_1 = fake_factors_[map_key]->value(tt_inputs_1)*0.5;
        double ff_nom_2 = fake_factors_[map_key]->value(tt_inputs_2)*0.5;
        event->Add("wt_ff_1",  ff_nom_1);
        event->Add("wt_ff_2",  ff_nom_2);
        double ff_real_up_1 = ff_nom_1*(1.-real_frac*1.1)/(1.-real_frac);
        double ff_real_down_1 = ff_nom_1*(1.-real_frac*0.9)/(1.-real_frac);
        double ff_real_up_2 = ff_nom_2*(1.-real_frac_2*1.1)/(1.-real_frac_2);
        double ff_real_down_2 = ff_nom_2*(1.-real_frac_2*0.9)/(1.-real_frac_2);
        event->Add("wt_ff_realtau_up_1",  ff_real_up_1);
        event->Add("wt_ff_realtau_down_1",  ff_real_down_1);
        event->Add("wt_ff_realtau_up_2",  ff_real_up_2);
        event->Add("wt_ff_realtau_down_2",  ff_real_down_2);
        
        if(do_systematics_){
          std::vector<std::string> systematics = {"ff_qcd_syst_up","ff_qcd_syst_down","ff_qcd_dm0_njet0_stat_up","ff_qcd_dm0_njet0_stat_down","ff_qcd_dm0_njet1_stat_up","ff_qcd_dm0_njet1_stat_down","ff_qcd_dm1_njet0_stat_up","ff_qcd_dm1_njet0_stat_down","ff_qcd_dm1_njet1_stat_up","ff_qcd_dm1_njet1_stat_down","ff_w_syst_up","ff_w_syst_down","ff_tt_syst_up","ff_tt_syst_down","ff_w_frac_syst_up", "ff_w_frac_syst_down", "ff_tt_frac_syst_up", "ff_tt_frac_syst_down"};
         
          if(strategy_ == strategy::cpsummer17 || strategy_ == strategy::cpdecays17 || strategy_ == strategy::cpdecays18) {
            systematics = {"ff_qcd_syst_up","ff_qcd_syst_down","ff_qcd_dm0_njet0_stat_up","ff_qcd_dm0_njet0_stat_down","ff_qcd_dm0_njet1_stat_up","ff_qcd_dm0_njet1_stat_down","ff_qcd_dm1_njet0_stat_up","ff_qcd_dm1_njet0_stat_down","ff_qcd_dm1_njet1_stat_up","ff_qcd_dm1_njet1_stat_down","ff_w_syst_up","ff_w_syst_down","ff_tt_syst_up","ff_tt_syst_down","ff_w_frac_syst_up", "ff_w_frac_syst_down", "ff_tt_frac_syst_up", "ff_tt_frac_syst_down"};
          }
 
          for(unsigned j=0; j<systematics.size(); ++j){
            std::string syst = systematics[j];
            double ff_syst_1 = fake_factors_[map_key]->value(tt_inputs_1,syst)*0.5;
            double ff_syst_2 = fake_factors_[map_key]->value(tt_inputs_2,syst)*0.5;

            if(std::isinf(ff_syst_1) || std::isnan(ff_syst_1)) ff_syst_1 = ff_nom_1;
            if(std::isinf(ff_syst_2) || std::isnan(ff_syst_2)) ff_syst_2 = ff_nom_2;

            std::string syst_name = "wt_"+syst;
            event->Add(syst_name+"_1", ff_syst_1);
            event->Add(syst_name+"_2", ff_syst_2);

            if(syst_name.find("_frac_syst_") != std::string::npos) {
              double w_frac_1 = tt_inputs_1[6], tt_frac_1 = tt_inputs_1[7], qcd_frac_1 = tt_inputs_1[5];
              double w_frac_2 = tt_inputs_2[6], tt_frac_2 = tt_inputs_2[7], qcd_frac_2 = tt_inputs_2[5];
              double w_frac_1_up = w_frac_1, tt_frac_1_up = tt_frac_1, qcd_frac_1_up = qcd_frac_1, w_frac_2_up = w_frac_2, tt_frac_2_up = tt_frac_2, qcd_frac_2_up = qcd_frac_2;
 
              if(syst=="ff_w_frac_syst_up") {
                w_frac_1_up =  (w_frac_1)*1.2;
                qcd_frac_1_up+=w_frac_1-w_frac_1_up;

                w_frac_2_up =  (w_frac_2)*1.2;
                qcd_frac_2_up+=w_frac_2-w_frac_2_up;
              } else if(syst=="ff_w_frac_syst_down") {
                w_frac_1_up =  (w_frac_1)*0.8;
                qcd_frac_1_up+=w_frac_1-w_frac_1_up;

                w_frac_2_up =  (w_frac_2)*0.8;
                qcd_frac_2_up+=w_frac_2-w_frac_2_up;
              } else if(syst=="ff_tt_frac_syst_up") {
                tt_frac_1_up =  (tt_frac_1)*1.2;
                qcd_frac_1_up+=tt_frac_1-tt_frac_1_up;

                tt_frac_2_up =  (tt_frac_2)*1.2;
                qcd_frac_2_up+=tt_frac_2-tt_frac_2_up;

              } else if(syst=="ff_tt_frac_syst_down") {
                tt_frac_1_up =  (tt_frac_1)*0.8;
                qcd_frac_1_up+=tt_frac_1-tt_frac_1_up;

                tt_frac_2_up =  (tt_frac_2)*0.8;
                qcd_frac_2_up+=tt_frac_2-tt_frac_2_up;
              } 
              auto tt_inputs_1_shift = tt_inputs_1;
              auto tt_inputs_2_shift = tt_inputs_2;
              tt_inputs_1_shift[5]=qcd_frac_1_up; tt_inputs_1_shift[6] = w_frac_1_up; tt_inputs_1_shift[7] = tt_frac_1_up;
              tt_inputs_2_shift[5]=qcd_frac_2_up; tt_inputs_2_shift[6] = w_frac_2_up; tt_inputs_2_shift[7] = tt_frac_2_up;
              double ff_shift_1 = fake_factors_[map_key]->value(tt_inputs_1_shift)*0.5;
              double ff_shift_2 = fake_factors_[map_key]->value(tt_inputs_2_shift)*0.5;
              event->Add(syst_name+"_alt_1", ff_shift_1);
              event->Add(syst_name+"_alt_2", ff_shift_2);
            }
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
