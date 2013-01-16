#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttPairSelector.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include <boost/functional/hash.hpp>
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"

#include "TMVA/Reader.h"
#include "TVector3.h"

namespace ic {

  HttPairSelector::HttPairSelector(std::string const& name) : ModuleBase(name), channel_(channel::et) {
    pair_label_ = "emtauCandidates";
    mva_met_from_vector_ = true;
    faked_tau_selector_ = 0;
    met_label_ = "pfMVAMet";
    fs_ = NULL;
    hists_.resize(1);
    use_most_isolated_ = false;
    scale_met_for_tau_ = 0;
    tau_scale_ = 1.0;
    allowed_tau_modes_ = "";
  }

  HttPairSelector::~HttPairSelector() {
    ;
  }

  int HttPairSelector::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for HTT Pair Selector" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Pair Collection Label: " << pair_label_ << std::endl;
    std::cout << "MET Label: " << met_label_ << std::endl;
    std::cout << "Use most isolated tau?: " << use_most_isolated_ << std::endl;
    std::cout << "Scale MET for Tau: " << scale_met_for_tau_ << std::endl;
    std::cout << "Tau Scale is: " << tau_scale_ << std::endl;
    std::cout << "Allowed tau decay modes: ";
    if (allowed_tau_modes_ == "") {
      std::cout << "all modes" << std::endl;
    } else {
      std::vector<std::string> tau_mode_vec;
      boost::split(tau_mode_vec, allowed_tau_modes_, boost::is_any_of(","));
      for (unsigned i = 0; i < tau_mode_vec.size(); ++i) {
        int tau_mode = boost::lexical_cast<int>(tau_mode_vec[i]);
        tau_mode_set_.insert(tau_mode);
        std::cout << tau_mode << " ";
      } 
      std::cout << std::endl;
    }

    if (faked_tau_selector_ == 1 && channel_ != channel::em) {
      std::cout << "Requring tau candidate matched to lepton!" << std::endl;
    }
    if (faked_tau_selector_ == 2 && channel_ != channel::em) {
      std::cout << "Requring tau candidate not matched to lepton!" << std::endl;
    }

    if (fs_) {
      hists_[0] = new Dynamic2DHistoSet(fs_->mkdir("httpairselector"));
      for (unsigned i = 0; i < hists_.size(); ++i) {
        hists_[i]->Create("n_pairs", 4, -0.5, 3.5, 4, -0.5, 3.5);
        hists_[i]->Create("taupt_ter_all", 50, 0, 250, 50, 0, 2);
        hists_[i]->Create("taupt_ter_iso_pass", 50, 0, 250, 50, 0, 2);
        hists_[i]->Create("taupt_ter_iso_iso_0p7", 50, 0, 250, 50, 0, 2);
        hists_[i]->Create("tauiso_ter_all", 50, -1, 1, 50, 0, 2);
      }
    }

    return 0;
  }

  int HttPairSelector::Execute(TreeEvent *event) {
    std::vector<CompositeCandidate *> & dilepton = event->GetPtrVec<CompositeCandidate>(pair_label_);
    std::vector<CompositeCandidate *> os_dilepton;
    std::vector<CompositeCandidate *> ss_dilepton;
    std::vector<CompositeCandidate *> result;

    for (unsigned i = 0; i < dilepton.size(); ++i) {
      if (PairOppSign(dilepton[i])) os_dilepton.push_back(dilepton[i]);
      if (PairSameSign(dilepton[i])) ss_dilepton.push_back(dilepton[i]);
    }

    if (fs_) {
      EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
      double wt = eventInfo->total_weight();
      hists_[0]->Fill("n_pairs", os_dilepton.size(), ss_dilepton.size(), wt);
      if (event->Exists("pileupInfo")) {
      std::vector<Tau*> taus = event->GetPtrVec<Tau>("taus");
      std::vector<GenJet *> & genjets = event->GetPtrVec<GenJet>("genJets");
      std::vector<std::pair<Tau*, GenJet*> > matches = MatchByDR(taus, genjets, 0.5, true, true);
      for (unsigned i = 0; i < matches.size(); ++i) {
        double ter = matches[i].first->pt() / matches[i].second->pt();
        double tpt = matches[i].first->pt();
        double tiso = matches[i].first->GetTauID("byIsolationMVAraw");
        bool pass_iso = matches[i].first->GetTauID("byLooseIsolationMVA") > 0.5;
        hists_[0]->Fill("taupt_ter_all", tpt, ter, wt);
        if (pass_iso) hists_[0]->Fill("taupt_ter_iso_pass", tpt, ter, wt);
        if (tiso > 0.7) hists_[0]->Fill("taupt_ter_iso_iso_0p7", tpt, ter, wt);
        hists_[0]->Fill("tauiso_ter_all", tiso, ter, wt);
      }
    }

    }

    // The first pair should have the highest "lepton 2" (0,1 = tau_h, 2 = muon) pT
    std::sort(os_dilepton.begin(),os_dilepton.end(), boost::bind(&CompositeCandidate::PtOf, _1, "lepton2") > boost::bind(&CompositeCandidate::PtOf, _2, "lepton2"));
    std::sort(ss_dilepton.begin(),ss_dilepton.end(), boost::bind(&CompositeCandidate::PtOf, _1, "lepton2") > boost::bind(&CompositeCandidate::PtOf, _2, "lepton2"));
    
    double max_tau_iso_os = -10.0;
    double max_tau_iso_ss = -10.0;
    if (use_most_isolated_ && channel_ != channel::em) {
      for (unsigned i = 0; i < os_dilepton.size(); ++i) {
        Tau const* temp  = dynamic_cast<Tau const*>(os_dilepton[i]->GetCandidate("lepton2"));
        if (temp->GetTauID("byIsolationMVAraw") > max_tau_iso_os) {
                os_dilepton[0] = os_dilepton[i];
                max_tau_iso_os = temp->GetTauID("byIsolationMVAraw");
        }
      }
      for (unsigned i = 0; i < ss_dilepton.size(); ++i) {
        Tau const* temp  = dynamic_cast<Tau const*>(ss_dilepton[i]->GetCandidate("lepton2"));
        if (temp->GetTauID("byIsolationMVAraw") > max_tau_iso_ss) {
                ss_dilepton[0] = ss_dilepton[i];
                max_tau_iso_ss = temp->GetTauID("byIsolationMVAraw");
        }
      }
    }

    if (os_dilepton.size() > 0) { // Take OS in preference to SS
      result.push_back(os_dilepton[0]);
    } else if (ss_dilepton.size() > 0) {
      result.push_back(ss_dilepton[0]);
    }
    if (result.size() == 0) return 1;  //Require at least one dilepton


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

    if (scale_met_for_tau_) {
      Met * met = event->GetPtr<Met>(met_label_);
      //std::cout << "Old: " << ROOT::Math::PxPyPzEVector(met->vector()) << std::endl;
      Tau const* tau = dynamic_cast<Tau const*>(result[0]->GetCandidate("lepton2"));
      //std::cout << "Tau: " << ROOT::Math::PxPyPzEVector(tau->vector()) << std::endl;
      double metx = met->vector().px();
      double mety = met->vector().py();
      double metet = met->vector().energy();
      double dx = tau->vector().px() * (( 1. / tau_scale_) - 1.);
      double dy = tau->vector().py() * (( 1. / tau_scale_) - 1.);
      metx = metx + dx;
      mety = mety + dy;
      metet = sqrt(metx*metx + mety*mety);
      ROOT::Math::PxPyPzEVector new_met(metx, mety, 0, metet);
      met->set_vector(ROOT::Math::PtEtaPhiEVector(new_met));
      //std::cout << "New: " << ROOT::Math::PxPyPzEVector(met->vector()) << std::endl;
    }

    // mode 0 = e-tau, mode 1 = mu-tau, mode 2 = e-mu
    // faked_tau_selector = 1 -> ZL, = 2 -> ZJ
    // This code only to be run on Z->ee or Z->mumu events (remove Z->tautau first!)
    if (faked_tau_selector_ > 0 && channel_ != channel::em) {
      std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>("genParticles");
      std::vector<GenParticle *> sel_particles;
      if (channel_ == channel::et) {
        // Add all status 3 electrons with pT > 8 to sel_particles
        for (unsigned i = 0; i < particles.size(); ++i) {
          if (particles[i]->status() == 3 && abs(particles[i]->pdgid()) == 11 && particles[i]->pt() > 8.) sel_particles.push_back(particles[i]);
        }
      } else if (channel_ == channel::mt || channel_ == channel::mtmet) {
        // Add all status 3 muons with pT > 8 to sel_particles
       for (unsigned i = 0; i < particles.size(); ++i) {
         if (particles[i]->status() == 3 && abs(particles[i]->pdgid()) == 13 && particles[i]->pt() > 8.) sel_particles.push_back(particles[i]);
       } 
      }
      // Get the reco tau from the pair
      std::vector<Candidate *> tau;
      tau.push_back(result[0]->GetCandidate("lepton2"));
      // Get the matches vector - require match within DR = 0.5, and pick the closest gen particle to the tau
      std::vector<std::pair<Candidate*, GenParticle*> > matches = MatchByDR(tau, sel_particles, 0.5, true, true);
      // If we want ZL and there's no match, fail the event
      if (faked_tau_selector_ == 1 && matches.size() == 0) return 1;
      // If we want ZJ and there is a match, fail the event
      if (faked_tau_selector_ == 2 && matches.size() > 0) return 1; 
    }

    Tau const* tau_ptr = dynamic_cast<Tau const*>(result[0]->GetCandidate("lepton2"));
    if (tau_ptr && allowed_tau_modes_ != "") {
      if (tau_mode_set_.find(tau_ptr->decay_mode()) == tau_mode_set_.end()) return 1;
    }


    dilepton = result;
    return 0;
  }
  int HttPairSelector::PostAnalysis() {
    return 0;
  }

  void HttPairSelector::PrintInfo() {
    ;
  }
}
