#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTFourTauSelector.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include <boost/functional/hash.hpp>
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"

namespace ic {

  HTTFourTauSelector::HTTFourTauSelector(std::string const& name) : ModuleBase(name), channel_(channel::tttt),strategy_(strategy::paper2013),mc_(mc::summer12_53X) {
    pair_label_ = "4tau";
    fs_ = NULL;
    use_most_isolated_ = false;
    use_charge_preference_ = true;
    allowed_tau_modes_ = "";
    metcl_mode_ = 0;
    metuncl_mode_ = 0;
  }

  HTTFourTauSelector::~HTTFourTauSelector() {
    ;
  }

  int HTTFourTauSelector::PreAnalysis() {
    std::string param_fmt = "%-25s %-40s\n";

    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HTTFourTauSelector" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << boost::format(param_fmt) % "pair_label"            % pair_label_;
    std::cout << boost::format(param_fmt) % "use_most_isolated"     % use_most_isolated_;
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

    tau_idiso_name_ = "byDeepTau2017v2p1VSjetraw";

    return 0;
  }

  int HTTFourTauSelector::Execute(TreeEvent *event) {

    std::vector<CompositeCandidate *> & multilepton = event->GetPtrVec<CompositeCandidate>(pair_label_);
    std::vector<CompositeCandidate *> all_multilepton;
    std::vector<CompositeCandidate *> zero_charge;
    std::vector<CompositeCandidate *> non_zero_charge;
    std::vector<CompositeCandidate *> result;

    // Remove duplicate decay products from multilepton
    std::cout << multilepton.size() << std::endl;
    for (unsigned i = 0; i < multilepton.size(); ++i) {
      std::cout << "RE " << i << std::endl;
      if (NoDuplicateParticle(multilepton[i],0.3)) {
        if (TotalZeroChargeFourParticles(multilepton[i])) zero_charge.push_back(multilepton[i]);
        if (TotalNonZeroChargeFourParticles(multilepton[i])) non_zero_charge.push_back(multilepton[i]);
        all_multilepton.push_back(multilepton[i]);
      }
    }
    

    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");

    // initiate variables
    if (all_multilepton.size() == 0) {
      return 1;
    }
    Candidate * lep1 = all_multilepton[0]->GetCandidate("lepton1");
    Candidate * lep2 = all_multilepton[0]->GetCandidate("lepton2");
    Candidate * lep3 = all_multilepton[0]->GetCandidate("lepton3");
    Candidate * lep4 = all_multilepton[0]->GetCandidate("lepton4");
    std::vector<Candidate *> pt_sorted_collection = {lep1, lep2, lep3, lep4};
    std::vector<Candidate *> sort_collection = {lep1, lep2, lep3, lep4};
    std::vector<Candidate *> sort_collection_1 = {lep1, lep2, lep3, lep4};
    std::vector<Candidate *> sort_collection_2 = {lep1, lep2, lep3, lep4};
    if(use_charge_preference_) {    

      // The first pair should have the highest "scalar sum pt"
      std::sort(zero_charge.begin(),zero_charge.end(), SortBySumPt); 
      std::sort(non_zero_charge.begin(),non_zero_charge.end(), SortBySumPt);
      
      // Or alternatively sort by isolation
      if (use_most_isolated_) {
        if(channel_ ==  channel::ettt) { 
           std::sort(zero_charge.begin(), zero_charge.end(), boost::bind(SortByIsoETTT,_1,_2,strategy_,eventInfo,tau_idiso_name_)) ;
           std::sort(non_zero_charge.begin(), non_zero_charge.end(), boost::bind(SortByIsoETTT,_1,_2,strategy_,eventInfo,tau_idiso_name_)) ;
        }
        if(channel_ ==  channel::mttt) { 
           std::sort(zero_charge.begin(), zero_charge.end(), boost::bind(SortByIsoMTTT,_1,_2,strategy_,tau_idiso_name_)) ;
           std::sort(non_zero_charge.begin(), non_zero_charge.end(), boost::bind(SortByIsoMTTT,_1,_2,strategy_,tau_idiso_name_)) ;
        }
        if(channel_ ==  channel::eett) {
           std::sort(zero_charge.begin(), zero_charge.end(), boost::bind(SortByIsoEETT,_1,_2,strategy_,eventInfo,tau_idiso_name_)) ;
           std::sort(non_zero_charge.begin(), non_zero_charge.end(), boost::bind(SortByIsoEETT,_1,_2,strategy_,eventInfo,tau_idiso_name_)) ;
        }
        if(channel_ ==  channel::mmtt) {                            
           std::sort(zero_charge.begin(), zero_charge.end(), boost::bind(SortByIsoMMTT,_1,_2,strategy_,tau_idiso_name_)) ;
           std::sort(non_zero_charge.begin(), non_zero_charge.end(), boost::bind(SortByIsoMMTT,_1,_2,strategy_,tau_idiso_name_)) ;
        }
        if(channel_ ==  channel::emtt) { 
           std::sort(zero_charge.begin(), zero_charge.end(), boost::bind(SortByIsoEMTT,_1,_2,strategy_,eventInfo,tau_idiso_name_)) ;
           std::sort(non_zero_charge.begin(), non_zero_charge.end(), boost::bind(SortByIsoEMTT,_1,_2,strategy_,eventInfo,tau_idiso_name_)) ;
        }
        if(channel_ ==  channel::tttt) { 
           std::sort(zero_charge.begin(), zero_charge.end(), boost::bind(SortByIsoTTTT,_1,_2,tau_idiso_name_)) ;
           std::sort(non_zero_charge.begin(), non_zero_charge.end(), boost::bind(SortByIsoTTTT,_1,_2,tau_idiso_name_)) ;
        }
      }

      if (zero_charge.size() > 0) { // Take zero total charge as preference
        lep1 = zero_charge[0]->GetCandidate("lepton1");
        lep2 = zero_charge[0]->GetCandidate("lepton2");
        lep3 = zero_charge[0]->GetCandidate("lepton3");
        lep4 = zero_charge[0]->GetCandidate("lepton4");
      } else if (non_zero_charge.size() > 0) {
        lep1 = non_zero_charge[0]->GetCandidate("lepton1");
        lep2 = non_zero_charge[0]->GetCandidate("lepton2");
        lep3 = non_zero_charge[0]->GetCandidate("lepton3");
        lep4 = non_zero_charge[0]->GetCandidate("lepton4");
      }
    } else {
        if(channel_ ==  channel::ettt) {
           std::sort(all_multilepton.begin(), all_multilepton.end(), boost::bind(SortByIsoETTT,_1,_2,strategy_,eventInfo,tau_idiso_name_)) ;
        }
        if(channel_ ==  channel::mttt) {
           std::sort(all_multilepton.begin(), all_multilepton.end(), boost::bind(SortByIsoMTTT,_1,_2,strategy_,tau_idiso_name_)) ;
        }
        if(channel_ ==  channel::eett) {
           std::sort(all_multilepton.begin(), all_multilepton.end(), boost::bind(SortByIsoEETT,_1,_2,strategy_,eventInfo,tau_idiso_name_)) ;
        }
        if(channel_ ==  channel::mmtt) {
           std::sort(all_multilepton.begin(), all_multilepton.end(), boost::bind(SortByIsoMMTT,_1,_2,strategy_,tau_idiso_name_)) ;
        }
        if(channel_ ==  channel::emtt) {
           std::sort(all_multilepton.begin(), all_multilepton.end(), boost::bind(SortByIsoEMTT,_1,_2,strategy_,eventInfo,tau_idiso_name_)) ;
        }
        if(channel_ ==  channel::tttt) {
           std::sort(all_multilepton.begin(), all_multilepton.end(), boost::bind(SortByIsoTTTT,_1,_2,tau_idiso_name_)) ;
        }
        if (all_multilepton.size() > 0) { // Take zero total charge as preference
          lep1 = all_multilepton[0]->GetCandidate("lepton1");
          lep2 = all_multilepton[0]->GetCandidate("lepton2");
          lep3 = all_multilepton[0]->GetCandidate("lepton3");
          lep4 = all_multilepton[0]->GetCandidate("lepton4");
        }
      }
    if ((!use_most_isolated_ && all_multilepton.size() > 0) || (use_most_isolated_ && (zero_charge.size() > 0 || non_zero_charge.size() > 0))) {
      if (channel_ == channel::tttt) {
        sort_collection = {lep1, lep2, lep3, lep4};
        std::sort(sort_collection.begin(),sort_collection.end(), SortByPt);
        pt_sorted_collection = sort_collection;
      }
      if (channel_ == channel::mttt || channel_ == channel::ettt) {
        sort_collection = {lep2, lep3, lep4};
        std::sort(sort_collection.begin(),sort_collection.end(), SortByPt);
        pt_sorted_collection = {lep1};
        pt_sorted_collection.insert(pt_sorted_collection.end(), sort_collection.begin(), sort_collection.end());
      }
      if (channel_ == channel::mmtt || channel_ == channel::eett) {
        sort_collection_1 = {lep1, lep2};
        sort_collection_2 = {lep3, lep4};
        std::sort(sort_collection_1.begin(),sort_collection_1.end(), SortByPt);
        std::sort(sort_collection_2.begin(),sort_collection_2.end(), SortByPt);
        pt_sorted_collection = sort_collection_1;
        pt_sorted_collection.insert(pt_sorted_collection.end(), sort_collection_2.begin(), sort_collection_2.end());
      }
      if (channel_ == channel::emtt) {
        sort_collection = {lep3, lep4};
        std::sort(sort_collection.begin(),sort_collection.end(), SortByPt);
        pt_sorted_collection = {lep1, lep2};
        pt_sorted_collection.insert(pt_sorted_collection.end(), sort_collection.begin(), sort_collection.end());
      }

      CompositeCandidate *pt_sorted_pair = new CompositeCandidate();
      pt_sorted_pair->AddCandidate("lepton1",pt_sorted_collection.at(0));
      pt_sorted_pair->AddCandidate("lepton2",pt_sorted_collection[1]);
      pt_sorted_pair->AddCandidate("lepton3",pt_sorted_collection[2]);
      pt_sorted_pair->AddCandidate("lepton4",pt_sorted_collection[3]);
      result.push_back(pt_sorted_pair);
    }
    if (result.size() == 0) return 1;  //Require at least one 4 tau collection

    std::vector<Met*> pfMet_vec = event->GetPtrVec<Met>("pfMetFromSlimmed");
    pfMet_vec = event->GetPtrVec<Met>("puppiMet");
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


    multilepton = result;
    return 0;
  }
  int HTTFourTauSelector::PostAnalysis() {
    return 0;
  }

  void HTTFourTauSelector::PrintInfo() {
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

  bool SortByPt(Candidate const* c1,
                                       Candidate const* c2) {
    return c1->pt() > c2->pt();
  }

  bool SortByIsoETTT(CompositeCandidate const* c1, CompositeCandidate const* c2, ic::strategy strategy, EventInfo const* eventInfo, std::string tau_idiso_name) {
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
    Tau const* t1_1 = static_cast<Tau const*>(c1->At(1));
    Tau const* t2_1 = static_cast<Tau const*>(c2->At(1));
    double t_iso1_1 = t1_1->GetTauID(tau_idiso_name);
    double t_iso2_1 = t2_1->GetTauID(tau_idiso_name);
    if (t_iso1_1 != t_iso2_1) return t_iso1_1 > t_iso2_1;
    if (t1_1->pt() != t2_1->pt()) return (t1_1->pt() > t2_1->pt());

    Tau const* t1_2 = static_cast<Tau const*>(c1->At(2));
    Tau const* t2_2 = static_cast<Tau const*>(c2->At(2));
    double t_iso1_2 = t1_2->GetTauID(tau_idiso_name);
    double t_iso2_2 = t2_2->GetTauID(tau_idiso_name);
    if (t_iso1_2 != t_iso2_2) return t_iso1_2 > t_iso2_2;
    if (t1_2->pt() != t2_2->pt()) return (t1_2->pt() > t2_2->pt());

    Tau const* t1_3 = static_cast<Tau const*>(c1->At(3));
    Tau const* t2_3 = static_cast<Tau const*>(c2->At(3));
    double t_iso1_3 = t1_3->GetTauID(tau_idiso_name);
    double t_iso2_3 = t2_3->GetTauID(tau_idiso_name);
    if (t_iso1_3 != t_iso2_3) return t_iso1_3 > t_iso2_3;
    return (t1_3->pt() > t2_3->pt());
  }

  bool SortByIsoEETT(CompositeCandidate const* c1, CompositeCandidate const* c2, ic::strategy strategy, EventInfo const* eventInfo, std::string tau_idiso_name) {
    // First we sort the electrons
    Electron const* e1_1 = static_cast<Electron const*>(c1->At(0));
    Electron const* e2_1 = static_cast<Electron const*>(c2->At(0));
    double e_iso1_1;
    double e_iso2_1;
    if(strategy==strategy::cpsummer17 || strategy==strategy::cpdecays17 || strategy==strategy::cpdecays18 || strategy==strategy::legacy16){
      e_iso1_1 = PF03EAIsolationVal(e1_1, eventInfo->jet_rho()); //lepton_rho
      e_iso2_1 = PF03EAIsolationVal(e2_1, eventInfo->jet_rho());
    }
    else {
      e_iso1_1 = PF03IsolationVal(e1_1, 0.5, 0);
      e_iso2_1 = PF03IsolationVal(e2_1, 0.5, 0);
    }
    // If the iso is different we just use this
    if (e_iso1_1 != e_iso2_1) return e_iso1_1 < e_iso2_1;
    // If not try the pT
    if (e1_1->pt() != e2_1->pt()) return e1_1->pt() > e2_1->pt();

    Electron const* e1_2 = static_cast<Electron const*>(c1->At(1));
    Electron const* e2_2 = static_cast<Electron const*>(c2->At(1));
    double e_iso1_2;
    double e_iso2_2;
    if(strategy==strategy::cpsummer17 || strategy==strategy::cpdecays17 || strategy==strategy::cpdecays18 || strategy==strategy::legacy16){
      e_iso1_2 = PF03EAIsolationVal(e1_2, eventInfo->jet_rho()); //lepton_rho
      e_iso2_2 = PF03EAIsolationVal(e2_2, eventInfo->jet_rho());
    }
    else {
      e_iso1_2 = PF03IsolationVal(e1_2, 0.5, 0);
      e_iso2_2 = PF03IsolationVal(e2_2, 0.5, 0);
    }
    // If the iso is different we just use this
    if (e_iso1_2 != e_iso2_2) return e_iso1_2 < e_iso2_2;
    // If not try the pT
    if (e1_2->pt() != e2_2->pt()) return e1_2->pt() > e2_2->pt();

    // If both of these are the same then try the taus
    Tau const* t1_1 = static_cast<Tau const*>(c1->At(2));
    Tau const* t2_1 = static_cast<Tau const*>(c2->At(2));
    double t_iso1_1 = t1_1->GetTauID(tau_idiso_name);
    double t_iso2_1 = t2_1->GetTauID(tau_idiso_name);
    if (t_iso1_1 != t_iso2_1) return t_iso1_1 > t_iso2_1;
    if (t1_1->pt() != t2_1->pt()) return (t1_1->pt() > t2_1->pt());

    Tau const* t1_2 = static_cast<Tau const*>(c1->At(3));
    Tau const* t2_2 = static_cast<Tau const*>(c2->At(3));
    double t_iso1_2 = t1_2->GetTauID(tau_idiso_name);
    double t_iso2_2 = t2_2->GetTauID(tau_idiso_name);
    if (t_iso1_2 != t_iso2_2) return t_iso1_2 > t_iso2_2;
    return (t1_2->pt() > t2_2->pt());
  }

  bool SortByIsoMMTT(CompositeCandidate const* c1, CompositeCandidate const* c2, ic::strategy strategy, std::string tau_idiso_name) {
    Muon const* m1_1 = static_cast<Muon const*>(c1->At(0));
    Muon const* m2_1 = static_cast<Muon const*>(c2->At(0));
    double m_iso1_1;
    m_iso1_1 = (strategy == strategy::fall15) ? PF03IsolationVal(m1_1, 0.5, 0) : PF04IsolationVal(m1_1, 0.5, 0);
    double m_iso2_1;
    m_iso2_1 = (strategy == strategy::fall15) ? PF03IsolationVal(m2_1, 0.5, 0) : PF04IsolationVal(m2_1, 0.5, 0);
    if (m_iso1_1 != m_iso2_1) return m_iso1_1 < m_iso2_1;
    if (m1_1->pt() != m2_1->pt()) return m1_1->pt() > m2_1->pt();

    Muon const* m1_2 = static_cast<Muon const*>(c1->At(1));
    Muon const* m2_2 = static_cast<Muon const*>(c2->At(1));
    double m_iso1_2;
    m_iso1_2 = (strategy == strategy::fall15) ? PF03IsolationVal(m1_2, 0.5, 0) : PF04IsolationVal(m1_2, 0.5, 0);
    double m_iso2_2;
    m_iso2_2 = (strategy == strategy::fall15) ? PF03IsolationVal(m2_2, 0.5, 0) : PF04IsolationVal(m2_2, 0.5, 0);
    if (m_iso1_2 != m_iso2_2) return m_iso1_2 < m_iso2_2;
    if (m1_2->pt() != m2_2->pt()) return m1_2->pt() > m2_2->pt();

    // If both of these are the same then try the taus
    Tau const* t1_1 = static_cast<Tau const*>(c1->At(2));
    Tau const* t2_1 = static_cast<Tau const*>(c2->At(2));
    double t_iso1_1 = t1_1->GetTauID(tau_idiso_name);
    double t_iso2_1 = t2_1->GetTauID(tau_idiso_name);
    if (t_iso1_1 != t_iso2_1) return t_iso1_1 > t_iso2_1;
    if (t1_1->pt() != t2_1->pt()) return (t1_1->pt() > t2_1->pt());

    Tau const* t1_2 = static_cast<Tau const*>(c1->At(3));
    Tau const* t2_2 = static_cast<Tau const*>(c2->At(3));
    double t_iso1_2 = t1_2->GetTauID(tau_idiso_name);
    double t_iso2_2 = t2_2->GetTauID(tau_idiso_name);
    if (t_iso1_2 != t_iso2_2) return t_iso1_2 > t_iso2_2;
    return (t1_2->pt() > t2_2->pt());
  }



  bool SortByIsoMTTT(CompositeCandidate const* c1, CompositeCandidate const* c2, ic::strategy strategy, std::string tau_idiso_name) {
    Muon const* m1 = static_cast<Muon const*>(c1->At(0));
    Muon const* m2 = static_cast<Muon const*>(c2->At(0));
    double m_iso1;
    m_iso1 = (strategy == strategy::fall15) ? PF03IsolationVal(m1, 0.5, 0) : PF04IsolationVal(m1, 0.5, 0);
    double m_iso2; 
    m_iso2 = (strategy == strategy::fall15) ? PF03IsolationVal(m2, 0.5, 0) : PF04IsolationVal(m2, 0.5, 0);
    if (m_iso1 != m_iso2) return m_iso1 < m_iso2;
    if (m1->pt() != m2->pt()) return m1->pt() > m2->pt();

    // If both of these are the same then try the taus
    Tau const* t1_1 = static_cast<Tau const*>(c1->At(1));
    Tau const* t2_1 = static_cast<Tau const*>(c2->At(1));
    double t_iso1_1 = t1_1->GetTauID(tau_idiso_name);
    double t_iso2_1 = t2_1->GetTauID(tau_idiso_name);
    if (t_iso1_1 != t_iso2_1) return t_iso1_1 > t_iso2_1;
    if (t1_1->pt() != t2_1->pt()) return (t1_1->pt() > t2_1->pt());

    Tau const* t1_2 = static_cast<Tau const*>(c1->At(2));
    Tau const* t2_2 = static_cast<Tau const*>(c2->At(2));
    double t_iso1_2 = t1_2->GetTauID(tau_idiso_name);
    double t_iso2_2 = t2_2->GetTauID(tau_idiso_name);
    if (t_iso1_2 != t_iso2_2) return t_iso1_2 > t_iso2_2;
    if (t1_2->pt() != t2_2->pt()) return (t1_2->pt() > t2_2->pt());

    Tau const* t1_3 = static_cast<Tau const*>(c1->At(3));
    Tau const* t2_3 = static_cast<Tau const*>(c2->At(3));
    double t_iso1_3 = t1_3->GetTauID(tau_idiso_name);
    double t_iso2_3 = t2_3->GetTauID(tau_idiso_name);
    if (t_iso1_3 != t_iso2_3) return t_iso1_3 > t_iso2_3;
    return (t1_3->pt() > t2_3->pt());
  }

  bool SortByIsoTTTT(CompositeCandidate const* c1, CompositeCandidate const* c2, std::string tau_idiso_name) {
    Tau const* t1_1 = static_cast<Tau const*>(c1->At(0));
    Tau const* t2_1 = static_cast<Tau const*>(c2->At(0));
    double t_iso1_1 = t1_1->GetTauID(tau_idiso_name);
    double t_iso2_1 = t2_1->GetTauID(tau_idiso_name);
    if (t_iso1_1 != t_iso2_1) return t_iso1_1 > t_iso2_1;
    if (t1_1->pt() != t2_1->pt()) return t1_1->pt() > t2_1->pt();
    Tau const* t1_2 = static_cast<Tau const*>(c1->At(1));
    Tau const* t2_2 = static_cast<Tau const*>(c2->At(1));
    double t_iso1_2 = t1_2->GetTauID(tau_idiso_name);
    double t_iso2_2 = t2_2->GetTauID(tau_idiso_name);
    if (t_iso1_2 != t_iso2_2) return t_iso1_2 > t_iso2_2;
    if (t1_2->pt() != t2_2->pt()) return t1_2->pt() > t2_2->pt();

    Tau const* t1_3 = static_cast<Tau const*>(c1->At(2));
    Tau const* t2_3 = static_cast<Tau const*>(c2->At(2));
    double t_iso1_3 = t1_3->GetTauID(tau_idiso_name);
    double t_iso2_3 = t2_3->GetTauID(tau_idiso_name);
    if (t_iso1_3 != t_iso2_3) return t_iso1_3 > t_iso2_3;
    if (t1_3->pt() != t2_3->pt()) return t1_3->pt() > t2_3->pt();

    Tau const* t1_4 = static_cast<Tau const*>(c1->At(3));
    Tau const* t2_4 = static_cast<Tau const*>(c2->At(3));
    double t_iso1_4 = t1_4->GetTauID(tau_idiso_name);
    double t_iso2_4 = t2_4->GetTauID(tau_idiso_name);
    if (t_iso1_4 != t_iso2_4) return t_iso1_4 > t_iso2_4;
    return t1_4->pt() > t2_4->pt();
  }

  bool SortByIsoEMTT(CompositeCandidate const* c1, CompositeCandidate const* c2, ic::strategy strategy, EventInfo const* eventInfo, std::string tau_idiso_name) {
    // First we sort the muons
    Muon const* m1 = static_cast<Muon const*>(c1->At(1));
    Muon const* m2 = static_cast<Muon const*>(c2->At(1));
    double m_iso1; 
    m_iso1 = (strategy == strategy::fall15) ? PF03IsolationVal(m1, 0.5, 0) : PF04IsolationVal(m1, 0.5, 0);
    double m_iso2;
    m_iso2 = (strategy == strategy::fall15) ? PF03IsolationVal(m2, 0.5, 0) : PF04IsolationVal(m2, 0.5, 0);
    if (m_iso1 != m_iso2) return m_iso1 < m_iso2;
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
    if (e1->pt() != e2->pt()) return e1->pt() > e2->pt();

    Tau const* t1_1 = static_cast<Tau const*>(c1->At(2));
    Tau const* t2_1 = static_cast<Tau const*>(c2->At(2));
    double t_iso1_1 = t1_1->GetTauID(tau_idiso_name);
    double t_iso2_1 = t2_1->GetTauID(tau_idiso_name);
    if (t_iso1_1 != t_iso2_1) return t_iso1_1 > t_iso2_1;
    if (t1_1->pt() != t2_1->pt()) return t1_1->pt() > t2_1->pt();

    Tau const* t1_2 = static_cast<Tau const*>(c1->At(3));
    Tau const* t2_2 = static_cast<Tau const*>(c2->At(3));
    double t_iso1_2 = t1_2->GetTauID(tau_idiso_name);
    double t_iso2_2 = t2_2->GetTauID(tau_idiso_name);
    if (t_iso1_2 != t_iso2_2) return t_iso1_2 > t_iso2_2;
    return t1_2->pt() > t2_2->pt();
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

  void HTTFourTauSelector::CorrectMETForShift(ic::Met * met, ROOT::Math::PxPyPzEVector const& shift) {
    double metx = met->vector().px() - shift.px();
    double mety = met->vector().py() - shift.py();
    double metet = sqrt(metx*metx + mety*mety);
    ROOT::Math::PxPyPzEVector new_met(metx, mety, 0, metet);
    met->set_vector(ROOT::Math::PtEtaPhiEVector(new_met));
  }

}
