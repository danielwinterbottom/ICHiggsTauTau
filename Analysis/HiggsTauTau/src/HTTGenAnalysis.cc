#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTGenAnalysis.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/PolarimetricA1.h"

std::vector<ic::GenParticle> FamilyTree (std::vector<ic::GenParticle> &v, ic::GenParticle p, std::vector<ic::GenParticle*> gen_particles, unsigned &outputID){ 
  if(p.daughters().size() == 0){
    unsigned ID = std::fabs(p.pdgid());
    if(ID == 11) outputID = 11;
    else if(ID == 13) outputID = 13;
    if(!(ID==12 || ID==14 || ID==16)){
      v.push_back(p);
    }
  }
  else{
    for(size_t i = 0; i < p.daughters().size(); ++i ){
      ic::GenParticle d = *gen_particles[p.daughters().at(i)];
      FamilyTree(v,d, gen_particles, outputID);
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

struct PtComparatorGenPart{
  bool operator() (ic::GenParticle *a, ic::GenParticle *b) {
    return (a->vector().Pt() > b->vector().Pt());
  }
};

  
namespace ic {

  HTTGenAnalysis::HTTGenAnalysis(std::string const& name) : ModuleBase(name) {
    fs_ = NULL;
    bbtag_eff_ = nullptr;
  }

  HTTGenAnalysis::~HTTGenAnalysis() {
    ;
  }

  int HTTGenAnalysis::PreAnalysis() {
    TFile f("input/mssm_higgspt/higgs_pt_v2_mssm_mode.root");
    mssm_w_ = std::shared_ptr<RooWorkspace>((RooWorkspace*)gDirectory->Get("w"));
    f.Close();
    std::string mass_str = mssm_mass_;
    if(mssm_w_->function(("h_"+mass_str+"_t_ratio").c_str())){
      fns_["h_t_ratio"] = std::shared_ptr<RooFunctor>(mssm_w_->function(("h_"+mass_str+"_t_ratio").c_str())->functor(mssm_w_->argSet("h_pt")));        
      fns_["h_b_ratio"] = std::shared_ptr<RooFunctor>(mssm_w_->function(("h_"+mass_str+"_b_ratio").c_str())->functor(mssm_w_->argSet("h_pt")));
      fns_["h_i_ratio"] = std::shared_ptr<RooFunctor>(mssm_w_->function(("h_"+mass_str+"_i_ratio").c_str())->functor(mssm_w_->argSet("h_pt")));
      fns_["H_t_ratio"] = std::shared_ptr<RooFunctor>(mssm_w_->function(("H_"+mass_str+"_t_ratio").c_str())->functor(mssm_w_->argSet("h_pt")));
      fns_["H_b_ratio"] = std::shared_ptr<RooFunctor>(mssm_w_->function(("H_"+mass_str+"_b_ratio").c_str())->functor(mssm_w_->argSet("h_pt")));
      fns_["H_i_ratio"] = std::shared_ptr<RooFunctor>(mssm_w_->function(("H_"+mass_str+"_i_ratio").c_str())->functor(mssm_w_->argSet("h_pt")));
      fns_["A_t_ratio"] = std::shared_ptr<RooFunctor>(mssm_w_->function(("A_"+mass_str+"_t_ratio").c_str())->functor(mssm_w_->argSet("h_pt")));
      fns_["A_b_ratio"] = std::shared_ptr<RooFunctor>(mssm_w_->function(("A_"+mass_str+"_b_ratio").c_str())->functor(mssm_w_->argSet("h_pt")));
      fns_["A_i_ratio"] = std::shared_ptr<RooFunctor>(mssm_w_->function(("A_"+mass_str+"_i_ratio").c_str())->functor(mssm_w_->argSet("h_pt"))); 
    }

      
    rand = new TRandom3(0);
    if(fs_){  
      outtree_ = fs_->make<TTree>("gen_ntuple","gen_ntuple");
      outtree_->Branch("event"       , &event_       );
      outtree_->Branch("wt"       , &wt_       );
      outtree_->Branch("wt_z_pol", &wt_z_pol_);
      outtree_->Branch("wt_stitch"       , &wt_stitch_       );
      outtree_->Branch("wt_topmass"       , &wt_topmass_       );
      outtree_->Branch("wt_topmass_2"     , &wt_topmass_2_  );
      outtree_->Branch("wt_ps_down", &wt_ps_down_);
      outtree_->Branch("wt_ps_up", &wt_ps_up_);
      outtree_->Branch("wt_ue_down", &wt_ue_down_);
      outtree_->Branch("wt_ue_up", &wt_ue_up_);
      outtree_->Branch("npNLO", &npNLO_);
      outtree_->Branch("cand_1"       , &cand_1_       );
      outtree_->Branch("cand_2"       , &cand_2_       );
      outtree_->Branch("match_1"       , &match_1_       );
      outtree_->Branch("match_2"       , &match_2_       );

      outtree_->Branch("wt_cp_sm"       , &wt_cp_sm_);
      outtree_->Branch("wt_cp_ps"       , &wt_cp_ps_);
      outtree_->Branch("wt_cp_mm"       , &wt_cp_mm_);

      outtree_->Branch("Ediff_1"       , &Ediff_1_);
      outtree_->Branch("Ediff_2"       , &Ediff_2_);

      outtree_->Branch("fakea1_dR", &fakea1_dR_);
      outtree_->Branch("rho_dR", &rho_dR_);
      outtree_->Branch("tauFlag", &tauFlag);

      if(do_theory_uncert_){
        outtree_->Branch("wt_mur1_muf1",    &scale1_);
        outtree_->Branch("wt_mur1_muf2",    &scale2_);
        outtree_->Branch("wt_mur1_muf0p5",  &scale3_);
        outtree_->Branch("wt_mur2_muf1",    &scale4_);
        outtree_->Branch("wt_mur2_muf2",    &scale5_);
        outtree_->Branch("wt_mur2_muf0p5",  &scale6_);
        outtree_->Branch("wt_mur0p5_muf1",  &scale7_);
        outtree_->Branch("wt_mur0p5_muf2",  &scale8_);
        outtree_->Branch("wt_mur0p5_muf0p5",&scale9_);
      }
      outtree_->Branch("passed"       ,&passed_       );
      outtree_->Branch("pt_1"        , &pt_1_        );
      outtree_->Branch("pt_2"        , &pt_2_        );
      outtree_->Branch("eta_1"       , &eta_1_       );
      outtree_->Branch("eta_2"       , &eta_2_       );
      outtree_->Branch("phi_1"       , &phi_1_       );
      outtree_->Branch("phi_2"       , &phi_2_       );
      outtree_->Branch("met"         , &met_         );
      outtree_->Branch("m_vis"       , &m_vis_       );
      outtree_->Branch("pt_tt"       , &pt_tt_       );
      outtree_->Branch("mass"       , &mass_       );
      outtree_->Branch("wtzpt"       , &wtzpt_       );
      outtree_->Branch("mt_1"        , &mt_1_        );
      outtree_->Branch("mt_2"        , &mt_2_        );
      outtree_->Branch("pzeta"       , &pzeta_       );
      outtree_->Branch("n_bjets"     , &n_bjets_     );
      outtree_->Branch("n_bjets_noscale"     , &n_bjets_noscale_);
      outtree_->Branch("n_jets"      , &n_jets_      );
      outtree_->Branch("n_jets_nofilter"      , &n_jets_nofilter_);
      outtree_->Branch("n_jetsingap" , &n_jetsingap_ );
      outtree_->Branch("jpt_1"       , &jpt_1_       );
      outtree_->Branch("jpt_2"       , &jpt_2_       );
      outtree_->Branch("jpt_3"       , &jpt_3_       );
      outtree_->Branch("jeta_1"      , &jeta_1_      );
      outtree_->Branch("jeta_2"      , &jeta_2_      );
      outtree_->Branch("jphi_1"      , &jphi_1_      );
      outtree_->Branch("jphi_2"      , &jphi_2_      );
      outtree_->Branch("mjj"         , &mjj_         );
      outtree_->Branch("largest_gen_mjj",   &largest_gen_mjj_);
      outtree_->Branch("jdeta"       , &jdeta_       );
      outtree_->Branch("higgsDecay"  , &decayType    );
      outtree_->Branch("genpt_1"     , &genpt_1_        );
      outtree_->Branch("genpt_2"     , &genpt_2_        );
      outtree_->Branch("geneta_2"    , &geneta_2_       );
      outtree_->Branch("geneta_1"    , &geneta_1_       );
      outtree_->Branch("HiggsPt"     , &HiggsPt_     );
      outtree_->Branch("partons"     , &partons_);
      outtree_->Branch("partons_lhe"     , &partons_lhe_);
      outtree_->Branch("parton_pt"     , &parton_pt_);
      outtree_->Branch("parton_pt_2"     , &parton_pt_2_);
      outtree_->Branch("parton_pt_3"     , &parton_pt_3_);
      outtree_->Branch("parton_mjj",    &parton_mjj_);
      outtree_->Branch("parton_HpT", &parton_HpT_);
      outtree_->Branch("D0"     , &D0_);
      outtree_->Branch("D0star"     , &D0star_);
      outtree_->Branch("DCP"     , &DCP_);
      outtree_->Branch("sjdphi"     , &sjdphi_);
      outtree_->Branch("spjdphi"     , &spjdphi_);
      outtree_->Branch("ysep"     , &ysep_);
      outtree_->Branch("n_pjets"     , &n_pjets_);
      outtree_->Branch("n_pu",      &n_pu_);
      outtree_->Branch("lead_b_pt", &lead_b_pt_);
      outtree_->Branch("lead_b_eta", &lead_b_eta_); 
      outtree_->Branch("aco_angle_1", &aco_angle_1_);
      outtree_->Branch("aco_angle_2", &aco_angle_2_);
      outtree_->Branch("aco_angle_3", &aco_angle_3_);
      outtree_->Branch("aco_angle_4", &aco_angle_4_);
      outtree_->Branch("p_aco_angle_1", &p_aco_angle_1_);
      outtree_->Branch("p_aco_angle_2", &p_aco_angle_2_);
      outtree_->Branch("p_aco_angle_3", &p_aco_angle_3_);
      outtree_->Branch("p_aco_angle_4", &p_aco_angle_4_);
      outtree_->Branch("dR1", &dR1_);
      outtree_->Branch("dR2", &dR2_); 
      outtree_->Branch("aco_angle_5", &aco_angle_5_);
      outtree_->Branch("aco_angle_6", &aco_angle_6_);
      outtree_->Branch("cp_sign_1",     &cp_sign_1_);
      outtree_->Branch("cp_sign_2",     &cp_sign_2_);
      outtree_->Branch("cp_sign_3",     &cp_sign_3_);
      outtree_->Branch("cp_sign_4",     &cp_sign_4_);
      outtree_->Branch("cp_sign_5",     &cp_sign_5_);
      outtree_->Branch("cp_sign_6",     &cp_sign_6_);
      outtree_->Branch("rand", &rand_);
      outtree_->Branch("y_1_1",     &y_1_1_);
      outtree_->Branch("y_2_1",     &y_3_1_);
      outtree_->Branch("y_3_1",     &y_4_1_);
      outtree_->Branch("y_4_1",     &y_4_1_);
      outtree_->Branch("y_1_2",     &y_1_2_);
      outtree_->Branch("y_2_2",     &y_3_2_);
      outtree_->Branch("y_3_2",     &y_4_2_);
      outtree_->Branch("y_4_2",     &y_4_2_);

      outtree_->Branch("a1_mass", &a1_mass_);
      outtree_->Branch("E_1_1", &E_1_1_);
      outtree_->Branch("E_2_1", &E_2_1_);
      outtree_->Branch("E_2_2", &E_1_2_);
      outtree_->Branch("E_2_2", &E_2_2_);
      outtree_->Branch("E_3_2", &E_3_2_);


      outtree_->Branch("cp_channel",    &cp_channel_);
      outtree_->Branch("pi0_E_1", &pi0_E_1_);
      outtree_->Branch("pi0_E_2", &pi0_E_2_);
      outtree_->Branch("pi_E_1", &pi_E_1_);
      outtree_->Branch("pi_E_2", &pi_E_2_);
      outtree_->Branch("mass1", &mass1_);
      outtree_->Branch("mass2", &mass2_);    
      outtree_->Branch("pt_rho_pi", &pt_rho_pi_);
      outtree_->Branch("pt_rho_pi0", &pt_rho_pi0_);
      outtree_->Branch("pt_other_pi0", &pt_other_pi0_); 
 
      outtree_->Branch("wt_ggh_t", &wt_ggh_t_);
      outtree_->Branch("wt_ggh_b", &wt_ggh_b_);
      outtree_->Branch("wt_ggh_i", &wt_ggh_i_);
      outtree_->Branch("wt_ggH_t", &wt_ggH_t_);
      outtree_->Branch("wt_ggH_b", &wt_ggH_b_);
      outtree_->Branch("wt_ggH_i", &wt_ggH_i_);
      outtree_->Branch("wt_ggA_t", &wt_ggA_t_);
      outtree_->Branch("wt_ggA_b", &wt_ggA_b_);
      outtree_->Branch("wt_ggA_i", &wt_ggA_i_);
      outtree_->Branch("wt_ggA_i", &wt_ggA_i_);
      outtree_->Branch("pT_A", &pT_A_);

      outtree_->Branch("reco_pvx", &reco_pvx_);
      outtree_->Branch("reco_pvy", &reco_pvy_);
      outtree_->Branch("reco_pvz", &reco_pvz_);
      outtree_->Branch("gen_pvx",  &gen_pvx_);
      outtree_->Branch("gen_pvy",  &gen_pvy_);
      outtree_->Branch("gen_pvz",  &gen_pvz_);

      outtree_->Branch("wt_ps_isr_up" , &wt_ps_isr_up_ );
      outtree_->Branch("wt_ps_isr_down" , &wt_ps_isr_down_ );
      outtree_->Branch("wt_ps_fsr_up" , &wt_ps_fsr_up_ );
      outtree_->Branch("wt_ps_fsr_down" , &wt_ps_fsr_down_ );
      
    }
    count_ee_ = 0;
    count_em_ = 0;
    count_et_ = 0;
    count_mm_ = 0;
    count_mt_ = 0;
    count_tt_ = 0;

    GetFromTFile<TH2D>("input/zpt_weights/dy_weights_2017.root","/","zptmass_histo").Copy(z_pt_weights_sm_);
    topmass_wts_ = GetFromTFile<TH1F>("input/ggh_weights/top_mass_weights.root","/","pt_weight");

    /* topmass_wts_toponly_ = GetFromTFile<TH1F>("input/ggh_weights/quarkmass_uncerts_hnnlo.root","/","nom"); */
   
    /* ps_0jet_up_ = GetFromTFile<TH1D>("input/ggh_weights/MG_ps_uncerts.root","/","ps_0jet_up"); */
    /* ps_0jet_down_ = GetFromTFile<TH1D>("input/ggh_weights/MG_ps_uncerts.root","/","ps_0jet_down"); */
    /* ps_1jet_up_ = GetFromTFile<TH1D>("input/ggh_weights/MG_ps_uncerts.root","/","ps_1jet_up"); */
    /* ps_1jet_down_ = GetFromTFile<TH1D>("input/ggh_weights/MG_ps_uncerts.root","/","ps_1jet_down"); */
    /* ps_2jet_up_ = GetFromTFile<TH1D>("input/ggh_weights/MG_ps_uncerts.root","/","ps_2jet_up"); */
    /* ps_2jet_down_ = GetFromTFile<TH1D>("input/ggh_weights/MG_ps_uncerts.root","/","ps_2jet_down"); */
    /* ps_3jet_up_ = GetFromTFile<TH1D>("input/ggh_weights/MG_ps_uncerts.root","/","ps_3jet_up"); */
    /* ps_3jet_down_ = GetFromTFile<TH1D>("input/ggh_weights/MG_ps_uncerts.root","/","ps_3jet_down"); */   
    /* ue_up_ = GetFromTFile<TH1D>("input/ggh_weights/MG_ps_uncerts.root","/","ue_up"); */
    /* ue_down_ = GetFromTFile<TH1D>("input/ggh_weights/MG_ps_uncerts.root","/","ue_down"); */ 
    return 0;
  }

  int HTTGenAnalysis::Execute(TreeEvent *event) {
    
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    event_ = (unsigned long long) eventInfo->event();
    wt_ = 1;
    rand->SetSeed(event_);
    rand_ = rand->Uniform();   
 
    wt_ = eventInfo->total_weight();



    wt_cp_sm_=0.0; 
    wt_cp_ps_=0.0; 
    wt_cp_mm_=0.0; 

    // test tauspinner weights
    if(event->ExistsInTree("tauspinner")){
      EventInfo const* tauspinner = event->GetPtr<EventInfo>("tauspinner");

      if (tauspinner->weight_defined("wt_z_pol")) wt_z_pol_ = tauspinner->weight("wt_z_pol"); else wt_z_pol_=1.0;

      if (tauspinner->weight_defined("wt_cp_0")){
        wt_cp_sm_ = tauspinner->weight("wt_cp_0");
        wt_cp_ps_ = tauspinner->weight("wt_cp_0p5"); 
        wt_cp_mm_ = tauspinner->weight("wt_cp_0p25");
      }
      if (eventInfo->weight_defined("sm_weight_nlo")){
        wt_cp_sm_ = eventInfo->weight("sm_weight_nlo");
        wt_cp_ps_ = eventInfo->weight("ps_weight_nlo");
        wt_cp_mm_ = eventInfo->weight("mm_weight_nlo");
      }
 

    }

    wt_ps_isr_down_ = eventInfo->weight_defined("genweight8") ? eventInfo->weight("genweight8") : 1.0;
    wt_ps_fsr_up_   = eventInfo->weight_defined("genweight7") ? eventInfo->weight("genweight7") : 1.0;
    wt_ps_fsr_down_ = eventInfo->weight_defined("genweight9") ? eventInfo->weight("genweight9") : 1.0;

    if(do_theory_uncert_){
      // note some of these labels may be generator dependent so need to make sure you check before using them
      if(eventInfo->weight_defined("1001")) scale1_ = eventInfo->weight("1001"); else scale1_=1.0;
      if(eventInfo->weight_defined("1002")) scale2_ = eventInfo->weight("1002"); else scale2_=1.0;
      if(eventInfo->weight_defined("1003")) scale3_ = eventInfo->weight("1003"); else scale3_=1.0;
      if(eventInfo->weight_defined("1004")) scale4_ = eventInfo->weight("1004"); else scale4_=1.0;
      if(eventInfo->weight_defined("1005")) scale5_ = eventInfo->weight("1005"); else scale5_=1.0;
      if(eventInfo->weight_defined("1006")) scale6_ = eventInfo->weight("1006"); else scale6_=1.0;
      if(eventInfo->weight_defined("1007")) scale7_ = eventInfo->weight("1007"); else scale7_=1.0;
      if(eventInfo->weight_defined("1008")) scale8_ = eventInfo->weight("1008"); else scale8_=1.0;
      if(eventInfo->weight_defined("1009")) scale9_ = eventInfo->weight("1009"); else scale9_=1.0; 

      // For MG5 samples:
      //  <weight MUF="1.0" MUR="2.0" PDF="292200" id="1002"> MUR=2.0  </weight>
      //  <weight MUF="1.0" MUR="0.5" PDF="292200" id="1003"> MUR=0.5  </weight>
      //  <weight MUF="2.0" MUR="1.0" PDF="292200" id="1004"> MUF=2.0  </weight>
      //  <weight MUF="2.0" MUR="2.0" PDF="292200" id="1005"> MUR=2.0 MUF=2.0  </weight>
      //  <weight MUF="2.0" MUR="0.5" PDF="292200" id="1006"> MUR=0.5 MUF=2.0  </weight>
      //  <weight MUF="0.5" MUR="1.0" PDF="292200" id="1007"> MUF=0.5  </weight>
      //  <weight MUF="0.5" MUR="2.0" PDF="292200" id="1008"> MUR=2.0 MUF=0.5  </weight>
      //  <weight MUF="0.5" MUR="0.5" PDF="292200" id="1009"> MUR=0.5 MUF=0.5  </weight>
      //

      // for NNLOPS:
      //  <weight id="1001"> muR=1 muF=1 </weight>
      //  <weight id="1002"> muR=1 muF=2 </weight>
      //  <weight id="1003"> muR=1 muF=0.5 </weight>
      //  <weight id="1004"> muR=2 muF=1 </weight>
      //  <weight id="1005"> muR=2 muF=2 </weight>
      //  <weight id="1006"> muR=2 muF=0.5 </weight>
      //  <weight id="1007"> muR=0.5 muF=1 </weight>
      //  <weight id="1008"> muR=0.5 muF=2 </weight>
      //  <weight id="1009"> muR=0.5 muF=0.5 </weight>

    }
    
    if (event->Exists("D0")) D0_ = event->Get<float>("D0");
    else D0_ = -9999;
    if (event->Exists("DCP")) DCP_ = event->Get<float>("DCP");
    else DCP_ = -9999; 
    if(D0_!=-9999&&DCP_!=-9999) D0star_ = D0_*DCP_/fabs(DCP_);
    else D0star_ = -9999;
    
    std::vector<ic::GenParticle*> gen_particles = event->GetPtrVec<ic::GenParticle>("genParticles");
    std::vector<ic::GenJet*> gen_jets = event->GetPtrVec<ic::GenJet>("genJets");

    std::vector<ic::GenParticle> higgs_products;
    std::vector<ic::GenParticle> gen_taus;
    ic::Candidate met; 
    std::vector<ic::GenParticle> prompt_leptons;
    std::vector<std::string> decay_types;

    double pT=0;
    HiggsPt_=-9999;
    partons_lhe_=0;
    partons_=0;
    parton_mjj_=-9999;
    parton_HpT_=-9999;
    double higgs_eta = 0;
    std::vector<double> parton_pt_vec = {};
    bool lhe_exists = event->ExistsInTree("lheParticles");
    if(lhe_exists){
      std::vector<GenParticle*> const& lhe_parts = event->GetPtrVec<GenParticle>("lheParticles");
      parton_pt_=-9999;
      std::vector<GenParticle*> outparts;
      double largest_b_pt=-1;
      double lead_b_eta=-9999;
      for(unsigned i = 0; i< lhe_parts.size(); ++i){
           if(lhe_parts[i]->status() != 1) continue;
           unsigned id = abs(lhe_parts[i]->pdgid());

           if(id==5 && lhe_parts[i]->pt()>largest_b_pt) {
             largest_b_pt = lhe_parts[i]->pt();
             lead_b_eta=lhe_parts[i]->eta();
           }
           if(id==25) parton_HpT_ = lhe_parts[i]->pt();
           if ((id >= 1 && id <=6) || id == 21){ 
             outparts.push_back(lhe_parts[i]);
             partons_++;
             parton_pt_vec.push_back(lhe_parts[i]->pt());
             if(lhe_parts[i]->pt()>=10) partons_lhe_++; 
        }
      }
      lead_b_pt_ = largest_b_pt;
      lead_b_eta_ = lead_b_eta;
      std::sort(outparts.begin(),outparts.end(),PtComparatorGenPart());
      if(outparts.size()>1) parton_mjj_ = (outparts[0]->vector()+outparts[1]->vector()).M();
      else parton_mjj_ = -9999;
      if(outparts.size()>2){
        double parton_mjj_2 = (outparts[0]->vector()+outparts[2]->vector()).M(); 
        double parton_mjj_3 = (outparts[1]->vector()+outparts[2]->vector()).M();  
        if(parton_mjj_ < std::max(parton_mjj_2, parton_mjj_3)) parton_mjj_ = std::max(parton_mjj_2, parton_mjj_3);
   
      }
    }
    std::sort(parton_pt_vec.begin(),parton_pt_vec.end());
    std::reverse(parton_pt_vec.begin(),parton_pt_vec.end());
    if (parton_pt_vec.size()>0) parton_pt_ = parton_pt_vec[0];
    else parton_pt_ = -9999;
    if (parton_pt_vec.size()>1) parton_pt_2_ = parton_pt_vec[1];
    else parton_pt_2_ = -9999;
    if (parton_pt_vec.size()>2) parton_pt_3_ = parton_pt_vec[2];
    else parton_pt_3_ = -9999;

    npNLO_ = eventInfo->npNLO();
    //if(npNLO_<0) npNLO_ = 2; 
    double n_inc_ = 3089015.;
    double n2_    = 14254055;
    double f2_   = 0.279662;
    if(npNLO_>=2) wt_stitch_ = (n_inc_*f2_) / ( (n_inc_*f2_) + n2_ );
    else wt_stitch_=1.;
    
    for(unsigned i=0; i<gen_particles.size(); ++i){
      if((gen_particles[i]->statusFlags()[FromHardProcessBeforeFSR] || gen_particles[i]->statusFlags()[IsLastCopy]) && gen_particles[i]->pdgid() == 25) {
          HiggsPt_ = gen_particles[i]->pt();
           higgs_eta = gen_particles[i]->eta();
           wt_topmass_ = topmass_wts_toponly_.GetBinContent(topmass_wts_toponly_.FindBin(HiggsPt_))*1.006;
           wt_topmass_2_ = topmass_wts_.GetBinContent(topmass_wts_.FindBin(HiggsPt_))*0.985; //*sm = 0.985022, mix= 0.985167 ps=0.985076 -> all = 0.985 to 3dp so use thsi number
      }

      
      ic::GenParticle part = *gen_particles[i];
      ic::GenParticle higgs_product;
      
      unsigned genID = std::fabs(part.pdgid());
      bool status_flag_t = part.statusFlags().at(0);
      bool status_flag_tlc = part.statusFlags().at(13);
      bool status_hard_process = part.statusFlags().at(7);
      
      if (!lhe_exists && status_hard_process &&(genID == 1 || genID == 2 || genID == 3 || genID == 4 || genID == 5 || genID == 6 || genID == 21) && gen_particles[part.mothers().at(0)]->pdgid() != 2212 ) partons_++;

      //if(genID==25 || genID==36 || genID==35 ) std::cout << genID << std::endl;
 
      if(genID==36 && gen_particles[i]->statusFlags()[IsLastCopy]){
        pT = gen_particles[i]->vector().Pt();
        pT_A_ = pT;
      }
      if(genID==25 && gen_particles[i]->statusFlags()[IsLastCopy]){
        pT = gen_particles[i]->vector().Pt();
        pT_A_ = pT;
      }

      std::vector<ic::Electron*> reco_electrons = {};//event->GetPtrVec<ic::Electron>("electrons");
      std::vector<ic::Muon*> reco_muons = {};//event->GetPtrVec<ic::Muon>("muons");     

      // add neutrinos 4-vectors to get gen met
      if(genID == 12 || genID == 14 || genID == 16){
        met.set_vector(met.vector() + part.vector());
        continue;
      }
      if(channel_str_=="zmm") {
        if(!(genID == 13 && gen_particles[i]->statusFlags()[IsPrompt] && gen_particles[i]->statusFlags()[IsLastCopy])) continue;
        higgs_products.push_back(*(gen_particles[i]));
        decay_types.push_back("m");
        std::vector<ic::GenParticle *> match_muons = {gen_particles[i]};
        if(fabs(gen_particles[i]->eta())<2.4 && gen_particles[i]->pt()>20.) {
          if(decay_types.size()==1){
            cand_1_ = true;
            match_1_ = (MatchByDR(match_muons,reco_muons,0.5,true,true).size()>0);
          } else if (decay_types.size()==2) {
            cand_2_ = true;
            match_2_ = (MatchByDR(match_muons,reco_muons,0.5,true,true).size()>0);
          }
        }
      }
      if(channel_str_=="zee") {
        if(!(genID == 11 && gen_particles[i]->statusFlags()[IsPrompt] && gen_particles[i]->statusFlags()[IsLastCopy])) continue;
        higgs_products.push_back(*(gen_particles[i]));
        decay_types.push_back("e");
        std::vector<ic::GenParticle *> match_elecs = {gen_particles[i]};
        if(fabs(gen_particles[i]->eta())<2.5 && gen_particles[i]->pt()>20.) {
          if(decay_types.size()==1){
            cand_1_ = true;
            match_1_ = (MatchByDR(match_elecs,reco_electrons,0.5,true,true).size()>0);
          } else if (decay_types.size()==2) {
            cand_2_ = true;
            match_2_ = (MatchByDR(match_elecs,reco_electrons,0.5,true,true).size()>0); 
          }
        }
      }
      if(!(genID == 15 && status_flag_t && status_flag_tlc)) continue;
      gen_taus.push_back(part);
      std::vector<ic::GenParticle> family;
      unsigned outputID = 15;
      FamilyTree(family, part, gen_particles, outputID);
      if(family.size()==1 && (outputID ==11 || outputID ==13)){
        higgs_products.push_back(family[0]);
        if (outputID == 11) {decay_types.push_back("e");}  
        else if (outputID == 13) {decay_types.push_back("m");}
      } else {
        decay_types.push_back("t");  
        ic::GenParticle had_tau;
        int charge = 0;
        int pdgid = 15;
        for(unsigned j=0; j<family.size(); ++j){
          had_tau.set_vector(had_tau.vector() + family[j].vector());
          charge += family[j].charge();
        }
        pdgid = 15*charge;
        had_tau.set_charge(charge);
        had_tau.set_pdgid(pdgid);
        higgs_products.push_back(had_tau);
      }
    }

    std::sort(higgs_products.begin(),higgs_products.end(),PtComparator());
    std::sort(gen_taus.begin(),gen_taus.end(),PtComparator());
    
    if(gen_taus.size()>=2){
      genpt_1_ = gen_taus[0].vector().Pt();
      genpt_2_ = gen_taus[1].vector().Pt();
      geneta_1_ = gen_taus[0].vector().Rapidity();
      geneta_2_ = gen_taus[1].vector().Rapidity();
    } else {
      genpt_1_ =  -9999;
      genpt_2_ =  -9999;
      geneta_1_ = -9999;
      geneta_2_ = -9999; 
    }
    
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
    decayType = "";
    std::sort(decay_types.begin(),decay_types.end(),swap_labels());
    for(unsigned i=0; i< decay_types.size(); ++i){
      decayType += decay_types[i];
    }
    
    if(decayType == "ee") count_ee_++;
    if(decayType == "em") count_em_++;
    if(decayType == "et") count_et_++;
    if(decayType == "mm") count_mm_++;
    if(decayType == "mt") count_mt_++;
    if(decayType == "tt") count_tt_++;

    pt_1_ = -9999.;
    pt_2_ = -9999.;
    ic::Candidate lep1;
    ic::Candidate lep2;
    passed_ = false;
    
    if(channel_str_ == "ee"){
      if(electrons.size() == 2){
        lep1 = electrons[0];
        lep2 = electrons[1];
        passed_ = true;
      }
    } else if(channel_str_ == "mm"){
      if(muons.size() == 2){
        lep1 = muons[0];
        lep2 = muons[1];
        passed_ = true;
      }
    } else if(channel_str_ == "zmm"){
      if(muons.size() == 2){
        lep1 = muons[0];
        lep2 = muons[1];
        passed_ = true;
      }
    } else if(channel_str_ == "em"){
      if(electrons.size() == 1 && muons.size() == 1){
        lep1 = electrons[0];
        lep2 = muons[0];
        passed_ = true;
      }
    } else if(channel_str_ == "et"){
      if(electrons.size() == 1 && taus.size() == 1){
        lep1 = electrons[0];
        lep2 = taus[0];
        passed_ = true;
      }
    } else if(channel_str_ == "mt"){
      if(muons.size() == 1 && taus.size() == 1){
        lep1 = muons[0];
        lep2 = taus[0];
        passed_ = true;
      }
    } else if(channel_str_ == "tt"){
      if(taus.size() == 2){
        lep1 = taus[0];
        lep2 = taus[1];
        passed_ = true;
      }
    }

    std::vector<GenJet> gen_tau_jets = BuildTauJets(gen_particles, false,true);
    std::vector<GenJet *> gen_tau_jets_ptr;
    for (auto & x : gen_tau_jets) gen_tau_jets_ptr.push_back(&x);
    ic::erase_if(gen_tau_jets_ptr, !boost::bind(MinPtMaxEta, _1, 15.0, 999.));
    std::sort(gen_tau_jets_ptr.begin(), gen_tau_jets_ptr.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    cp_sign_1_ = 999;
    cp_sign_2_ = 999;
    cp_sign_3_ = 999;
    cp_sign_4_ = 999;
    aco_angle_1_ = 9999.;
    aco_angle_2_ = 9999.;
    aco_angle_3_ = 9999.;
    aco_angle_4_ = 9999.;
    cp_channel_=-1;
    Ediff_1_ = -9999.;
    Ediff_2_ = -9999.;  
    Pfrac_1_=-9999.; 
    Pfrac_2_=-9999.;

    if(passed_){
      pt_1_  = lep1.vector().Pt();
      pt_2_  = lep2.vector().Pt();
      eta_1_ = lep1.vector().Rapidity();
      eta_2_ = lep2.vector().Rapidity();
      phi_1_ = lep1.vector().Phi();
      phi_2_ = lep2.vector().Phi();
      met_   = met.vector().Pt();
      pt_tt_ = (met.vector()+lep1.vector()+lep2.vector()).Pt();
      mass_ = (met.vector()+lep1.vector()+lep2.vector()).M();
      wtzpt_ = z_pt_weights_sm_.GetBinContent(z_pt_weights_sm_.FindBin(mass_,pt_tt_));
      m_vis_ = (lep1.vector()+lep2.vector()).M();
      if(!(channel_str_ == "zmm")){
        mt_1_ = MT(&lep1, &met);
        mt_2_ = MT(&lep2, &met);
      }

      ic::CompositeCandidate *ditau = new ic::CompositeCandidate();
      ditau->AddCandidate("lep1",&lep1);
      ditau->AddCandidate("lep2",&lep2);
      pzeta_ = PZeta(ditau, &met , 0.85);
    } else {
      pt_1_  = -9999;
      pt_2_  = -9999;
      eta_1_ = -9999;
      eta_2_ = -9999;
      phi_1_ = -9999;
      phi_2_ = -9999;
      met_   = -9999;
      pt_tt_ = -9999;
      mass_= -9999;
      m_vis_ = -9999;
      mt_1_ = -9999;
      mt_2_ = -9999;
      pzeta_ = -9999;
    }
    
    std::vector<ic::GenJet> filtered_jets;
    std::vector<ic::GenJet> bjets;
    
    std::vector<GenParticle *> sel_bquarks;
    for (unsigned i=0; i < gen_particles.size(); ++i){
      std::vector<bool> status_flags = gen_particles[i]->statusFlags();
      unsigned id = abs(gen_particles[i]->pdgid());  
      if(id == 5 && status_flags[FromHardProcess] && status_flags[IsLastCopy] && gen_particles[i]->vector().Pt()>0){
        sel_bquarks.push_back(gen_particles[i]);
      }
    }
     
    for(unsigned i=0; i<gen_jets.size(); ++i){
      ic::GenJet jet = *gen_jets[i];
      double jetPt = jet.vector().Pt();
      double jetEta = std::fabs(jet.vector().Rapidity());
      if(jetPt > min_jet_pt_ && jetEta < max_jet_eta_) filtered_jets.push_back(jet); 
      if(jetPt > 20 && jetEta < 2.4){
          bool MatchedToB = false;
          for(unsigned j=0; j<sel_bquarks.size(); ++j) if(DRLessThan(std::make_pair(&jet, sel_bquarks[j]),0.5)) MatchedToB = true;
          if(MatchedToB) bjets.push_back(jet); 
      }
    }
    
    for(unsigned i=0; i<bjets.size(); ++i){
      ic::GenJet jet = bjets[i];
      bool MatchedToPrompt = false;
      for(unsigned j=0; j<higgs_products.size(); ++j){
        if(DRLessThan(std::make_pair(&jet, &higgs_products[j]),0.5)) MatchedToPrompt = true;
      }
      //remove jets that are matched to Higgs decay products
      if(MatchedToPrompt) bjets.erase (bjets.begin()+i);
    }
    
    n_bjets_noscale_ = bjets.size();
    
    for(unsigned i=0;  i<bjets.size(); ++i){
      ic::GenJet jet = bjets[i];
      double pt = bjets[i].vector().Pt();
      double eta = fabs(bjets[i].vector().Rapidity());
      double eff=0;
      if(pt > bbtag_eff_->GetXaxis()->GetBinLowEdge(bbtag_eff_->GetNbinsX()+1)){
        eff = bbtag_eff_->GetBinContent(bbtag_eff_->GetNbinsX(),bbtag_eff_->GetYaxis()->FindBin(eta));
      } else{
        eff = bbtag_eff_->GetBinContent(bbtag_eff_->GetXaxis()->FindBin(pt),bbtag_eff_->GetYaxis()->FindBin(eta));
      }
      rand->SetSeed((int)((bjets[i].eta()+5)*100000));
      double randVal = rand->Uniform();
      if (randVal > eff) bjets.erase (bjets.begin()+i);
    }
    n_bjets_ = bjets.size();
    n_jets_nofilter_ = filtered_jets.size();
    
    for(unsigned i=0; i<filtered_jets.size(); ++i){
      ic::GenJet jet = filtered_jets[i];
      bool MatchedToPrompt = false;
      for(unsigned j=0; j<higgs_products.size(); ++j){
        if(DRLessThan(std::make_pair(&jet, &higgs_products[j]),0.5)) MatchedToPrompt = true;
      }
      //remove jets that are matched to Higgs decay products
      if(MatchedToPrompt) filtered_jets.erase (filtered_jets.begin()+i);
    }

    for(unsigned i=0; i<gen_jets.size(); ++i){
      ic::GenJet jet = *(gen_jets[i]);
      bool MatchedToPrompt = false;
      for(unsigned j=0; j<higgs_products.size(); ++j){
        if(DRLessThan(std::make_pair(&jet, &higgs_products[j]),0.5)) MatchedToPrompt = true;
      }
      //remove jets that are matched to Higgs decay products
            if(MatchedToPrompt) gen_jets.erase (gen_jets.begin()+i);
    }

    largest_gen_mjj_=-9999;
    if(gen_jets.size()>1){
      for(unsigned i=0; i<gen_jets.size()-1;++i){
        for(unsigned j=i+1; j<gen_jets.size();++j){
          double mass = (gen_jets[i]->vector()+gen_jets[j]->vector()).M();
         if(mass>largest_gen_mjj_) largest_gen_mjj_ = mass;
         }
      }
    }

    n_jets_ = filtered_jets.size();
    jpt_1_       = -9999;
    jeta_1_      = -9999;
    jphi_1_      = -9999;
    jpt_2_       = -9999;
    jpt_3_       = -9999;
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
    if(n_jets_ > 2){
      jpt_3_  = filtered_jets[2].vector().Pt();      
    }

    n_pjets_=0;    
    if(filtered_jets.size()>=2){
      if(filtered_jets[0].eta() > filtered_jets[1].eta()){
        sjdphi_ =  ROOT::Math::VectorUtil::DeltaPhi(filtered_jets[0].vector(), filtered_jets[1].vector());
      }
      else{
        sjdphi_ =  ROOT::Math::VectorUtil::DeltaPhi(filtered_jets[1].vector(), filtered_jets[0].vector());
      }
      // sort jets higher and lower than higgs eta_1
      std::vector<GenJet> jets_high;
      std::vector<GenJet> jets_low;
      for (unsigned i=0; i<filtered_jets.size(); ++i){
        if (filtered_jets[i].eta() > higgs_eta) jets_high.push_back(filtered_jets[i]);
        else jets_low.push_back(filtered_jets[i]);
      }
      if(jets_low.size()>0) n_pjets_++;
      if(jets_high.size()>0) n_pjets_++;
      Candidate pseudo_jet_a;
      Candidate pseudo_jet_b;
      for (auto j : jets_low) pseudo_jet_a.set_vector(pseudo_jet_a.vector()+j.vector());
      for (auto j : jets_high) pseudo_jet_b.set_vector(pseudo_jet_b.vector()+j.vector());
      spjdphi_ =  ROOT::Math::VectorUtil::DeltaPhi(pseudo_jet_a.vector(),pseudo_jet_b.vector());
      for (unsigned i=0; i<filtered_jets.size(); ++i){
        double dEta = std::fabs(higgs_eta - filtered_jets[i].eta());
        if(i==0 || dEta<ysep_) ysep_ = dEta;
      }
    } else {
      sjdphi_ = -9999;
      spjdphi_ = -9999;
    }

    /* wt_ps_down_ = 1.0; */
    /* wt_ps_up_ = 1.0; */
    /* if(n_jets_==0){ */
    /*   wt_ps_up_ =  ps_0jet_up_  .GetBinContent(ps_0jet_up_  .FindBin(HiggsPt_)); */  
    /*   wt_ps_down_ =  ps_0jet_down_.GetBinContent(ps_0jet_down_.FindBin(HiggsPt_)); */   
    /* } */
    /* if(n_jets_==1){ */ 
    /*   wt_ps_up_ =  ps_1jet_up_ .GetBinContent(ps_1jet_up_  .FindBin(HiggsPt_)); */  
    /*   wt_ps_down_ =  ps_1jet_down_.GetBinContent(ps_1jet_down_.FindBin(HiggsPt_)); */   
    /* } */
    /* if(n_jets_==2){ */
    /*   wt_ps_up_ =  ps_2jet_up_  .GetBinContent(ps_2jet_up_  .FindBin(HiggsPt_)); */  
    /*   wt_ps_down_ =  ps_2jet_down_.GetBinContent(ps_2jet_down_.FindBin(HiggsPt_)); */     
    /* } */
    /* if(n_jets_>2){ */
    /*   wt_ps_up_ =  ps_3jet_up_  .GetBinContent(ps_3jet_up_  .FindBin(HiggsPt_)); */
    /*   wt_ps_down_ =  ps_3jet_down_.GetBinContent(ps_3jet_down_.FindBin(HiggsPt_)); */
    /* } */
    /* wt_ue_up_ = ue_up_  .GetBinContent(ue_up_  .FindBin(n_jets_)); */
    /* wt_ue_down_ = ue_down_  .GetBinContent(ue_down_  .FindBin(n_jets_)); */


    std::vector<PileupInfo *> puInfo;
    float true_int = -1;
    
    if(event->ExistsInTree("pileupInfo")){
      puInfo = event->GetPtrVec<PileupInfo>("pileupInfo");
        for (unsigned i = 0; i < puInfo.size(); ++i) {
          if (puInfo[i]->bunch_crossing() == 0)
            true_int = puInfo[i]->true_num_interactions();
        }
    
      n_pu_ = true_int;
    }
    
    auto args = std::vector<double>{pT};
    if(fns_["h_t_ratio"]){
      wt_ggh_t_ = fns_["h_t_ratio"]->eval(args.data());        
      wt_ggh_b_ = fns_["h_b_ratio"]->eval(args.data());
      wt_ggh_i_ = fns_["h_i_ratio"]->eval(args.data());
      wt_ggH_t_ = fns_["H_t_ratio"]->eval(args.data());
      wt_ggH_b_ = fns_["H_b_ratio"]->eval(args.data());
      wt_ggH_i_ = fns_["H_i_ratio"]->eval(args.data());
      wt_ggA_t_ = fns_["A_t_ratio"]->eval(args.data());
      wt_ggA_b_ = fns_["A_b_ratio"]->eval(args.data());
      wt_ggA_i_ = fns_["A_i_ratio"]->eval(args.data());
    }

    
    /*std::vector<std::pair<GenParticle*, GenParticle*>> rho_daughters;
    std::vector<std::pair<GenParticle*, GenParticle*>> prho_daughters;
    std::vector<std::pair<GenParticle*, GenParticle*>> l_daughters;
    std::vector<std::vector<GenParticle*>> a1_daughters;
    std::vector<std::vector<GenParticle*>> fakea1_daughters;
    std::vector<std::vector<GenParticle*>> charged_daughters;
    std::vector<std::vector<GenParticle*>> a1_2_daughters;
    std::vector<Candidate*> tau_rhos;
    std::vector<GenParticle*> pi_daughters;
    unsigned count_taus=0;   
    for (unsigned i = 0; i < gen_particles.size(); ++i) {
      if(std::fabs(gen_particles[i]->pdgid()) == 15 && gen_particles[i]->statusFlags()[IsLastCopy]){
        GenParticle* tau = gen_particles[i];
        ++count_taus;
        bool foundRho = false;
        bool foundPi = false;
        bool foundLep = false;
        bool foundA1 = false;
        bool foundA1_2 = false;
        bool found3Pi1P0 = false;
        unsigned count_pi0 = 0;
        unsigned count_pi = 0;
        unsigned count_K = 0;
        unsigned count_hadr = 0;
        unsigned count_gamma =0;
        unsigned count_lep=0;
        unsigned count_charged=0;

        std::pair<GenParticle*,GenParticle*> rho = std::make_pair(new GenParticle(), new GenParticle());
        std::pair<GenParticle*,GenParticle*> prho = std::make_pair(new GenParticle(), new GenParticle());
        //std::pair<GenParticle*,GenParticle*> lep = std::make_pair(new GenParticle(), new GenParticle());
        std::vector<GenParticle*> charged_vec = {};
        std::vector<GenParticle*> a1 = {};
        std::vector<GenParticle*> fakea1 = {};
        std::vector<GenParticle*> a1_2 = {};
        GenParticle* pi = new GenParticle();
        std::vector<int> daughters = gen_particles[i]->daughters();
        charged_vec.push_back(gen_particles[i]);
        for (unsigned d : daughters){
          unsigned daughter_id = fabs(gen_particles[d]->pdgid());
          int daughter_charge = gen_particles[d]->charge();
          if(daughter_id == 12 || daughter_id == 14 || daughter_id == 16) continue;
          ++count_hadr;
          if(daughter_id == 11 || daughter_id == 13) {
            ++count_lep;
            //lep.first = gen_particles[d];
            pi = gen_particles[d];
            //lep.second = tau;
            prho.first = pi;
            prho.second = tau;

            continue;
          }
          if(daughter_charge!=0) {
            ++count_charged;
            charged_vec.push_back(gen_particles[d]);
          }
          if (daughter_id == 22) {
            ++count_gamma;
            continue;
          }
          if (daughter_id == 211) {
            ++count_pi;
            //std::cout << "pi charge  " << gen_particles[d]->vector() << std::endl;
            rho.first = gen_particles[d];
            pi = gen_particles[d];
            prho.first = pi;
            prho.second = tau;
            a1.push_back(gen_particles[d]);
            fakea1.push_back(gen_particles[d]);
            a1_2.push_back(gen_particles[d]);
            continue;
          }
          if (daughter_id == 130 || daughter_id == 310 
                  || daughter_id == 311 || daughter_id == 321) {
            ++count_K;
            continue;
          }
          if (daughter_id == 111) {
            ++count_pi0;
            rho.second = gen_particles[d];
            a1_2.push_back(gen_particles[d]); 
            fakea1.push_back(gen_particles[d]);
            //std::cout << "pi zero " << gen_particles[d]->vector() << std::endl;
            continue;
          }
          if (daughter_id == 213) {
            //std::cout << "found 213 rho " << daughter_id << std::endl;
            count_pi0 = 0;
            count_pi = 0;
            count_K = 0;
            count_hadr=0;
            count_gamma=0;
            std::vector<int> gdaughters = gen_particles[d]->daughters(); 
            for (unsigned g : gdaughters){
              unsigned gdaughter_id = fabs(gen_particles[g]->pdgid());
              ++count_hadr;
              if (gdaughter_id == 22) {
                ++count_gamma;
                continue;
              }
              if (gdaughter_id == 211) {
                ++count_pi;
                rho.first = gen_particles[g];
                continue;
              }
              if (gdaughter_id == 111) {
                ++count_pi0;
                rho.second = gen_particles[g];
                continue;
              }
            }
            foundRho = true;
            break;
          }
        }
        foundRho = foundRho || (count_hadr-count_gamma==2 && count_pi==1 && count_pi0==1);
        foundPi = (count_hadr-count_gamma==1 && count_pi==1 && count_pi0==0);
        foundLep = count_lep==1;
        foundA1 = foundA1 || (count_hadr-count_gamma==3 && count_pi==3 && count_pi0==0);
        found3Pi1P0 = found3Pi1P0 || (count_hadr-count_gamma==4 && count_pi==3 && count_pi0==1);
        foundA1_2 = foundA1_2 || (count_hadr-count_gamma==3 && count_pi==1 && count_pi0==2);
        if(foundRho) {
          rho_daughters.push_back(rho);
          Candidate * tauvis = new Candidate();
          tauvis->set_vector(rho.first->vector()+rho.second->vector());
          tau_rhos.push_back(tauvis);
        }
        if(foundPi || foundLep) {
          prho_daughters.push_back(prho);
          pi_daughters.push_back(pi);
        }
        //if(foundLep) {
        //  //l_daughters.push_back(lep);
        //  prho_daughters.push_back(prho);
        //  pi_daughters.push_back(pi);
        //}
        if(foundA1){
          a1_daughters.push_back(a1);
        }
        if(found3Pi1P0){
          fakea1_daughters.push_back(fakea1);
        }
        if(count_charged==3) {
          charged_daughters.push_back(charged_vec);
        }

        if(foundA1_2){
          a1_2_daughters.push_back(a1_2); 
        }
      }
    }

    fakea1_dR_ = -1;
    rho_dR_ = -1;
    tauFlag = -1;

    if(rho_daughters.size()==1) {
      rho_dR_ = ROOT::Math::VectorUtil::DeltaR(rho_daughters[0].first->vector(),rho_daughters[0].second->vector());
      tauFlag = 1;
    }


    if(fakea1_daughters.size()==1) {
      ROOT::Math::PtEtaPhiEVector charged_vec;
      ROOT::Math::PtEtaPhiEVector neutral_vec;
      for (auto p : fakea1_daughters[0]) {
        if (std::fabs(p->pdgid()) == 111) neutral_vec = p->vector();
        else charged_vec += p->vector();
      }
      fakea1_dR_ = ROOT::Math::VectorUtil::DeltaR(neutral_vec,charged_vec);
      tauFlag = 11;
    }
    

    TLorentzVector lvec1;
    TLorentzVector lvec2;
    TLorentzVector lvec3;
    TLorentzVector lvec4;

    if(a1_2_daughters.size()==1 && rho_daughters.size()==1){
      cp_channel_=6;
      if (a1_2_daughters[0][1]->charge()!=0) {
        auto temp = a1_2_daughters[0][1];
        a1_2_daughters[0][1] = a1_2_daughters[0][0];
        a1_2_daughters[0][0] = temp;
      }
      if (a1_2_daughters[0][2]->charge()!=0) {
        auto temp = a1_2_daughters[0][2];
        a1_2_daughters[0][2] = a1_2_daughters[0][0];
        a1_2_daughters[0][0] = temp;
      }
      mass1_ = (a1_2_daughters[0][0]->vector() + a1_2_daughters[0][1]->vector()).M();
      mass2_ = (a1_2_daughters[0][0]->vector() + a1_2_daughters[0][2]->vector()).M();
      if(std::fabs(mass2_-0.775) < std::fabs(mass1_-0.775)) {
        double temp_mass = mass1_;
        mass1_ = mass2_;
        mass2_ = temp_mass;
        auto temp = a1_2_daughters[0][1];
        a1_2_daughters[0][1] = a1_2_daughters[0][2];
        a1_2_daughters[0][2] = temp;
      }

      std::pair<GenParticle*,GenParticle*> p_rho = std::make_pair(new GenParticle(), new GenParticle());
      std::pair<GenParticle*,GenParticle*> p_rho_1 = std::make_pair(new GenParticle(), new GenParticle());
      p_rho_1.first = a1_2_daughters[0][0];
      p_rho_1.second = a1_2_daughters[0][1];
      p_rho.first = a1_2_daughters[0][0];
      p_rho.second = a1_2_daughters[0][1];
      p_rho.second->set_vector(a1_2_daughters[0][1]->vector()+a1_2_daughters[0][2]->vector());

      pt_rho_pi_ = a1_2_daughters[0][0]->pt();
      pt_rho_pi0_ = a1_2_daughters[0][1]->pt();
      pt_other_pi0_ = a1_2_daughters[0][2]->pt();

      GenParticle* rho_1  = new GenParticle();
      GenParticle* rho_2  = new GenParticle();
     
      //rho_1->set_vector(a1_2_daughters[0][0]->vector()+a1_2_daughters[0][1]->vector());

      lvec1 = ConvertToLorentz(rho_daughters[0].second->vector());
      lvec2 = ConvertToLorentz(p_rho_1.second->vector());
      lvec3 = ConvertToLorentz(rho_daughters[0].first->vector());
      lvec4 = ConvertToLorentz(p_rho_1.first->vector());
      cp_sign_1_ = YRho(std::vector<GenParticle*>(
                    {rho_daughters[0].first, rho_daughters[0].second}),TVector3())*
                    YRho(std::vector<GenParticle*>(
                    {p_rho_1.first, p_rho_1.second}),TVector3());

      aco_angle_1_ = IPAcoAngle(lvec1, lvec2, lvec3, lvec4,false);

      if (cp_sign_1_<0) {
        if (aco_angle_1_<M_PI) aco_angle_1_ += M_PI;
        else                   aco_angle_1_ -= M_PI;
      }

      lvec1 = ConvertToLorentz(rho_daughters[0].second->vector());
      lvec2 = ConvertToLorentz(p_rho.second->vector());
      lvec3 = ConvertToLorentz(rho_daughters[0].first->vector());
      lvec4 = ConvertToLorentz(p_rho.first->vector());
      cp_sign_2_ = YRho(std::vector<GenParticle*>(
                    {rho_daughters[0].first, rho_daughters[0].second}),TVector3())*
                    YRho(std::vector<GenParticle*>(
                    {p_rho.first, p_rho.second}),TVector3());

      aco_angle_2_ = IPAcoAngle(lvec1, lvec2, lvec3, lvec4,false);

      if (cp_sign_2_<0) {
        if (aco_angle_2_<M_PI) aco_angle_2_ += M_PI;
        else                   aco_angle_2_ -= M_PI;
      }



      //aco_angle_2_ = AcoplanarityAngle(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({rho_1,a1_2_daughters[0][2]}));
      //cp_sign_2_ = YRho(std::vector<GenParticle*>({rho_daughters[0].first, rho_daughters[0].second}),TVector3())*YA1(std::vector<GenParticle*>({rho_1, a1_2_daughters[0][2]}),TVector3());
      ////std::cout << aco_angle_2_ << std::endl;

      //aco_angle_1_ = AcoplanarityAngle(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({a1_2_daughters[0][0],a1_2_daughters[0][1]}));
      //aco_angle_3_ = AcoplanarityAngle(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({a1_2_daughters[0][0],a1_2_daughters[0][2]}));
      ////std::cout << a1_2_daughters[0][0]->charge() << "    " << a1_2_daughters[0][1]->charge() << "    " << a1_2_daughters[0][2]->charge() << "    " << aco_angle_1_ << std::endl;
      //aco_angle_5_ = aco_angle_1_;
      //aco_angle_6_ = aco_angle_3_;

      //GenParticle* rho_1  = new GenParticle();
      //GenParticle* rho_2  = new GenParticle();
      //rho_1->set_vector(a1_2_daughters[0][0]->vector()+a1_2_daughters[0][1]->vector());
      //rho_2->set_vector(a1_2_daughters[0][0]->vector()+a1_2_daughters[0][2]->vector());
      //aco_angle_2_ = AcoplanarityAngle(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({rho_1,a1_2_daughters[0][2]}));
      //aco_angle_4_ = AcoplanarityAngle(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({rho_2,a1_2_daughters[0][1]}));


      //cp_sign_1_ = YRho(std::vector<GenParticle*>({rho_daughters[0].first, rho_daughters[0].second}),TVector3())*YRho(std::vector<GenParticle*>({a1_2_daughters[0][0], a1_2_daughters[0][1]}),TVector3());
      //cp_sign_3_ = YRho(std::vector<GenParticle*>({rho_daughters[0].first, rho_daughters[0].second}),TVector3())*YRho(std::vector<GenParticle*>({a1_2_daughters[0][0], a1_2_daughters[0][2]}),TVector3());

      //cp_sign_2_ = YRho(std::vector<GenParticle*>({rho_daughters[0].first, rho_daughters[0].second}),TVector3())*YA1(std::vector<GenParticle*>({rho_1, a1_2_daughters[0][2]}),TVector3());
      //cp_sign_4_ = YRho(std::vector<GenParticle*>({rho_daughters[0].first, rho_daughters[0].second}),TVector3())*YA1(std::vector<GenParticle*>({rho_2, a1_2_daughters[0][1]}),TVector3()); 

      //mass1_ = (a1_2_daughters[0][0]->vector() + a1_2_daughters[0][1]->vector()).M();
      //mass2_ = (a1_2_daughters[0][0]->vector() + a1_2_daughters[0][2]->vector()).M();


      if (cp_sign_1_<0) {
        if (aco_angle_1_<M_PI) aco_angle_1_ += M_PI;
        else                   aco_angle_1_ -= M_PI;
      }
      if (cp_sign_2_<0) {
        if (aco_angle_2_<M_PI) aco_angle_2_ += M_PI;
        else                   aco_angle_2_ -= M_PI;
      }
      if (cp_sign_3_<0) {
        if (aco_angle_3_<M_PI) aco_angle_3_ += M_PI;
        else                   aco_angle_3_ -= M_PI;
      }
      if (cp_sign_4_<0) {
        if (aco_angle_4_<M_PI) aco_angle_4_ += M_PI;
        else                   aco_angle_4_ -= M_PI;
      }

    
      a1_daughters = a1_2_daughters;

      dR1_ = ROOT::Math::VectorUtil::DeltaR(a1_daughters[0][0]->vector(), a1_daughters[0][1]->vector());
      if(rand_<0.5) dR2_ = ROOT::Math::VectorUtil::DeltaR(a1_daughters[0][0]->vector(), a1_daughters[0][2]->vector());
      else dR2_ = ROOT::Math::VectorUtil::DeltaR(a1_daughters[0][1]->vector(), a1_daughters[0][2]->vector()); // pick this randomly so that it more closly follows what would happen for 3 prong channel

      // designed to mimic what is one for the 3 prong case where each of neutral particle are assigned a chare at random
      if(rand_<0.5){
        a1_daughters[0][0]->set_charge(-1);
        a1_daughters[0][1]->set_charge(1);
      } else {
        a1_daughters[0][0]->set_charge(1);
        a1_daughters[0][1]->set_charge(-1);
      }
      a1_daughters[0] = SortA1Products(a1_daughters[0]);

      //mass1_ = (a1_daughters[0][0]->vector() + a1_daughters[0][1]->vector()).M();
      //mass2_ = (a1_daughters[0][0]->vector() + a1_daughters[0][2]->vector()).M();

      //std::cout << a1_daughters[0][0]->charge() << "    " << a1_daughters[0][1]->charge() << "    " << a1_daughters[0][2]->charge() << "    " << mass1_ << "    " << mass2_ << std::endl;

      p_aco_angle_1_ = AcoplanarityAngle(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({a1_daughters[0][0],a1_daughters[0][1]}));
      p_aco_angle_3_ = AcoplanarityAngle(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({a1_daughters[0][0],a1_daughters[0][2]}));

      rho_1  = new GenParticle();
      rho_2  = new GenParticle();
      rho_1->set_vector(a1_daughters[0][0]->vector()+a1_daughters[0][1]->vector());
      rho_2->set_vector(a1_daughters[0][0]->vector()+a1_daughters[0][2]->vector());
      p_aco_angle_2_ = AcoplanarityAngle(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({rho_1,a1_daughters[0][2]}));
      p_aco_angle_4_ = AcoplanarityAngle(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({rho_2,a1_daughters[0][1]}));

      double cp_sign_1 = YRho(std::vector<GenParticle*>({rho_daughters[0].first, rho_daughters[0].second}),TVector3())*YRho(std::vector<GenParticle*>({a1_daughters[0][0], a1_daughters[0][1]}),TVector3());
      double cp_sign_3 = YRho(std::vector<GenParticle*>({rho_daughters[0].first, rho_daughters[0].second}),TVector3())*YRho(std::vector<GenParticle*>({a1_daughters[0][0], a1_daughters[0][2]}),TVector3());

      double cp_sign_2 = YRho(std::vector<GenParticle*>({rho_daughters[0].first, rho_daughters[0].second}),TVector3())*YA1(std::vector<GenParticle*>({rho_1, a1_daughters[0][2]}),TVector3());
      double cp_sign_4 = YRho(std::vector<GenParticle*>({rho_daughters[0].first, rho_daughters[0].second}),TVector3())*YA1(std::vector<GenParticle*>({rho_2, a1_daughters[0][1]}),TVector3());

      if (cp_sign_1<0) {
        if (p_aco_angle_1_<M_PI) p_aco_angle_1_ += M_PI;
        else                   p_aco_angle_1_ -= M_PI;
      }
      if (cp_sign_2<0) {
        if (p_aco_angle_2_<M_PI) p_aco_angle_2_ += M_PI;
        else                   p_aco_angle_2_ -= M_PI;
      }
      if (cp_sign_3<0) {
        if (p_aco_angle_3_<M_PI) p_aco_angle_3_ += M_PI;
        else                   p_aco_angle_3_ -= M_PI;
      }
      if (cp_sign_4<0) {
        if (p_aco_angle_4_<M_PI) p_aco_angle_4_ += M_PI;
        else                   p_aco_angle_4_ -= M_PI;
      }

      a1_daughters = {};

    }

    if(a1_daughters.size()==1 && rho_daughters.size()==1){
      cp_channel_=4;
      cp_sign_1_ = 999;
      cp_sign_2_ = 999;
      cp_sign_3_ = 999;
      cp_sign_4_ = 999;
      aco_angle_1_ = 9999.;
      aco_angle_2_ = 9999.;
      aco_angle_3_ = 9999.;
      aco_angle_4_ = 9999.;
      a1_daughters[0] = SortA1Products(a1_daughters[0]);
      mass1_ = (a1_daughters[0][0]->vector() + a1_daughters[0][1]->vector()).M();
      mass2_ = (a1_daughters[0][0]->vector() + a1_daughters[0][2]->vector()).M();

      aco_angle_1_ = AcoplanarityAngle(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({a1_daughters[0][0],a1_daughters[0][1]}));
      aco_angle_3_ = AcoplanarityAngle(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({a1_daughters[0][0],a1_daughters[0][2]}));
      //aco_angle_1_ = AcoplanarityAngle2(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({a1_daughters[0][0],a1_daughters[0][1]}));
      //aco_angle_3_ = AcoplanarityAngle2(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({a1_daughters[0][0],a1_daughters[0][2]}));


      GenParticle* rho_1  = new GenParticle();
      GenParticle* rho_2  = new GenParticle();
      rho_1->set_vector(a1_daughters[0][0]->vector()+a1_daughters[0][1]->vector());
      rho_2->set_vector(a1_daughters[0][0]->vector()+a1_daughters[0][2]->vector());
      aco_angle_2_ = AcoplanarityAngle(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({rho_1,a1_daughters[0][2]}));
      aco_angle_4_ = AcoplanarityAngle(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({rho_2,a1_daughters[0][1]}));

      //aco_angle_2_ = AcoplanarityAngle2(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({a1_daughters[0][2],rho_1}));
      //aco_angle_4_ = AcoplanarityAngle2(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({a1_daughters[0][1],rho_1}));

      cp_sign_1_ = YRho(std::vector<GenParticle*>({rho_daughters[0].first, rho_daughters[0].second}),TVector3())*YRho(std::vector<GenParticle*>({a1_daughters[0][0], a1_daughters[0][1]}),TVector3());
      cp_sign_3_ = YRho(std::vector<GenParticle*>({rho_daughters[0].first, rho_daughters[0].second}),TVector3())*YRho(std::vector<GenParticle*>({a1_daughters[0][0], a1_daughters[0][2]}),TVector3()); 

      cp_sign_2_ = YRho(std::vector<GenParticle*>({rho_daughters[0].first, rho_daughters[0].second}),TVector3())*YA1(std::vector<GenParticle*>({rho_1, a1_daughters[0][2]}),TVector3());
      cp_sign_4_ = YRho(std::vector<GenParticle*>({rho_daughters[0].first, rho_daughters[0].second}),TVector3())*YA1(std::vector<GenParticle*>({rho_2, a1_daughters[0][1]}),TVector3());

      //aco_angle_5_ = AcoplanarityAngle2(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({a1_daughters[0][1],a1_daughters[0][2]}));
      aco_angle_5_ = AcoplanarityAngle(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({a1_daughters[0][1],a1_daughters[0][2]}));
      cp_sign_5_ = YRho(std::vector<GenParticle*>({rho_daughters[0].first, rho_daughters[0].second}),TVector3())*YRho(std::vector<GenParticle*>({a1_daughters[0][1], a1_daughters[0][2]}),TVector3());
      GenParticle* rho_3  = new GenParticle();
      rho_3->set_vector(a1_daughters[0][1]->vector()+a1_daughters[0][2]->vector());
      //aco_angle_6_ = AcoplanarityAngle2(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({a1_daughters[0][0],rho_3}));
      aco_angle_6_ = AcoplanarityAngle(std::vector<GenParticle*> ({rho_daughters[0].first,rho_daughters[0].second}), std::vector<GenParticle*> ({rho_3,a1_daughters[0][0]}));
      cp_sign_6_ = YRho(std::vector<GenParticle*>({rho_daughters[0].first, rho_daughters[0].second}),TVector3())*YA1(std::vector<GenParticle*>({rho_3, a1_daughters[0][0]}),TVector3());
   
      y_1_1_ = YRho(std::vector<GenParticle*>({rho_daughters[0].first, rho_daughters[0].second}),TVector3());
      y_2_1_ = YRho(std::vector<GenParticle*>({rho_daughters[0].first, rho_daughters[0].second}),TVector3());
      y_3_1_ = YRho(std::vector<GenParticle*>({rho_daughters[0].first, rho_daughters[0].second}),TVector3());
      y_4_1_ = YRho(std::vector<GenParticle*>({rho_daughters[0].first, rho_daughters[0].second}),TVector3());

      y_1_2_ = YRho(std::vector<GenParticle*>({a1_daughters[0][0], a1_daughters[0][1]}),TVector3());
      y_2_2_ = YRho(std::vector<GenParticle*>({a1_daughters[0][0], a1_daughters[0][2]}),TVector3());
      y_3_2_ = YA1(std::vector<GenParticle*>({rho_1, a1_daughters[0][2]}),TVector3());
      y_4_2_ = YA1(std::vector<GenParticle*>({rho_2, a1_daughters[0][1]}),TVector3());

      a1_mass_ = (a1_daughters[0][0]->vector() + a1_daughters[0][1]->vector()+a1_daughters[0][2]->vector()).M();
      E_1_1_ = rho_daughters[0].first->energy();
      E_2_1_ = rho_daughters[0].second->energy();
      E_1_2_ = a1_daughters[0][0]->energy();
      E_2_2_ = a1_daughters[0][1]->energy();
      E_3_2_ = a1_daughters[0][2]->energy();


      if (cp_sign_1_<0) {
        if (aco_angle_1_<M_PI) aco_angle_1_ += M_PI;
        else                   aco_angle_1_ -= M_PI;
      } 
      if (cp_sign_2_<0) {
        if (aco_angle_2_<M_PI) aco_angle_2_ += M_PI;
        else                   aco_angle_2_ -= M_PI;
      }
      if (cp_sign_3_<0) {
        if (aco_angle_3_<M_PI) aco_angle_3_ += M_PI;
        else                   aco_angle_3_ -= M_PI;
      }
      if (cp_sign_4_<0) {
        if (aco_angle_4_<M_PI) aco_angle_4_ += M_PI;
        else                   aco_angle_4_ -= M_PI;
      }
      if (cp_sign_5_<0) {
        if (aco_angle_5_<M_PI) aco_angle_5_ += M_PI;
        else                   aco_angle_5_ -= M_PI;
      }
      if (cp_sign_6_<0) {
        if (aco_angle_6_<M_PI) aco_angle_6_ += M_PI;
        else                   aco_angle_6_ -= M_PI;
      }
  
    }
 
    if(prho_daughters.size()==1){
      Pfrac_1_ = prho_daughters[0].first->vector().P()/prho_daughters[0].second->vector().P();
    }
    if(prho_daughters.size()==2){
      Pfrac_2_ = prho_daughters[1].first->vector().P()/prho_daughters[1].second->vector().P();
    }

    if(prho_daughters.size()==2){
      //cp_channel_=1;
    }

    if (pi_daughters.size()==2 && prho_daughters.size()==2){
        cp_channel_=1;
        std::vector<ic::Vertex*> gen_vertices = event->GetPtrVec<ic::Vertex>("genVertices"); //gen  

        TLorentzVector pvtosv1(
                pi_daughters[0]->vtx().vx() - prho_daughters[0].second->vtx().vx(),
                pi_daughters[0]->vtx().vy() - prho_daughters[0].second->vtx().vy(),
                pi_daughters[0]->vtx().vz() - prho_daughters[0].second->vtx().vz(),
                0.);

        TLorentzVector pvtosv2(
                pi_daughters[1]->vtx().vx() - prho_daughters[1].second->vtx().vx(),
                pi_daughters[1]->vtx().vy() - prho_daughters[1].second->vtx().vy(),
                pi_daughters[1]->vtx().vz() - prho_daughters[1].second->vtx().vz(),
                0.);

        if(pi_daughters[0]->charge()<0) {
          lvec4 = ConvertToLorentz(pi_daughters[1]->vector()); 
          lvec3 = ConvertToLorentz(pi_daughters[0]->vector()); 

          TVector3 ip1 = (pvtosv1.Vect() - pvtosv1.Vect().Dot(lvec3.Vect().Unit())*lvec3.Vect().Unit()).Unit();
          lvec1 = TLorentzVector(ip1, 0.);

          TVector3 ip2 = (pvtosv2.Vect() - pvtosv2.Vect().Dot(lvec4.Vect().Unit())*lvec4.Vect().Unit()).Unit();
          lvec2 = TLorentzVector(ip2, 0.);
        }
    }

    //if(decayType == "mt") std::cout << rho_daughters.size() << "    " <<  pi_daughters.size() << std::endl;

    if (rho_daughters.size()==1 && pi_daughters.size()==1){
        cp_channel_=2;
        cp_sign_1_ = YRho(std::vector<GenParticle*>({rho_daughters[0].first, rho_daughters[0].second}),TVector3());

        //std::vector<ic::Vertex*> & vertex_vec = event->GetPtrVec<ic::Vertex>("vertices"); // reco
        std::vector<ic::Vertex*> gen_vertices = event->GetPtrVec<ic::Vertex>("genVertices"); //gen  
        //for (unsigned i = 0; i < vertex_vec.size(); i++) {
        //  reco_pvx_ = vertex_vec[0]->vx();
        //  reco_pvy_ = vertex_vec[0]->vy();
        //  reco_pvz_ = vertex_vec[0]->vz();
        //}
        for (unsigned i = 0; i < gen_vertices.size(); i++) {
          gen_pvx_ = gen_vertices[0]->vx();
          gen_pvy_ = gen_vertices[0]->vy();
          gen_pvz_ = gen_vertices[0]->vz();
        }
        TLorentzVector pvtosv(
                pi_daughters[0]->vtx().vx() - prho_daughters[0].second->vtx().vx(), 
                pi_daughters[0]->vtx().vy() - prho_daughters[0].second->vtx().vy(), 
                pi_daughters[0]->vtx().vz() - prho_daughters[0].second->vtx().vz(), 
                0.);

        lvec1 = ConvertToLorentz(rho_daughters[0].second->vector()); //pi zero from rho
        lvec3 = ConvertToLorentz(rho_daughters[0].first->vector()); //pi charge from rho
        lvec4 = ConvertToLorentz(pi_daughters[0]->vector()); //pi charge from tau

        TVector3 ip = (pvtosv.Vect() - pvtosv.Vect().Dot(lvec4.Vect().Unit())*lvec4.Vect().Unit()).Unit();
        lvec2 = TLorentzVector(ip, 0.);
    }

//    else if (rho_daughters.size()==1 && l_daughters.size()==1){
//        cp_channel_=5;
//        std::vector<ic::Vertex*> & vertex_vec = event->GetPtrVec<ic::Vertex>("vertices"); // reco
//        std::vector<ic::Vertex*> gen_vertices = event->GetPtrVec<ic::Vertex>("genVertices"); //gen  
//        for (unsigned i = 0; i < vertex_vec.size(); i++) {
//          reco_pvx_ = vertex_vec[0]->vx();
//          reco_pvy_ = vertex_vec[0]->vy();
//          reco_pvz_ = vertex_vec[0]->vz();
//        }
//        for (unsigned i = 0; i < gen_vertices.size(); i++) {
//          gen_pvx_ = gen_vertices[0]->vx();
//          gen_pvy_ = gen_vertices[0]->vy();
//          gen_pvz_ = gen_vertices[0]->vz();
//        }
//        TLorentzVector pvtosv(
//                l_daughters[0].first->vtx().vx() - l_daughters[0].second->vtx().vx(), 
//                l_daughters[0].first->vtx().vy() - l_daughters[0].second->vtx().vy(), 
//                l_daughters[0].first->vtx().vz() - l_daughters[0].second->vtx().vz(), 
//                0.);
//
//        if(l_daughters[0].first->charge()<0) {
//
//          lvec1 = ConvertToLorentz(rho_daughters[0].second->vector());
//          lvec3 = ConvertToLorentz(rho_daughters[0].first->vector());
//          lvec4 = ConvertToLorentz(l_daughters[0].first->vector());
//          TVector3 ip = (pvtosv.Vect() - pvtosv.Vect().Dot(lvec4.Vect().Unit())*lvec4.Vect().Unit()).Unit();
//          lvec2 = TLorentzVector(ip, 0.);
//        } else {
//          lvec2 = ConvertToLorentz(rho_daughters[0].second->vector());
//          lvec4 = ConvertToLorentz(rho_daughters[0].first->vector());
//          lvec3 = ConvertToLorentz(l_daughters[0].first->vector());
//          TVector3 ip = (pvtosv.Vect() - pvtosv.Vect().Dot(lvec4.Vect().Unit())*lvec4.Vect().Unit()).Unit();
//          lvec1 = TLorentzVector(ip, 0.);
//        }
//        cp_sign_1_ = YRho(std::vector<GenParticle*>(
//                    {rho_daughters[0].first, rho_daughters[0].second}),TVector3());
//    }

//    if (pi_daughters.size()==1&&l_daughters.size()==1){
//        cp_channel_=1;
//        //lvec1 = ConvertToLorentz(prho_daughters[0].second->vector());
//        //lvec2 = ConvertToLorentz(rho_daughters[0].second->vector());
//        //lvec3 = ConvertToLorentz(prho_daughters[0].first->vector());
//        //lvec4 = ConvertToLorentz(rho_daughters[0].first->vector());
//        std::vector<ic::Vertex*> & vertex_vec = event->GetPtrVec<ic::Vertex>("vertices"); // reco
//        std::vector<ic::Vertex*> gen_vertices = event->GetPtrVec<ic::Vertex>("genVertices"); //gen  
//        for (unsigned i = 0; i < vertex_vec.size(); i++) {
//          reco_pvx_ = vertex_vec[0]->vx();
//          reco_pvy_ = vertex_vec[0]->vy();
//          reco_pvz_ = vertex_vec[0]->vz();
//        }
//        for (unsigned i = 0; i < gen_vertices.size(); i++) {
//          gen_pvx_ = gen_vertices[0]->vx();
//          gen_pvy_ = gen_vertices[0]->vy();
//          gen_pvz_ = gen_vertices[0]->vz();
//        }
//        TLorentzVector pvtosv(
//                pi_daughters[0]->vtx().vx() - prho_daughters[0].second->vtx().vx(),
//                pi_daughters[0]->vtx().vy() - prho_daughters[0].second->vtx().vy(),
//                pi_daughters[0]->vtx().vz() - prho_daughters[0].second->vtx().vz(),
//                0.);
//
//        TLorentzVector pvtosv1(
//                l_daughters[0]->vtx().vx() - prho_daughters[1].second->vtx().vx(),
//                l_daughters[0]->vtx().vy() - prho_daughters[1].second->vtx().vy(),
//                l_daughters[0]->vtx().vz() - prho_daughters[1].second->vtx().vz(),
//                0.);
//
//        lvec3 = ConvertToLorentz(pi_daughters[1]->vector()); //pi charge from rho
//        lvec4 = ConvertToLorentz(pi_daughters[0]->vector()); //pi charge from tau
//
//        TVector3 ip1 = (pvtosv1.Vect() - pvtosv1.Vect().Dot(lvec3.Vect().Unit())*lvec3.Vect().Unit()).Unit();
//        lvec1 = TLorentzVector(ip1, 0.);
//
//        TVector3 ip = (pvtosv.Vect() - pvtosv.Vect().Dot(lvec4.Vect().Unit())*lvec4.Vect().Unit()).Unit();
//        lvec2 = TLorentzVector(ip, 0.);
//    }

    if (rho_daughters.size()==2){
        cp_channel_=3;  
        if(rho_daughters[1].first->charge()<0) {
          lvec1 = ConvertToLorentz(rho_daughters[0].second->vector());   
          lvec2 = ConvertToLorentz(rho_daughters[1].second->vector());
          lvec3 = ConvertToLorentz(rho_daughters[0].first->vector());   
          lvec4 = ConvertToLorentz(rho_daughters[1].first->vector());
        } else {
          lvec1 = ConvertToLorentz(rho_daughters[1].second->vector());
          lvec2 = ConvertToLorentz(rho_daughters[0].second->vector());
          lvec3 = ConvertToLorentz(rho_daughters[1].first->vector());
          lvec4 = ConvertToLorentz(rho_daughters[0].first->vector());
        }
        cp_sign_1_ = YRho(std::vector<GenParticle*>(
                    {rho_daughters[0].first, rho_daughters[0].second}),TVector3())*
                    YRho(std::vector<GenParticle*>(
                    {rho_daughters[1].first, rho_daughters[1].second}),TVector3());
        Ediff_1_ = (rho_daughters[0].first->vector().E() - rho_daughters[0].second->vector().E())/
            (rho_daughters[0].first->vector().E() + rho_daughters[0].second->vector().E());
        Ediff_2_ = (rho_daughters[1].first->vector().E() - rho_daughters[1].second->vector().E())/
            (rho_daughters[1].first->vector().E() + rho_daughters[1].second->vector().E());
    }
    if(cp_channel_==2 || cp_channel_==5){
      aco_angle_1_ = IPAcoAngle(lvec1, lvec2, lvec3, lvec4,false);    
      if (cp_sign_1_>0) {
        if (aco_angle_1_<M_PI) aco_angle_1_ += M_PI;
        else                   aco_angle_1_ -= M_PI;
      }  
    }

    if(cp_channel_==1) {
      aco_angle_1_ = IPAcoAngle(lvec1, lvec2, lvec3, lvec4,false);
    }

    if(cp_channel_!=-1 && cp_channel_==3){
      aco_angle_1_ = IPAcoAngle(lvec1, lvec2, lvec3, lvec4,false);    

      if (cp_sign_1_>0) {
        if (aco_angle_1_<M_PI) aco_angle_1_ += M_PI;
        else                   aco_angle_1_ -= M_PI;
      }  

      pi_E_1_ = rho_daughters[0].first->vector().E();
      pi_E_2_ = rho_daughters[1].first->vector().E();
      pi0_E_1_ = rho_daughters[0].second->vector().E();
      pi0_E_2_ = rho_daughters[1].second->vector().E();

    }
    
    if(charged_daughters.size()==2){
      cp_channel_=7;
      int taucharge1 = -charged_daughters[0][0]->charge(), taucharge2=-charged_daughters[1][0]->charge();
      for(auto x : charged_daughters[0]) taucharge1+=x->charge();
      for(auto x : charged_daughters[1]) taucharge2+=x->charge();
      // first tau
      auto os_pi_1 = charged_daughters[0][1];
      auto ss_pi_1_1 = charged_daughters[0][2];
      auto ss_pi_2_1 = charged_daughters[0][3]; 
      if(charged_daughters[0][2]->charge()!=charged_daughters[0][1]->charge() && charged_daughters[0][2]->charge()!=charged_daughters[0][3]->charge()) {
        os_pi_1 = charged_daughters[0][2];
        ss_pi_1_1 = charged_daughters[0][1];
        ss_pi_2_1 = charged_daughters[0][3];
      }
      if(charged_daughters[0][3]->charge()!=charged_daughters[0][1]->charge() && charged_daughters[0][3]->charge()!=charged_daughters[0][2]->charge()) {
        os_pi_1 = charged_daughters[0][3];
        ss_pi_1_1 = charged_daughters[0][1];
        ss_pi_2_1 = charged_daughters[0][2];
      }
      auto os_pi_2 = charged_daughters[1][1];
      auto ss_pi_1_2 = charged_daughters[1][2];
      auto ss_pi_2_2 = charged_daughters[1][3];
      if(charged_daughters[1][2]->charge()!=charged_daughters[1][1]->charge() && charged_daughters[1][2]->charge()!=charged_daughters[1][3]->charge()) {
        os_pi_2 = charged_daughters[1][2];
        ss_pi_1_2 = charged_daughters[1][1];
        ss_pi_2_2 = charged_daughters[1][3];
      }
      if(charged_daughters[1][3]->charge()!=charged_daughters[1][1]->charge() && charged_daughters[1][3]->charge()!=charged_daughters[1][2]->charge()) {
        os_pi_2 = charged_daughters[1][3];
        ss_pi_1_2 = charged_daughters[1][1];
        ss_pi_2_2 = charged_daughters[1][2];
      }
      TVector3 boost1 = ConvertToLorentz(charged_daughters[0][0]->vector()).BoostVector();
      TVector3 boost2 = ConvertToLorentz(charged_daughters[1][0]->vector()).BoostVector();
      std::vector<TLorentzVector> TauA1andProd1 = {
        ConvertToLorentz(charged_daughters[0][0]->vector()),
        ConvertToLorentz(os_pi_1->vector()), 
        ConvertToLorentz(ss_pi_1_1->vector()), 
        ConvertToLorentz(ss_pi_2_1->vector()) 
      };
      std::vector<TLorentzVector> TauA1andProd2 = {
        ConvertToLorentz(charged_daughters[1][0]->vector()),
        ConvertToLorentz(os_pi_2->vector()),
        ConvertToLorentz(ss_pi_1_2->vector()),
        ConvertToLorentz(ss_pi_2_2->vector())
      };

      PolarimetricA1  a1pol_1;
      a1pol_1.Configure(TauA1andProd1,taucharge1);
      auto l_h1 = a1pol_1.PVC();
      PolarimetricA1  a1pol_2;
      a1pol_2.Configure(TauA1andProd2,taucharge2);
      auto l_h2 = a1pol_2.PVC();
      l_h1.Boost(-boost1);
      l_h2.Boost(-boost2);
 
      auto h1 = l_h1.Vect().Unit();
      auto h2 = l_h2.Vect().Unit();

      //auto h1 = a1pol_1.PVC().Vect().Unit();
      //auto h2 = a1pol_2.PVC().Vect().Unit();

      TVector3 boost = (ConvertToLorentz(charged_daughters[0][0]->vector()+charged_daughters[1][0]->vector())).BoostVector();
      auto l_n1 = ConvertToLorentz(charged_daughters[0][0]->vector());
      auto l_n2 = ConvertToLorentz(charged_daughters[1][0]->vector());

      l_n1.Boost(-boost);
      l_n2.Boost(-boost);

      TVector3 n1 = l_n1.Vect().Unit();
      TVector3 n2 = l_n2.Vect().Unit();

      //std::cout << "----------------" << std::endl;
      //std::cout << n1.X() << "    " << n1.Y() << "    " << n1.Z() << std::endl;
      //std::cout << n2.X() << "    " << n2.Y() << "    " << n2.Z() << std::endl;

      //std::cout << h1.X() << "    " << h1.Y() << "    " << h1.Z() << std::endl;
      //std::cout << h2.X() << "    " << h2.Y() << "    " << h2.Z() << std::endl;

      auto k1 = h1.Cross(n1).Unit(); 
      auto k2 = h2.Cross(n2).Unit();

      //std::cout << k1.X() << "    " << k1.Y() << "    " << k1.Z() << std::endl;
      //std::cout << k2.X() << "    " << k2.Y() << "    " << k2.Z() << std::endl;
std::cout << k1.Dot(k2) << std::endl;
      aco_angle_1_ = acos(k1.Dot(k2));
      double sign = h1.Cross(h2).Dot(n1);
      if(sign>0) aco_angle_1_ = 2*M_PI-aco_angle_1_;

    }*/

    if(fs_) outtree_->Fill();
    return 0;
  }
  int HTTGenAnalysis::PostAnalysis() {
    std::cout << "ee count = " << count_ee_ << std::endl;
    std::cout << "em count = " << count_em_ << std::endl;
    std::cout << "et count = " << count_et_ << std::endl;
    std::cout << "mm count = " << count_mm_ << std::endl;
    std::cout << "mt count = " << count_mt_ << std::endl;
    std::cout << "tt count = " << count_tt_ << std::endl;

    return 0;
  }

  void HTTGenAnalysis::PrintInfo() {
    ;
  }

}
