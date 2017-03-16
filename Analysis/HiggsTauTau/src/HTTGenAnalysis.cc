#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTGenAnalysis.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnRootTools.h"

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
  
namespace ic {

  HTTGenAnalysis::HTTGenAnalysis(std::string const& name) : ModuleBase(name) {
    fs_ = NULL;
  }

  HTTGenAnalysis::~HTTGenAnalysis() {
    ;
  }

  int HTTGenAnalysis::PreAnalysis() {
     
    z_pt_mass_hist_ = GetFromTFile<TH2D>("input/zpt_weights/zpt_weights_summer2016.root","/","zptmass_histo");
      
    if(fs_){  
      outtree_ = fs_->make<TTree>("gen_ntuple","gen_ntuple");
      outtree_->Branch("event"       , &event_       );
      outtree_->Branch("wt"       , &wt_       );
      outtree_->Branch("wt_dy"       , &wt_dy_       );
      outtree_->Branch("wt_zpt"       , &wtzpt_       );
      if(do_theory_uncert_){
        outtree_->Branch("scale_variation_wts", &scale_variation_wts_);
        outtree_->Branch("NNPDF_wts", &NNPDF_wts_);
        outtree_->Branch("alpha_s_wts", &alpha_s_wts_);
        outtree_->Branch("CT10_wts", &CT10_wts_);
        outtree_->Branch("CT10_alpha_s_wts", &CT10_alpha_s_wts_);
        outtree_->Branch("MMHT_wts", &MMHT_wts_);
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
      outtree_->Branch("HiggsPt"     , &HiggsPt_     );
      outtree_->Branch("FirstHiggsPt" , &FirstHiggsPt_     );
      outtree_->Branch("mt_1"        , &mt_1_        );
      outtree_->Branch("mt_2"        , &mt_2_        );
      outtree_->Branch("pzeta"       , &pzeta_       );
      outtree_->Branch("n_bjets"     , &n_bjets_     );
      outtree_->Branch("n_jets"      , &n_jets_      );
      outtree_->Branch("n_jets_nofilter"      , &n_jets_nofilter_      );
      outtree_->Branch("n_jetsingap" , &n_jetsingap_ );
      outtree_->Branch("jpt_1"       , &jpt_1_       );
      outtree_->Branch("jpt_2"       , &jpt_2_       );
      outtree_->Branch("jpt_3"       , &jpt_3_       );
      outtree_->Branch("jeta_1"      , &jeta_1_      );
      outtree_->Branch("jeta_2"      , &jeta_2_      );
      outtree_->Branch("jphi_1"      , &jphi_1_      );
      outtree_->Branch("jphi_2"      , &jphi_2_      );
      outtree_->Branch("mjj"         , &mjj_         );
      outtree_->Branch("jdeta"       , &jdeta_       );
      outtree_->Branch("higgsDecay"  , &decayType    );
      outtree_->Branch("genpt_1"     , &genpt_1_        );
      outtree_->Branch("genpt_2"     , &genpt_2_        );
      outtree_->Branch("geneta_2"    , &geneta_2_       );
      outtree_->Branch("geneta_1"    , &geneta_1_       );
      //outtree_->Branch("wt_ggh_pt", &wt_ggh_pt_           );
      //outtree_->Branch("wt_ggh_pt_up", &wt_ggh_pt_up_        );
      //outtree_->Branch("wt_ggh_pt_down", &wt_ggh_pt_down_      );
      //outtree_->Branch("wt_ggh_pt_herwig", &wt_ggh_pt_herwig_    );
      //outtree_->Branch("wt_ggh_pt_amc", &wt_ggh_pt_amc_       );
      //outtree_->Branch("wt_ggh_pt_pythiaup", &wt_ggh_pt_pythiaup_  );
      //outtree_->Branch("wt_ggh_pt_pythiadown", &wt_ggh_pt_pythiadown_);
      //outtree_->Branch("wt_ggh_pt_scalehigh", &wt_ggh_pt_scalehigh_ );
      //outtree_->Branch("wt_ggh_pt_scalelow", &wt_ggh_pt_scalelow_  );
      outtree_->Branch("hasFSR", &hasFSR_  );
      outtree_->Branch("n_bquarks",    &n_bquarks_);
      outtree_->Branch("genpT",    &genpT_);
      outtree_->Branch("genM",    &genM_);
    }
    count_ee_ = 0;
    count_em_ = 0;
    count_et_ = 0;
    count_mm_ = 0;
    count_mt_ = 0;
    count_tt_ = 0;
    
    //std::string file = "input/ggh_weights/HqT_weight_pTH_summer16_80X_AllSamples.root";
    //ggh_weights_ = new TFile(file.c_str());
    //ggh_weights_->cd();
    //ggh_hist_ = (TH1F*)gDirectory->Get("Powheg_Nominal");
    //ggh_hist_up_ = (TH1F*)gDirectory->Get("Powheg_ScaleUp");
    //ggh_hist_down_ = (TH1F*)gDirectory->Get("Powheg_ScaleDown");
    //ggh_herwig_hist_      = (TH1F*)gDirectory->Get("Herwig_Nominal"            );
    //ggh_amcnlo_hist_      = (TH1F*)gDirectory->Get("aMC_Nominal"           );
    //ggh_pythiaup_hist_    = (TH1F*)gDirectory->Get("PythiaFragmentUp_Nominal"  );
    //ggh_pythiadown_hist_ = (TH1F*)gDirectory->Get("PythiaFragmentDown_Nominal");
    //ggh_scalehigh_        = (TH1F*)gDirectory->Get("Powheg_Nominal_ScaleHigh"   );
    //ggh_scalelow_         = (TH1F*)gDirectory->Get("Powheg_Nominal_ScaleLow"   );
    return 0;
  }

  int HTTGenAnalysis::Execute(TreeEvent *event) {
    
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    std::vector<ic::GenParticle*> gen_particles = event->GetPtrVec<ic::GenParticle>("genParticles");
    std::vector<ic::GenJet*> gen_jets = event->GetPtrVec<ic::GenJet>("genJets");
    event_ = (unsigned long long) eventInfo->event();
    wt_ = 1;
    wt_ = eventInfo->weight("wt_mc_sign");
    wt_dy_=1;
    if (event->Exists("wt_dy")) wt_dy_ = event->Get<double>("wt_dy");
    //wt_ggh_pt_            = 1;
    //wt_ggh_pt_up_         = 1;
    //wt_ggh_pt_down_       = 1;
    //wt_ggh_pt_herwig_     = 1;
    //wt_ggh_pt_amc_        = 1;
    //wt_ggh_pt_pythiaup_   = 1;
    //wt_ggh_pt_pythiadown_ = 1;
    //wt_ggh_pt_scalehigh_  = 1;
    //wt_ggh_pt_scalelow_   = 1;
    
    n_bquarks_=0;
    if (event->Exists("n_bquarks")) n_bquarks_ = event->Get<unsigned>("n_bquarks");
    
    std::vector<GenParticle *> sel_bquarks;
    for (unsigned i=0; i < gen_particles.size(); ++i){
      std::vector<bool> status_flags = gen_particles[i]->statusFlags();
      unsigned id = abs(gen_particles[i]->pdgid());  
      if(id == 5 && status_flags[FromHardProcess] && status_flags[IsLastCopy]){
        sel_bquarks.push_back(gen_particles[i]);
      }
    }
    n_bquarks_ = sel_bquarks.size();
    
    std::vector<GenParticle *> sel_gen_parts;
    
    for(unsigned i = 0; i < gen_particles.size(); ++i){
        std::vector<bool> status_flags = gen_particles[i]->statusFlags();
        unsigned id = abs(gen_particles[i]->pdgid());
        unsigned status = abs(gen_particles[i]->status());
        if ( (id >= 11 && id <= 16 && status_flags[FromHardProcess] && status==1) || status_flags[IsDirectHardProcessTauDecayProduct]) sel_gen_parts.push_back(gen_particles[i]);
     }

     ROOT::Math::PtEtaPhiEVector gen_boson;
     for( unsigned i = 0; i < sel_gen_parts.size() ; ++i){
       gen_boson += sel_gen_parts[i]->vector();
     }
     genpT_ = gen_boson.pt();
     genM_ = gen_boson.M();
     
     wtzpt_ = z_pt_mass_hist_.GetBinContent(z_pt_mass_hist_.GetXaxis()->FindBin(genM_),z_pt_mass_hist_.GetYaxis()->FindBin(genpT_));
    
    
    if(do_theory_uncert_){
      scale_variation_wts_.clear();
      for(unsigned i=1001; i<=1009; ++i){
        std::string label = Form("%u",i);
        double wt_temp = eventInfo->weight(label);
        scale_variation_wts_.push_back(wt_temp);
      }
      NNPDF_wts_.clear();
      for(unsigned i=2001; i<=2100; ++i){
        std::string label = Form("%u",i);
        double wt_temp = eventInfo->weight(label);
        NNPDF_wts_.push_back(wt_temp);
      }
      alpha_s_wts_.clear();
      for(unsigned i=2101; i<=2102; ++i){
        std::string label = Form("%u",i);
        double wt_temp = eventInfo->weight(label);
        alpha_s_wts_.push_back(wt_temp);
      }
      CT10_wts_.clear();
      for(unsigned i=3001; i<=3053; ++i){
        std::string label = Form("%u",i);
        double wt_temp = eventInfo->weight(label);
        CT10_wts_.push_back(wt_temp);
      }
      CT10_alpha_s_wts_.clear();
      for(unsigned i=3054; i<=3055; ++i){
        std::string label = Form("%u",i);
        double wt_temp = eventInfo->weight(label);
        CT10_alpha_s_wts_.push_back(wt_temp);
      }
      MMHT_wts_.clear();
      for(unsigned i=4001; i<=4056; ++i){
        std::string label = Form("%u",i);
        double wt_temp = eventInfo->weight(label);
        MMHT_wts_.push_back(wt_temp);
      }
    }

    std::vector<ic::GenParticle> higgs_products;
    std::vector<ic::GenParticle> gen_taus;
    ic::Candidate met; 
    std::vector<ic::GenParticle> prompt_leptons;
    std::vector<std::string> decay_types;

    
    HiggsPt_=-9999;
    FirstHiggsPt_ = -9999;
    hasFSR_ = false;
    for(unsigned i=0; i<gen_particles.size(); ++i){
      if(gen_particles[i]->pdgid() == 25 && gen_particles[gen_particles[i]->mothers()[0]]->pdgid()!=25){FirstHiggsPt_ = gen_particles[i]->pt();}
      if((gen_particles[i]->statusFlags()[FromHardProcessBeforeFSR] || gen_particles[i]->statusFlags()[IsLastCopy]) && gen_particles[i]->pdgid() == 25) {
          HiggsPt_ = gen_particles[i]->pt();
      }

      //int fbin = ggh_hist_->FindBin(HiggsPt_);
      //if (fbin > 0 && fbin <= ggh_hist_->GetNbinsX()) {
      //  wt_ggh_pt_            = ggh_hist_->GetBinContent(fbin);
      //  wt_ggh_pt_up_         = ggh_hist_up_->GetBinContent(fbin)  ;
      //  wt_ggh_pt_down_       = ggh_hist_down_->GetBinContent(fbin);
      //  wt_ggh_pt_herwig_     = ggh_herwig_hist_->GetBinContent(fbin);
      //  wt_ggh_pt_amc_        = ggh_amcnlo_hist_->GetBinContent(fbin);
      //  wt_ggh_pt_pythiaup_   = ggh_pythiaup_hist_->GetBinContent(fbin);
      //  wt_ggh_pt_pythiadown_ = ggh_pythiadown_hist_->GetBinContent(fbin);
      //  wt_ggh_pt_scalehigh_  = ggh_scalehigh_->GetBinContent(fbin);
      //  wt_ggh_pt_scalelow_   = ggh_scalelow_->GetBinContent(fbin);
      //}
      
      ic::GenParticle part = *gen_particles[i];
      ic::GenParticle higgs_product;
      
      unsigned genID = std::fabs(part.pdgid());
      bool status_flag_t = part.statusFlags().at(0);
      bool status_flag_tlc = part.statusFlags().at(13);

      // add neutrinos 4-vectors to get gen met
      if(genID == 12 || genID == 14 || genID == 16){
        met.set_vector(met.vector() + part.vector());
        continue;
      }
      
      if(genID == 25){
        for(unsigned j=0; j<part.daughters().size(); ++j){
          ic::GenParticle d = *gen_particles[part.daughters().at(j)];
          unsigned daughterID = std::fabs(d.pdgid());
          if(daughterID == 22) hasFSR_ = true;
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

    if(channel_str_ == "em"){
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
    } else if(channel_str_ == "zmm"){
      if(muons.size() == 2){
        lep1 = muons[0];
        lep2 = muons[1];
        passed_ = true;
      }
    } else if(channel_str_ == "zee"){
      if(electrons.size() == 2){
        lep1 = electrons[0];
        lep2 = electrons[1];
        passed_ = true;
      }
    }

    if(passed_){
      pt_1_  = lep1.vector().Pt();
      pt_2_  = lep2.vector().Pt();
      eta_1_ = lep1.vector().Rapidity();
      eta_2_ = lep2.vector().Rapidity();
      phi_1_ = lep1.vector().Phi();
      phi_2_ = lep2.vector().Phi();
      met_   = met.vector().Pt();
      pt_tt_ = (met.vector()+lep1.vector()+lep2.vector()).Pt();
      m_vis_ = (lep1.vector()+lep2.vector()).M();
      n_bjets_ = 0;
      mt_1_ = MT(&lep1, &met);
      mt_2_ = MT(&lep2, &met);

      ic::CompositeCandidate *ditau = new ic::CompositeCandidate();
      ditau->AddCandidate("lep1",&lep1);
      ditau->AddCandidate("lep2",&lep2);
      pzeta_ = PZeta(ditau, &met , 0.85);
    } else {
      n_bjets_ = -9999;  
      pt_1_  = -9999;
      pt_2_  = -9999;
      eta_1_ = -9999;
      eta_2_ = -9999;
      phi_1_ = -9999;
      phi_2_ = -9999;
      met_   = -9999;
      pt_tt_ = -9999;
      m_vis_ = -9999;
      mt_1_ = -9999;
      mt_2_ = -9999;
      pzeta_ = -9999;
    }
    
    std::vector<ic::GenJet> filtered_jets;
     
    for(unsigned i=0; i<gen_jets.size(); ++i){
      ic::GenJet jet = *gen_jets[i];
      double jetPt = jet.vector().Pt();
      double jetEta = std::fabs(jet.vector().Rapidity());
      if(jetPt > min_jet_pt_ && jetEta < max_jet_eta_) filtered_jets.push_back(jet); 
    }
    
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

    if(fs_) outtree_->Fill();
    
    return 0;
  }
  int HTTGenAnalysis::PostAnalysis() {
    return 0;
  }

  void HTTGenAnalysis::PrintInfo() {
    ;
  }

}
