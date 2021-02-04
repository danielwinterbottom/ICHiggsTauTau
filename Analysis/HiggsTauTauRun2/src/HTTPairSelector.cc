#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTPairSelector.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include <boost/functional/hash.hpp>
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"

namespace ic {

  HTTPairSelector::HTTPairSelector(std::string const& name) : ModuleBase(name), channel_(channel::et),strategy_(strategy::paper2013),mc_(mc::summer12_53X) {
    pair_label_ = "emtauCandidates";
    mva_met_from_vector_ = true;
    faked_tau_selector_ = 0;
    hadronic_tau_selector_ = 0;
    ztt_mode_ = 0;
    use_status_flags_ = true;
    met_label_ = "pfMVAMet";
    fs_ = NULL;
    hists_.resize(1);
    use_most_isolated_ = false;
    use_os_preference_ = true;
    scale_met_for_tau_ = 0;
    tau_scale_ = 1.0;
    allowed_tau_modes_ = "";
    gen_taus_label_ = "genParticlesTaus";
    metcl_mode_ = 0;
    metuncl_mode_ = 0;
    shift_jes_ = false;
  }

  HTTPairSelector::~HTTPairSelector() {
    ;
  }

  int HTTPairSelector::PreAnalysis() {
    std::string param_fmt = "%-25s %-40s\n";

    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HTTPairSelector" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << boost::format(param_fmt) % "pair_label"            % pair_label_;
    std::cout << boost::format(param_fmt) % "met_label"             % met_label_;
    std::cout << boost::format(param_fmt) % "use_most_isolated"     % use_most_isolated_;
    std::cout << boost::format(param_fmt) % "scale_met_for_tau"     % scale_met_for_tau_;
    std::cout << boost::format(param_fmt) % "tau_scale"             % tau_scale_;
    std::string allowed_str = "";
    if (allowed_tau_modes_ == "") {
      allowed_str = "all modes";
    } else {
      std::vector<std::string> tau_mode_vec;
      boost::split(tau_mode_vec, allowed_tau_modes_, boost::is_any_of(","));
      for (unsigned i = 0; i < tau_mode_vec.size(); ++i) {
        int tau_mode = boost::lexical_cast<int>(tau_mode_vec[i]);
        tau_mode_set_.insert(tau_mode);
        allowed_str += (tau_mode_vec[i] + " ");
      } 
    }
    std::cout << boost::format(param_fmt) % "allowed_tau_modes"     % allowed_str;
    std::cout << boost::format(param_fmt) % "faked_tau_selector"    % faked_tau_selector_;
    std::cout << boost::format(param_fmt) % "ztt_mode"              % ztt_mode_;
    std::cout << boost::format(param_fmt) % "use_status_flags"      % use_status_flags_;
    std::cout << boost::format(param_fmt) % "hadronic_tau_selector" % hadronic_tau_selector_;
    std::cout << boost::format(param_fmt) % "gen_taus_label"        % gen_taus_label_;

    if (fs_) {
      hists_[0] = new Dynamic2DHistoSet(fs_->mkdir("httpairselector"));
      for (unsigned i = 0; i < hists_.size(); ++i) {
        hists_[i]->Create("n_pairs", 4, -0.5, 3.5, 4, -0.5, 3.5);
        hists_[i]->Create("pt_gen_reco", 50, 0, 100, 50, 0, 100);
        hists_[i]->Create("lepton_source",6,0.5,6.5,6,0.5,6.5);
      }
    }

    tau_idiso_name_ = "byDeepTau2017v2p1VSjetraw";

    return 0;
  }

  int HTTPairSelector::Execute(TreeEvent *event) {

    // ************************************************************************
    // Do the actual pair selection, either by highest pT or most isolated tau
    // ************************************************************************
    std::vector<CompositeCandidate *> & dilepton = event->GetPtrVec<CompositeCandidate>(pair_label_);
    std::vector<CompositeCandidate *> nosign_dilepton;
    std::vector<CompositeCandidate *> os_dilepton;
    std::vector<CompositeCandidate *> ss_dilepton;
    std::vector<CompositeCandidate *> result;


    for (unsigned i = 0; i < dilepton.size(); ++i) {
      if (PairOppSign(dilepton[i])) os_dilepton.push_back(dilepton[i]);
      if (PairSameSign(dilepton[i])) ss_dilepton.push_back(dilepton[i]);
      nosign_dilepton.push_back(dilepton[i]);
    }

    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    if (fs_) {
      double wt = eventInfo->total_weight();
      hists_[0]->Fill("n_pairs", os_dilepton.size(), ss_dilepton.size(), wt);
    }

    if(use_os_preference_) {
      // The first pair should have the highest "scalar sum pt" 
      std::sort(os_dilepton.begin(),os_dilepton.end(), SortBySumPt);
      std::sort(ss_dilepton.begin(),ss_dilepton.end(), SortBySumPt);
      
      // Or alternatively sort by isolation
      if (use_most_isolated_) {
        if(channel_ ==  channel::et) { 
           std::sort(os_dilepton.begin(), os_dilepton.end(), boost::bind(SortByIsoET,_1,_2,strategy_,eventInfo,tau_idiso_name_)) ;
           std::sort(ss_dilepton.begin(), ss_dilepton.end(), boost::bind(SortByIsoET,_1,_2,strategy_,eventInfo,tau_idiso_name_)) ;
        }
        if(channel_ ==  channel::mt || channel_ == channel::tpmt) { 
           std::sort(os_dilepton.begin(), os_dilepton.end(), boost::bind(SortByIsoMT,_1,_2,strategy_,tau_idiso_name_)) ;
           std::sort(ss_dilepton.begin(), ss_dilepton.end(), boost::bind(SortByIsoMT,_1,_2,strategy_,tau_idiso_name_)) ;
        }
        if(channel_ ==  channel::em) { 
           std::sort(os_dilepton.begin(), os_dilepton.end(), boost::bind(SortByIsoEM,_1,_2,strategy_,eventInfo)) ;
           std::sort(ss_dilepton.begin(), ss_dilepton.end(), boost::bind(SortByIsoEM,_1,_2,strategy_,eventInfo)) ;
        }
        if(channel_ ==  channel::tt) { 
           std::sort(os_dilepton.begin(), os_dilepton.end(), boost::bind(SortByIsoTT,_1,_2,tau_idiso_name_)) ;
           std::sort(ss_dilepton.begin(), ss_dilepton.end(), boost::bind(SortByIsoTT,_1,_2,tau_idiso_name_)) ;
        }
      }
      if (os_dilepton.size() > 0) { // Take OS in preference to SS
        if(!(channel_ ==  channel::tt)) { 
            result.push_back(os_dilepton[0]);
        } else{
            CompositeCandidate *pt_sorted_pair = new CompositeCandidate();
            Candidate * lep1 = os_dilepton[0]->GetCandidate("lepton1"); 
            Candidate * lep2 = os_dilepton[0]->GetCandidate("lepton2");
            if(lep2->pt()>=lep1->pt()) {
                pt_sorted_pair->AddCandidate("lepton1",lep2); 
                pt_sorted_pair->AddCandidate("lepton2",lep1); 
            } else {
                pt_sorted_pair->AddCandidate("lepton1",lep1); 
                pt_sorted_pair->AddCandidate("lepton2",lep2); 
            }
            result.push_back(pt_sorted_pair);
        }
      } else if (ss_dilepton.size() > 0) {
        if(!(channel_ ==  channel::tt)) { 
            result.push_back(ss_dilepton[0]);
        } else {
            CompositeCandidate *pt_sorted_pair = new CompositeCandidate();
            Candidate * lep1 = os_dilepton[0]->GetCandidate("lepton1"); 
            Candidate * lep2 = os_dilepton[0]->GetCandidate("lepton2");
            if(lep2->pt()>=lep1->pt()) {
                pt_sorted_pair->AddCandidate("lepton1",lep2); 
                pt_sorted_pair->AddCandidate("lepton2",lep1); 
            } else {
                pt_sorted_pair->AddCandidate("lepton1",lep1); 
                pt_sorted_pair->AddCandidate("lepton2",lep2); 
            }
            result.push_back(pt_sorted_pair);
        }
      }
    } else {
      // The first pair should have the highest "scalar sum pt" 
      std::sort(nosign_dilepton.begin(),nosign_dilepton.end(), SortBySumPt);
      
      // Or alternatively sort by isolation
      if (use_most_isolated_ ) {
        if(channel_ ==  channel::et) { 
           std::sort(nosign_dilepton.begin(), nosign_dilepton.end(), boost::bind(SortByIsoET,_1,_2,strategy_,eventInfo,tau_idiso_name_)); 
        }
        if(channel_ ==  channel::mt || channel_ == channel::tpmt) { 
           std::sort(nosign_dilepton.begin(), nosign_dilepton.end(), boost::bind(SortByIsoMT,_1,_2,strategy_,tau_idiso_name_)) ;
        }
        if(channel_ ==  channel::em) { 
           std::sort(nosign_dilepton.begin(), nosign_dilepton.end(), boost::bind(SortByIsoEM,_1,_2,strategy_,eventInfo)) ;
        }
        if(channel_ ==  channel::tt) { 
           std::sort(nosign_dilepton.begin(), nosign_dilepton.end(), boost::bind(SortByIsoTT,_1,_2,tau_idiso_name_)) ;
        }
      }
      if (nosign_dilepton.size() > 0) { // No preference for OS over SS
        if(!(channel_ ==  channel::tt)) { 
            result.push_back(nosign_dilepton[0]);
        } else {
            CompositeCandidate *pt_sorted_pair = new CompositeCandidate();
            Candidate * lep1 = nosign_dilepton[0]->GetCandidate("lepton1"); 
            Candidate * lep2 = nosign_dilepton[0]->GetCandidate("lepton2");
            if(lep2->pt()>=lep1->pt()) {
                pt_sorted_pair->AddCandidate("lepton1",lep2); 
                pt_sorted_pair->AddCandidate("lepton2",lep1); 
            } else {
                pt_sorted_pair->AddCandidate("lepton1",lep1); 
                pt_sorted_pair->AddCandidate("lepton2",lep2); 
            }
            result.push_back(pt_sorted_pair);
        }
      }
    }
    if (result.size() == 0) return 1;  //Require at least one dilepton
    //Rest of code is not needed for Z->ee and Z->mumu channels    
    if(channel_ == channel::zee || channel_ == channel::zmm || channel_ == channel::tpzmm || channel_ == channel::tpzee) {
      dilepton = result;
 //     return 0;
    }

    // ************************************************************************
    // If using pair-wise mva met select the appropriate met
    // ************************************************************************
    if (mva_met_from_vector_) {
      std::map<std::size_t, Met *> const& met_map = event->GetIDMap<Met>("pfMVAMetVector");
      std::size_t id = 0;
      boost::hash_combine(id, result[0]->GetCandidate("lepton1")->id());
      boost::hash_combine(id, result[0]->GetCandidate("lepton2")->id());
      std::size_t id_inv = 0;
      boost::hash_combine(id_inv, result[0]->GetCandidate("lepton2")->id());
      boost::hash_combine(id_inv, result[0]->GetCandidate("lepton1")->id());
      std::map<std::size_t, Met *>::const_iterator it = met_map.find(id);
      std::map<std::size_t, Met *>::const_iterator it_inv = met_map.find(id_inv);
      if (it != met_map.end()) {
        Met * mva_met = it->second;
        event->Add("pfMVAMet", mva_met);
      } else if (it_inv != met_map.end()){
        //lepton1 <--> lepton2 (needed for fully hadronic)
        Met * mva_met = it_inv->second;
        event->Add("pfMVAMet", mva_met);
      } else {
        std::cerr << "Could not find Met in collection for ID: " << id << " or " << id_inv <<std::endl;
        exit(0);
      }
    }
   
    std::vector<Met*> pfMet_vec = event->GetPtrVec<Met>("pfMetFromSlimmed");
    if((mc_ == mc::mcleg2016 || mc_ == mc::mc2018 || mc_ == mc::mc2017) && !usePFMET_) pfMet_vec = event->GetPtrVec<Met>("puppiMet"); // for legacy 2016 and 2018 use the puppi MET (but still call this "pfMET" to avoid having to change other sections of the code)
    Met *pfmet = pfMet_vec.at(0);
    // shift MET for systematic shifts
    if(metuncl_mode_!=0 && metcl_mode_!=0) std::cout<< "HTTPairSelector:: Trying to perform more that 1 MET shift at once, MET will not be shifted!" << std::endl;
    else if(metuncl_mode_!=0 || metcl_mode_!=0){
      std::string shift_type = "NoShift";  
      if(metcl_mode_==1) shift_type = "JetEnDown";
      if(metcl_mode_==2) shift_type = "JetEnUp";
      if(metuncl_mode_==1) shift_type = "UnclusteredEnDown";
      if(metuncl_mode_==2) shift_type = "UnclusteredEnUp";
      double pt = pfmet->GetShiftedMet(shift_type).pt();
      double phi = pfmet->GetShiftedMet(shift_type).phi();
      pfmet->set_vector(ROOT::Math::PtEtaPhiEVector(pt,0,phi,pt));
    }
    event->Add("pfMET", pfmet);
   
   // ************************************************************************
   // Scale met for the tau energy scale shift
   // ************************************************************************
    typedef std::map<std::size_t, ROOT::Math::PxPyPzEVector> map_id_vec;
    if (/*scale_met_for_tau_ &&*/ channel_ != channel::em && channel_ != channel::tt && channel_ != channel::zmm && channel_ != channel::zee) {
      Met * met = event->GetPtr<Met>(met_label_);
      event->Add("met_noscale", met->pt());
      Tau const* tau = dynamic_cast<Tau const*>(result[0]->GetCandidate("lepton2"));
      //double t_scale = tau_scale_;
      std::vector<std::string> tau_shifts {"scales_taues","scales_taues_1prong0pi0",
       "scales_taues_1prong1pi0","scales_taues_3prong0pi0","scales_taues_3prong1pi0","scales_efaketaues_1prong0pi0","scales_efaketaues_1prong1pi0", "scales_mufaketaues_1prong0pi0", "scales_mufaketaues_1prong1pi0"};
      for(unsigned int i = 0; i < tau_shifts.size(); ++i) {
        if(event->Exists(tau_shifts.at(i))){
          auto const& es_shifts = event->Get<map_id_vec>(tau_shifts.at(i));
          if(es_shifts.count(tau->id()) > 0){
             this->CorrectMETForShift(met, es_shifts.at(tau->id()));
          }
        }
      }
      if (channel_ == channel::et && event->Exists("elec_scales")) {
        double t_scale_ = 1.0;  
        Electron const* elec = dynamic_cast<Electron const*>(result[0]->GetCandidate("lepton1"));
        std::map<std::size_t, double> const& elec_scales = event->Get< std::map<std::size_t, double>  > ("elec_scales");
        std::map<std::size_t, double>::const_iterator it = elec_scales.find(elec->id());
        if (it != elec_scales.end()) {
          t_scale_ = it->second;
        } else {
          std::cout << "Scale for chosen electron not found!" << std::endl;
          throw;
        }
        double metx = met->vector().px();
        double mety = met->vector().py();
        double metet = met->vector().energy();
        double dx = elec->vector().px() * (( 1. / t_scale_) - 1.);
        double dy = elec->vector().py() * (( 1. / t_scale_) - 1.);
        metx = metx + dx;
        mety = mety + dy;
        metet = sqrt(metx*metx + mety*mety);
        ROOT::Math::PxPyPzEVector new_met(metx, mety, 0, metet);
        met->set_vector(ROOT::Math::PtEtaPhiEVector(new_met));
      }
      if ((channel_ == channel::mt || channel_ == channel::tpmt) && event->Exists("muon_scales")) {
        Muon const* muon = dynamic_cast<Muon const*>(result[0]->GetCandidate("lepton1"));
        auto const& es_shifts = event->Get<map_id_vec>("muon_scales");
        if(es_shifts.count(muon->id()) > 0){
          this->CorrectMETForShift(met, es_shifts.at(muon->id()));
        }
      }
    }

   // ************************************************************************
   // Scale met for the tau energy scale shift in fully hadronic channel
   // ************************************************************************
    if (/*scale_met_for_tau_ && */channel_ == channel::tt) {
      Met * met = event->GetPtr<Met>(met_label_);
      event->Add("met_noscale", met->pt());
      Tau const* tau1 = dynamic_cast<Tau const*>(result[0]->GetCandidate("lepton1"));
      Tau const* tau2 = dynamic_cast<Tau const*>(result[0]->GetCandidate("lepton2"));
      std::vector<std::string> tau_shifts {"scales_taues","scales_taues_1prong0pi0",
       "scales_taues_1prong1pi0","scales_taues_3prong0pi0","scales_efaketaues_1prong0pi0","scales_efaketaues_1prong1pi0","scales_efaketaues_1prong0pi0_endcap","scales_efaketaues_1prong1pi0_endcap", "scales_mufaketaues_1prong0pi0", "scales_mufaketaues_1prong1pi0"};
      for(unsigned int i = 0; i < tau_shifts.size(); ++i) {
        if(event->Exists(tau_shifts.at(i))){
          auto const& es_shifts = event->Get<map_id_vec>(tau_shifts.at(i));
          if(es_shifts.count(tau1->id()) > 0){
             this->CorrectMETForShift(met, es_shifts.at(tau1->id()));
          }
          if(es_shifts.count(tau2->id()) > 0){
             this->CorrectMETForShift(met, es_shifts.at(tau2->id()));
          }
        }
      }
    }
    // ************************************************************************
    // Scale met for the electron energy scale shift
    // ************************************************************************
    if (scale_met_for_tau_ && channel_ == channel::em) {
      //Still to be moved to the better method used above
      Met * met = event->GetPtr<Met>(met_label_);
      event->Add("met_noscale", met->pt());
      double t_scale_ = tau_scale_;
      Electron const* elec = dynamic_cast<Electron const*>(result[0]->GetCandidate("lepton1"));
      if (event->Exists("elec_scales")) {
        std::map<std::size_t, double> const& elec_scales = event->Get< std::map<std::size_t, double>  > ("elec_scales");
        std::map<std::size_t, double>::const_iterator it = elec_scales.find(elec->id());
        if (it != elec_scales.end()) {
          t_scale_ = it->second;
        } else {
          std::cout << "Scale for chosen electron not found!" << std::endl;
          throw;
        }
      }
      double metx = met->vector().px();
      double mety = met->vector().py();
      double metet = met->vector().energy();
      double dx = elec->vector().px() * (( 1. / t_scale_) - 1.);
      double dy = elec->vector().py() * (( 1. / t_scale_) - 1.);
      metx = metx + dx;
      mety = mety + dy;
      metet = sqrt(metx*metx + mety*mety);
      ROOT::Math::PxPyPzEVector new_met(metx, mety, 0, metet);
      met->set_vector(ROOT::Math::PtEtaPhiEVector(new_met));
      if (event->Exists("muon_scales")) {
        Muon const* muon = dynamic_cast<Muon const*>(result[0]->GetCandidate("lepton2"));
        auto const& es_shifts = event->Get<map_id_vec>("muon_scales");
        if(es_shifts.count(muon->id()) > 0){
          this->CorrectMETForShift(met, es_shifts.at(muon->id()));
        }
      }
    }
    
    // ************************************************************************
    // Scale met for the jet energy scale shift in fully hadronic channel
    // ************************************************************************

    if(event->Exists("jes_shift") && shift_jes_){
      Met * met = event->GetPtr<Met>(met_label_);
      ROOT::Math::PxPyPzEVector jes_shift = event->Get<ROOT::Math::PxPyPzEVector>("jes_shift");
      this->CorrectMETForShift(met, jes_shift);
    }

    // ************************************************************************
    // Scale met for the jet energy resolution
    // ************************************************************************

    if(event->Exists("jer_shift") && shift_jes_){
      Met * met = event->GetPtr<Met>(met_label_);
      ROOT::Math::PxPyPzEVector jer_shift = event->Get<ROOT::Math::PxPyPzEVector>("jer_shift");
      this->CorrectMETForShift(met, jer_shift);
    }

    // ************************************************************************
    // Select l->tau or jet->tau fakes
    // ************************************************************************
    // mode 0 = e-tau, mode 1 = mu-tau, mode 2 = e-mu
    // faked_tau_selector = 1 -> ZL, = 2 -> ZJ
    // This code only to be run on Z->ee or Z->mumu events (remove Z->tautau first!)
    if(strategy_ != strategy::spring15 && strategy_ != strategy::fall15 && strategy_ != strategy::mssmspring16 && strategy_ != strategy::smspring16 && strategy_!=strategy::mssmsummer16 && strategy_ != strategy::smsummer16 && strategy_ != strategy::cpsummer16 &&  strategy_ != strategy::legacy16 && strategy_ != strategy::cpdecays16 && strategy_ != strategy::cpsummer17 && strategy_ != strategy::cpdecays17 && strategy_ != strategy::cpdecays18) {
      if (faked_tau_selector_ > 0  && channel_ != channel::em && channel_ != channel::zmm && channel_ != channel::zee ) {
        std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>("genParticles");
        std::vector<GenParticle *> sel_particles;
        if (channel_ == channel::et || channel_ == channel::etmet) {
          // Add all status 3 electrons with pT > 8 to sel_particles
          for (unsigned i = 0; i < particles.size(); ++i) {
            if ( (abs(particles[i]->pdgid()) == 11 || abs(particles[i]->pdgid()) == 13) && particles[i]->pt() > 8.) sel_particles.push_back(particles[i]);
          }
        } else if (channel_ == channel::mt || channel_ == channel::mtmet || channel_ == channel::tpmt) {
          // Add all status 3 muons with pT > 8 to sel_particles
         for (unsigned i = 0; i < particles.size(); ++i) {
           if ( (abs(particles[i]->pdgid()) == 11 || abs(particles[i]->pdgid()) == 13) && particles[i]->pt() > 8.) sel_particles.push_back(particles[i]);
         } 
        }
        // Get the reco tau from the pair
        std::vector<Candidate *> tau;
        if(channel_ == channel::tt){
          tau.push_back(result[0]->GetCandidate("lepton1"));
        }
        tau.push_back(result[0]->GetCandidate("lepton2"));
        // Get the matches vector - require match within DR = 0.5, and pick the closest gen particle to the tau
        std::vector<std::pair<Candidate*, GenParticle*> > matches = MatchByDR(tau, sel_particles, 0.5, true, true);
        // If we want ZL and there's no match, fail the event
        if (faked_tau_selector_ == 1 && matches.size() == 0) return 1;
        // If we want ZJ and there is a match, fail the event
        if (faked_tau_selector_ == 2 && matches.size() > 0) return 1;
      }
      if (hadronic_tau_selector_ > 0 && channel_ != channel::em) {
        std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>(gen_taus_label_);
        std::vector<GenJet> gen_taus = BuildTauJets(particles, false,false);
        std::vector<GenJet *> gen_taus_ptr;
        for (auto & x : gen_taus) gen_taus_ptr.push_back(&x);
        ic::erase_if(gen_taus_ptr, !boost::bind(MinPtMaxEta, _1, 18.0, 999.));
        std::vector<Candidate *> tau;
        tau.push_back(result[0]->GetCandidate("lepton2"));
        // Get the matches vector - require match within DR = 0.5, and pick the closest gen particle to the tau
        std::vector<std::pair<Candidate*, GenJet*> > matches = MatchByDR(tau, gen_taus_ptr, 0.5, true, true);
        // If we want ZL and there's no match, fail the event
        if (hadronic_tau_selector_ == 1 && matches.size() == 0) return 1;
        if (fs_ && matches.size() == 1) {
          hists_[0]->Fill("pt_gen_reco", matches[0].second->pt(), matches[0].first->pt(), 1);
        }
        // If we want ZJ and there is a match, fail the event
        if (hadronic_tau_selector_ == 2 && matches.size() > 0) return 1;
      }
    }

     
    // ************************************************************************
    // Restrict decay modes
    // ************************************************************************
    Tau const* tau_ptr = dynamic_cast<Tau const*>(result[0]->GetCandidate("lepton2"));
    if (tau_ptr && allowed_tau_modes_ != "") {
      if (tau_mode_set_.find(tau_ptr->decay_mode()) == tau_mode_set_.end()) return 1;
    }

    dilepton = result;
    return 0;
  }
  int HTTPairSelector::PostAnalysis() {
    return 0;
  }

  void HTTPairSelector::PrintInfo() {
    ;
  }

  // Sorting
  // ----------------------------------------------------------------
  bool SortBySumPt(CompositeCandidate const* c1,
                                       CompositeCandidate const* c2) {
  //  if (c1->charge() == 0 && c2->charge() != 0) return true;
   // if (c1->charge() != 0 && c2->charge() == 0) return false;
    return ScalarPtSum(c1->AsVector()) > ScalarPtSum(c2->AsVector());
  }

  bool SortByIsoET(CompositeCandidate const* c1, CompositeCandidate const* c2, ic::strategy strategy, EventInfo const* eventInfo, std::string tau_idiso_name) {
    // First we sort the electrons
    Electron const* e1 = static_cast<Electron const*>(c1->At(0));
    Electron const* e2 = static_cast<Electron const*>(c2->At(0));
    double e_iso1;
    double e_iso2;
    if(strategy==strategy::cpsummer17 || strategy==strategy::cpdecays17 || strategy==strategy::cpdecays18 || strategy==strategy::legacy16){ 
      e_iso1 = PF03EAIsolationVal(e1, eventInfo->jet_rho()); //lepton_rho
      e_iso2 = PF03EAIsolationVal(e2, eventInfo->jet_rho());    
    }
    else {    
      e_iso1 = PF03IsolationVal(e1, 0.5, 0);
      e_iso2 = PF03IsolationVal(e2, 0.5, 0);
    }
    // If the iso is different we just use this
    if (e_iso1 != e_iso2) return e_iso1 < e_iso2;
    // If not try the pT
    if (e1->pt() != e2->pt()) return e1->pt() > e2->pt();
    // If both of these are the same then try the taus
    Tau const* t1 = static_cast<Tau const*>(c1->At(1));
    Tau const* t2 = static_cast<Tau const*>(c2->At(1));
    /*double t_iso1 = t1->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    double t_iso2 = t2->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");*/
    double t_iso1 = t1->GetTauID(tau_idiso_name);
    double t_iso2 = t2->GetTauID(tau_idiso_name);
    //if (t_iso1 != t_iso2) return t_iso1 < t_iso2;
    if (t_iso1 != t_iso2) return t_iso1 > t_iso2;
    return (t1->pt() > t2->pt());
  }

  bool SortByIsoMT(CompositeCandidate const* c1, CompositeCandidate const* c2, ic::strategy strategy, std::string tau_idiso_name) {
    // First we sort the electrons
    Muon const* m1 = static_cast<Muon const*>(c1->At(0));
    Muon const* m2 = static_cast<Muon const*>(c2->At(0));
    double m_iso1;
    m_iso1 = (strategy == strategy::fall15) ? PF03IsolationVal(m1, 0.5, 0) : PF04IsolationVal(m1, 0.5, 0);
    double m_iso2; 
    m_iso2 = (strategy == strategy::fall15) ? PF03IsolationVal(m2, 0.5, 0) : PF04IsolationVal(m2, 0.5, 0);
    // If the iso is different we just use this
    if (m_iso1 != m_iso2) return m_iso1 < m_iso2;
    // If not try the pT
    if (m1->pt() != m2->pt()) return m1->pt() > m2->pt();
    // If both of these are the same then try the taus
    Tau const* t1 = static_cast<Tau const*>(c1->At(1));
    Tau const* t2 = static_cast<Tau const*>(c2->At(1));
    /*double t_iso1 = t1->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    double t_iso2 = t2->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");*/
    double t_iso1 = t1->GetTauID(tau_idiso_name);
    double t_iso2 = t2->GetTauID(tau_idiso_name);
   // if (t_iso1 != t_iso2) return t_iso1 < t_iso2;
    if (t_iso1 != t_iso2) return t_iso1 > t_iso2;
    return (t1->pt() > t2->pt());
  }

  bool SortByIsoEM(CompositeCandidate const* c1, CompositeCandidate const* c2, ic::strategy strategy, EventInfo const* eventInfo) {
    // First we sort the muons
    Muon const* m1 = static_cast<Muon const*>(c1->At(1));
    Muon const* m2 = static_cast<Muon const*>(c2->At(1));
    double m_iso1; 
    m_iso1 = (strategy == strategy::fall15) ? PF03IsolationVal(m1, 0.5, 0) : PF04IsolationVal(m1, 0.5, 0);
    double m_iso2;
    m_iso2 = (strategy == strategy::fall15) ? PF03IsolationVal(m2, 0.5, 0) : PF04IsolationVal(m2, 0.5, 0);
    // If the iso is different we just use this
    if (m_iso1 != m_iso2) return m_iso1 < m_iso2;
    // If not try the pT
    if (m1->pt() != m2->pt()) return m1->pt() > m2->pt();
    // If both of these are the same then try the electrons
    Electron const* e1 = static_cast<Electron const*>(c1->At(0));
    Electron const* e2 = static_cast<Electron const*>(c2->At(0));
    double e_iso1 = PF03IsolationVal(e1, 0.5, 0);
    double e_iso2 = PF03IsolationVal(e2, 0.5, 0);
    if(strategy == strategy::cpsummer17 || strategy==strategy::cpdecays17 || strategy==strategy::cpdecays18 || strategy==strategy::legacy16){
      e_iso1 = PF03EAIsolationVal(e1, eventInfo->jet_rho()); //lepton_rho
      e_iso2 = PF03EAIsolationVal(e2, eventInfo->jet_rho());
    } else {
      e_iso1 = PF03IsolationVal(e1, 0.5, 0);
      e_iso2 = PF03IsolationVal(e2, 0.5, 0);    
    }
    if (e_iso1 != e_iso2) return e_iso1 < e_iso2;
    return e1->pt() > e2->pt();
  }

  bool SortByIsoTT(CompositeCandidate const* c1, CompositeCandidate const* c2, std::string tau_idiso_name) {
    // First we sort the electrons
    Tau const* t1_1 = static_cast<Tau const*>(c1->At(0));
    Tau const* t1_2 = static_cast<Tau const*>(c2->At(0));
    /*double t1_iso1 = t1_1->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    double t1_iso2 = t1_2->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");*/
    double t1_iso1 = t1_1->GetTauID(tau_idiso_name);
    double t1_iso2 = t1_2->GetTauID(tau_idiso_name);
    // If the iso is different we just use this
    //if (t1_iso1 != t1_iso2) return t1_iso1 < t1_iso2;
    if (t1_iso1 != t1_iso2) return t1_iso1 > t1_iso2;
    // If not try the pT
    if (t1_1->pt() != t1_2->pt()) return t1_1->pt() > t1_2->pt();
    // If both of these are the same then try the taus
    Tau const* t2_1 = static_cast<Tau const*>(c1->At(1));
    Tau const* t2_2 = static_cast<Tau const*>(c2->At(1));
    /*double t2_iso1 = t2_1->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    double t2_iso2 = t2_2->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");*/
    double t2_iso1 = t2_1->GetTauID(tau_idiso_name);
    double t2_iso2 = t2_2->GetTauID(tau_idiso_name);
    // If the iso is different we just use this
    //if (t2_iso1 != t2_iso2) return t2_iso1 < t2_iso2;
    if (t2_iso1 != t2_iso2) return t2_iso1 > t2_iso2;
    // If not try the pT
    return t2_1->pt() > t2_2->pt();
  }

  void HTTPairSelector::CorrectMETForShift(ic::Met * met, ROOT::Math::PxPyPzEVector const& shift) {
    double metx = met->vector().px() - shift.px();
    double mety = met->vector().py() - shift.py();
    double metet = sqrt(metx*metx + mety*mety);
    ROOT::Math::PxPyPzEVector new_met(metx, mety, 0, metet);
    met->set_vector(ROOT::Math::PtEtaPhiEVector(new_met));
  }

}
