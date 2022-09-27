#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTThreeTauSelector.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include <boost/functional/hash.hpp>
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"

namespace ic {

  HTTThreeTauSelector::HTTThreeTauSelector(std::string const& name) : ModuleBase(name), channel_(channel::ttt),strategy_(strategy::paper2013),mc_(mc::summer12_53X) {
    pair_label_ = "4tau";
    fs_ = NULL;
    use_most_isolated_ = false;
    use_charge_preference_ = true;
    allowed_tau_modes_ = "";
    metcl_mode_ = 0;
    metuncl_mode_ = 0;
  }

  HTTThreeTauSelector::~HTTThreeTauSelector() {
    ;
  }

  int HTTThreeTauSelector::PreAnalysis() {
    std::string param_fmt = "%-25s %-40s\n";

    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HTTThreeTauSelector" << std::endl;
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

  int HTTThreeTauSelector::Execute(TreeEvent *event) {

    std::vector<CompositeCandidate *> & multilepton = event->GetPtrVec<CompositeCandidate>(pair_label_);
    std::vector<CompositeCandidate *> all_multilepton;
    std::vector<CompositeCandidate *> zero_charge;
    std::vector<CompositeCandidate *> non_zero_charge;
    std::vector<CompositeCandidate *> result;
    //std::cout << multilepton.size() << std::endl;
    // Remove duplicate decay products from multilepton
    for (unsigned i = 0; i < multilepton.size(); ++i) {
      if (NoDuplicateParticle_(multilepton[i],0.3)) {
        if (TotalZeroChargeThreeParticles(multilepton[i])) zero_charge.push_back(multilepton[i]);
        if (TotalNonZeroChargeThreeParticles(multilepton[i])) non_zero_charge.push_back(multilepton[i]);
		  
        all_multilepton.push_back(multilepton[i]);
      }
    }
    


    // initiate variables
    if (all_multilepton.size() == 0) {
      return 1;
    }
    Candidate * lep1 = all_multilepton[0]->GetCandidate("lepton1");
    Candidate * lep2 = all_multilepton[0]->GetCandidate("lepton2");
    Candidate * lep3 = all_multilepton[0]->GetCandidate("lepton3");

    std::vector<Candidate *> pt_sorted_collection = {lep1, lep2, lep3};
    std::vector<Candidate *> sort_collection = {lep1, lep2, lep3};
    std::vector<Candidate *> sort_collection_1 = {lep1, lep2, lep3};
    std::vector<Candidate *> sort_collection_2 = {lep1, lep2, lep3};
    if(use_charge_preference_) {    

      // The first pair should have the highest "scalar sum pt"
      std::sort(zero_charge.begin(),zero_charge.end(), SortBySumPt_); 
      std::sort(non_zero_charge.begin(),non_zero_charge.end(), SortBySumPt_);
      
      // Or alternatively sort by isolation
      if (use_most_isolated_) {
        if(channel_ ==  channel::ttt) { 
           std::sort(zero_charge.begin(), zero_charge.end(), boost::bind(SortByIsoTTT,_1,_2,tau_idiso_name_)) ;
           std::sort(non_zero_charge.begin(), non_zero_charge.end(), boost::bind(SortByIsoTTT,_1,_2,tau_idiso_name_)) ;
        }
      }

      if (zero_charge.size() > 0) { // Take zero total charge as preference
        lep1 = zero_charge[0]->GetCandidate("lepton1");
        lep2 = zero_charge[0]->GetCandidate("lepton2");
        lep3 = zero_charge[0]->GetCandidate("lepton3");
      } else if (non_zero_charge.size() > 0) {
        lep1 = non_zero_charge[0]->GetCandidate("lepton1");
        lep2 = non_zero_charge[0]->GetCandidate("lepton2");
        lep3 = non_zero_charge[0]->GetCandidate("lepton3");
      }
    } else {
        if(channel_ ==  channel::ttt) {
           std::sort(all_multilepton.begin(), all_multilepton.end(), boost::bind(SortByIsoTTT,_1,_2,tau_idiso_name_)) ;
        }
        if (all_multilepton.size() > 0) { // Take zero total charge as preference
          lep1 = all_multilepton[0]->GetCandidate("lepton1");
          lep2 = all_multilepton[0]->GetCandidate("lepton2");
          lep3 = all_multilepton[0]->GetCandidate("lepton3");
        }
      }
    if ((!use_most_isolated_ && all_multilepton.size() > 0) || (use_most_isolated_ && (zero_charge.size() > 0 || non_zero_charge.size() > 0))) {
      if (channel_ == channel::ttt) {
        sort_collection = {lep1, lep2, lep3};
        std::sort(sort_collection.begin(),sort_collection.end(), SortByPt_);
        pt_sorted_collection = sort_collection;
      }

      CompositeCandidate *pt_sorted_pair = new CompositeCandidate();
      pt_sorted_pair->AddCandidate("lepton1",pt_sorted_collection.at(0));
      pt_sorted_pair->AddCandidate("lepton2",pt_sorted_collection[1]);
      pt_sorted_pair->AddCandidate("lepton3",pt_sorted_collection[2]);
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
  int HTTThreeTauSelector::PostAnalysis() {
    return 0;
  }

  void HTTThreeTauSelector::PrintInfo() {
    ;
  }

  // Sorting
  // ----------------------------------------------------------------
  bool SortBySumPt_(CompositeCandidate const* c1,
                                       CompositeCandidate const* c2) {
  //  if (c1->charge() == 0 && c2->charge() != 0) return true;
   // if (c1->charge() != 0 && c2->charge() == 0) return false;
    return ScalarPtSum(c1->AsVector()) > ScalarPtSum(c2->AsVector());
  }

  bool SortByPt_(Candidate const* c1,
                                       Candidate const* c2) {
    return c1->pt() > c2->pt();
  }

  

  bool SortByIsoTTT(CompositeCandidate const* c1, CompositeCandidate const* c2, std::string tau_idiso_name) {
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
    return t1_3->pt() > t2_3->pt();
  }

  void HTTThreeTauSelector::CorrectMETForShift(ic::Met * met, ROOT::Math::PxPyPzEVector const& shift) {
    double metx = met->vector().px() - shift.px();
    double mety = met->vector().py() - shift.py();
    double metet = sqrt(metx*metx + mety*mety);
    ROOT::Math::PxPyPzEVector new_met(metx, mety, 0, metet);
    met->set_vector(ROOT::Math::PtEtaPhiEVector(new_met));
  }

}
