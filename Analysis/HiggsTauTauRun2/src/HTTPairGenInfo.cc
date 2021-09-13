#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTPairGenInfo.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include <boost/functional/hash.hpp>
#include "boost/algorithm/string.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTConfig.h"
#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"



namespace ic {

TVector3 GenIP (ic::GenParticle *h, ic::GenParticle *t) {

  TVector3 pvtosv(
           t->vtx().vx() - h->vtx().vx(),
           t->vtx().vy() - h->vtx().vy(),
           t->vtx().vz() - h->vtx().vz()
           );

  TVector3 momenta(
                  t->vector().Px(),
                  t->vector().Py(),
                  t->vector().Pz()
                  );

  double projection=pvtosv*momenta/momenta.Mag2();
  TVector3 gen_ip=pvtosv-momenta*projection;

  return gen_ip;
}

  HTTPairGenInfo::HTTPairGenInfo(std::string const& name) : ModuleBase(name), channel_(channel::mt) {
    ditau_label_ = "ditau";
    fs_ = NULL;
    write_plots_ = false;
    hists_.resize(1);
    ngenjets_=false;
  }

  HTTPairGenInfo::~HTTPairGenInfo() {
    ;
  }

  int HTTPairGenInfo::PreAnalysis() {
  if (fs_ && write_plots_) {
      hists_[0] = new Dynamic2DHistoSet(fs_->mkdir("httpairgeninfo"));
      for (unsigned i = 0; i < hists_.size(); ++i) {
        hists_[i]->Create("relpt_vs_drlep_lead", 50, 0, 10, 20, 0, 0.5);
        hists_[i]->Create("relpt_vs_drtau_lead", 50, 0, 10, 20, 0, 0.5);
        hists_[i]->Create("relpt_vs_drlep_sublead", 50, 0, 10, 20, 0, 0.5);
        hists_[i]->Create("relpt_vs_drtau_sublead", 50, 0, 10, 20, 0, 0.5);
      }
    }

    return 0;
  }

  int HTTPairGenInfo::Execute(TreeEvent *event) {


    std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>(ditau_label_);
    CompositeCandidate const* ditau = ditau_vec.at(0);

    std::vector<Candidate *> leading_lepton;
    std::vector<Candidate *> subleading_lepton;
    leading_lepton.push_back(ditau->GetCandidate("lepton1"));
    subleading_lepton.push_back(ditau->GetCandidate("lepton2")); 

    std::vector<GenParticle *> const& particles = event->GetPtrVec<GenParticle>("genParticles");
    std::vector<GenParticle *> sel_particles;
    std::vector<GenParticle *> sel_particles_noptcut;
    std::vector<GenParticle *> undecayed_taus;
    
    double gen_match_undecayed_1_pt = -1;
    double gen_match_undecayed_2_pt = -1;
    double gen_match_undecayed_1_eta = -1;
    double gen_match_undecayed_2_eta = -1;
    double gen_met=0.;

    TVector3 gen_ip_1(0.,0.,0.);
    TVector3 gen_ip_2(0.,0.,0.);
    bool foundboson=false;
    GenParticle *h = new GenParticle();

    ROOT::Math::PtEtaPhiEVector neutrinos; 
    ROOT::Math::PtEtaPhiEVector gen_neutral_1; 
    ROOT::Math::PtEtaPhiEVector gen_neutral_2; 
 
    for (unsigned i=0; i < particles.size(); ++i){
      std::vector<bool> status_flags_start = particles[i]->statusFlags();
      if ( ((abs(particles[i]->pdgid()) == 11 )||(abs(particles[i]->pdgid()) == 13 /*&& particles[i]->status()==1*/)) && particles[i]->pt() > 8. && (status_flags_start[IsPrompt] || status_flags_start[IsDirectPromptTauDecayProduct] /*|| status_flags_start[IsDirectHadronDecayProduct]*/)){
        sel_particles.push_back(particles[i]);
      }
      if ( ((abs(particles[i]->pdgid()) == 11 )||(abs(particles[i]->pdgid()) == 13)) && (status_flags_start[IsPrompt] || status_flags_start[IsDirectPromptTauDecayProduct])){
        sel_particles_noptcut.push_back(particles[i]);
      }
      if ( ((abs(particles[i]->pdgid()) == 12 )||(abs(particles[i]->pdgid()) == 14 /*&& particles[i]->status()==1*/)||(abs(particles[i]->pdgid()) == 16)) && particles[i]->pt() > 8. && (status_flags_start[IsPrompt] || status_flags_start[IsDirectPromptTauDecayProduct] /*|| status_flags_start[IsDirectHadronDecayProduct]*/)) neutrinos+=particles[i]->vector();
      if(channel_!=channel::zmm&&status_flags_start[IsPrompt] && status_flags_start[IsLastCopy] && abs(particles[i]->pdgid()) == 15) undecayed_taus.push_back(particles[i]);
      if(channel_==channel::zmm&&status_flags_start[IsPrompt] && status_flags_start[IsLastCopy] && abs(particles[i]->pdgid()) == 13) undecayed_taus.push_back(particles[i]);

      if(status_flags_start[IsLastCopy] && (abs(particles[i]->pdgid()) == 23 || abs(particles[i]->pdgid()) == 24 || abs(particles[i]->pdgid()) == 25 || abs(particles[i]->pdgid()) == 35 || abs(particles[i]->pdgid()) == 6) ) {h = particles[i]; foundboson=true; }

    }

    gen_met=neutrinos.Pt();

    event->Add("gen_met",gen_met);
    Met const* mets = NULL;
    mets = event->GetPtr<Met>("pfMET");
    double fake_met = (mets->vector() - neutrinos).Pt();
    event->Add("fake_met",fake_met);

    if(undecayed_taus.size()>0){
      gen_match_undecayed_1_pt = undecayed_taus[0]->pt();
      gen_match_undecayed_1_eta = undecayed_taus[0]->eta();
    }
    if(undecayed_taus.size()>1){
      gen_match_undecayed_2_pt = undecayed_taus[1]->pt();
      gen_match_undecayed_2_eta = undecayed_taus[1]->eta();
    }

    int tauFlag1 = 0;
    int tauFlag2 = 0;
    
    std::vector<GenJet> gen_taus = BuildTauJets(particles, false,true);
    std::vector<GenJet *> gen_taus_ptr;
    std::vector<GenJet *> gen_taus_ptr_noptcut;
    for (auto & x : gen_taus) gen_taus_ptr.push_back(&x);
    for (auto & x : gen_taus) gen_taus_ptr_noptcut.push_back(&x);
    ic::erase_if(gen_taus_ptr, !boost::bind(MinPtMaxEta, _1, 15.0, 999.));
 
 
    std::vector<std::pair<Candidate*, GenParticle*> > leading_lepton_match = MatchByDR(leading_lepton, sel_particles, 0.2, true, true);
    std::vector<std::pair<Candidate*, GenJet*> > leading_tau_match  = MatchByDR(leading_lepton, gen_taus_ptr, 0.2, true, true);

    std::vector<std::pair<Candidate*, GenParticle*> > subleading_lepton_match = MatchByDR(subleading_lepton, sel_particles, 0.2, true, true);
    std::vector<std::pair<Candidate*, GenJet*> > subleading_tau_match  = MatchByDR(subleading_lepton, gen_taus_ptr, 0.2, true, true);


   double gen_nu_p_1=-9999.;
   double gen_nu_p_2=-9999.;
   double gen_nu_phi_1=-9999.;
   double gen_nu_phi_2=-9999.;
   double gen_nu_eta_1=-9999.;
   double gen_nu_eta_2=-9999.;    

   double gen_vis_p_1=-9999.;
   double gen_vis_p_2=-9999.;
   double gen_vis_phi_1=-9999.;
   double gen_vis_phi_2=-9999.;
   double gen_vis_eta_1=-9999.;
   double gen_vis_eta_2=-9999.;
   double gen_vis_E_1=-9999.;
   double gen_vis_E_2=-9999.;

   double gen_neutral_p_1=-9999.;
   double gen_neutral_p_2=-9999.;
   double gen_neutral_phi_1=-9999.;
   double gen_neutral_phi_2=-9999.;
   double gen_neutral_eta_1=-9999.;
   double gen_neutral_eta_2=-9999.;
   double gen_neutral_E_1=-9999.;
   double gen_neutral_E_2=-9999.;

   mcorigin gen_match_1 = mcorigin::fake;
   mcorigin gen_match_2 = mcorigin::fake;
   int leptonsize = leading_lepton_match.size();
   int tausize = leading_tau_match.size();
   double gen_match_1_pt = -1;
   double gen_match_2_pt = -1;

   if(leptonsize!=0&&tausize!=0){
     DR(leading_lepton_match.at(0).first,leading_lepton_match.at(0).second) < DR(leading_tau_match.at(0).first,leading_tau_match.at(0).second) ? tausize=0 : leptonsize = 0;
   }

   // get gen IP and neutral component for candidate 1
   if ((leptonsize==0&&tausize==0) && !foundboson) {
     gen_ip_1.SetXYZ(0.,0.,0.);
   } else if(leptonsize!=0  && foundboson) {
     GenParticle *t = leading_lepton_match.at(0).second;
     gen_ip_1 = GenIP(h,t);
   } else if (tausize!=0 && foundboson) {
     std::vector<std::size_t> daughter_ids = leading_tau_match.at(0).second->constituents();
     bool found=false;
     for (auto id: daughter_ids) {
       for(auto p: particles) {
         if(p->id() == id && p->charge()!=0 && !found) {
           gen_ip_1 = GenIP(h,p);
           found=true;
           break;
         }
         if(p->id() == id && p->charge()==0 && fabs(p->pdgid())==22) {
           gen_neutral_1 += p->vector();
           break;
         }
       }
     } 
   }


   if(leptonsize==0&&tausize==0) gen_match_1 = mcorigin::fake;
   if(leptonsize!=0) {
      std::vector<bool> status_flags = leading_lepton_match.at(0).second->statusFlags();
      gen_match_1_pt = leading_lepton_match.at(0).second->pt();
//      leading_lepton_match_DR = DR(leading_lepton_match.at(0).first,leading_lepton_match.at(0).second);
      if(fs_ && write_plots_){ 
       hists_[0]->Fill("relpt_vs_drlep_lead",fabs(leading_lepton_match.at(0).second->pt()-leading_lepton_match.at(0).first->pt())/leading_lepton_match.at(0).first->pt(),DR(leading_lepton_match.at(0).first,leading_lepton_match.at(0).second));
      }
      if(status_flags[IsPrompt]){
        if(abs(leading_lepton_match.at(0).second->pdgid())==11){
          gen_match_1 = mcorigin::promptE;
         } else gen_match_1 = mcorigin::promptMu;
       }
       if(status_flags[IsDirectPromptTauDecayProduct]){
        if(abs(leading_lepton_match.at(0).second->pdgid())==11){
          gen_match_1 = mcorigin::tauE;
         } else gen_match_1 = mcorigin::tauMu;
        }
       if(status_flags[IsDirectHadronDecayProduct]){
         if(abs(leading_lepton_match.at(0).second->pdgid())==11){
           gen_match_1 = mcorigin::hadE;
          } else gen_match_1 = mcorigin::hadMu;
        } 
      }
     if(tausize!=0){
       if(fs_ && write_plots_){
       hists_[0]->Fill("relpt_vs_drtau_lead",fabs(leading_tau_match.at(0).second->pt()-leading_tau_match.at(0).first->pt())/leading_tau_match.at(0).first->pt(),DR(leading_tau_match.at(0).first,leading_tau_match.at(0).second));
       }
       gen_match_1_pt = leading_tau_match.at(0).second->pt();
  //     leading_lepton_match_DR = DR(leading_tau_match.at(0).first,leading_tau_match.at(0).second);
       gen_match_1 = mcorigin::tauHad;
       tauFlag1 = leading_tau_match.at(0).second->flavour();
       gen_nu_p_1=leading_tau_match.at(0).second->nu_vector().P();
       gen_nu_phi_1=leading_tau_match.at(0).second->nu_vector().Phi();
       gen_nu_eta_1=leading_tau_match.at(0).second->nu_vector().Rapidity();

       gen_vis_p_1=leading_tau_match.at(0).second->vector().P();
       gen_vis_E_1=leading_tau_match.at(0).second->vector().E();
       gen_vis_phi_1=leading_tau_match.at(0).second->vector().Phi();
       gen_vis_eta_1=leading_tau_match.at(0).second->vector().Rapidity();

       gen_neutral_p_1=gen_neutral_1.P();
       gen_neutral_E_1=gen_neutral_1.E();
       gen_neutral_phi_1=gen_neutral_1.Phi();
       gen_neutral_eta_1=gen_neutral_1.Rapidity();
      }
   
//Now for subleading lepton:
   leptonsize = subleading_lepton_match.size();
   tausize = subleading_tau_match.size();

   if(leptonsize!=0&&tausize!=0){
     DR(subleading_lepton_match.at(0).first,subleading_lepton_match.at(0).second) < DR(subleading_tau_match.at(0).first,subleading_tau_match.at(0).second) ? tausize=0 : leptonsize = 0;
   }

   // get gen IP for candidate 2
   if ((leptonsize==0&&tausize==0) && !foundboson) {
     gen_ip_2.SetXYZ(0.,0.,0.);
   } else if(leptonsize!=0 && foundboson) {
     GenParticle *t = subleading_lepton_match.at(0).second;
     gen_ip_2 = GenIP(h,t);
   } else if (tausize!=0 && foundboson) {
     std::vector<std::size_t> daughter_ids = subleading_tau_match.at(0).second->constituents();
     bool found=false;
     for (auto id: daughter_ids) {
       for(auto p: particles) {
         if(p->id() == id && p->charge()!=0 && !found) {
           gen_ip_2 = GenIP(h,p);
           found=true;
           break;
         }
         if(p->id() == id && p->charge()==0 && fabs(p->pdgid())==22) {
           gen_neutral_2 += p->vector();
           break;
         }
       }
     }
   }


   if(leptonsize==0&&tausize==0) gen_match_2 = mcorigin::fake;
   if(leptonsize!=0) {
      std::vector<bool> status_flags = subleading_lepton_match.at(0).second->statusFlags();
      gen_match_2_pt = subleading_lepton_match.at(0).second->pt();
    //  subleading_lepton_match_DR = DR(subleading_lepton_match.at(0).first,subleading_lepton_match.at(0).second);
      if(fs_ && write_plots_){
       hists_[0]->Fill("relpt_vs_drlep_sublead",fabs(subleading_lepton_match.at(0).second->pt()-subleading_lepton_match.at(0).first->pt())/subleading_lepton_match.at(0).first->pt(),DR(subleading_lepton_match.at(0).first,subleading_lepton_match.at(0).second));
      }
      if(status_flags[IsPrompt]){
        if(abs(subleading_lepton_match.at(0).second->pdgid())==11){
          gen_match_2 = mcorigin::promptE;
         } else gen_match_2 = mcorigin::promptMu;
       }
       if(status_flags[IsDirectPromptTauDecayProduct]){
        if(abs(subleading_lepton_match.at(0).second->pdgid())==11){
          gen_match_2 = mcorigin::tauE;
         } else gen_match_2 = mcorigin::tauMu;
        }
       if(status_flags[IsDirectHadronDecayProduct]){
         if(abs(subleading_lepton_match.at(0).second->pdgid())==11){
           gen_match_2 = mcorigin::hadE;
          } else gen_match_2 = mcorigin::hadMu;
        } 
      }
     if(tausize!=0){
       if(fs_&&write_plots_){
       hists_[0]->Fill("relpt_vs_drtau_sublead",fabs(subleading_tau_match.at(0).second->pt()-subleading_tau_match.at(0).first->pt())/subleading_tau_match.at(0).first->pt(),DR(subleading_tau_match.at(0).first,subleading_tau_match.at(0).second));
       }
       gen_match_2_pt = subleading_tau_match.at(0).second->pt();
      // subleading_lepton_match_DR = DR(subleading_tau_match.at(0).first,subleading_tau_match.at(0).second);
       gen_match_2 = mcorigin::tauHad;
       tauFlag2 = subleading_tau_match.at(0).second->flavour(); 
       gen_nu_p_2=subleading_tau_match.at(0).second->nu_vector().P();
       gen_nu_phi_2=subleading_tau_match.at(0).second->nu_vector().Phi();
       gen_nu_eta_2=subleading_tau_match.at(0).second->nu_vector().Rapidity();

       gen_vis_p_2=subleading_tau_match.at(0).second->vector().P();
       gen_vis_E_2=subleading_tau_match.at(0).second->vector().E();
       gen_vis_phi_2=subleading_tau_match.at(0).second->vector().Phi();
       gen_vis_eta_2=subleading_tau_match.at(0).second->vector().Rapidity();

       gen_neutral_p_2=gen_neutral_2.P();
       gen_neutral_E_2=gen_neutral_2.E();
       gen_neutral_phi_2=gen_neutral_2.Phi();
       gen_neutral_eta_2=gen_neutral_2.Rapidity();
      }

   if(gen_match_1 == mcorigin::tauHad) event->Add("leading_gen_tau", new ic::GenJet(*(leading_tau_match.at(0).second)));
   if(gen_match_2 == mcorigin::tauHad) event->Add("subleading_gen_tau", new ic::GenJet(*(subleading_tau_match.at(0).second)));


   event->Add("gen_match_1",gen_match_1);
   event->Add("gen_match_2",gen_match_2);
   event->Add("gen_match_1_pt", gen_match_1_pt);
   event->Add("gen_match_2_pt", gen_match_2_pt);
   event->Add("gen_match_undecayed_1_pt", gen_match_undecayed_1_pt);
   event->Add("gen_match_undecayed_2_pt", gen_match_undecayed_2_pt);
   event->Add("gen_match_undecayed_1_eta", gen_match_undecayed_1_eta);
   event->Add("gen_match_undecayed_2_eta", gen_match_undecayed_2_eta);
   event->Add("tauFlag1", tauFlag1);
   event->Add("tauFlag2", tauFlag2);

   event->Add("gen_ip_1", gen_ip_1);

   event->Add("gen_ip_2", gen_ip_2);

   event->Add("gen_nu_p_1",gen_nu_p_1);
   event->Add("gen_nu_phi_1",gen_nu_phi_1);
   event->Add("gen_nu_eta_1",gen_nu_eta_1);

   event->Add("gen_nu_p_2",gen_nu_p_2);
   event->Add("gen_nu_phi_2",gen_nu_phi_2);
   event->Add("gen_nu_eta_2",gen_nu_eta_2);

   event->Add("gen_vis_p_1",  gen_vis_p_1);
   event->Add("gen_vis_E_1",  gen_vis_E_1);
   event->Add("gen_vis_phi_1",gen_vis_phi_1);
   event->Add("gen_vis_eta_1",gen_vis_eta_1);
   event->Add("gen_vis_p_2",  gen_vis_p_2);
   event->Add("gen_vis_E_2",  gen_vis_E_2);
   event->Add("gen_vis_phi_2",gen_vis_phi_2);
   event->Add("gen_vis_eta_2",gen_vis_eta_2);

   event->Add("gen_neutral_p_1",  gen_neutral_p_1);
   event->Add("gen_neutral_E_1",  gen_neutral_E_1);
   event->Add("gen_neutral_phi_1",gen_neutral_phi_1);
   event->Add("gen_neutral_eta_1",gen_neutral_eta_1);
   event->Add("gen_neutral_p_2",  gen_neutral_p_2);
   event->Add("gen_neutral_E_2",  gen_neutral_E_2);
   event->Add("gen_neutral_phi_2",gen_neutral_phi_2);
   event->Add("gen_neutral_eta_2",gen_neutral_eta_2);

    if(ngenjets_){
      //Get gen-jets collection, filter Higgs decay products and add Njets variable to event
      std::vector<ic::GenJet*> gen_jets = event->GetPtrVec<ic::GenJet>("genJets");

      // erase jets matched to our taus or leptons
      for(unsigned i=0; i<gen_jets.size(); ++i){
        ic::GenJet *genjet = gen_jets[i];
        bool MatchedToPrompt = false;
        for(unsigned j=0; j<sel_particles_noptcut.size(); ++j){
         if(DRLessThan(std::make_pair(genjet, sel_particles_noptcut[j]),0.5)) MatchedToPrompt = true;
        }
        for(unsigned j=0; j<gen_taus_ptr_noptcut.size(); ++j){
         if(DRLessThan(std::make_pair(genjet, gen_taus_ptr_noptcut[j]),0.5)) MatchedToPrompt = true;
        }
        //remove jets that are matched to Higgs decay products
        if(MatchedToPrompt) gen_jets.erase (gen_jets.begin()+i);
      }

      double genjetpt_1=-1., genjetpt_2=-1.;
      double genjeteta_1=-1., genjeteta_2=-1.;
      if(gen_jets.size()>0) {
        genjetpt_1 = gen_jets[0]->pt();
        genjeteta_1 = fabs(gen_jets[0]->eta());
      }
      if(gen_jets.size()>1) {
        genjetpt_2 = gen_jets[1]->pt();
        genjeteta_2 = fabs(gen_jets[1]->eta());
      }
      event->Add("genjetpt_1", genjetpt_1);
      event->Add("genjetpt_2", genjetpt_2);
      event->Add("genjeteta_1", genjeteta_1);
      event->Add("genjeteta_2", genjeteta_2);

      ic::erase_if(gen_jets,!boost::bind(MinPtMaxEta, _1, 20.0, 5.0));
      unsigned ngenjets20 = gen_jets.size();
      event->Add("ngenjets20", ngenjets20);   
      ic::erase_if(gen_jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
      double gen_sjdphi_ = -9999; 
      double gen_mjj_=-9999;
      double gen_deta_=-9999;
      if (gen_jets.size() >= 2) {
        gen_mjj_ = (gen_jets[0]->vector()+gen_jets[1]->vector()).M();
        gen_deta_ = fabs(gen_jets[0]->eta() - gen_jets[1]->eta());
        if(gen_jets[0]->eta() > gen_jets[1]->eta())
          gen_sjdphi_ =  ROOT::Math::VectorUtil::DeltaPhi(gen_jets[0]->vector(), gen_jets[1]->vector());
        
        else
          gen_sjdphi_ =  ROOT::Math::VectorUtil::DeltaPhi(gen_jets[1]->vector(), gen_jets[0]->vector());
        
      }
      unsigned ngenjets = gen_jets.size();
      event->Add("ngenjets", ngenjets);
      event->Add("gen_sjdphi", gen_sjdphi_);
      event->Add("gen_mjj", gen_mjj_);
      event->Add("gen_jdeta", gen_deta_);
 
    }

    return 0;
  }
  int HTTPairGenInfo::PostAnalysis() {
    return 0;
  }

  void HTTPairGenInfo::PrintInfo() {
    ;
  }

}
