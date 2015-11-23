#include "HiggsTauTau/interface/HTTElectronEfficiency.h"
#include "Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnPairs.h"
#include "TMath.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/Track.hh"

namespace ic {

HTTElectronEfficiency::HTTElectronEfficiency(std::string const& name) : ModuleBase(name) {

}

HTTElectronEfficiency::~HTTElectronEfficiency() { ; }

int HTTElectronEfficiency::PreAnalysis() {
  if (fs_){
    dir_ = new TFileDirectory(fs_->mkdir(dirname_.c_str()));
    outtree_ = dir_->make<TTree>("tree","tree");
    outtree_->Branch("eta", &eta_);
    outtree_->Branch("pt", &pt_);
    outtree_->Branch("et", &et_);
    outtree_->Branch("jet_rho",&jet_rho_);
//    outtree_->Branch("iso_heep",&iso_heep_);
 //   outtree_->Branch("iso_heep_cluster",&iso_heep_cluster_);
    outtree_->Branch("iso_tk",&iso_tk_);
    outtree_->Branch("iso_ea03",&iso_ea03_);
    outtree_->Branch("iso_db03",&iso_db03_);
    outtree_->Branch("iso_db03allch",&iso_db03allch_);
    outtree_->Branch("iso_db04allch",&iso_db04allch_);
    outtree_->Branch("wt",&wt_);
    outtree_->Branch("electronmvaID",&electronmvaID_);
    outtree_->Branch("electronTrigmvaID",&electronTrigmvaID_);
    outtree_->Branch("gen_match",&gen_match_);
    outtree_->Branch("pass_preselection",&pass_preselection);
    outtree_->Branch("pass_cut_preselection",&pass_cut_preselection);
    outtree_->Branch("allcharged03iso",&allcharged03iso_);
    outtree_->Branch("allcharged04iso",&allcharged04iso_);
    outtree_->Branch("sc_eta",&sc_eta);
  }

  return 0;
}


int HTTElectronEfficiency::Execute(TreeEvent* event) {
 std::vector<Electron *> elecs = event->GetPtrVec<Electron>("sel_electrons");
 EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");

 ic::erase_if_not(elecs,(boost::bind(&Electron::dxy_vertex,_1))<0.045);
 ic::erase_if_not(elecs,(boost::bind(&Electron::dxy_vertex,_1))>-0.045);
 ic::erase_if_not(elecs,(boost::bind(&Electron::dz_vertex,_1))<0.2);
 ic::erase_if_not(elecs,(boost::bind(&Electron::dz_vertex,_1))>-0.2);
 

 if (eventInfo->weight_defined("wt_mc_sign")) wt_ = eventInfo->weight("wt_mc_sign"); else wt_= 1;

    std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>("genParticles");
    std::vector<GenParticle *> prompt_sel_particles;
    std::vector<GenParticle *> tau_decay_sel_particles;
    std::vector<GenParticle *> had_sel_particles;
    for (unsigned i=0; i < particles.size(); ++i){
      std::vector<bool> status_flags = particles[i]->statusFlags();
      if ( ((abs(particles[i]->pdgid()) == 11 && status_flags[IsPrompt])||(abs(particles[i]->pdgid()) == 13 && status_flags[IsPrompt] && particles[i]->status()==1)) && particles[i]->pt() > 8. ){
        prompt_sel_particles.push_back(particles[i]);
      }
      if ( ((abs(particles[i]->pdgid())==11 && status_flags[IsDirectPromptTauDecayProduct]) || (abs(particles[i]->pdgid()) == 13 && status_flags[IsDirectPromptTauDecayProduct] && particles[i]->status()==1)) && particles[i]->pt() > 8.){
       tau_decay_sel_particles.push_back(particles[i]);
      }
      if ( ((abs(particles[i]->pdgid())==11 && status_flags[IsDirectHadronDecayProduct]) || (abs(particles[i]->pdgid()) == 13 && status_flags[IsDirectHadronDecayProduct] && particles[i]->status()==1)) && particles[i]->pt() > 8.){
      had_sel_particles.push_back(particles[i]);
     }
    }

    
    std::vector<GenJet> gen_taus = BuildTauJets(particles, false,true);
    std::vector<GenJet *> gen_taus_ptr;
    for (auto & x : gen_taus) gen_taus_ptr.push_back(&x);
    ic::erase_if(gen_taus_ptr, !boost::bind(MinPtMaxEta, _1, 15.0, 999.));


 for(unsigned i=0; i<elecs.size(); i++){
    std::vector<Candidate *> leading_lepton;
    leading_lepton.push_back(elecs.at(i));
    std::vector<std::pair<Candidate*, GenParticle*> > leading_prompt_match = MatchByDR(leading_lepton, prompt_sel_particles, 0.5, true, true);
    std::vector<std::pair<Candidate*, GenParticle*> > leading_tau_decay_match = MatchByDR(leading_lepton, tau_decay_sel_particles, 0.5, true, true);
    std::vector<std::pair<Candidate*, GenParticle*> > leading_had_match = MatchByDR(leading_lepton, had_sel_particles, 0.5, true, true);
    std::vector<std::pair<Candidate*, GenJet*> > leading_tau_match  = MatchByDR(leading_lepton, gen_taus_ptr, 0.5, true, true);

   mcorigin gen_match_1 = mcorigin::fake;
   int promptsize = leading_prompt_match.size();
   int taudecaysize = leading_tau_decay_match.size();
   int tausize = leading_tau_match.size();
   int hadsize = leading_had_match.size();


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


   if(promptsize==0 && taudecaysize==0&&tausize==0){
    if(hadsize==0){
     gen_match_1 = mcorigin::fake;
    } else {
      if(abs(leading_had_match.at(0).second->pdgid()==11)){
         gen_match_1 = mcorigin::hadE;
      } else gen_match_1 = mcorigin::hadMu;
     }
   }
   if(promptsize!=0 && taudecaysize==0 && tausize ==0){
     if(abs(leading_prompt_match.at(0).second->pdgid())==11){
              gen_match_1 = mcorigin::promptE;
     } else gen_match_1 = mcorigin::promptMu;
   }
   if(promptsize==0 && taudecaysize !=0 &&tausize==0){
     if(abs(leading_tau_decay_match.at(0).second->pdgid())==11){
       gen_match_1 = mcorigin::tauE;
     } else gen_match_1 = mcorigin::tauMu;
   }
   if(promptsize==0 && taudecaysize==0 && tausize!=0){
    gen_match_1 = mcorigin::tauHad;
   }


  gen_match_ = MCOrigin2UInt(gen_match_1);
  eta_ = elecs.at(i)->eta();
  pt_ = elecs.at(i)->pt();
  et_ = TMath::Sqrt(pt_*pt_+(elecs.at(i)->M())*(elecs.at(i)->M()));
  allcharged03iso_ = elecs.at(i)->dr03_pfiso_charged_all();
  allcharged04iso_ = elecs.at(i)->dr04_pfiso_charged_all();
  iso_ea03_ = PF03EAIsolationVal(elecs.at(i),eventInfo);
  iso_db03_ = PF03IsolationVal(elecs.at(i),0.5,0);
  iso_db03allch_ = PF03IsolationVal(elecs.at(i),0.5,1);
  iso_db04allch_ = PF04IsolationVal(elecs.at(i),0.5,1);
/*  iso_heep_ = HEEPIso(elecs.at(i));
  iso_heep_cluster_  =HEEPClusterIso(elecs.at(i));*/
  iso_tk_ = elecs.at(i)->dr03_tk_sum_pt(); 
  jet_rho_ = eventInfo->jet_rho();
  pass_preselection = false;
  pass_cut_preselection = true;
  sc_eta = fabs(elecs.at(i)->sc_eta());
 
  if(sc_eta <= 1.479 && pt_ > 15 && elecs.at(i)->full5x5_sigma_IetaIeta()<0.012&&elecs.at(i)->hadronic_over_em()<0.09 && elecs.at(i)->dr03_tk_sum_pt()/pt_ <0.18&&elecs.at(i)->ecal_pf_cluster_iso()/pt_<0.37&&elecs.at(i)->hcal_pf_cluster_iso()/pt_<0.25&&abs(elecs.at(i)->deta_sc_tk_at_vtx()<0.0095)&&abs(elecs.at(i)->dphi_sc_tk_at_vtx()<0.065)) pass_preselection = true;
  if(sc_eta > 1.479 && pt_ > 15 && elecs.at(i)->full5x5_sigma_IetaIeta()<0.033&&elecs.at(i)->hadronic_over_em()<0.09  && elecs.at(i)->dr03_tk_sum_pt()/pt_ <0.18&&elecs.at(i)->ecal_pf_cluster_iso()/pt_<0.45&&elecs.at(i)->hcal_pf_cluster_iso()/pt_<0.28) pass_preselection = true;
  if (elecs.at(i)->has_matched_conversion()) pass_cut_preselection=false;
  if (elecs.at(i)->gsf_tk_nhits() > 1) pass_cut_preselection=false;


  electronmvaID_ = elecs.at(i)->GetIdIso("mvaNonTrigSpring15");
  electronTrigmvaID_ = elecs.at(i)->GetIdIso("mvaTrigSpring15");

  //if(elecs.at(i)->pt()>10){
  if(fs_){
    outtree_->Fill();
  }
  //}
 }
  return 0;
}


int HTTElectronEfficiency::PostAnalysis() { return 0; }

void HTTElectronEfficiency::PrintInfo() { ; }
}
