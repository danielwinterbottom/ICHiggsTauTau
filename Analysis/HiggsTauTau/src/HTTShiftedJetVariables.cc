#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTShiftedJetVariables.h"

  
namespace ic {

  HTTShiftedJetVariables::HTTShiftedJetVariables(std::string const& name) : ModuleBase(name), 
    strategy_(strategy::smsummer16){
    jets_label_ = "ak4PFJetsCHSTotal";
    source_= "Total";
  }

  HTTShiftedJetVariables::~HTTShiftedJetVariables() {
    ;
  }

  int HTTShiftedJetVariables::PreAnalysis() {
    return 0;
  }

  int HTTShiftedJetVariables::Execute(TreeEvent *event) {

    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
    
    std::sort(jets.begin(), jets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    std::vector<PFJet*> bjets = jets;
    std::vector<PFJet*> lowpt_jets = jets;
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
    ic::erase_if(lowpt_jets,!boost::bind(MinPtMaxEta, _1, 20.0, 4.7));
    ic::erase_if(bjets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
    
    // filter b-jets not passing b-taggers
    std::string btag_label="combinedSecondaryVertexBJetTags";
    double btag_wp =  0.679;
    if(strategy_ == strategy::phys14) btag_label = "combinedInclusiveSecondaryVertexV2BJetTags";
    if(strategy_ == strategy::phys14) btag_wp = 0.814 ;
    if(strategy_ == strategy::spring15) btag_label = "pfCombinedInclusiveSecondaryVertexV2BJetTags";
    if(strategy_ == strategy::spring15) btag_wp = 0.89 ;
    if(strategy_ == strategy::fall15 || strategy_ == strategy::mssmspring16 ||strategy_ == strategy::smspring16  || strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16) btag_label = "pfCombinedInclusiveSecondaryVertexV2BJetTags";
    if(strategy_ == strategy::fall15 || strategy_ == strategy::mssmspring16 ||strategy_ ==strategy::smspring16) btag_wp = 0.8;
    if(strategy_ == strategy::mssmsummer16 || strategy_ == strategy::smsummer16) btag_wp = 0.8484;

    if (event->Exists("retag_result"+source_)) {
      auto const& retag_result = event->Get<std::map<std::size_t,bool>>("retag_result"+source_); 
      ic::erase_if(bjets, !boost::bind(IsReBTagged, _1, retag_result));
    } else{ 
      ic::erase_if(bjets, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label) < btag_wp);
    } 
    
    // compute all desired variables
    double n_jets = jets.size();
    double n_bjets = bjets.size();
    double n_lowpt_jets = lowpt_jets.size();
    
    double mjj = -9999;
    double sjdphi = -9999;
    
    if(n_lowpt_jets>1){
      if(lowpt_jets[0]->eta() > lowpt_jets[1]->eta()) sjdphi =  ROOT::Math::VectorUtil::DeltaPhi(lowpt_jets[0]->vector(), lowpt_jets[1]->vector());
      else sjdphi =  ROOT::Math::VectorUtil::DeltaPhi(lowpt_jets[1]->vector(), lowpt_jets[0]->vector());
      mjj = (lowpt_jets[0]->vector() + lowpt_jets[1]->vector()).M();
    }

    event->Add("n_jets_"+ std::to_string(JES2UInt(source_)), n_jets);
    event->Add("n_bjets_"+ std::to_string(JES2UInt(source_)), n_bjets);
    event->Add("mjj_"+ std::to_string(JES2UInt(source_)), mjj);
    event->Add("sjdphi_"+ std::to_string(JES2UInt(source_)), sjdphi);
    
    // remove all of the deep copied jet collection from the event once it is no longer needed otherwise get problems due to using too much memory
    jets = event->GetPtrVec<PFJet>(jets_label_);
    for (unsigned i=0; i<jets.size(); ++i) delete jets.at(i);
    jets.clear();
    event->Remove(jets_label_);
    
    return 0;
  }
  int HTTShiftedJetVariables::PostAnalysis() {
    return 0;
  }

  void HTTShiftedJetVariables::PrintInfo() {
    ;
  }

}
