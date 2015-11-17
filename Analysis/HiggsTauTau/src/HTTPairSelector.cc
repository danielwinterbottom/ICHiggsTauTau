#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTPairSelector.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include <boost/functional/hash.hpp>
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"

namespace ic {

  HTTPairSelector::HTTPairSelector(std::string const& name) : ModuleBase(name), channel_(channel::et),strategy_(strategy::paper2013) {
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

    if (fs_) {
      EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
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
           std::sort(os_dilepton.begin(), os_dilepton.end(), SortByIsoET) ;
           std::sort(ss_dilepton.begin(), ss_dilepton.end(), SortByIsoET) ;
        }
        if(channel_ ==  channel::mt) { 
           std::sort(os_dilepton.begin(), os_dilepton.end(), SortByIsoMT) ;
           std::sort(ss_dilepton.begin(), ss_dilepton.end(), SortByIsoMT) ;
        }
        if(channel_ ==  channel::em) { 
           std::sort(os_dilepton.begin(), os_dilepton.end(), SortByIsoEM) ;
           std::sort(ss_dilepton.begin(), ss_dilepton.end(), SortByIsoEM) ;
        }
        if(channel_ ==  channel::tt) { 
           std::sort(os_dilepton.begin(), os_dilepton.end(), SortByIsoTT) ;
           std::sort(ss_dilepton.begin(), ss_dilepton.end(), SortByIsoTT) ;
        }
      }
      if (os_dilepton.size() > 0) { // Take OS in preference to SS
        result.push_back(os_dilepton[0]);
      } else if (ss_dilepton.size() > 0) {
        result.push_back(ss_dilepton[0]);
      }
    } else {
      // The first pair should have the highest "scalar sum pt" 
      std::sort(nosign_dilepton.begin(),nosign_dilepton.end(), SortBySumPt);
      
      // Or alternatively sort by isolation
      if (use_most_isolated_ ) {
        if(channel_ ==  channel::et) { 
           std::sort(nosign_dilepton.begin(), nosign_dilepton.end(), SortByIsoET) ;
        }
        if(channel_ ==  channel::mt) { 
           std::sort(nosign_dilepton.begin(), nosign_dilepton.end(), SortByIsoMT) ;
        }
        if(channel_ ==  channel::em) { 
           std::sort(nosign_dilepton.begin(), nosign_dilepton.end(), SortByIsoEM) ;
        }
        if(channel_ ==  channel::tt) { 
           std::sort(nosign_dilepton.begin(), nosign_dilepton.end(), SortByIsoTT) ;
        }
      }
      if (nosign_dilepton.size() > 0) { // No preference for OS over SS
        result.push_back(nosign_dilepton[0]);
      }
    }
    if (result.size() == 0) return 1;  //Require at least one dilepton
    //Rest of code is not needed for Z->ee and Z->mumu channels    
    if(channel_ == channel::zee || channel_ == channel::zmm || channel_ == channel::tpzmm || channel_ == channel::tpzee) {
      dilepton = result;
      return 0;
    }

    // ************************************************************************
    // If using pair-wise mva met select the appropriate met
    // ************************************************************************
    if (mva_met_from_vector_) {
      std::map<std::size_t, Met *> const& met_map = event->GetIDMap<Met>("pfMVAMetVector");
      std::size_t id = 0;
      boost::hash_combine(id, result[0]->GetCandidate("lepton1")->id());
      boost::hash_combine(id, result[0]->GetCandidate("lepton2")->id());
      std::map<std::size_t, Met *>::const_iterator it = met_map.find(id);
      if (it != met_map.end()) {
        Met * mva_met = it->second;
        event->Add("pfMVAMet", mva_met);
      } else {
        std::cerr << "Could not find Met in collection for ID: " << id << std::endl;
        exit(0);
      }
    }
    
   // ************************************************************************
   // Scale met for the tau energy scale shift
   // ************************************************************************
    if (scale_met_for_tau_ && channel_ != channel::em) {
      Met * met;
      if(strategy_ == strategy::paper2013){
       met = event->GetPtr<Met>(met_label_);
      } else{
       //std::vector<Met*> met_vec = event->GetPtrVec<Met>(met_label_);
       std::vector<Met*> met_vec = event->GetPtrVec<Met>("pfMet");
       met = met_vec.at(0);
      }
      Tau const* tau = dynamic_cast<Tau const*>(result[0]->GetCandidate("lepton2"));
      double t_scale = tau_scale_;
      if (event->Exists("tau_scales")) {
        std::map<std::size_t, double> const& tau_scales = event->Get< std::map<std::size_t, double>  > ("tau_scales");
        std::map<std::size_t, double>::const_iterator it = tau_scales.find(tau->id());
        if (it != tau_scales.end()) {
          t_scale = it->second;
        } else {
          std::cout << "Scale for chosen tau not found!" << std::endl;
          throw;
        }
      }
      double metx = met->vector().px();
      double mety = met->vector().py();
      double metet = met->vector().energy();
      double dx = tau->vector().px() * (( 1. / t_scale) - 1.);
      double dy = tau->vector().py() * (( 1. / t_scale) - 1.);
      metx = metx + dx;
      mety = mety + dy;
      metet = sqrt(metx*metx + mety*mety);
      ROOT::Math::PxPyPzEVector new_met(metx, mety, 0, metet);
      met->set_vector(ROOT::Math::PtEtaPhiEVector(new_met));
    }
    // ************************************************************************
    // Scale met for the electron energy scale shift
    // ************************************************************************
    if (scale_met_for_tau_ && channel_ == channel::em) {
      Met * met;// = event->GetPtr<Met>(met_label_);
      if(strategy_ == strategy::paper2013){
        met = event->GetPtr<Met>(met_label_);
      } else {
        //std::vector<Met*> met_vec = event->GetPtrVec<Met>(met_label_);
        std::vector<Met*> met_vec = event->GetPtrVec<Met>("pfMet");
        met = met_vec.at(0);
      }

      Electron const* elec = dynamic_cast<Electron const*>(result[0]->GetCandidate("lepton1"));
      double metx = met->vector().px();
      double mety = met->vector().py();
      double metet = met->vector().energy();
      double dx = elec->vector().px() * (( 1. / tau_scale_) - 1.);
      double dy = elec->vector().py() * (( 1. / tau_scale_) - 1.);
      metx = metx + dx;
      mety = mety + dy;
      metet = sqrt(metx*metx + mety*mety);
      ROOT::Math::PxPyPzEVector new_met(metx, mety, 0, metet);
      met->set_vector(ROOT::Math::PtEtaPhiEVector(new_met));
    }

    // ************************************************************************
    // Select l->tau or jet->tau fakes
    // ************************************************************************
    // mode 0 = e-tau, mode 1 = mu-tau, mode 2 = e-mu
    // faked_tau_selector = 1 -> ZL, = 2 -> ZJ
    // This code only to be run on Z->ee or Z->mumu events (remove Z->tautau first!)
    if(strategy_ != strategy::spring15) {
      if (faked_tau_selector_ > 0  && channel_ != channel::em) {
        std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>("genParticles");
        std::vector<GenParticle *> sel_particles;
        if (channel_ == channel::et || channel_ == channel::etmet) {
          // Add all status 3 electrons with pT > 8 to sel_particles
          for (unsigned i = 0; i < particles.size(); ++i) {
            if ( (abs(particles[i]->pdgid()) == 11 || abs(particles[i]->pdgid()) == 13) && particles[i]->pt() > 8.) sel_particles.push_back(particles[i]);
          }
        } else if (channel_ == channel::mt || channel_ == channel::mtmet) {
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
        std::vector<GenJet> gen_taus = BuildTauJets(particles, false);
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

   /* std::vector<Candidate *> leading_lepton;
    std::vector<Candidate *> subleading_lepton;
    leading_lepton.push_back(result[0]->GetCandidate("lepton1"));
    subleading_lepton.push_back(result[0]->GetCandidate("lepton2")); 

    std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>("genParticles");
    std::vector<GenParticle *> prompt_sel_particles;
    std::vector<GenParticle *> tau_decay_sel_particles;
    for (unsigned i=0; i < particles.size(); ++i){
      std::vector<bool> status_flags = particles[i]->statusFlags();
      if ( (abs(particles[i]->pdgid()) == 11 || abs(particles[i]->pdgid()) == 13) && status_flags[IsPrompt] && particles[i]->status()==1 && particles[i]->pt() > 8.){
        prompt_sel_particles.push_back(particles[i]);
      }
      if ( (abs(particles[i]->pdgid())==11 || abs(particles[i]->pdgid()) == 13) && status_flags[IsTauDecayProduct] && particles[i]->pt() > 8.){
       tau_decay_sel_particles.push_back(particles[i]);
      }
    }

    
//    std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>("genParticles");
    std::vector<GenJet> gen_taus = BuildTauJets(particles, false);
    std::vector<GenJet *> gen_taus_ptr;
    for (auto & x : gen_taus) gen_taus_ptr.push_back(&x);
    ic::erase_if(gen_taus_ptr, !boost::bind(MinPtMaxEta, _1, 18.0, 999.));
 
 
 
    std::vector<std::pair<Candidate*, GenParticle*> > leading_prompt_match = MatchByDR(leading_lepton, prompt_sel_particles, 0.5, true, true);
    std::vector<std::pair<Candidate*, GenParticle*> > leading_tau_decay_match = MatchByDR(leading_lepton, tau_decay_sel_particles, 0.5, true, true);
    std::vector<std::pair<Candidate*, GenJet*> > leading_tau_match  = MatchByDR(leading_lepton, gen_taus_ptr, 0.5, true, true);

    std::vector<std::pair<Candidate*, GenParticle*> > subleading_prompt_match = MatchByDR(subleading_lepton, prompt_sel_particles, 0.5, true, true);
    std::vector<std::pair<Candidate*, GenParticle*> > subleading_tau_decay_match = MatchByDR(subleading_lepton, tau_decay_sel_particles, 0.5, true, true);
    std::vector<std::pair<Candidate*, GenJet*> > subleading_tau_match  = MatchByDR(subleading_lepton, gen_taus_ptr, 0.5, true, true);

   int leading_lepton_source;
   int subleading_lepton_source;
   int promptsize = leading_prompt_match.size();
   int taudecaysize = leading_tau_decay_match.size();
   int tausize = leading_tau_match.size();


   if(promptsize!=0 && taudecaysize!=0 && tausize == 0){
     DR(leading_prompt_match.at(0).first,leading_prompt_match.at(0).second) < DR(leading_tau_decay_match.at(0).first,leading_tau_decay_match.at(0).second) ? taudecaysize=0 : promptsize = 0;
   } else if (promptsize!=0 && taudecaysize==0 && tausize != 0){
     DR(leading_prompt_match.at(0).first,leading_prompt_match.at(0).second) < DR(leading_tau_match.at(0).first,leading_tau_match.at(0).second) ? tausize=0 : promptsize = 0;
  } else if (promptsize==0 && taudecaysize!=0 && tausize != 0){
     DR(leading_tau_decay_match.at(0).first,leading_tau_decay_match.at(0).second) < DR(leading_tau_match.at(0).first,leading_tau_match.at(0).second) ? tausize=0 : taudecaysize = 0;
 } else if(promptsize!=0 && taudecaysize!=0 && tausize!=0){
   if(DR(leading_tau_decay_match.at(0).first,leading_tau_decay_match.at(0).second) < DR(leading_tau_match.at(0).first,leading_tau_match.at(0).second)){
      tausize=0;
      DR(leading_tau_decay_match.at(0).first,leading_tau_decay_match.at(0).second) < DR(leading_prompt_match.at(0).first,leading_prompt_match.at(0).second) ? promptsize=0:taudecaysize=0;
    } else {
      taudecaysize = 0;
      DR(leading_tau_match.at(0).first,leading_tau_match.at(0).second) < DR(leading_prompt_match.at(0).first,leading_prompt_match.at(0).second) ? promptsize=0:tausize=0;
    } 
 }


   if(promptsize==0 && taudecaysize==0&&tausize==0) leading_lepton_source = 6;
   if(promptsize!=0 && taudecaysize==0 && tausize ==0){
     if(abs(leading_prompt_match.at(0).second->pdgid())==11){
       leading_lepton_source = 1;
     } else leading_lepton_source = 2;
   }
   if(promptsize==0 && taudecaysize !=0 &&tausize==0){
     if(abs(leading_tau_decay_match.at(0).second->pdgid())==11){
       leading_lepton_source = 3;
     } else leading_lepton_source = 4;
   }
   if(promptsize==0 && taudecaysize==0 && tausize!=0) leading_lepton_source = 5;

//Now for subleading lepton:
   promptsize = subleading_prompt_match.size();
   taudecaysize = subleading_tau_decay_match.size();
   tausize = subleading_tau_match.size();

   if(promptsize!=0 && taudecaysize!=0 && tausize == 0){
     DR(subleading_prompt_match.at(0).first,subleading_prompt_match.at(0).second) < DR(subleading_tau_decay_match.at(0).first,subleading_tau_decay_match.at(0).second) ? taudecaysize=0 : promptsize = 0;
   } else if (promptsize!=0 && taudecaysize==0 && tausize != 0){
     DR(subleading_prompt_match.at(0).first,subleading_prompt_match.at(0).second) < DR(subleading_tau_match.at(0).first,subleading_tau_match.at(0).second) ? tausize=0 : promptsize = 0;
  } else if (promptsize==0 && taudecaysize!=0 && tausize != 0){
     DR(subleading_tau_decay_match.at(0).first,subleading_tau_decay_match.at(0).second) < DR(subleading_tau_match.at(0).first,subleading_tau_match.at(0).second) ? tausize=0 : taudecaysize = 0;
 } else if(promptsize!=0 && taudecaysize!=0 && tausize!=0){
   if(DR(subleading_tau_decay_match.at(0).first,subleading_tau_decay_match.at(0).second) < DR(subleading_tau_match.at(0).first,subleading_tau_match.at(0).second)){
      tausize=0;
      DR(subleading_tau_decay_match.at(0).first,subleading_tau_decay_match.at(0).second) < DR(subleading_prompt_match.at(0).first,subleading_prompt_match.at(0).second) ? promptsize=0:taudecaysize=0;
    } else {
      taudecaysize = 0;
      DR(subleading_tau_match.at(0).first,subleading_tau_match.at(0).second) < DR(subleading_prompt_match.at(0).first,subleading_prompt_match.at(0).second) ? promptsize=0:tausize=0;
    } 
 }
   if(promptsize==0 && taudecaysize==0&&tausize==0) subleading_lepton_source = 6;
   if(promptsize!=0 && taudecaysize==0 && tausize ==0){
     if(abs(subleading_prompt_match.at(0).second->pdgid())==11){
       subleading_lepton_source = 1;
     } else subleading_lepton_source = 2;
   }
   if(promptsize==0 && taudecaysize !=0 &&tausize==0){
     if(abs(subleading_tau_decay_match.at(0).second->pdgid())==11){
       subleading_lepton_source = 3;
     } else subleading_lepton_source = 4;
   }
   if(promptsize==0 && taudecaysize==0 && tausize!=0) subleading_lepton_source = 5;

   EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
   double wt = eventInfo->total_weight();

   hists_[0]->Fill("lepton_source",leading_lepton_source,subleading_lepton_source,wt);
   event->Add("leading_lepton_source",leading_lepton_source);
   event->Add("subleading_lepton_source",subleading_lepton_source);
*/

    if(strategy_ == strategy::spring15 && ztt_mode_>0) {
    //if(strategy_ == strategy::spring15 && channel_ != channel::em && ztt_mode_>0) {
     if(!use_status_flags_){
      bool is_ztt=false; 
      bool has_z = false;
      // First find out if the gen level info points to Z->ll or Z->tautau
  //    if ((faked_tau_selector_ > 0 || ztt_mode_ > 0)) {
        std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>("genParticles");
        std::vector<GenParticle *> sel_particles;
        //Find Z and check if daughters are taus or electrons/muons 
        for (unsigned i = 0; i < particles.size(); ++i) {
          if ( (abs(particles[i]->pdgid())) == 23)  {
            has_z = true;
            std::vector<GenParticle *> const& daughters = ExtractDaughters(particles[i], particles);
            for (unsigned j = 0; j < daughters.size(); ++j) {
              if ((abs(daughters[j]->pdgid())) == 15 && daughters[j]->pt() > 8.) {
                is_ztt = true;
               }
             }
           }
          if ( (abs(particles[i]->pdgid()) == 11 || abs(particles[i]->pdgid()) == 13) && particles[i]->pt() > 8.){
            sel_particles.push_back(particles[i]);
          }
        } 
      if(!has_z){
       // bool has_tau_orphan = false;
        bool has_tau_from_incoming = false;
//        bool has_emu_from_incoming = false;
        for (unsigned i = 0; i< particles.size(); ++i){
          if ( abs(particles[i]->pdgid()) == 15 && particles[i]->pt() > 8.){
            std::vector<GenParticle *> const& mothers = ExtractMothers(particles[i],particles);
        //    if(mothers.size() ==0) has_tau_orphan = true;
            for(unsigned j = 0; j<mothers.size(); ++j){
              if(mothers[j]->status()==21){
                has_tau_from_incoming=true;
              }
            }
          /*} else if ((abs(particles[i]->pdgid()) == 13 || abs(particles[i]->pdgid()) == 11) && particles[i]->pt()>8.){
            std::vector<GenParticle *> const& mothers = ExtractMothers(particles[i], particles);
            for( unsigned j =0; j < mothers.size(); ++j){
             if(mothers[j]->status()==21){
               has_emu_from_incoming = true;
             }
           }*/
         }
       }
       if(has_tau_from_incoming){
       //If we have a tau that comes from one of the incoming partons we have a Z->tautau event with virtual Z:
          is_ztt = true;
       } //else if (has_tau_orphan&&!has_emu_from_incoming){
       //Otherwise if we have a tau that the mothers haven't been stored for and we have no electrons/muons
       //with an incoming parton mother, also classify the event as Z->tautau
          /*is_ztt = true;
       }*/
     }


        //Fail the event if no gen-level Z's found for ZTT
//        if (!has_z && ztt_mode_==1) return 1;
        // If we want Z->tautau and we have found a Z->ll, fail the event
        if (ztt_mode_ == 1 && !is_ztt) return 1;
        // If we want Z->ll and we have found a Z->tautau, fail the event
        if (ztt_mode_ == 2 && is_ztt) return 1;
//        if (sel_particles.size() > 0) {
          // Get the reco tau from the pair
         if (channel_ != channel::em){
          std::vector<Candidate *> tau;
          if(channel_ == channel::tt) tau.push_back(result[0]->GetCandidate("lepton1"));
          tau.push_back(result[0]->GetCandidate("lepton2"));
          // Get the matches vector - require match within DR = 0.5, and pick the closest gen particle to the tau
          std::vector<std::pair<Candidate*, GenParticle*> > matches = MatchByDR(tau, sel_particles, 0.5, true, true);
          // If we want ZL and there's no match, fail the event
          if (faked_tau_selector_ == 1 && matches.size() == 0 && channel_ != channel::tt) return 1;
          // If we want ZL and there are less than 2 matches, fail the event for the tt channel
          if (faked_tau_selector_ == 1 && matches.size() < 2 && channel_ == channel::tt) return 1;
          // If we want ZJ and there is a match, fail the event
          if (faked_tau_selector_ == 2 && matches.size() > 0 && channel_ != channel::tt) return 1;
          //If we want ZJ and there are at least 2 matches for tt, fail the event
          if (faked_tau_selector_ == 2 && matches.size() > 1 && channel_ == channel::tt) return 1;
//        }
  //    }
  //    Not figured out how the below should work yet
      if (hadronic_tau_selector_ > 0) {
        std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>(gen_taus_label_);
        std::vector<GenJet> gen_taus = BuildTauJets(particles, false);
        std::vector<GenJet *> gen_taus_ptr;
        for (auto & x : gen_taus) gen_taus_ptr.push_back(&x);
        ic::erase_if(gen_taus_ptr, !boost::bind(MinPtMaxEta, _1, 18.0, 999.));
        std::vector<Candidate *> tau;
        tau.push_back(result[0]->GetCandidate("lepton2"));
        // Get the matches vector - require match within DR = 0.5, and pick the closest gen particle to the tau
        std::vector<std::pair<Candidate*, GenJet*> > matches = MatchByDR(tau, gen_taus_ptr, 0.5, true, true);
        // If we want ZTT and there's no match, fail the event
        if (hadronic_tau_selector_ == 1 && matches.size() == 0) return 1;
        if (fs_ && matches.size() == 1) {
          hists_[0]->Fill("pt_gen_reco", matches[0].second->pt(), matches[0].first->pt(), 1);
        }
        // If we want ZJ and there is a match, fail the event
        if (hadronic_tau_selector_ == 2 && matches.size() > 0) return 1;
      }
     }
    }
/*   else{
    bool is_ztt = false;
    std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>("genParticles"); 
    std::vector<GenParticle *> sel_particles;
    int ntaus=0;
    for (unsigned i = 0; i < particles.size(); ++i){
      std::vector<bool> status_flags = particles[i]->statusFlags();
      if (status_flags[IsPrompt] && abs(particles[i]->pdgid()) == 15) ntaus++;
      if ((abs(particles[i]->pdgid()) == 11|| abs(particles[i]->pdgid() == 13)) && particles[i]->pt() > 8.){
        sel_particles.push_back(particles[i]);
      }
    }
    if(ntaus > 1) is_ztt = true;
    if (ztt_mode_ == 1 && !is_ztt) return 1;
    // If we want Z->ll and we have found a Z->tautau, fail the event
    if (ztt_mode_ == 2 && is_ztt) return 1;
    std::vector<Candidate *> tau;
    tau.push_back(result[0]->GetCandidate("lepton2"));
    std::vector<std::pair<Candidate*, GenParticle*>> matches = MatchByDR(tau, sel_particles, 0.5, true, true);
    int pdgid=0;
    if(matches.size()!=0) pdgid = abs((matches.at(0).second)->pdgid());
    if( faked_tau_selector_ == 1 && (matches.size() ==0) return 1; 
    if( faked_tau_selector_ == 2 && matches.size()!=0) return 1;
   if (hadronic_tau_selector_ > 0) {
        std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>(gen_taus_label_);
        std::vector<GenJet> gen_taus = BuildTauJets(particles, false);
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
    
   }*/
    
     

  else{
    bool is_ztt = false;
    std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>("genParticles"); 
    std::vector<GenParticle *> sel_particles;
    int ntaus=0;
    for (unsigned i = 0; i < particles.size(); ++i){
      std::vector<bool> status_flags = particles[i]->statusFlags();
      if (status_flags[IsPrompt] && abs(particles[i]->pdgid()) == 15) ntaus++;
      if ((abs(particles[i]->pdgid()) == 11 || abs(particles[i]->pdgid()) ==13) && particles[i]->pt() > 8.){
        sel_particles.push_back(particles[i]);
      }
    }
    if(ntaus > 1) is_ztt = true;
    if (ztt_mode_ == 1 && !is_ztt) return 1;
    // If we want Z->ll and we have found a Z->tautau, fail the event
    if (ztt_mode_ == 2 && is_ztt) return 1;
         if (channel_ != channel::em){
          std::vector<Candidate *> tau;
          if (channel_ == channel::tt) tau.push_back(result[0]->GetCandidate("lepton1"));
          tau.push_back(result[0]->GetCandidate("lepton2"));
          // Get the matches vector - require match within DR = 0.5, and pick the closest gen particle to the tau
          std::vector<std::pair<Candidate*, GenParticle*> > matches = MatchByDR(tau, sel_particles, 0.5, true, true);
          // If we want ZL and there's no match for etau/mutau, fail the event
          if (faked_tau_selector_ == 1 && matches.size() == 0 && channel_ != channel::tt) return 1;
          // If we want ZL and there aren't two matches, fail the event 
          if (faked_tau_selector_ == 1 && matches.size() < 2 && channel_ ==channel::tt ) return 1;
          // If we want ZJ and there is a match for etau/mutau, fail the event
          if (faked_tau_selector_ == 2 && matches.size() > 0 && channel_ != channel::tt) return 1;
          // If we want ZJ and there are at least two matches  for tautau, fail the event
          if (faked_tau_selector_ == 2 && matches.size() > 1 && channel_ == channel::tt) return 1; 
//        }
  //    }
  //    Not figured out how the below should work yet
      if (hadronic_tau_selector_ > 0) {
        std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>(gen_taus_label_);
        std::vector<GenJet> gen_taus = BuildTauJets(particles, false);
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

   }
 }
/*    if(strategy_==strategy::spring15 && channel_==channel::em && ztt_mode_>0){
      bool is_ztt=false; 
      bool has_z = false;
      // First find out if the gen level info points to Z->ll or Z->tautau
        std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>("genParticles");
        std::vector<GenParticle *> sel_particles;
        //Find Z and check if daughters are taus or electrons/muons 
        for (unsigned i = 0; i < particles.size(); ++i) {
          if ( (abs(particles[i]->pdgid())) == 23)  {
            has_z = true;
            std::vector<GenParticle *> const& daughters = ExtractDaughters(particles[i], particles);
            for (unsigned j = 0; j < daughters.size(); ++j) {
              if ((abs(daughters[j]->pdgid())) == 15 && daughters[j]->pt() > 8.) {
                is_ztt = true;
               }
             }
           }*/
        /*  if ( (abs(particles[i]->pdgid()) == 11 || abs(particles[i]->pdgid()) == 13) && particles[i]->pt() > 8.){
            sel_particles.push_back(particles[i]);
          }*/
     /*   } 
      if(!has_z){
//        bool has_tau_orphan = false;
        bool has_tau_from_incoming = false;
//        bool has_emu_from_incoming = false;
        for (unsigned i = 0; i< particles.size(); ++i){
          if ( abs(particles[i]->pdgid()) == 15 && particles[i]->pt() > 8.){
            std::vector<GenParticle *> const& mothers = ExtractMothers(particles[i],particles);
 //           if(mothers.size() ==0) has_tau_orphan = true;
            for(unsigned j = 0; j<mothers.size(); ++j){
              if(mothers[j]->status()==21){
                has_tau_from_incoming=true;
              }
            }*/
          /*} else if ((abs(particles[i]->pdgid()) == 13 || abs(particles[i]->pdgid()) == 11) && particles[i]->pt()>8.){
            std::vector<GenParticle *> const& mothers = ExtractMothers(particles[i], particles);
            for( unsigned j =0; j < mothers.size(); ++j){
             if(mothers[j]->status()==21){
               has_emu_from_incoming = true;
             }
           }*/
      /*   }
       }
       if(has_tau_from_incoming){
       //If we have a tau that comes from one of the incoming partons we have a Z->tautau event with virtual Z:
          is_ztt = true;
       } //else if (has_tau_orphan&&!has_emu_from_incoming){
       //Otherwise if we have a tau that the mothers haven't been stored for and we have no electrons/muons
       //with an incoming parton mother, also classify the event as Z->tautau
          //is_ztt = true;
       //}
     }
     // If we want Z->tautau and we have found a Z->ll, fail the event
     if (ztt_mode_ == 1 && !is_ztt) return 1;
     // If we want Z->ll and we have found a Z->tautau, fail the event
     if (ztt_mode_ == 2 && is_ztt) return 1;
    }*/

    
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

  bool SortByIsoET(CompositeCandidate const* c1, CompositeCandidate const* c2) {
    // First we sort the electrons
    Electron const* e1 = static_cast<Electron const*>(c1->At(0));
    Electron const* e2 = static_cast<Electron const*>(c2->At(0));
    double e_iso1 = PF03IsolationVal(e1, 0.5, 0);
    double e_iso2 = PF03IsolationVal(e2, 0.5, 0);
    // If the iso is different we just use this
    if (e_iso1 != e_iso2) return e_iso1 < e_iso2;
    // If not try the pT
    if (e1->pt() != e2->pt()) return e1->pt() > e2->pt();
    // If both of these are the same then try the taus
    Tau const* t1 = static_cast<Tau const*>(c1->At(1));
    Tau const* t2 = static_cast<Tau const*>(c2->At(1));
    double t_iso1 = t1->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    double t_iso2 = t2->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    if (t_iso1 != t_iso2) return t_iso1 < t_iso2;
    return (t1->pt() > t2->pt());
  }

  bool SortByIsoMT(CompositeCandidate const* c1, CompositeCandidate const* c2) {
    // First we sort the electrons
    Muon const* m1 = static_cast<Muon const*>(c1->At(0));
    Muon const* m2 = static_cast<Muon const*>(c2->At(0));
    double m_iso1 = PF03IsolationVal(m1, 0.5, 0);
    double m_iso2 = PF03IsolationVal(m2, 0.5, 0);
    // If the iso is different we just use this
    if (m_iso1 != m_iso2) return m_iso1 < m_iso2;
    // If not try the pT
    if (m1->pt() != m2->pt()) return m1->pt() > m2->pt();
    // If both of these are the same then try the taus
    Tau const* t1 = static_cast<Tau const*>(c1->At(1));
    Tau const* t2 = static_cast<Tau const*>(c2->At(1));
    double t_iso1 = t1->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    double t_iso2 = t2->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    if (t_iso1 != t_iso2) return t_iso1 < t_iso2;
    return (t1->pt() > t2->pt());
  }

  bool SortByIsoEM(CompositeCandidate const* c1, CompositeCandidate const* c2) {
    // First we sort the muons
    Muon const* m1 = static_cast<Muon const*>(c1->At(1));
    Muon const* m2 = static_cast<Muon const*>(c2->At(1));
    double m_iso1 = PF03IsolationVal(m1, 0.5, 0);
    double m_iso2 = PF03IsolationVal(m2, 0.5, 0);
    // If the iso is different we just use this
    if (m_iso1 != m_iso2) return m_iso1 < m_iso2;
    // If not try the pT
    if (m1->pt() != m2->pt()) return m1->pt() > m2->pt();
    // If both of these are the same then try the electrons
    Electron const* e1 = static_cast<Electron const*>(c1->At(0));
    Electron const* e2 = static_cast<Electron const*>(c2->At(0));
    double e_iso1 = PF03IsolationVal(e1, 0.5, 0);
    double e_iso2 = PF03IsolationVal(e2, 0.5, 0);
    if (e_iso1 != e_iso2) return e_iso1 < e_iso2;
    return e1->pt() > e2->pt();
  }

  bool SortByIsoTT(CompositeCandidate const* c1, CompositeCandidate const* c2) {
    // First we sort the electrons
    Tau const* t1_1 = static_cast<Tau const*>(c1->At(0));
    Tau const* t1_2 = static_cast<Tau const*>(c2->At(0));
    double t1_iso1 = t1_1->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    double t1_iso2 = t1_2->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    // If the iso is different we just use this
    if (t1_iso1 != t1_iso2) return t1_iso1 < t1_iso2;
    // If not try the pT
    if (t1_1->pt() != t1_2->pt()) return t1_1->pt() > t1_2->pt();
    // If both of these are the same then try the taus
    Tau const* t2_1 = static_cast<Tau const*>(c1->At(1));
    Tau const* t2_2 = static_cast<Tau const*>(c2->At(1));
    double t2_iso1 = t2_1->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    double t2_iso2 = t2_2->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
    // If the iso is different we just use this
    if (t2_iso1 != t2_iso2) return t2_iso1 < t2_iso2;
    // If not try the pT
    return t2_1->pt() > t2_2->pt();
  }

}
