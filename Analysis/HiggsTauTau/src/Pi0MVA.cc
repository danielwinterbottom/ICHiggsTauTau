#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/Pi0MVA.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/ImpactParameter.h" 

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
      outtree_->Branch("NelecspT1_1", &NelecspT1_1_);
      outtree_->Branch("pi0_pt", &pi0_pt_);
      outtree_->Branch("Nseeds", &Nseeds_);
      outtree_->Branch("Ngams", &Ngams_);

      outtree_->Branch("new_pi0_phi", &new_pi0_phi_);
      outtree_->Branch("new_pi0_eta", &new_pi0_eta_);
      outtree_->Branch("new_pi0_pt", &new_pi0_pt_);
      outtree_->Branch("new_pi0_E", &new_pi0_E_);

      outtree_->Branch("new_pi0_E_1", & new_pi0_E_1_);      
      outtree_->Branch("new_pi0_E_2", & new_pi0_E_2_);
      outtree_->Branch("new_pi0_E_3", & new_pi0_E_3_);
      outtree_->Branch("new_pi0_E_4", & new_pi0_E_4_);
      outtree_->Branch("new_pi0_pt_1", & new_pi0_pt_1_);
      outtree_->Branch("new_pi0_pt_2", & new_pi0_pt_2_);
      outtree_->Branch("new_pi0_pt_3", & new_pi0_pt_3_);
      outtree_->Branch("new_pi0_pt_4", & new_pi0_pt_4_);
      outtree_->Branch("new_pi0_eta_1", & new_pi0_eta_1_);
      outtree_->Branch("new_pi0_eta_2", & new_pi0_eta_2_);
      outtree_->Branch("new_pi0_eta_3", & new_pi0_eta_3_);
      outtree_->Branch("new_pi0_eta_4", & new_pi0_eta_4_);
      outtree_->Branch("new_pi0_phi_1", & new_pi0_phi_1_);
      outtree_->Branch("new_pi0_phi_2", & new_pi0_phi_2_);
      outtree_->Branch("new_pi0_phi_3", & new_pi0_phi_3_);
      outtree_->Branch("new_pi0_phi_4", & new_pi0_phi_4_);
 
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
      outtree_->Branch("gengamma_dR", &gengamma_dR_);
      outtree_->Branch("genpi0_dR", &genpi0_dR_);
//      outtree_->Branch("",&);
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

      outtree_->Branch("rho_dphi"      , &rho_dphi_ );
      outtree_->Branch("gen_rho_dphi"      , &gen_rho_dphi_ );
      outtree_->Branch("corr_rho_dphi"      , &corr_rho_dphi_ );
      outtree_->Branch("corr_Phigammas_1"      , &corr_Phigammas_1_ );

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
      outtree_->Branch("dR_match_gamma1",&dR_match_gamma1_);
      outtree_->Branch("dR_match_gamma2",&dR_match_gamma2_);
      outtree_->Branch("dR_match_gamma1_wrong",&dR_match_gamma1_wrong_);
      outtree_->Branch("dR_match_gamma2_wrong",&dR_match_gamma2_wrong_);
      outtree_->Branch("dR_match_pi01",&dR_match_pi01_);
      outtree_->Branch("dR_match_pi02",&dR_match_pi02_);
      outtree_->Branch("dR_match_pi01_wrong",&dR_match_pi01_wrong_);
      outtree_->Branch("dR_match_pi02_wrong",&dR_match_pi02_wrong_);

      outtree_->Branch("Nstrips", &Nstrips_); 
      outtree_->Branch("Nstrips_inciso", &Nstrips_inciso_);
      outtree_->Branch("lead_strip_pt", &lead_strip_pt_);
      outtree_->Branch("sublead_strip_pt", &sublead_strip_pt_);
      outtree_->Branch("lead_strip_dR", &lead_strip_dR_);
      outtree_->Branch("closest_strip_pt", &closest_strip_pt_);
      outtree_->Branch("closest_strip_dR", &closest_strip_dR_);
      outtree_->Branch("lead_strip_pt_inciso", &lead_strip_pt_inciso_);
      outtree_->Branch("lead_strip_dR_inciso", &lead_strip_dR_inciso_);
      outtree_->Branch("closest_strip_pt_inciso", &closest_strip_pt_inciso_);
      outtree_->Branch("closest_strip_dR_inciso", &closest_strip_dR_inciso_);

      outtree_->Branch("Nstrips_dm0", &Nstrips_dm0_);
      outtree_->Branch("new_strips_lead_gamma_pt", &new_strips_lead_gamma_pt_);
      outtree_->Branch("new_strips_pt", &new_strips_pt_);
      outtree_->Branch("new_tau_pt", &new_tau_pt_);
      outtree_->Branch("pt_genpi0_1", &pt_genpi0_1_); 
      outtree_->Branch("pt_genpi0_2", &pt_genpi0_2_);
      outtree_->Branch("pt_gengammas_1", &pt_gengammas_1_);
      outtree_->Branch("pt_gengammas_2", &pt_gengammas_2_);
      outtree_->Branch("genpi0_deta", &genpi0_deta_);
      outtree_->Branch("genpi0_dphi", &genpi0_dphi_);
      outtree_->Branch("gengamma_deta", &gengamma_deta_);
      outtree_->Branch("gengamma_dphi", &gengamma_dphi_);
      outtree_->Branch("lead_cluster_pt", &lead_cluster_pt_);
      outtree_->Branch("sublead_cluster_pt", &sublead_cluster_pt_);
      outtree_->Branch("lead_cluster_E", &lead_cluster_E_);
      outtree_->Branch("sublead_cluster_E", &sublead_cluster_E_);

      outtree_->Branch("lead_cluster_pt_new", &lead_cluster_pt_new_);
      outtree_->Branch("sublead_cluster_pt_new", &sublead_cluster_pt_new_);
      outtree_->Branch("lead_cluster_E_new", &lead_cluster_E_new_);
      outtree_->Branch("sublead_cluster_E_new", &sublead_cluster_E_new_);

      //TFile *fout = new TFile("phi_cdf_2018.root");
      input_cdf_ = new TH1D(GetFromTFile<TH1D>("phi_cdf_2018.root","/","mc_cdf"));
      output_cdf_ = new TH1D(GetFromTFile<TH1D>("phi_cdf_2018.root","/","data_cdf"));
      //output_cdf_ = (TH1D*)fout->Get("data_cdf")->Clone();

      //fout->Close();

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
   
    std::vector<GenParticle*> genpi0s = {};
    std::vector<GenParticle*> gengammas = {};
 
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
      gengamma_dR_=0.;
      for(auto p : gen_particles) {
        if(p->id()==gen_tau_id){
          std::vector<GenParticle *> daughters = ExtractDaughters(p, gen_particles);
          ROOT::Math::PtEtaPhiEVector pi0_vec; 
          int count_pi0=0;
          for(auto d : daughters) {
            unsigned pdgId = abs(d->pdgid());
            if(pdgId == 111) {
              genpi0s.push_back(d);
              count_pi0++;
              pi0_vec+=d->vector();
              int i = d->daughters().at(0);
              int j = d->daughters().at(1);
              //if(fabs(gen_particles[i]->pdgid()) == 22) std::cout << gen_particles[i]->daughters().size() << std::endl;
              float max_dR = std::max(ROOT::Math::VectorUtil::DeltaR(gen_particles[i]->vector(),gen_tau_jet->vector()),ROOT::Math::VectorUtil::DeltaR(gen_particles[j]->vector(),gen_tau_jet->vector()));
              max_gengamma_dR_ = std::max(max_gengamma_dR_,max_dR);
              //float max_gammas_dR = std::max(ROOT::Math::VectorUtil::DeltaR(gen_particles[i]->vector(),d->vector()),ROOT::Math::VectorUtil::DeltaR(gen_particles[j]->vector(),d->vector()));
              //gengamma_dR_ = ROOT::Math::VectorUtil::DeltaR(gen_particles[i]->vector(),d->vector());//std::max(gengamma_dR_,max_gammas_dR);
             if(count_pi0==1) {
                Egengamma_1_ = gen_particles[i]->energy();
                Egengamma_2_ = gen_particles[j]->energy();
                if(Egengamma_1_<Egengamma_2_){ 
                  double temp = Egengamma_1_;
                  Egengamma_1_ = Egengamma_2_;
                  Egengamma_2_=temp;
                  gengammas.push_back(gen_particles[j]);
                  gengammas.push_back(gen_particles[i]);
                } else {
                  gengammas.push_back(gen_particles[j]);
                  gengammas.push_back(gen_particles[i]);
                }

                Egenpi0_1_1_ = d->energy(); 
              } 
              if(count_pi0==2) Egenpi0_1_2_ = d->energy();  
            }
          }
          if (genpi0s.size()==2) {
            //genpi0_dR_ = ROOT::Math::VectorUtil::DeltaR(genpi0s[0]->vector(),genpi0s[1]->vector());

            if(Egenpi0_1_2_>Egenpi0_1_1_) {
              double temp = Egenpi0_1_2_;
              Egenpi0_1_2_ = Egenpi0_1_1_;
              Egenpi0_1_1_ = temp;

              auto t = genpi0s[0];
              genpi0s[0] = genpi0s[1];
              genpi0s[1] = t;
            }
            //genpi01_eta_ = genpi0s[0]->eta();
            //genpi02_eta_ = genpi0s[1]->eta();
            //genpi01_phi_ = genpi0s[0]->phi();
            //genpi02_phi_ = genpi0s[1]->phi();
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
    if(gengammas.size()==2) {
      gengamma_dR_ = ROOT::Math::VectorUtil::DeltaR(gengammas[0]->vector(),gengammas[1]->vector());
      pt_gengammas_1_=gengammas[0]->pt();
      pt_gengammas_2_=gengammas[1]->pt();
      gengamma_deta_ = std::fabs(gengammas[0]->eta()-gengammas[1]->eta());
      gengamma_dphi_ = ROOT::Math::VectorUtil::DeltaPhi(gengammas[0]->vector(),gengammas[1]->vector());
    }
    if(genpi0s.size()==2) {
      genpi0_dR_ = ROOT::Math::VectorUtil::DeltaR(genpi0s[0]->vector(),genpi0s[1]->vector());
      genpi0_deta_ = std::fabs(genpi0s[0]->eta()-genpi0s[1]->eta());
      genpi0_dphi_ = ROOT::Math::VectorUtil::DeltaPhi(genpi0s[0]->vector(),genpi0s[1]->vector());
      pt_genpi0_1_=genpi0s[0]->pt();
      pt_genpi0_2_=genpi0s[1]->pt();
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
      Tau const *tau1;
      if(channel_==channel::tt) tau1 = dynamic_cast<Tau const*>(lep1);
      else tau1 = dynamic_cast<Tau const*>(lep2);
      tau_decay_mode_1_=tau1->decay_mode();
      eta_ = tau1->eta();
      pt_ = tau1->pt();
      std::vector<ic::PFCandidate*> gammas_pt1 = GetTauGammas(tau1, pfcands, 1.0);
      NgammaspT1_1_ = gammas_pt1.size();

      NelecspT1_1_ = 0;
      for (auto g : gammas_pt1) {
        if(fabs(g->pdgid())==11) NelecspT1_1_++;
      }

      //ImpactParameter::CalculatePCA(double B, std::vector<float> h_param, RMPoint ref, RMPoint PrV, RMFLV p4);
      //std::vector<ic::Vertex*> & vertex_vec = event->GetPtrVec<ic::Vertex>("vertices");
      //auto vertex = vertex_vec[0];
      //
      //std::vector<float> h_param = {};
      //ic::Tau  *tau = const_cast<Tau*>(tau1);
      //for(auto i :  tau->track_params()) h_param.push_back(i);
      //double B = tau->bfield();
      //ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<float> > ref(tau->svx(),tau->svy(),tau->svz());
      //ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<float> > pv(vertex->vx(),vertex->vy(),vertex->vz());
      //std::vector<ic::PFCandidate*> charged_cands = GetTauHads(tau, pfcands);
      //ROOT::Math::PtEtaPhiEVector charged_vec;
      //for(auto c : charged_cands) charged_vec+=c->vector();

      //ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float>> p4; 

      //p4.SetPxPyPzE(charged_vec.Px(),charged_vec.Py(),charged_vec.Pz(),charged_vec.E());
      //ImpactParameter IP;
      //TVector3 ip = IP.CalculatePCA(B, h_param, ref, pv, p4);

      //ROOT::Math::SMatrix<double,5,5, ROOT::Math::MatRepSym<double,5>> helixCov = tau->track_params_covariance();
      //ROOT::Math::SMatrix<double, 3, 3, ROOT::Math::MatRepSym<double, 3> > SigmaPrV = vertex->covariance();

      //ROOT::Math::SMatrix<double,3,3, ROOT::Math::MatRepStd< double, 3, 3 >> ip_cov = IP.CalculatePCACovariance(helixCov, SigmaPrV);
      //std::cout << "---------" << std::endl;
      //std::cout << ip_cov[0][0] << "    " << ip_cov[1][1] << "    " << ip_cov[2][2] << std::endl;
      //double mag = ip.Mag();
      //ROOT::Math::SVector<double, 3> ip_svec;
      //ip_svec(0) = ip.X()/mag;
      //ip_svec(1) = ip.Y()/mag;
      //ip_svec(2) = ip.Z()/mag;
      //std::cout << ip_svec(0) << "    " << ip_svec(1) << "    " << ip_svec(2) << std::endl;

      //double uncert = ROOT::Math::Dot( ip_svec, ip_cov * ip_svec);
      //std::cout << uncert << std::endl; 
 
      std::vector<ic::PFCandidate*> gammas1 = GetTauGammas(tau1, pfcands, 0.0);

// HPS stuff
      std::vector<std::pair<ic::PFCandidate*,std::vector<ic::PFCandidate*>>> strip_pairs = HPSGammas(gammas1, 0, 0, 0, 0., 1);
      std::vector<ic::PFCandidate*> iso_gammas = GetTauIsoGammas(tau1, pfcands, 0.0); //iso gammas 
      iso_gammas.insert(iso_gammas.end(), gammas1.begin(), gammas1.end()  );
      std::vector<std::pair<ic::PFCandidate*,std::vector<ic::PFCandidate*>>> strip_pairs_inciso = HPSGammas(iso_gammas, 0, 0, 0, 0., 1);

      Nstrips_ = strip_pairs.size();
      Nstrips_inciso_ = strip_pairs_inciso.size();
      if(Nstrips_>0){
        lead_strip_pt_ = strip_pairs[0].first->pt(); 
        if(Nstrips_>1) sublead_strip_pt_ = strip_pairs[1].first->pt(); 
        lead_strip_dR_ = ROOT::Math::VectorUtil::DeltaR(strip_pairs[0].first->vector(),tau1->vector());
        int min_i = -1;
        double mindR=0;
        for(unsigned i=0; i<strip_pairs.size(); ++i) {
          double dR = ROOT::Math::VectorUtil::DeltaR(strip_pairs[i].first->vector(),tau1->vector());
          if(min_i==-1 || dR<mindR) {
            mindR = dR;
            min_i = i;
          }
        }
        closest_strip_pt_ = strip_pairs[min_i].first->pt();
        closest_strip_dR_ = ROOT::Math::VectorUtil::DeltaR(strip_pairs[min_i].first->vector(),tau1->vector());
      }

      if(Nstrips_inciso_>0){
        lead_strip_pt_inciso_ = strip_pairs_inciso[0].first->pt();
        lead_strip_dR_inciso_ = ROOT::Math::VectorUtil::DeltaR(strip_pairs_inciso[0].first->vector(),tau1->vector());
        int min_i = -1;
        double mindR=0;
        for(unsigned i=0; i<strip_pairs_inciso.size(); ++i) {
          double dR = ROOT::Math::VectorUtil::DeltaR(strip_pairs_inciso[i].first->vector(),tau1->vector());
          if(min_i==-1 || dR<mindR) {
            mindR = dR;
            min_i = i;
          }
        }
        closest_strip_pt_inciso_ = strip_pairs_inciso[min_i].first->pt();
        closest_strip_dR_inciso_ = ROOT::Math::VectorUtil::DeltaR(strip_pairs_inciso[min_i].first->vector(),tau1->vector());
      }

      if(tau_decay_mode_1_==0) {
     
        std::vector<std::pair<ic::PFCandidate*,std::vector<ic::PFCandidate*>>> passing_strips;
        for(auto s : strip_pairs) {
          double strip_pt = s.first->pt();
          //double taupt = (s.first->vector()+tau1->vector()).Pt();    
          double dR = ROOT::Math::VectorUtil::DeltaR(tau1->vector()+s.first->vector(), s.first->vector());
          //double cone_size = max(3./taupt,0.05)+(min(9.25858e-06*strip_pt*strip_pt-0.00152088*strip_pt+0.0904073,0.08)*(strip_pt<80) + (strip_pt>80)*0.03);
          double cone_size=0.15;
          if(strip_pt>1&&dR<cone_size) passing_strips.push_back(s);
        }
        Nstrips_dm0_ = passing_strips.size();
        if(passing_strips.size()>0) {
          new_strips_lead_gamma_pt_ = passing_strips[0].second[0]->pt();
          new_strips_pt_  = passing_strips[0].first->pt();
          new_tau_pt_ = (passing_strips[0].first->vector()+tau1->vector()).Pt();
        }
        else {
          new_strips_lead_gamma_pt_ = -1;
          new_strips_pt_ = -1;
          new_tau_pt_=pt_;
        }
        if(Nstrips_dm0_>0) gammas1 = passing_strips[0].second;
        else gammas1 = {};
      }
      if(tau_decay_mode_1_==1) {
        if(Nstrips_>0) gammas1 = strip_pairs[0].second;
        else gammas1 = {};
      } 
// /////////////////////////////



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
      double theta = atan(exp(-Etapi0_1_))*2;
      pi0_pt_ = Epi0_1_*sin(theta); 

      std::vector<PFCandidate*> seed_gammas = {};
      std::vector<PFCandidate*> other_gammas = {};
      unsigned count_gammas=0;
      //double cone_size = std::max(std::min(0.1, 3./tau1->pt()),0.05);
      //double cone_size = std::max(std::min(0.4, 2./pi0_pt_),0.05);

      //double cone_size = std::max(std::min(0.1, 3./tau1->pt()),0.05) + std::min(0.2, 2./pi0_pt_);
      double cone_size = std::max(std::min(0.2, 4./pi0_pt_),0.05);
      //double cone_size=0.1;
      //double cone_size = std::max(std::min(0.20, 8./tau1->pt()),0.1);
      for(auto g : gammas1) {
        double E = g->energy(); 
        //double dR = ROOT::Math::VectorUtil::DeltaR(g->vector(),tau1->vector());
        double dR = ROOT::Math::VectorUtil::DeltaR(g->vector(),gammas1[0]->vector());
        //if(E/Epi0_1_>0.1) std::cout << cone_size << "    " << dR << "    " << E/Epi0_1_ << std::endl ; 
        if((E/Epi0_1_>0.1 && g->pt()>1 && dR<cone_size && seed_gammas.size()<2) || count_gammas==0) seed_gammas.push_back(g);  // tune threshold
        //if((E/Epi0_1_>0.1 && g->pt()>1 && seed_gammas.size()<2) || count_gammas==0) seed_gammas.push_back(g);  // tune threshold
        else other_gammas.push_back(g);
        count_gammas++;
        //if(seed_gammas.size()>=2) break; // 4
      }
      Nseeds_ = seed_gammas.size();

      std::vector<PFCandidate*> sel_gammas = {};
      for(auto g : gammas1) {
        double E = g->energy();
        if(E/Epi0_1_>0.1 && g->pt()>1) sel_gammas.push_back(g);  // tune threshold
        //else other_gammas.push_back(g);
      }
      Ngams_ = sel_gammas.size();

      ROOT::Math::PtEtaPhiEVector sum_vec;
      for(unsigned i=0; i<4 && i<Ngams_; ++i){
        sum_vec+=sel_gammas[i]->vector();
      }


      new_pi0_phi_ = sum_vec.Phi();
      new_pi0_eta_ = sum_vec.Rapidity();
      new_pi0_pt_ = sum_vec.Pt();
      new_pi0_E_ = sum_vec.E();

 //     double mass = 0.1349;
 //     double p = sqrt(Epi0_1_*Epi0_1_-mass*mass);
 //     theta = atan(exp(-new_pi0_eta_))*2;
 //     double pt = p*sin(theta);
 //
      //ROOT::Math::PtEtaPhiEVector sum_vec(pt, new_pi0_eta_, new_pi0_phi_,Epi0_1_);

      

      //ROOT::Math::PtEtaPhiEVector sum_vec;
      //for(unsigned i=0; i<4&&i<seed_gammas.size();++i) {
      //  sum_vec+=seed_gammas[0]->vector();  
      //}
      //new_pi0_eta = sum_vec.Rapidity();
      //new_pi0_phi = sum_vec.Phi();
    
      std::vector<std::vector<PFCandidate*>> sorted_gammas =  {{},{},{},{}};
//      // push seeds as first gammas in vectors 
//      for(unsigned i=0; i<4&&i<Nseeds_;++i) {
//        sorted_gammas[i].push_back(seed_gammas[i]);
//      }
//      // loop over remaining gammas and associate gammas based on dR*E
//      for(auto g : other_gammas) {
//        int min_j=-1;
//        double mindRE=-1;
//        for(unsigned j=0; j<4&&j<Nseeds_;++j) {
//          //double dRE = fabs((seed_gammas[j]->energy()+g->energy())*ROOT::Math::VectorUtil::DeltaR(seed_gammas[j]->vector(),g->vector()));
//          double seedE = 0;
//          for(auto x : sorted_gammas[j]) seedE+=x->energy();
//          double dRE = fabs((seedE+g->energy())*ROOT::Math::VectorUtil::DeltaR(seed_gammas[j]->vector(),g->vector()));
//          if(dRE<mindRE || min_j==-1) {
//            min_j=j; 
//            mindRE = dRE; 
//          }
//        }
//        sorted_gammas[min_j].push_back(g);
//      }


//      // push seeds as first gammas in vectors 
//      for(unsigned i=0; i<4&&i<Nseeds_;++i) {
//        sorted_gammas[i].push_back(seed_gammas[i]);
//      }
//      // loop over remaining gammas and associate gammas based on dR*E
//      for(auto g : other_gammas) {
//        int min_j=-1;
//        double mindRPt=-1;
//        for(unsigned j=0; j<4&&j<Nseeds_;++j) {
//          double seedPt = 0;
//          for(auto x : sorted_gammas[j]) seedPt+=x->pt();
//          double dRPt = fabs((seedPt+g->pt())*ROOT::Math::VectorUtil::DeltaR(seed_gammas[j]->vector(),g->vector()));
//          if(dRPt<mindRPt || min_j==-1) {
//            min_j=j;
//            mindRPt = dRPt;
//          }
//        }
//        sorted_gammas[min_j].push_back(g);
//      }

      for(unsigned i=0; i<4&&i<Nseeds_;++i) {
        sorted_gammas[i].push_back(seed_gammas[i]);
      }
//      // loop over remaining gammas and associate gammas based on dR*E
//      for(auto g : other_gammas) {
//        int min_j=-1;
//        double mindRPt=-1;
//        for(unsigned j=0; j<4&&j<Nseeds_;++j) {
//          double seedPt = 0;
//          for(auto x : sorted_gammas[j]) seedPt+=x->pt();
//          double dRPt = fabs((seedPt+g->pt())*ROOT::Math::VectorUtil::DeltaR(seed_gammas[j]->vector(),g->vector()));
//          if(dRPt<mindRPt || min_j==-1) {
//            min_j=j;
//            mindRPt = dRPt;
//          }
//        }
//        sorted_gammas[min_j].push_back(g);
//      }

// associate gammas pased on dR = dEta^2 + dPhi^2 normalized to the strip sizes
//            etaAssociationDistance = 0.20*pow(cands[i]->pt(),-0.66) + 0.20*pow(stripVector.Pt(),-0.66);
//            phiAssociationDistance = 0.35*pow(cands[i]->pt(),-0.71) + 0.35*pow(stripVector.Pt(),-0.71);
//            
      for(auto g : other_gammas) {
        int min_j=-1;
        double mindR=-1;
        double g_pt = g->pt();
        for(unsigned j=0; j<4&&j<Nseeds_;++j) {
          //double seedPt = seed_gammas[j]->pt();
          //double etaAssociationDistance = 0.20*pow(g_pt,-0.66) + 0.20*pow(seedPt,-0.66);
          //double phiAssociationDistance = 0.35*pow(g_pt,-0.71) + 0.35*pow(seedPt,-0.71); 
          double etaAssociationDistance = 0.20*pow(g_pt,-0.66);// + 0.20*pow(seedPt,-0.66);
          double phiAssociationDistance = 0.35*pow(g_pt,-0.71);// + 0.35*pow(seedPt,-0.71);
          //double etaAssociationDistance = 1., phiAssociationDistance = 1.;
          double dphi = fabs(ROOT::Math::VectorUtil::DeltaPhi(seed_gammas[j]->vector(),g->vector()));
          double deta = fabs(seed_gammas[j]->eta()-g->eta());
          double dR = sqrt(pow(dphi/phiAssociationDistance,2) + pow(deta/etaAssociationDistance,2));//*(seedPt+g_pt);
          if(dR<mindR || min_j==-1) {
            min_j=j;
            mindR = dR;
          }
        }
        sorted_gammas[min_j].push_back(g);
      }

      new_pi0_E_1_ = 0., new_pi0_E_2_ = 0., new_pi0_E_3_ =0., new_pi0_E_4_=0.;

//std::cout << "---------------" << std::endl;
      for(auto g : sorted_gammas[0]) {
        new_pi0_E_1_+=g->energy();
        //std::cout << new_pi0_E_1_ << std::endl;
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

      new_pi0_pt_1_ = Cluster(sorted_gammas[0]).Pt();
      new_pi0_pt_2_ = Cluster(sorted_gammas[1]).Pt();
      new_pi0_pt_3_ = Cluster(sorted_gammas[2]).Pt();
      new_pi0_pt_4_ = Cluster(sorted_gammas[3]).Pt();

      new_pi0_eta_1_ = Cluster(sorted_gammas[0]).Rapidity();
      new_pi0_eta_2_ = Cluster(sorted_gammas[1]).Rapidity();
      new_pi0_eta_3_ = Cluster(sorted_gammas[2]).Rapidity();
      new_pi0_eta_4_ = Cluster(sorted_gammas[3]).Rapidity();

      new_pi0_phi_1_ = Cluster(sorted_gammas[0]).Phi();
      new_pi0_phi_2_ = Cluster(sorted_gammas[1]).Phi();
      new_pi0_phi_3_ = Cluster(sorted_gammas[2]).Phi();
      new_pi0_phi_4_ = Cluster(sorted_gammas[3]).Phi();

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

      std::vector<std::pair<PFCandidate*,std::vector<PFCandidate*>>> kt_clusters = ClusterGammas(gammas1, 1);
      if(kt_clusters.size()>0) lead_cluster_pt_ = kt_clusters[0].first->pt();
      else lead_cluster_pt_ = -9999;
      if(kt_clusters.size()>1) sublead_cluster_pt_ = kt_clusters[1].first->pt();
      else sublead_cluster_pt_ = -9999;

      if(kt_clusters.size()>0) lead_cluster_E_ = kt_clusters[0].first->energy();
      else lead_cluster_E_ = -9999;
      if(kt_clusters.size()>1) sublead_cluster_E_ = kt_clusters[1].first->energy();
      else sublead_cluster_E_ = -9999;


      std::vector<std::pair<PFCandidate*,std::vector<PFCandidate*>>> kt_clusters_new = ClusterGammasNew(gammas1, 1);
      if(kt_clusters_new.size()>0) lead_cluster_pt_new_ = kt_clusters_new[0].first->pt();
      else lead_cluster_pt_new_ = -9999;
      if(kt_clusters_new.size()>1) sublead_cluster_pt_new_ = kt_clusters_new[1].first->pt();
      else sublead_cluster_pt_new_ = -9999;

      if(kt_clusters_new.size()>0) lead_cluster_E_new_ = kt_clusters_new[0].first->energy();
      else lead_cluster_E_new_ = -9999;
      if(kt_clusters_new.size()>1) sublead_cluster_E_new_ = kt_clusters_new[1].first->energy();
      else sublead_cluster_E_new_ = -9999;

      a1_rho_mass_1_=-1, a1_rho_mass_2_=-1;
      if(Nseeds_==2) {
        a1_rho_mass_1_ = (pi_1->vector()+Cluster(sorted_gammas[0])).M();
        a1_rho_mass_2_ = (pi_1->vector()+Cluster(sorted_gammas[1])).M();
        if(fabs(a1_rho_mass_1_-0.77)>fabs(a1_rho_mass_2_-0.77)) {
          float temp = a1_rho_mass_1_;
          a1_rho_mass_1_ = a1_rho_mass_2_;
          a1_rho_mass_2_ = temp;
        }

        // match seeds to gen pi0s / gen gammas
        // genpi0s
        // gengammas
        auto lead_seed = Cluster(sorted_gammas[0]);
        auto sublead_seed = Cluster(sorted_gammas[1]);
        if(sublead_seed.E()>lead_seed.E()) {
          lead_seed = Cluster(sorted_gammas[1]);
          sublead_seed = Cluster(sorted_gammas[0]);
        }
        if(gengammas.size()>=1) dR_match_gamma1_ = ROOT::Math::VectorUtil::DeltaR(lead_seed, gengammas[0]->vector());
        if(gengammas.size()>=2) dR_match_gamma2_ = ROOT::Math::VectorUtil::DeltaR(sublead_seed, gengammas[1]->vector());
        if(genpi0s.size()>=1) dR_match_pi01_ = ROOT::Math::VectorUtil::DeltaR(lead_seed, genpi0s[0]->vector());
        if(genpi0s.size()>=2) dR_match_pi02_ = ROOT::Math::VectorUtil::DeltaR(sublead_seed, genpi0s[1]->vector());

        if(gengammas.size()>=2) dR_match_gamma1_wrong_ = ROOT::Math::VectorUtil::DeltaR(lead_seed, gengammas[1]->vector());
        if(gengammas.size()>=1) dR_match_gamma2_wrong_ = ROOT::Math::VectorUtil::DeltaR(sublead_seed, gengammas[0]->vector());
        if(genpi0s.size()>=2) dR_match_pi01_wrong_ = ROOT::Math::VectorUtil::DeltaR(lead_seed, genpi0s[1]->vector());
        if(genpi0s.size()>=1) dR_match_pi02_wrong_ = ROOT::Math::VectorUtil::DeltaR(sublead_seed, genpi0s[0]->vector());
      }
//      double subvar(std::vector<std::vector<PFCandidate*>> sorted_gammas){
//        double tot_pt=0.;
//        double subvar=0.;
//        for(unsigned i=0; i<sorted_gammas.size();++i) {
//          auto seed = sorted_gammas[i];
//        }
//
//      }

      //if(tau_decay_mode_1_==1) {
      //  std::cout << tauFlag1_ << "    " << Nseeds_ << "    " << gammas1.size() << "    " << sorted_gammas[0].size() << "    " << sorted_gammas[1].size() << std::endl;  
      //}


      Epi_1_=pi_1->energy(); Phipi_1_=pi_1->phi(); Etapi_1_=pi_1->eta(); Mpi_1_=pi_1->M(); Pxpi_1_=pi_1->vector().Px(); Pypi_1_=pi_1->vector().Py(); Pzpi_1_=pi_1->vector().Pz(); 
      Etau_1_=tau1->energy(); Phitau_1_=tau1->phi(); Etatau_1_=tau1->eta(); Mtau_1_=tau1->M(); Pxtau_1_=tau1->vector().Px(); Pytau_1_=tau1->vector().Py(); Pztau_1_=tau1->vector().Pz();

      if(Ngammas_1_>=1) {Egammas_1_=gammas1[0]->energy(); Phigammas_1_=gammas1[0]->phi(); Etagammas_1_=gammas1[0]->eta(); Mgammas_1_=gammas1[0]->M(); Pxgammas_1_=gammas1[0]->vector().Px(); Pygammas_1_=gammas1[0]->vector().Py(); Pzgammas_1_=gammas1[0]->vector().Pz();}
      else {Egammas_1_=0.; Phigammas_1_=0.; Etagammas_1_=0.; Mgammas_1_=0.; Pxgammas_1_=0.; Pygammas_1_=0.; Pzgammas_1_=0.;}

      std::pair<ic::Candidate*, ic::Candidate*> rho = GetRho(tau1, pfcands);
      ic::Candidate *pi_tau2 = rho.first;
      ic::Candidate *pi0_tau2 = rho.second;
      rho_dphi_  = ROOT::Math::VectorUtil::DeltaPhi(pi0_tau2->vector(),pi_tau2->vector());
      corr_rho_dphi_ = (fabs(rho_dphi_)<0.1) ? quantile_mapping(rho_dphi_, input_cdf_, output_cdf_) : rho_dphi_;
      if (rho_dphi_<-0.15) corr_rho_dphi_ =-0.15; 
      if (rho_dphi_>0.15) corr_rho_dphi_ =0.15;
      corr_Phigammas_1_ = Phigammas_1_+(corr_rho_dphi_-rho_dphi_);
      gen_rho_dphi_=-9999;
      if(genpi0s.size()==1) {
        gen_rho_dphi_  = ROOT::Math::VectorUtil::DeltaPhi(genpi0s[0]->vector(),pi_tau2->vector());
      }

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
