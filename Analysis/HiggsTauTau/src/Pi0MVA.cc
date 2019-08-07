#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/Pi0MVA.h"
 

ROOT::Math::PtEtaPhiEVector Cluster(std::vector<ic::PFCandidate*> gammas) {
  ROOT::Math::PtEtaPhiEVector cluster;
  if (gammas.size()==0) return cluster;
  double E=0;
  for(auto g : gammas) E+=g->energy();
  double phi = gammas[0]->phi();
  double eta = gammas[0]->eta();
  double mass = 0.;//0.1349;
  double p = sqrt(E*E-mass*mass);
  double theta = atan(exp(-eta))*2;
  double pt = p*sin(theta);
  cluster = ROOT::Math::PtEtaPhiEVector(pt, eta, phi, E);
  return cluster;
}

namespace ic {

  Pi0MVA::Pi0MVA(std::string const& name) : ModuleBase(name), channel_(channel::tt), strategy_(strategy::cpdecays16) {
    fs_ = NULL;
    maketrees_ = false;
  }

  Pi0MVA::~Pi0MVA() {
    ;
  }


  int Pi0MVA::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "Pi0MVA" << std::endl;
    std::cout << "-------------------------------------" << std::endl;


    if(fs_&&maketrees_){  
      outtree_ = fs_->make<TTree>("pi0_train_ntuple","pi0_train_ntuple");
      outtree_->Branch("tauFlag1"       , &tauFlag1_ );     
      outtree_->Branch("tauFlag2"       , &tauFlag2_ );
      outtree_->Branch("gen_match_1"    , &gen_match_1_);
      outtree_->Branch("gen_match_2"    , &gen_match_2_);
      outtree_->Branch("wt"             , &wt_       );
      outtree_->Branch("Ngammas_1"      , &Ngammas_1_ );
      outtree_->Branch("NgammaspT1_1"      , &NgammaspT1_1_ );
      outtree_->Branch("Nseeds", &Nseeds_);
      outtree_->Branch("new_pi0_E_1", & new_pi0_E_1_);      
      outtree_->Branch("new_pi0_E_2", & new_pi0_E_2_);
      outtree_->Branch("new_pi0_E_3", & new_pi0_E_3_);
      outtree_->Branch("new_pi0_E_4", & new_pi0_E_4_); 
      outtree_->Branch("new_M_1", &new_M_1_);
      outtree_->Branch("new_M_2", &new_M_2_);
      outtree_->Branch("new_M_3", &new_M_3_);
      outtree_->Branch("new_M_4", &new_M_4_);
      outtree_->Branch("new_eta_1", &new_eta_1_);
      outtree_->Branch("new_eta_2", &new_eta_2_);
      outtree_->Branch("new_eta_3", &new_eta_3_);
      outtree_->Branch("new_eta_4", &new_eta_4_);
      outtree_->Branch("new_phi_1", &new_phi_1_);
      outtree_->Branch("new_phi_2", &new_phi_2_);
      outtree_->Branch("new_phi_3", &new_phi_3_);
      outtree_->Branch("new_phi_4", &new_phi_4_);
      outtree_->Branch("mod_M", &mod_M_);
      outtree_->Branch("tau_decay_mode_1",&tau_decay_mode_1_);
      outtree_->Branch("tau_decay_mode_2",&tau_decay_mode_2_);
      outtree_->Branch("rho",&rho_);
      outtree_->Branch("n_vtx",&n_vtx_);
      outtree_->Branch("n_pu", &n_pu_);
      outtree_->Branch("Egentau_1"      , &Egentau_1_ );
      outtree_->Branch("Egenpi0_1_1", &Egenpi0_1_1_);
      outtree_->Branch("Egenpi0_1_2", &Egenpi0_1_2_);
      outtree_->Branch("Egengamma_1", &Egengamma_1_);
      outtree_->Branch("Egengamma_2", &Egengamma_2_); 
      outtree_->Branch("max_gengamma_dR", &max_gengamma_dR_);
      outtree_->Branch("gen_tau_pt", &gen_tau_pt_);
      outtree_->Branch("a1_rho_mass_1", &a1_rho_mass_1_);
      outtree_->Branch("a1_rho_mass_2", &a1_rho_mass_2_);

      outtree_->Branch("Phigentau_1"      , &Phigentau_1_ );
      outtree_->Branch("Etagentau_1"      , &Etagentau_1_ );
      outtree_->Branch("Mgentau_1"      , &Mgentau_1_ );
      //outtree_->Branch("Mgentau_1"      , &pt_ ); 
      outtree_->Branch("pi0_2gammas_E", &pi0_2gammas_E_);
      outtree_->Branch("pi0_3gammas_E", &pi0_3gammas_E_);
      outtree_->Branch("pi0_4gammas_E", &pi0_4gammas_E_);
      outtree_->Branch("pi0_2gammas_eta", &pi0_2gammas_eta_);
      outtree_->Branch("pi0_3gammas_eta", &pi0_3gammas_eta_);
      outtree_->Branch("pi0_4gammas_eta", &pi0_4gammas_eta_);
      outtree_->Branch("pi0_2gammas_phi", &pi0_2gammas_phi_);
      outtree_->Branch("pi0_3gammas_phi", &pi0_3gammas_phi_);
      outtree_->Branch("pi0_4gammas_phi", &pi0_4gammas_phi_); 
      outtree_->Branch("pi0_1gammas_M", &pi0_1gammas_M_);
      outtree_->Branch("pi0_2gammas_M", &pi0_2gammas_M_);
      outtree_->Branch("pi0_3gammas_M", &pi0_3gammas_M_);
      outtree_->Branch("pi0_4gammas_M", &pi0_4gammas_M_);
 
      outtree_->Branch("Egenpi0_1"      , &Egenpi0_1_ );
      outtree_->Branch("Phigenpi0_1"      , &Phigenpi0_1_ );
      outtree_->Branch("Etagenpi0_1"      , &Etagenpi0_1_ );
      outtree_->Branch("Mgenpi0_1"      , &Mgenpi0_1_ );
      outtree_->Branch("Epi_1"      , &Epi_1_ );
      outtree_->Branch("Phipi_1"      , &Phipi_1_ );
      outtree_->Branch("Etapi_1"      , &Etapi_1_ );
      outtree_->Branch("Mpi_1"      , &Mpi_1_ );
      outtree_->Branch("Epi0_1"      , &Epi0_1_ );
      outtree_->Branch("Phipi0_1"      , &Phipi0_1_ );
      outtree_->Branch("Etapi0_1"      , &Etapi0_1_ );
      outtree_->Branch("Mpi0_1"      , &Mpi0_1_ );

      outtree_->Branch("Egammas_1"      , &Egammas_1_ );
      outtree_->Branch("Phigammas_1"      , &Phigammas_1_ );
      outtree_->Branch("Etagammas_1"      , &Etagammas_1_ );
      outtree_->Branch("Mgammas_1"      , &Mgammas_1_ );
      outtree_->Branch("Egammas_2"      , &Egammas_2_ );
      outtree_->Branch("Phigammas_2"      , &Phigammas_2_ );
      outtree_->Branch("Etagammas_2"      , &Etagammas_2_ );
      outtree_->Branch("Mgammas_2"      , &Mgammas_2_ );
      outtree_->Branch("Egammas_3"      , &Egammas_3_ );
      outtree_->Branch("Phigammas_3"      , &Phigammas_3_ );
      outtree_->Branch("Etagammas_3"      , &Etagammas_3_ );
      outtree_->Branch("Mgammas_3"      , &Mgammas_3_ );
      outtree_->Branch("Egammas_4"      , &Egammas_4_ );
      outtree_->Branch("Egammas_4ormore"      , &Egammas_4ormore_ );
      outtree_->Branch("Phigammas_4"      , &Phigammas_4_ );
      outtree_->Branch("Etagammas_4"      , &Etagammas_4_ );
      outtree_->Branch("Mgammas_4"      , &Mgammas_4_ );
      outtree_->Branch("Egammas_5"      , &Egammas_5_ );
      outtree_->Branch("Phigammas_5"      , &Phigammas_5_ );
      outtree_->Branch("Etagammas_5"      , &Etagammas_5_ );
      outtree_->Branch("Mgammas_5"      , &Mgammas_5_ );
      outtree_->Branch("Egammas_6"      , &Egammas_6_ );
      outtree_->Branch("Phigammas_6"      , &Phigammas_6_ );
      outtree_->Branch("Etagammas_6"      , &Etagammas_6_ );
      outtree_->Branch("Mgammas_6"      , &Mgammas_6_ );
      outtree_->Branch("Egammas_7"      , &Egammas_7_ );
      outtree_->Branch("Phigammas_7"      , &Phigammas_7_ );
      outtree_->Branch("Etagammas_7"      , &Etagammas_7_ );
      outtree_->Branch("Mgammas_7"      , &Mgammas_7_ );
      outtree_->Branch("Pxgammas_1"      , &Pxgammas_1_ );
      outtree_->Branch("Pygammas_1"      , &Pygammas_1_ );
      outtree_->Branch("Pzgammas_1"      , &Pzgammas_1_ );
      outtree_->Branch("Pxgammas_2"      , &Pxgammas_2_ );
      outtree_->Branch("Pygammas_2"      , &Pygammas_2_ );
      outtree_->Branch("Pzgammas_2"      , &Pzgammas_2_ );
      outtree_->Branch("Pxgammas_3"      , &Pxgammas_3_ );
      outtree_->Branch("Pygammas_3"      , &Pygammas_3_ );
      outtree_->Branch("Pzgammas_3"      , &Pzgammas_3_ );
      outtree_->Branch("Pxgammas_4"      , &Pxgammas_4_ );
      outtree_->Branch("Pygammas_4"      , &Pygammas_4_ );
      outtree_->Branch("Pzgammas_4"      , &Pzgammas_4_ );
      outtree_->Branch("Pxgammas_5"      , &Pxgammas_5_ );
      outtree_->Branch("Pygammas_5"      , &Pygammas_5_ );
      outtree_->Branch("Pzgammas_5"      , &Pzgammas_5_ );
      outtree_->Branch("Pxgammas_6"      , &Pxgammas_6_ );
      outtree_->Branch("Pygammas_6"      , &Pygammas_6_ );
      outtree_->Branch("Pzgammas_6"      , &Pzgammas_6_ );
      outtree_->Branch("Pxgammas_7"      , &Pxgammas_7_ );
      outtree_->Branch("Pygammas_7"      , &Pygammas_7_ );
      outtree_->Branch("Pzgammas_7"      , &Pzgammas_7_ );
      outtree_->Branch("Pxpi_1"      , &Pxpi_1_ );
      outtree_->Branch("Pypi_1"      , &Pypi_1_ );
      outtree_->Branch("Pzpi_1"      , &Pzpi_1_ );
      outtree_->Branch("Pxtau_1"      , &Pxtau_1_ );
      outtree_->Branch("Pytau_1"      , &Pytau_1_ );
      outtree_->Branch("Pztau_1"      , &Pztau_1_ );
      outtree_->Branch("Etau_1"      , &Etau_1_ );
      outtree_->Branch("Phitau_1"      , &Phitau_1_ );
      outtree_->Branch("Etatau_1"      , &Etatau_1_ );
      outtree_->Branch("Mtau_1"      , &Mtau_1_ );
      outtree_->Branch("Pxgentau_1"      , &Pxgentau_1_ );
      outtree_->Branch("Pygentau_1"      , &Pygentau_1_ );
      outtree_->Branch("Pzgentau_1"      , &Pzgentau_1_ );
      outtree_->Branch("Pxgenpi0_1"      , &Pxgenpi0_1_ );
      outtree_->Branch("Pygenpi0_1"      , &Pygenpi0_1_ );
      outtree_->Branch("Pzgenpi0_1"      , &Pzgenpi0_1_ );
      outtree_->Branch("pt", &pt_);
      outtree_->Branch("eta", &eta_); 
 
    }

    return 0;
  }

  int Pi0MVA::Execute(TreeEvent *event) {

    //if(event->Exists("gen_match_1")) gen_match_1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
    //
    std::vector<PileupInfo *> puInfo = event->GetPtrVec<PileupInfo>("pileupInfo");
    n_pu_ = -1;
    for (unsigned i = 0; i < puInfo.size(); ++i) {
      if (puInfo[i]->bunch_crossing() == 0) n_pu_ = puInfo[i]->true_num_interactions();
    }
    
    // get gen information
    if((event->Exists("leading_gen_tau")&&channel_==channel::tt) || (event->Exists("subleading_gen_tau")&&channel_!=channel::tt)){
      ic::GenJet * gen_tau_jet;
      if(channel_==channel::tt) gen_tau_jet = event->GetPtr<ic::GenJet>("leading_gen_tau");
      else gen_tau_jet = event->GetPtr<ic::GenJet>("subleading_gen_tau");
      Egentau_1_ = gen_tau_jet->energy();
      Phigentau_1_ = gen_tau_jet->phi();
      Etagentau_1_ = gen_tau_jet->eta();
      Mgentau_1_ = gen_tau_jet->M();
      Pxgentau_1_= gen_tau_jet->vector().Px();
      Pygentau_1_= gen_tau_jet->vector().Py();
      Pzgentau_1_= gen_tau_jet->vector().Pz(); 
      gen_tau_pt_ = gen_tau_jet->pt();

      std::size_t gen_tau_id = gen_tau_jet->id();
      std::vector<GenParticle *> const& gen_particles = event->GetPtrVec<GenParticle>("genParticles");
      max_gengamma_dR_=0.;
      for(auto p : gen_particles) {
        if(p->id()==gen_tau_id){
          std::vector<GenParticle *> daughters = ExtractDaughters(p, gen_particles);
          ROOT::Math::PtEtaPhiEVector pi0_vec; 
          int count_pi0=0;
          for(auto d : daughters) {
            unsigned pdgId = abs(d->pdgid());
            if(pdgId == 111) {
              count_pi0++;
              pi0_vec+=d->vector();
              int i = d->daughters().at(0);
              int j = d->daughters().at(1);
              float max_dR = std::max(ROOT::Math::VectorUtil::DeltaR(gen_particles[i]->vector(),gen_tau_jet->vector()),ROOT::Math::VectorUtil::DeltaR(gen_particles[j]->vector(),gen_tau_jet->vector()));
              max_gengamma_dR_ = std::max(max_gengamma_dR_,max_dR);
             if(count_pi0==1) { 
                Egengamma_1_ = gen_particles[i]->energy();
                Egengamma_2_ = gen_particles[j]->energy();
                if(Egengamma_1_<Egengamma_2_){ 
                  double temp = Egengamma_1_;
                  Egengamma_1_ = Egengamma_2_;
                  Egengamma_2_=temp;
                }
                Egenpi0_1_1_ = d->energy(); 
            } 
            if(count_pi0==2) Egenpi0_1_2_ = d->energy();  
            }
          }
          if(Egenpi0_1_2_>Egenpi0_1_1_) {
            double temp = Egenpi0_1_2_;
            Egenpi0_1_2_ = Egenpi0_1_1_;
            Egenpi0_1_1_ = temp;
          }
          Egenpi0_1_ = pi0_vec.E();
          Phigenpi0_1_ = pi0_vec.Phi();
          Etagenpi0_1_ = pi0_vec.Rapidity();
          Mgenpi0_1_ = pi0_vec.M();
          Pxgenpi0_1_=pi0_vec.Px();
          Pygenpi0_1_=pi0_vec.Py();
          Pzgenpi0_1_=pi0_vec.Pz();
        
        }
      }
    }

    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    event_ = eventInfo->event() % 2 == 0; // if even then event_ = 1, odd = 0


    wt_cp_sm_=1; wt_cp_ps_=1; wt_cp_mm_=1;
    if(event->ExistsInTree("tauspinner")){
      EventInfo const* tauspinner = event->GetPtr<EventInfo>("tauspinner");
      wt_cp_sm_ = tauspinner->weight("wt_cp_0");
      wt_cp_ps_ = tauspinner->weight("wt_cp_0p5");
      wt_cp_mm_ = tauspinner->weight("wt_cp_0p25");
    }


    if (!(channel_ == channel::tt||channel_ == channel::mt||channel_ == channel::et)) return 0;
    
    wt_ = 1;
    wt_ = eventInfo->total_weight();

    rho_ = eventInfo->jet_rho();
    n_vtx_ = eventInfo->good_vertices();

    std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>("ditau");
    CompositeCandidate const* ditau = ditau_vec.at(0);
    Candidate const* lep1 = ditau->GetCandidate("lepton1");
    Candidate const* lep2 = ditau->GetCandidate("lepton2");
   
    ///////////////////
    //std::vector<ic::Vertex*> & vertex_vec = event->GetPtrVec<ic::Vertex>("refittedVertices");
    //std::cout << "------------------" << std::endl;
    //std::cout << lep1->id() << "    " << lep2->id() << std::endl;
    //std::size_t hash1 = lep1->id() + lep2->id();
    //
    //std::cout << hash1 << std::endl;
    //for(auto v : vertex_vec) std::cout << v->id() << std::endl;
    //

    //////////////////
 
    std::vector<ic::PFCandidate*> pfcands =  event->GetPtrVec<ic::PFCandidate>("pfCandidates");

    if (event->Exists("tauFlag1")) tauFlag1_ = event->Get<int>("tauFlag1");
    if (event->Exists("tauFlag2")) tauFlag2_ = event->Get<int>("tauFlag2");

    if(event->Exists("gen_match_1")) gen_match_1_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_1"));
    if(event->Exists("gen_match_2")) gen_match_2_ = MCOrigin2UInt(event->Get<ic::mcorigin>("gen_match_2"));

    if(channel_!=channel::tt) {
      //for semi-leptonic channels switch these round for consistency
      int temp = gen_match_1_;
      gen_match_1_ = gen_match_2_;
      gen_match_2_ = temp;
      temp = tauFlag1_;
      tauFlag1_ = tauFlag2_;
      tauFlag2_ = temp;
    }

    if (event->ExistsInTree("pfCandidates")) {
      Tau const* tau1;
      if(channel_==channel::tt) tau1 = dynamic_cast<Tau const*>(lep1);
      else tau1 = dynamic_cast<Tau const*>(lep2);
      tau_decay_mode_1_=tau1->decay_mode();
      eta_ = tau1->eta();
      pt_ = tau1->pt();
      std::vector<ic::PFCandidate*> gammas_pt1 = GetTauGammas(tau1, pfcands, 1.0);
      NgammaspT1_1_ = gammas_pt1.size();
 
      std::vector<ic::PFCandidate*> gammas1 = GetTauGammas(tau1, pfcands, 0.0);
      //std::vector<ic::PFCandidate*> gammas1 = GetTauGammas(tau1, pfcands, 1.0); // only use gammas > 1 GeV -> to be consistent with 94X samples
      ic::Candidate* pi_1 = GetTauHads(tau1, pfcands)[0];
      Ngammas_1_ = gammas1.size();

      pi0_2gammas_E_=0., pi0_3gammas_E_=0., pi0_4gammas_E_=0.;
      pi0_2gammas_eta_=0., pi0_3gammas_eta_=0., pi0_4gammas_eta_=0.;
      pi0_2gammas_phi_=0., pi0_3gammas_phi_=0., pi0_4gammas_phi_=0.;

      if(Ngammas_1_>=1) pi0_1gammas_M_ = (gammas1[0]->vector()+pi_1->vector()).M();

      if(Ngammas_1_>=2) {
        auto pi0_vec = gammas1[0]->vector() + gammas1[1]->vector();
        pi0_2gammas_eta_ = pi0_vec.Rapidity();
        pi0_2gammas_phi_ = pi0_vec.Phi();
        pi0_2gammas_E_ = pi0_vec.E();
        pi0_2gammas_M_ = (pi0_vec+pi_1->vector()).M();
      } 
      if(Ngammas_1_>=3) {
        auto pi0_vec = gammas1[0]->vector() + gammas1[1]->vector() + gammas1[2]->vector();
        pi0_3gammas_eta_ = pi0_vec.Rapidity();
        pi0_3gammas_phi_ = pi0_vec.Phi();
        pi0_3gammas_E_ = pi0_vec.E();
        pi0_3gammas_M_ = (pi0_vec+pi_1->vector()).M();
      }
      if(Ngammas_1_>=4) {
        auto pi0_vec = gammas1[0]->vector() + gammas1[1]->vector() + gammas1[2]->vector() + gammas1[3]->vector();
        pi0_4gammas_eta_ = pi0_vec.Rapidity();
        pi0_4gammas_phi_ = pi0_vec.Phi();
        pi0_4gammas_E_ = pi0_vec.E();
        pi0_4gammas_M_ = (pi0_vec+pi_1->vector()).M();
      }

      //auto neutral_vec = tau1->vector()-pi_1->vector();
      //Epi0_1_=neutral_vec.E(); Phipi0_1_=neutral_vec.Phi(); neutral_vec.Rapidity(); Mpi0_1_=neutral_vec.M(); Pxpi0_1_=neutral_vec.Px(); Pypi0_1_=neutral_vec.Py(); Pzpi0_1_=neutral_vec.Pz();
     
      Epi0_1_=0, Phipi0_1_=0, Etapi0_1_=0; 
      for(auto g : gammas1) {
        double E = g->energy();
        double phi = g->phi();
        double eta = g->eta();
        Epi0_1_+=E;
        Phipi0_1_+=E*phi;
        Etapi0_1_+=E*eta; 
      }
      Phipi0_1_/=Epi0_1_;
      Etapi0_1_/=Epi0_1_; 

      std::vector<PFCandidate*> seed_gammas = {};
      std::vector<PFCandidate*> other_gammas = {};
      unsigned count_gammas=0;
      //double cone_size = std::max(std::min(0.1, 3./tau1->pt()),0.05);
      //double cone_size=0.1;
      double cone_size = std::max(std::min(0.20, 8./tau1->pt()),0.1);
      for(auto g : gammas1) {
        double E = g->energy(); 
        double dR = ROOT::Math::VectorUtil::DeltaR(g->vector(),tau1->vector());
        //if(E/Epi0_1_>0.1) std::cout << cone_size << "    " << dR << "    " << E/Epi0_1_ << std::endl ; 
        if((E/Epi0_1_>0.1 && dR<cone_size) || count_gammas==0) seed_gammas.push_back(g);  // tune threshold
        else other_gammas.push_back(g);
        count_gammas++;
        if(seed_gammas.size()>=4) break;
      }
      Nseeds_ = seed_gammas.size();
      //ROOT::Math::PtEtaPhiEVector sum_vec;
      //for(unsigned i=0; i<4&&i<seed_gammas.size();++i) {
      //  sum_vec+=seed_gammas[0]->vector();  
      //}
      //new_pi0_eta = sum_vec.Rapidity();
      //new_pi0_phi = sum_vec.Phi();
    
      std::vector<std::vector<PFCandidate*>> sorted_gammas =  {{},{},{},{}};
      // push seeds as first gammas in vectors 
      for(unsigned i=0; i<4&&i<Nseeds_;++i) {
        sorted_gammas[i].push_back(seed_gammas[i]);
      }
      // loop over remaining gammas and associate gammas based on dR*E
      for(auto g : other_gammas) {
        int min_j=-1;
        double mindRE=-1;
        for(unsigned j=0; j<4&&j<Nseeds_;++j) {
          //double dRE = fabs((seed_gammas[j]->energy()+g->energy())*ROOT::Math::VectorUtil::DeltaR(seed_gammas[j]->vector(),g->vector()));
          double seedE = 0;
          for(auto x : sorted_gammas[j]) seedE+=x->energy();
          double dRE = fabs((seedE+g->energy())*ROOT::Math::VectorUtil::DeltaR(seed_gammas[j]->vector(),g->vector()));
          if(dRE<mindRE || min_j==-1) {
            min_j=j; 
            mindRE = dRE; 
          }
        }
        sorted_gammas[min_j].push_back(g);
      }
      new_pi0_E_1_ = 0., new_pi0_E_2_ = 0., new_pi0_E_3_ =0., new_pi0_E_4_=0.;

      for(auto g : sorted_gammas[0]) {
        new_pi0_E_1_+=g->energy();
      }
      for(auto g : sorted_gammas[1]) {
        new_pi0_E_2_+=g->energy();
      }
      for(auto g : sorted_gammas[2]) {
        new_pi0_E_3_+=g->energy();
      }
      for(auto g : sorted_gammas[3]) {
        new_pi0_E_4_+=g->energy();
      }

      new_M_1_ = (pi_1->vector()+Cluster(sorted_gammas[0])).M(); 
      new_M_2_ = (pi_1->vector()+Cluster(sorted_gammas[0])+Cluster(sorted_gammas[1])).M();
      new_M_3_ = (pi_1->vector()+Cluster(sorted_gammas[0])+Cluster(sorted_gammas[1])+Cluster(sorted_gammas[2])).M();
      new_M_4_ = (pi_1->vector()+Cluster(sorted_gammas[0])+Cluster(sorted_gammas[1])+Cluster(sorted_gammas[2])+Cluster(sorted_gammas[3])).M();
     
      new_eta_1_ = (Cluster(sorted_gammas[0])).Rapidity();
      new_eta_2_ = (Cluster(sorted_gammas[0])+Cluster(sorted_gammas[1])).Rapidity();
      new_eta_3_ = (Cluster(sorted_gammas[0])+Cluster(sorted_gammas[1])+Cluster(sorted_gammas[2])).Rapidity();
      new_eta_4_ = (Cluster(sorted_gammas[0])+Cluster(sorted_gammas[1])+Cluster(sorted_gammas[2])+Cluster(sorted_gammas[3])).Rapidity(); 

      new_phi_1_ = (Cluster(sorted_gammas[0])).Phi();
      new_phi_2_ = (Cluster(sorted_gammas[0])+Cluster(sorted_gammas[1])).Phi();
      new_phi_3_ = (Cluster(sorted_gammas[0])+Cluster(sorted_gammas[1])+Cluster(sorted_gammas[2])).Phi();
      new_phi_4_ = (Cluster(sorted_gammas[0])+Cluster(sorted_gammas[1])+Cluster(sorted_gammas[2])+Cluster(sorted_gammas[3])).Phi();
      mod_M_ = 0.;
      if(gammas1.size()>0){
        double E=0.;
        for(auto g : gammas1) E+=g->energy();
        double phi = gammas1[0]->phi();
        double eta = gammas1[0]->eta();
        double mass = 0.1349;
        double p = sqrt(E*E-mass*mass);
        double theta = atan(exp(-eta))*2;
        double pt = p*sin(theta);
        ROOT::Math::PtEtaPhiEVector mod_pi0(pt, eta, phi, E);
        mod_M_ = (pi_1->vector()+mod_pi0).M();
      }

      a1_rho_mass_1_=-1, a1_rho_mass_2_=-1;
      if(Nseeds_==2) {
        a1_rho_mass_1_ = (pi_1->vector()+Cluster(sorted_gammas[0])).M();
        a1_rho_mass_2_ = (pi_1->vector()+Cluster(sorted_gammas[1])).M();
        if(fabs(a1_rho_mass_1_-0.77)>fabs(a1_rho_mass_2_-0.77)) {
          float temp = a1_rho_mass_1_;
          a1_rho_mass_1_ = a1_rho_mass_2_;
          a1_rho_mass_2_ = temp;
        }
      }

      //if(tau_decay_mode_1_==1) {
      //  std::cout << tauFlag1_ << "    " << Nseeds_ << "    " << gammas1.size() << "    " << sorted_gammas[0].size() << "    " << sorted_gammas[1].size() << std::endl;  
      //}


      Epi_1_=pi_1->energy(); Phipi_1_=pi_1->phi(); Etapi_1_=pi_1->eta(); Mpi_1_=pi_1->M(); Pxpi_1_=pi_1->vector().Px(); Pypi_1_=pi_1->vector().Py(); Pzpi_1_=pi_1->vector().Pz(); 
      Etau_1_=tau1->energy(); Phitau_1_=tau1->phi(); Etatau_1_=tau1->eta(); Mtau_1_=tau1->M(); Pxtau_1_=tau1->vector().Px(); Pytau_1_=tau1->vector().Py(); Pztau_1_=tau1->vector().Pz();

      if(Ngammas_1_>=1) {Egammas_1_=gammas1[0]->energy(); Phigammas_1_=gammas1[0]->phi(); Etagammas_1_=gammas1[0]->eta(); Mgammas_1_=gammas1[0]->M(); Pxgammas_1_=gammas1[0]->vector().Px(); Pygammas_1_=gammas1[0]->vector().Py(); Pzgammas_1_=gammas1[0]->vector().Pz();}
      else {Egammas_1_=0.; Phigammas_1_=0.; Etagammas_1_=0.; Mgammas_1_=0.; Pxgammas_1_=0.; Pygammas_1_=0.; Pzgammas_1_=0.;}
      if(Ngammas_1_>=2) {Egammas_2_=gammas1[1]->energy(); Phigammas_2_=gammas1[1]->phi(); Etagammas_2_=gammas1[1]->eta(); Mgammas_2_=gammas1[1]->M(); Pxgammas_2_=gammas1[1]->vector().Px(); Pygammas_2_=gammas1[1]->vector().Py(); Pzgammas_2_=gammas1[1]->vector().Pz();}
      else {Egammas_2_=0.; Phigammas_2_=0.; Etagammas_2_=0.; Mgammas_2_=0.; Pxgammas_2_=0.; Pygammas_2_=0.; Pzgammas_2_=0.;}
      if(Ngammas_1_>=3) {Egammas_3_=gammas1[2]->energy(); Phigammas_3_=gammas1[2]->phi(); Etagammas_3_=gammas1[2]->eta(); Mgammas_3_=gammas1[2]->M(); Pxgammas_3_=gammas1[2]->vector().Px(); Pygammas_3_=gammas1[2]->vector().Py(); Pzgammas_3_=gammas1[2]->vector().Pz();}
      else {Egammas_3_=0.; Phigammas_3_=0.; Etagammas_3_=0.; Mgammas_3_=0.; Pxgammas_3_=0.; Pygammas_3_=0.; Pzgammas_3_=0.;}
      if(Ngammas_1_>=4) {Egammas_4_=gammas1[3]->energy(); Phigammas_4_=gammas1[3]->phi(); Etagammas_4_=gammas1[3]->eta(); Mgammas_4_=gammas1[3]->M(); Pxgammas_4_=gammas1[3]->vector().Px(); Pygammas_4_=gammas1[3]->vector().Py(); Pzgammas_4_=gammas1[3]->vector().Pz();}
      else {Egammas_4_=0.; Phigammas_4_=0.; Etagammas_4_=0.; Mgammas_4_=0.; Pxgammas_4_=0.; Pygammas_4_=0.; Pzgammas_4_=0.;}
      if(Ngammas_1_>=5) {Egammas_5_=gammas1[4]->energy(); Phigammas_5_=gammas1[4]->phi(); Etagammas_5_=gammas1[4]->eta(); Mgammas_5_=gammas1[4]->M(); Pxgammas_5_=gammas1[4]->vector().Px(); Pygammas_5_=gammas1[4]->vector().Py(); Pzgammas_5_=gammas1[4]->vector().Pz();}
      else {Egammas_5_=0.; Phigammas_5_=0.; Etagammas_5_=0.; Mgammas_5_=0.; Pxgammas_5_=0.; Pygammas_5_=0.; Pzgammas_5_=0.;}
      if(Ngammas_1_>=6) {Egammas_6_=gammas1[5]->energy(); Phigammas_6_=gammas1[5]->phi(); Etagammas_6_=gammas1[5]->eta(); Mgammas_6_=gammas1[5]->M(); Pxgammas_6_=gammas1[5]->vector().Px(); Pygammas_6_=gammas1[5]->vector().Py(); Pzgammas_6_=gammas1[5]->vector().Pz();}
      else {Egammas_6_=0.; Phigammas_6_=0.; Etagammas_6_=0.; Mgammas_6_=0.; Pxgammas_6_=0.; Pygammas_6_=0.; Pzgammas_6_=0.;}
      if(Ngammas_1_>=7) {Egammas_7_=gammas1[6]->energy(); Phigammas_7_=gammas1[6]->phi(); Etagammas_7_=gammas1[6]->eta(); Mgammas_7_=gammas1[6]->M(); Pxgammas_7_=gammas1[6]->vector().Px(); Pygammas_7_=gammas1[6]->vector().Py(); Pzgammas_7_=gammas1[6]->vector().Pz();}
      else {Egammas_7_=0.; Phigammas_7_=0.; Etagammas_7_=0.; Mgammas_7_=0.; Pxgammas_7_=0.; Pygammas_7_=0.; Pzgammas_7_=0.;}
     
      Egammas_4ormore_=0.;
      for(unsigned i=3; i<gammas1.size();++i){
        Egammas_4ormore_+=gammas1[i]->energy(); 
      } 

    }
    if(fs_&&maketrees_) outtree_->Fill();

    return 0;
  }
  int Pi0MVA::PostAnalysis() {
    return 0;
  }

  void Pi0MVA::PrintInfo() {
    ;
  }

}
