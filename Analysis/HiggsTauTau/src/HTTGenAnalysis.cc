#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTGenAnalysis.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"

std::vector<ic::GenParticle> FamilyTree (std::vector<ic::GenParticle> &v, ic::GenParticle p, std::vector<ic::GenParticle*> gen_particles){
  if(p.status() == 1){
    unsigned ID = std::fabs(p.pdgid());
    if(!(ID==12 || ID==14 || ID==16)){
      v.push_back(p);
    }
  }
  else{
    for(size_t i = 0; i < p.daughters().size(); ++i ){
      ic::GenParticle d = *gen_particles[p.daughters().at(i)];
      FamilyTree(v,d, gen_particles);
    }
  }
  return v;
}

struct swap_labels{
  bool operator() (std::string a,std::string b) {
    if(a=="t" && b!="t") return false;
    else if(a=="m" &&  (b!="m" && b!="t")) return false;
    else return true;
  }
};

struct PtComparator{
  bool operator() (ic::Candidate a, ic::Candidate b) {
    return (a.vector().Pt() > b.vector().Pt());
  }
};
  
namespace ic {

  HTTGenAnalysis::HTTGenAnalysis(std::string const& name) : ModuleBase(name) {
    fs_ = NULL;
  }

  HTTGenAnalysis::~HTTGenAnalysis() {
    ;
  }

  int HTTGenAnalysis::PreAnalysis() {
    if(fs_){  
      outtree_ = fs_->make<TTree>("gen_ntuple","gen_ntuple");
      outtree_->Branch("event"       , &event_       );
      outtree_->Branch("pt_1"        , &pt_1_        );
      outtree_->Branch("pt_2"        , &pt_2_        );
      outtree_->Branch("eta_1"       , &eta_1_       );
      outtree_->Branch("eta_2"       , &eta_2_       );
      outtree_->Branch("phi_1"       , &phi_1_       );
      outtree_->Branch("phi_2"       , &phi_2_       );
      outtree_->Branch("met"         , &met_         );
      outtree_->Branch("m_vis"       , &m_vis_       );
      outtree_->Branch("pt_tt"       , &pt_tt_       );
      outtree_->Branch("mt_1"        , &mt_1_        );
      outtree_->Branch("mt_2"        , &mt_2_        );
      outtree_->Branch("n_jets"      , &n_jets_      );
      outtree_->Branch("n_jetsingap" , &n_jetsingap_ );
      outtree_->Branch("jpt_1"       , &jpt_1_       );
      outtree_->Branch("jpt_2"       , &jpt_2_       );
      outtree_->Branch("jeta_1"      , &jeta_1_      );
      outtree_->Branch("jeta_2"      , &jeta_2_      );
      outtree_->Branch("jphi_1"      , &jphi_1_      );
      outtree_->Branch("jphi_2"      , &jphi_2_      );
      outtree_->Branch("mjj"         , &mjj_         );
      outtree_->Branch("jdeta"       , &jdeta_       );
    }
    return 0;
  }

  int HTTGenAnalysis::Execute(TreeEvent *event) {
    
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    event_ = (unsigned long long) eventInfo->event();
    
    std::vector<ic::GenParticle*> gen_particles = event->GetPtrVec<ic::GenParticle>("genParticles");
    std::vector<ic::GenJet*> gen_jets = event->GetPtrVec<ic::GenJet>("genJets");

    std::vector<ic::GenParticle> higgs_products;
    ic::Candidate met; 
    std::vector<ic::GenParticle> prompt_leptons;
    std::vector<std::string> decay_types;
    
    for(unsigned i=0; i<gen_particles.size(); ++i){
      
      ic::GenParticle part = *gen_particles[i];
      ic::GenParticle higgs_product;
      
      unsigned genID = std::fabs(part.pdgid());
      
      // add neutrinos 4-vectors to get gen met
      if(genID == 12 || genID == 14 || genID == 16) met.set_vector(met.vector() + part.vector());
      
      // check if particle is a tau, if it is and mother is a Higgs find stable visible daughters
      if(genID != 15) continue;
      unsigned motherID = std::fabs(gen_particles[part.mothers().at(0)]->pdgid());
      if(motherID != 25) continue;

      std::vector<ic::GenParticle> family;
      FamilyTree(family, part, gen_particles);
      
      if(family.size() == 1){
        higgs_products.push_back(family[0]);
        unsigned familyID = std::fabs(family[0].pdgid());
        if     (familyID == 11) decay_types.push_back("e");
        else if(familyID == 13) decay_types.push_back("m");
      } else {
        decay_types.push_back("t");  
        ic::GenParticle had_tau;
        int charge = 0;
        int pdgid = part.pdgid();
        for(unsigned j=0; j<family.size(); ++j){
          had_tau.set_vector(had_tau.vector() + family[j].vector());
          charge += family[j].charge();
        }
          had_tau.set_charge(charge);
          had_tau.set_pdgid(pdgid);
          higgs_products.push_back(had_tau);
      }
    }
    std::sort(higgs_products.begin(),higgs_products.end(),PtComparator());

    std::vector<ic::GenParticle> electrons;
    std::vector<ic::GenParticle> muons;
    std::vector<ic::GenParticle> taus;
    
    double min_tau_pt[2];
    min_tau_pt[0]     = min_tau1_pt_;
    min_tau_pt[1]     = min_tau2_pt_;
    
    for(unsigned i=0; i<higgs_products.size(); ++i){
      unsigned ID = std::fabs(higgs_products[i].pdgid());
      double eta = std::fabs(higgs_products[i].vector().Rapidity());
      double pt = higgs_products[i].vector().Pt();
      if(ID == 11){
        if(pt > min_e_pt_ && eta < max_e_eta_) electrons.push_back(higgs_products[i]);  
      } else if(ID == 13){
        if(pt > min_mu_pt_ && eta < max_mu_eta_) muons.push_back(higgs_products[i]);  
      } else if(ID == 15){
        if(pt > min_tau_pt[i] && eta < max_tau_eta_) taus.push_back(higgs_products[i]);  
      }
    }
    
    //size of decay_types vector should always be 2 but added this if statement just to be sure
    std::string decayType = "";
    if(decay_types.size()==2){
      std::sort(decay_types.begin(),decay_types.end(),swap_labels());
      decayType = decay_types[0]+decay_types[1];
    }
    
    pt_1_ = -9999.;
    pt_2_ = -9999.;
    ic::Candidate lep1;
    ic::Candidate lep2;
    bool passed = false;
    if(channel_str_ == "em"){
      if(electrons.size() == 1 && muons.size() == 1){
        lep1 = electrons[0];
        lep2 = muons[0];
        passed = true;
      }
    } else if(channel_str_ == "et"){
      if(electrons.size() == 1 && taus.size() == 1){
        lep1 = electrons[0];
        lep2 = taus[0];
        passed = true;
      }
    } else if(channel_str_ == "mt"){
      if(muons.size() == 1 && taus.size() == 1){
        lep1 = muons[0];
        lep2 = taus[0];
        passed = true;
      }
    } else if(channel_str_ == "tt"){
      if(taus.size() == 2){
        lep1 = taus[0];
        lep2 = taus[1];
        passed = true;
      }
    }
    if(passed){
      pt_1_  = lep1.vector().Pt();
      pt_2_  = lep2.vector().Pt();
      eta_1_ = lep1.vector().Rapidity();
      eta_2_ = lep2.vector().Rapidity();
      phi_1_ = lep1.vector().Phi();
      phi_2_ = lep2.vector().Phi();
      met_   = met.vector().Pt();
      pt_tt_ = (met.vector()+lep1.vector()+lep2.vector()).Pt();
      m_vis_ = (lep1.vector()+lep2.vector()).M();
      if(pt_1_ >= pt_2_){
        mt_1_ = MT(&lep1, &met);
        mt_2_ = MT(&lep2, &met);
      } else{
        mt_1_ = MT(&lep2, &met);
        mt_2_ = MT(&lep1, &met);  
      }
    }
    
    std::vector<ic::GenJet> filtered_jets;

    for(unsigned i=0; i<gen_jets.size(); ++i){
      ic::GenJet jet = *gen_jets[i];
      double jetPt = jet.vector().Pt();
      double jetEta = std::fabs(jet.vector().Rapidity());
      bool MatchedToPrompt = false;
      for(unsigned j=0; j<higgs_products.size(); ++j){
        if(DRLessThan(std::make_pair(&jet, &higgs_products[j]),0.5)) MatchedToPrompt = true;
      }
      //select jets above minimum pt threshold that aren't matched to Higgs decay products
      if(jetPt > min_jet_pt_ && jetEta < max_jet_eta_ && !MatchedToPrompt) filtered_jets.push_back(jet); 
    }

    n_jets_ = filtered_jets.size();
    jpt_1_       = -9999;
    jeta_1_      = -9999;
    jphi_1_      = -9999;
    jpt_2_       = -9999;
    jeta_2_      = -9999;
    jphi_2_      = -9999;
    mjj_         = -9999;
    jdeta_       = -9999;
    n_jetsingap_ = 9999;
    if(n_jets_ > 0){
      jpt_1_  = filtered_jets[0].vector().Pt();
      jeta_1_ = filtered_jets[0].vector().Rapidity();
      jphi_1_ = filtered_jets[0].vector().Phi();
    }
    if(n_jets_ > 1){
      jpt_2_  = filtered_jets[1].vector().Pt();
      jeta_2_ = filtered_jets[1].vector().Rapidity();
      jphi_2_ = filtered_jets[1].vector().Phi();
      mjj_   = (filtered_jets[0].vector()+filtered_jets[1].vector()).M();
      jdeta_  = std::fabs(filtered_jets[0].vector().Rapidity() - filtered_jets[1].vector().Rapidity());
      double max_jeta = std::max(jeta_1_,jeta_2_);
      double min_jeta = std::min(jeta_1_,jeta_2_);
      n_jetsingap_ = 0;
      for(unsigned i=2; i<n_jets_; ++i){
         double jeta_3 = filtered_jets[i].vector().Rapidity();
         if(jeta_3 > min_jeta && jeta_3 < max_jeta) n_jetsingap_++;    
      }
    }

    if(fs_ && passed) outtree_->Fill();
    
    return 0;
  }
  int HTTGenAnalysis::PostAnalysis() {
    return 0;
  }

  void HTTGenAnalysis::PrintInfo() {
    ;
  }

}
