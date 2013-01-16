#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HTTCategories.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"

#include "TMVA/Reader.h"
#include "TVector3.h"

namespace ic {

  HTTCategories::HTTCategories(std::string const& name) : ModuleBase(name), channel_(channel::et) {
    ditau_label_ = "emtauCandidates";
    met_label_ = "pfMVAMet";
    is_embedded_ = false;
    fs_ = NULL;
  }

  HTTCategories::~HTTCategories() {
    ;
  }

  void HTTCategories::InitSelection(std::string const& selection) {
    selections_[selection] = false;

  }
  void HTTCategories::InitCategory(std::string const& category) {
    categories_[category] = false;
    InitMassPlots(category);
  }

  int HTTCategories::PreAnalysis() {
    std::cout << "** PreAnalysis Info for HTT Categories **" << std::endl;
    if (fs_) {
      std::cout << "Channel: " << Channel2String(channel_) << std::endl;
      std::cout << "Ditau Label: " << ditau_label_ << std::endl;
      std::cout << "MET Label: " << met_label_ << std::endl;
    }

    misc_plots_ = new DynamicHistoSet(fs_->mkdir("misc_plots"));
    misc_2dplots_ = new Dynamic2DHistoSet(fs_->mkdir("misc_2dplots"));

    InitSelection("os");
    InitSelection("os_sel");
    InitSelection("os_con");
    InitSelection("os_con_mt_60-120");
    InitSelection("ss");
    InitSelection("ss_sel");
    InitSelection("ss_con");
    InitSelection("ss_con_mt_60-120");

    InitCategory("inclusive");
    InitCoreControlPlots("inclusive");

    InitCategory("vbf");
    InitCategory("vbf_no_cjv");
    InitCategory("vbf_loose");
    InitCategory("vbf_loose_jets20");
    InitCategory("1jet_high");
    InitCategory("1jet_low");
    InitCategory("0jet_high");
    InitCategory("0jet_low");
    InitCategory("btag");
    InitCategory("btag_loose");
    InitCategory("nobtag");

    return 0;
  }

  int HTTCategories::Execute(TreeEvent *event) {

    Reset();

    // Get the objects we need from the event
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    wt_ = eventInfo->total_weight();
    std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>(ditau_label_);
    CompositeCandidate const* ditau = ditau_vec.at(0);
    Candidate const* lep1 = ditau->GetCandidate("lepton1");
    Candidate const* lep2 = ditau->GetCandidate("lepton2");
    Met const* met = event->GetPtr<Met>(met_label_);
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("pfJetsPFlow");
    std::sort(jets.begin(), jets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    std::vector<PFJet*> lowpt_jets = jets;
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
    ic::erase_if(lowpt_jets,!boost::bind(MinPtMaxEta, _1, 20.0, 4.7));
    std::vector<PFJet*> bjets = lowpt_jets;
    ic::erase_if(bjets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
    std::vector<PFJet*> loose_bjets = bjets;
    ic::erase_if(loose_bjets, boost::bind(&PFJet::GetBDiscriminator, _1, "combinedSecondaryVertexBJetTags") < 0.244);
    ic::erase_if(bjets, boost::bind(&PFJet::GetBDiscriminator, _1, "combinedSecondaryVertexBJetTags") < 0.679);


    // Define event properties
    // IMPORTANT: Make sure each property is re-set
    // for each new event
    if (PairOppSign(ditau)) {
      os_ = true;
    } else {
      os_ = false;
    }

    n_vtx_ = eventInfo->good_vertices();

    if (event->Exists("svfitMass")) {
      m_sv_ = event->Get<double>("svfitMass");
    } else {
      m_sv_ = -9999;
    }

    m_vis_ = ditau->M();

    // This is the HCP hack for the em channel
    // to better align the data with the embedded
    // mass.  
    if (channel_ == channel::em && is_embedded_) {
      m_sv_ = m_sv_ * 1.01;
      m_vis_ = m_vis_ * 1.01;
    }
    if (event->Exists("mass_scale")) {
      m_sv_ = m_sv_ * event->Get<double>("mass_scale");
      m_vis_ = m_vis_ * event->Get<double>("mass_scale");
    }

    mt_1_ = MT(lep1, met);
    pzeta_ = PZeta(ditau, met, 0.85);

    pt_1_ = lep1->pt();
    pt_2_ = lep2->pt();
    eta_1_ = lep1->eta();
    eta_2_ = lep2->eta();
    met_ = met->pt();
    met_phi_ = met->phi();

    Tau const* tau = dynamic_cast<Tau const*>(lep2);
    if (tau) {
      tau_decay_mode_ = tau->decay_mode();
    } else {
      tau_decay_mode_ = 0;
    }

    n_jets_ = jets.size();
    n_lowpt_jets_ = lowpt_jets.size();
    n_bjets_ = bjets.size();
    n_loose_bjets_ = loose_bjets.size();

    if (n_jets_ >= 1) {
      jpt_1_ = jets[0]->pt();
      jeta_1_ = jets[0]->eta();
    } else {
      jpt_1_ = -9999;
      jeta_1_ = -9999;
    }

    if (n_jets_ >= 2) {
      jpt_2_ = jets[1]->pt();
      jeta_2_ = jets[1]->eta();
      mjj_ = (jets[0]->vector() + jets[1]->vector()).M();
      jdeta_ = fabs(jets[0]->eta() - jets[1]->eta());
      double eta_high = (jets[0]->eta() > jets[1]->eta()) ? jets[0]->eta() : jets[1]->eta();
      double eta_low = (jets[0]->eta() > jets[1]->eta()) ? jets[1]->eta() : jets[0]->eta();
      n_jetsingap_ = 0;
      if (n_jets_ > 2) {
        for (unsigned i = 2; i < jets.size(); ++i) {
         if (jets[i]->pt() > 30.0 &&  jets[i]->eta() > eta_low && jets[i]->eta() < eta_high) ++n_jetsingap_;
        }
      }
    } else {
      jpt_2_ = -9999;
      jeta_2_ = -9999;
      mjj_ = -9999;
      jdeta_ = -9999;
      n_jetsingap_ = 9999;
    }

    if (n_lowpt_jets_ >= 2) {
      mjj_lowpt_ = (lowpt_jets[0]->vector() + lowpt_jets[1]->vector()).M();
      jdeta_lowpt_ = fabs(lowpt_jets[0]->eta() - lowpt_jets[1]->eta());
      double eta_high = (lowpt_jets[0]->eta() > lowpt_jets[1]->eta()) ? lowpt_jets[0]->eta() : lowpt_jets[1]->eta();
      double eta_low = (lowpt_jets[0]->eta() > lowpt_jets[1]->eta()) ? lowpt_jets[1]->eta() : lowpt_jets[0]->eta();
      n_jetsingap_lowpt_ = 0;
      if (n_lowpt_jets_ > 2) {
        for (unsigned i = 2; i < lowpt_jets.size(); ++i) {
         if (lowpt_jets[i]->pt() > 30.0 &&  lowpt_jets[i]->eta() > eta_low && lowpt_jets[i]->eta() < eta_high) ++n_jetsingap_lowpt_;
        }
      }
    } else {
      mjj_lowpt_ = -9999;
      jdeta_lowpt_ = -9999;
      n_jetsingap_lowpt_ = 9999;
    }

    if (n_bjets_ >= 1) {
      bpt_1_ = bjets[0]->pt();
      beta_1_ = bjets[0]->eta();
    } else {
      bpt_1_ = -9999;
      beta_1_ = -9999;
    }

    // Define which selections this event passes
    if (channel_ == channel::et || channel_ == channel::mt || channel_ == channel::mtmet) {
      if (os_ && mt_1_ < 20.0) SetPassSelection("os_sel");
      if (os_) SetPassSelection("os");
      if (!os_) SetPassSelection("ss");
      if (os_ && mt_1_ > 70.0) SetPassSelection("os_con");
      if (os_ && mt_1_ > 60.0 && mt_1_ < 120.) SetPassSelection("os_con_mt_60-120");
      if (!os_ && mt_1_ < 20.0) SetPassSelection("ss_sel");
      if (!os_ && mt_1_ > 70.0) SetPassSelection("ss_con");
      if (!os_ && mt_1_ > 60.0 && mt_1_ < 120.) SetPassSelection("ss_con_mt_60-120");
    }

    if (channel_ == channel::em) {
      if (os_ && pzeta_ > -20) SetPassSelection("os_sel");
      if (os_) SetPassSelection("os");
      if (!os_) SetPassSelection("ss");
      if (!os_ && pzeta_ > -20) SetPassSelection("ss_sel");

    }

    // Define the 1- and 0-jet split based on pt_2
    double pt2_split = 40.0; // Tau pT for et,mt and mtmet
    if (channel_ == channel::em) pt2_split = 35.0;  // Mu pT for em

    // Inclusive Category
    SetPassCategory("inclusive");
    FillCoreControlPlots("inclusive");

    // VBF Selection
    // In the em channel, additionally apply b-jet veto
    if (n_jets_ >= 2 && n_jetsingap_ == 0 && mjj_ > 500. && jdeta_ > 3.5) {
      if ( (channel_ == channel::em) ? (n_bjets_ == 0) : true) SetPassCategory("vbf");
    }

    // VBF Selection with no CJV, used for VBF category Fakes estimate in em
    if (n_jets_ >= 2 && mjj_ > 500. && jdeta_ > 3.5) {
      if ( (channel_ == channel::em) ? (n_bjets_ == 0) : true) SetPassCategory("vbf_no_cjv");
    }

    // Loose VBF Selection
    // Used for background shape estimation in VBF category
    if (n_jets_ >= 2 && n_jetsingap_ == 0 && mjj_ > 200. && jdeta_ > 2.0) {
      if ( (channel_ == channel::em) ? (n_bjets_ == 0) : true) SetPassCategory("vbf_loose");
    }

    // Loose VBF Selection with low pT Jet Requirement
    // Used for background shape of QCD in VBF Category
    if (n_lowpt_jets_ >= 2 && n_jetsingap_lowpt_ == 0 && mjj_lowpt_ > 200. && jdeta_lowpt_ > 2.0) {
      if ( (channel_ == channel::em) ? (n_bjets_ == 0) : true) SetPassCategory("vbf_loose_jets20");
    }

    // 1-jet High Category
    // In the et channel, apply a MET > 30 cut
    if (!PassesCategory("vbf") && n_jets_ >= 1 && pt_2_ > pt2_split && n_bjets_ == 0) {
      if ( (channel_ == channel::et) ? (met_ > 30.) : true) SetPassCategory("1jet_high");
    }

    // 1-jet Low Category
    // In the et channel, apply a MET > 30 cut
    if (!PassesCategory("vbf") && n_jets_ >= 1 && pt_2_ <= pt2_split && n_bjets_ == 0) {
      if ( (channel_ == channel::et) ? (met_ > 30.) : true) SetPassCategory("1jet_low");
    }


    // 0-jet High Category
    if (n_jets_ == 0 && pt_2_ > pt2_split && n_bjets_ == 0) SetPassCategory("0jet_high");
    // 0-jet Low Category
    if (n_jets_ == 0 && pt_2_ <= pt2_split && n_bjets_ == 0) SetPassCategory("0jet_low");


    if (n_jets_ <= 1 && n_bjets_ > 0) SetPassCategory("btag");
    if (n_jets_ <= 1 && n_loose_bjets_ > 0) SetPassCategory("btag_loose");
    if (!PassesCategory("vbf") && n_bjets_ == 0) SetPassCategory("nobtag");

    return 0;
  }

  bool HTTCategories::PassesCategory(std::string const& category) const {
    std::map<std::string, bool>::const_iterator it = categories_.find(category);
    if (it != categories_.end()) {
      return it->second;
    } else {
      std::cerr << "Error in HTTCategories::PassesCategory: No category registered with label " << category << std::endl;
      throw;
      return false;
    }
  }


  void HTTCategories::InitMassPlots(std::string const& category) {
    for (std::map<std::string, bool>::const_iterator it = selections_.begin(); it != selections_.end(); ++it) {
      massplots_[category+"_"+it->first] = new MassPlots(fs_->mkdir(category+"_"+it->first));
    }

  }
  void HTTCategories::InitCoreControlPlots(std::string const& category) {
    for (std::map<std::string, bool>::const_iterator it = selections_.begin(); it != selections_.end(); ++it) {
      controlplots_[category+"_"+it->first] = new CoreControlPlots(fs_->mkdir(category+"_"+it->first));
    }
  }

  void HTTCategories::Reset() {
    for (std::map<std::string, bool>::iterator it = selections_.begin(); it != selections_.end(); ++it) {
      it->second = false;
    }
    for (std::map<std::string, bool>::iterator it = categories_.begin(); it != categories_.end(); ++it) {
      it->second = false;
    }
  }

  void HTTCategories::SetPassSelection(std::string const& selection) {
    std::map<std::string, bool>::iterator it = selections_.find(selection);
    if (it != selections_.end()) {
      it->second = true;
    } else {
      std::cerr << "Error in HTTCategories::SetPassSelection: No selection registered with label " << selection << std::endl;
      throw;
    }
  }

  void HTTCategories::SetPassCategory(std::string const& category) {
    std::map<std::string, bool>::iterator it = categories_.find(category);
    if (it != categories_.end()) {
      it->second = true;
      FillMassPlots(category);
      FillYields(category);
    } else {
      std::cerr << "Error in HTTCategories::SetPassCategory: No category registered with label " << category << std::endl;
      throw;
    }
  }

  void HTTCategories::FillMassPlots(std::string const& category) {
    for (std::map<std::string, bool>::iterator it = selections_.begin(); it != selections_.end(); ++it) {
      if (it->second) {
        std::map<std::string, MassPlots*>::iterator p_it = massplots_.find(category+"_"+it->first);
        if (p_it != massplots_.end()) {
          MassPlots *plots = p_it->second;
          plots->m_sv->Fill(m_sv_, wt_);
          plots->m_sv_sm->Fill(m_sv_, wt_);
          plots->m_sv_sm_fine->Fill(m_sv_, wt_);
          plots->m_sv_mssm->Fill(m_sv_, wt_);
          plots->m_sv_mssm_fine->Fill(m_sv_, wt_);
          plots->m_vis->Fill(m_vis_, wt_);
          plots->m_vis_sm->Fill(m_vis_, wt_);
          plots->m_vis_sm_fine->Fill(m_vis_, wt_);
          plots->m_vis_mssm->Fill(m_vis_, wt_);
          plots->m_vis_mssm_fine->Fill(m_vis_, wt_);
        }
      } 
    }
  }

  void HTTCategories::FillYields(std::string const& category) {
    for (std::map<std::string, bool>::iterator it = selections_.begin(); it != selections_.end(); ++it) {
      if (it->second) {
        yields_[category+"_"+it->first] = yields_[category+"_"+it->first] + wt_;
      } 
    }
  }


  void HTTCategories::FillCoreControlPlots(std::string const& category) {
    for (std::map<std::string, bool>::iterator it = selections_.begin(); it != selections_.end(); ++it) {
      if (it->second) {
        std::map<std::string, CoreControlPlots*>::iterator p_it = controlplots_.find(category+"_"+it->first);
        if (p_it != controlplots_.end()) {
          CoreControlPlots *plots = p_it->second;
          plots->n_vtx->Fill(n_vtx_, wt_);
          plots->mt_1->Fill(mt_1_, wt_);
          plots->pzeta->Fill(pzeta_, wt_);
          plots->pt_1->Fill(pt_1_, wt_);
          plots->pt_2->Fill(pt_2_, wt_);
          plots->eta_1->Fill(eta_1_, wt_);
          plots->eta_2->Fill(eta_2_, wt_);
          plots->met->Fill(met_, wt_);
          plots->met_phi->Fill(met_phi_, wt_);
          plots->n_jets->Fill(n_jets_, wt_);
          plots->n_bjets->Fill(n_bjets_, wt_);
          plots->n_jetsingap->Fill(n_jetsingap_, wt_);
          if (tau_decay_mode_ == 0) plots->tau_decay_mode->Fill(0. , wt_);
          if (tau_decay_mode_ == 1 || tau_decay_mode_ == 2) plots->tau_decay_mode->Fill(1., wt_);
          if (tau_decay_mode_ == 10) plots->tau_decay_mode->Fill(2., wt_);
          if (n_jets_ >= 1) {
            plots->jpt_1->Fill(jpt_1_, wt_);
            plots->jeta_1->Fill(jeta_1_, wt_);
          }
          if (n_jets_ >= 2) {
            plots->jpt_2->Fill(jpt_2_, wt_);
            plots->jeta_2->Fill(jeta_2_, wt_);            
            plots->mjj->Fill(mjj_, wt_);
            plots->jdeta->Fill(jdeta_, wt_);
          }
          if (n_bjets_ >= 1) {
            plots->bpt_1->Fill(bpt_1_, wt_);
            plots->beta_1->Fill(beta_1_, wt_);
          }
        }
      } 
    }
  }



  int HTTCategories::PostAnalysis() {
    std::cout << "** Post-Analysis Info for HTT Selection **" << std::endl;
    std::vector<std::string> print_selections;
    print_selections.push_back("os_sel");
    print_selections.push_back("os_con");
    print_selections.push_back("ss_sel");
    print_selections.push_back("ss_con");
    std::vector<std::string> print_cats;
    print_cats.push_back("inclusive");
    print_cats.push_back("vbf_loose");
    print_cats.push_back("vbf");
    print_cats.push_back("vbf_test");
    print_cats.push_back("1jet_high");
    print_cats.push_back("1jet_low");
    print_cats.push_back("0jet_high");
    print_cats.push_back("0jet_low");
    std::cout << boost::format("%-20s") % "Selections:";
    for (unsigned i = 0; i < print_selections.size(); ++i) {
      std::cout << boost::format("%-12s") % print_selections[i];
    }
    std::cout << std::endl;
    for (unsigned i = 0; i < print_cats.size(); ++i) {
      std::cout << boost::format("%-20s") % print_cats[i];
      for (unsigned j = 0; j < print_selections.size(); ++j) {
        std::cout << boost::format("%-12s") % (yields_[print_cats[i]+"_"+print_selections[j]]);
      }
      std::cout << std::endl;
    }
    return 0;
  }

  void HTTCategories::PrintInfo() {
    ;
  }
}
