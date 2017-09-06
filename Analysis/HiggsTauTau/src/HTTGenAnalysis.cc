#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTGenAnalysis.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"

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
    bbtag_eff_ = nullptr;
  }

  HTTGenAnalysis::~HTTGenAnalysis() {
    ;
  }

  int HTTGenAnalysis::PreAnalysis() {
    rand = new TRandom3(0);
    if(fs_){  
      outtree_ = fs_->make<TTree>("gen_ntuple","gen_ntuple");
      outtree_->Branch("event"       , &event_       );
      outtree_->Branch("wt"       , &wt_       );
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
      outtree_->Branch("jdeta"       , &jdeta_       );
      outtree_->Branch("jdphi"       , &jdphi_       );
      outtree_->Branch("jdR"       , &jdR_       );
      outtree_->Branch("channel"  , &decayType    );
      outtree_->Branch("genpt_1"     , &genpt_1_        );
      outtree_->Branch("genpt_2"     , &genpt_2_        );
      outtree_->Branch("geneta_2"    , &geneta_2_       );
      outtree_->Branch("geneta_1"    , &geneta_1_       );
      outtree_->Branch("HiggsPt"     , &HiggsPt_     );
      outtree_->Branch("HiggsPt"     , &HiggsPt_     );
      outtree_->Branch("n_jets_offline"     , &n_jets_offline_);
      outtree_->Branch("n_bjets_offline"     , &n_bjets_offline_); 
      outtree_->Branch("aco_angle_1", &aco_angle_1_);
      outtree_->Branch("aco_angle_2", &aco_angle_2_);
      outtree_->Branch("aco_angle_3", &aco_angle_3_);
      outtree_->Branch("aco_angle_4", &aco_angle_4_);
      outtree_->Branch("cp_sign_1",     &cp_sign_1_);
      outtree_->Branch("cp_sign_2",     &cp_sign_2_);
      outtree_->Branch("cp_sign_3",     &cp_sign_3_);
      outtree_->Branch("cp_sign_4",     &cp_sign_4_);
      outtree_->Branch("cp_channel",    &cp_channel_);
      
      outtree_->Branch("ip_dx_res_1",    &ip_dx_res_1_);
      outtree_->Branch("ip_dx_res_2",    &ip_dx_res_2_);
      outtree_->Branch("ip_dy_res_1",    &ip_dy_res_1_);
      outtree_->Branch("ip_dy_res_2",    &ip_dy_res_2_);
      outtree_->Branch("ip_dz_res_1",    &ip_dz_res_1_);
      outtree_->Branch("ip_dz_res_2",    &ip_dz_res_2_);
    }
    count_ee_ = 0;
    count_em_ = 0;
    count_et_ = 0;
    count_mm_ = 0;
    count_mt_ = 0;
    count_tt_ = 0;
    
    return 0;
  }

  int HTTGenAnalysis::Execute(TreeEvent *event) {
    
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    event_ = (unsigned long long) eventInfo->event();
    wt_ = 1;
    if(eventInfo->weight_defined("wt_mc_sign")) wt_ = eventInfo->weight("wt_mc_sign");
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
    }
    
    std::vector<ic::GenParticle*> gen_particles = event->GetPtrVec<ic::GenParticle>("genParticles");
    std::vector<ic::GenJet*> gen_jets = event->GetPtrVec<ic::GenJet>("genJets");

    std::vector<ic::GenParticle> higgs_products;
    std::vector<ic::GenParticle> gen_taus;
    ic::Candidate met; 
    std::vector<ic::GenParticle> prompt_leptons;
    std::vector<std::string> decay_types;

    
    HiggsPt_=-9999;
    for(unsigned i=0; i<gen_particles.size(); ++i){
      if((gen_particles[i]->statusFlags()[FromHardProcessBeforeFSR] || gen_particles[i]->statusFlags()[IsLastCopy]) && gen_particles[i]->pdgid() == 25) {
          HiggsPt_ = gen_particles[i]->pt();
          //ic::Vertex higgs_vtxs = gen_particles[i]->vtx();
          //std::cout << "----------------------" << std::endl;
          //std::cout << higgs_vtxs.vx() << "    " << higgs_vtxs.vy() << "    " << higgs_vtxs.vy() << std::endl;
      }

      
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
    }
    
    std::vector<GenJet> gen_tau_jets = BuildTauJets(gen_particles, false,true);
    std::vector<GenJet> gen_tau_jets_incnus = BuildTauJetsIncNus(gen_particles, false,true);
    std::vector<GenJet *> gen_tau_jets_ptr;
    for (auto & x : gen_tau_jets) gen_tau_jets_ptr.push_back(&x);
    ic::erase_if(gen_tau_jets_ptr, !boost::bind(MinPtMaxEta, _1, 15.0, 999.));
    std::sort(gen_tau_jets_ptr.begin(), gen_tau_jets_ptr.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    std::vector<GenJet *> gen_tau_jets_ptr_incnus;
    for (auto & x : gen_tau_jets_incnus) gen_tau_jets_ptr_incnus.push_back(&x);
    ic::erase_if(gen_tau_jets_ptr_incnus, !boost::bind(MinPtMaxEta, _1, 15.0, 999.));
    std::sort(gen_tau_jets_ptr_incnus.begin(), gen_tau_jets_ptr_incnus.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    cp_sign_1_ = 0;
    cp_sign_2_ = 0;
    cp_sign_3_ = 0;
    cp_sign_4_ = 0;
    aco_angle_1_ = 0;
    aco_angle_2_ = 0;
    aco_angle_3_ = 0;
    aco_angle_4_ = 0;
    cp_channel_=-1;
    ip_dx_res_1_ = -9999;
    ip_dx_res_2_ = -9999;
    ip_dy_res_1_ = -9999;
    ip_dy_res_2_ = -9999;
    ip_dz_res_1_ = -9999;
    ip_dz_res_2_ = -9999;
    std::vector<ic::Vertex*> primary_vtxs = event->GetPtrVec<ic::Vertex>("genVertices"); 
    std::vector<ic::Vertex*> offline_primary_vtxs = event->GetPtrVec<ic::Vertex>("vertices");
    
    // use these for loops to compare generator level to offline taus/leptons
    std::vector<ic::Tau*> offline_taus = event->GetPtrVec<ic::Tau>("taus");
    bool FirstTau = true;
    for (unsigned i=0; i<offline_taus.size(); ++i){
      for(unsigned j=0; j<gen_tau_jets_ptr.size(); ++j){
        double dR = ROOT::Math::VectorUtil::DeltaR(offline_taus[i]->vector(), gen_tau_jets_ptr[j]->vector());
        if(dR < 0.5){
          std::pair<bool,GenParticle*> pi = GetTauPiDaughter(gen_particles, gen_tau_jets_ptr[j]->constituents());
          if (!pi.first) continue;
          //ic::Vertex offline_tau_point;
          //offline_tau_point.set_vx(offline_taus[i]->vx()); offline_tau_point.set_vy(offline_taus[i]->vy()); offline_tau_point.set_vz(offline_taus[i]->vz());
          //TVector3 ip_offline = GetGenImpactParam(*(offline_primary_vtxs[0]),offline_tau_point, offline_taus[i]->vector());
          //std::cout << "--------------------" << std::endl;
          //std::cout << offline_primary_vtxs[0]->vx() << "    " << offline_primary_vtxs[0]->vy() << "    " << offline_primary_vtxs[0]->vz() << std::endl;
          //std::cout << offline_tau_point.vx() << "    " << offline_tau_point.vy() << "    " << offline_tau_point.vz() << std::endl;
          TVector3 ip = GetGenImpactParam(*(primary_vtxs[0]),pi.second->vtx(), pi.second->vector());
          
          //ip = ip.Unit();

          //double dy_res = (ip_offline.Y() - ip.Y())/ip.Y();
          //double dz_res = (ip_offline.Z() - ip.Z())/ip.Z();
          //double dx_res = (ip_offline.X() - ip.X())/ip.X();
          double dz = offline_taus[i]->lead_dz_vertex();
          double dxy = offline_taus[i]->lead_dxy_vertex();
          double m = offline_taus[i]->vector().Py()/offline_taus[i]->vector().Px();
          double dx = sqrt( pow(dxy,2)/(1/pow(m,2)-1) );
          double dy = sqrt(pow(dxy,2) - pow(dx,2));
          if(m*dxy*offline_taus[i]->vector().Px()<0) dx*=-1;
          if(m*dxy*offline_taus[i]->vector().Py()>0) dy*=-1;
          
          TVector3 ip_offline(dx,dy,dz);
          //ip_offline = ip_offline.Unit();
          
          double dx_res = (ip_offline.X() - ip.X())/ip.X();
          double dy_res = (ip_offline.Y() - ip.Y())/ip.Y();
          double dz_res = (ip_offline.Z() - ip.Z())/ip.Z();
          
          //dz_res = (dz/dxy - ip.Z()/sqrt(pow(ip.X(),2)+pow(ip.Y(),2)))/(sqrt(pow(ip.X(),2)+pow(ip.Y(),2)));
          double offline_dz = offline_taus[i]->lead_dz_vertex();
          double gen_dz = ip.Z();
          dz_res = (gen_dz-offline_dz)/gen_dz;
          if (FirstTau){
            ip_dz_res_1_ = dz_res;  
            ip_dy_res_1_ = dy_res;
            ip_dx_res_1_ = dx_res;
            FirstTau = false;
          } else {
            ip_dz_res_2_ = dz_res;   
            ip_dy_res_2_ = dy_res;
            ip_dx_res_2_ = dx_res;
          }
          break;
        }
      }
    }
    
    std::pair<bool,GenParticle*> pi_1 = std::make_pair(false, new GenParticle());
    std::pair<bool,std::vector<GenParticle*>> rho_1 = std::make_pair(false, std::vector<GenParticle*>()); 
    std::pair<bool,std::vector<GenParticle*>> a1_1 = std::make_pair(false, std::vector<GenParticle*>());
    std::pair<bool,GenParticle*> pi_2 = std::make_pair(false, new GenParticle());
    std::pair<bool,std::vector<GenParticle*>> rho_2 = std::make_pair(false, std::vector<GenParticle*>()); 
    std::pair<bool,std::vector<GenParticle*>> a1_2 = std::make_pair(false, std::vector<GenParticle*>());
    
    if(gen_tau_jets_ptr.size()>=1){
      pi_1 = GetTauPiDaughter(gen_particles, gen_tau_jets_ptr[0]->constituents()); 
      rho_1 = GetTauRhoDaughter(gen_particles, gen_tau_jets_ptr[0]->constituents());  
      a1_1 = GetTauA1Daughter(gen_particles, gen_tau_jets_ptr[0]->constituents());  
    } 
    if(gen_tau_jets_ptr.size()>=2){
      pi_2 = GetTauPiDaughter(gen_particles, gen_tau_jets_ptr[1]->constituents()); 
      rho_2 = GetTauRhoDaughter(gen_particles, gen_tau_jets_ptr[1]->constituents());  
      a1_2 = GetTauA1Daughter(gen_particles, gen_tau_jets_ptr[1]->constituents()); 
    }
    std::vector<ic::GenParticle> leptons;
    for (unsigned i=0; i<electrons.size(); ++i) leptons.push_back(electrons[i]);
    for (unsigned i=0; i<muons.size(); ++i) leptons.push_back(muons[i]);
    TLorentzVector lvec1;
    TLorentzVector lvec2;
    TLorentzVector lvec3;
    TLorentzVector lvec4;
    if(leptons.size()>=2){
      cp_channel_=1;
      lvec1 = TLorentzVector(GetGenImpactParam(*(primary_vtxs[0]),leptons[0].vtx(), leptons[0].vector()),0);    
      lvec2 = TLorentzVector(GetGenImpactParam(*(primary_vtxs[0]),leptons[1].vtx(), leptons[1].vector()),0);
      lvec3 = ConvertToLorentz(leptons[0].vector());
      lvec4 = ConvertToLorentz(leptons[1].vector());
    } else if(leptons.size()==1){
      lvec1 = TLorentzVector(GetGenImpactParam(*(primary_vtxs[0]),leptons[0].vtx(), leptons[0].vector()),0);
      lvec3 = ConvertToLorentz(leptons[0].vector());
      if(pi_1.first){cp_channel_=1; lvec2 = TLorentzVector(GetGenImpactParam(*(primary_vtxs[0]),pi_1.second->vtx(), pi_1.second->vector()),0); lvec4 = ConvertToLorentz(pi_1.second->vector());}
      if(pi_2.first){cp_channel_=1; lvec2 = TLorentzVector(GetGenImpactParam(*(primary_vtxs[0]),pi_2.second->vtx(), pi_2.second->vector()),0); lvec4 = ConvertToLorentz(pi_2.second->vector());}
      if(rho_1.first){cp_channel_=2; lvec2 = ConvertToLorentz(rho_1.second[1]->vector()); lvec4 = ConvertToLorentz(rho_1.second[0]->vector()); cp_sign_1_ = YRho(rho_1.second,TVector3());}
      if(rho_2.first){cp_channel_=2; lvec2 = ConvertToLorentz(rho_2.second[1]->vector()); lvec4 = ConvertToLorentz(rho_2.second[0]->vector()); cp_sign_1_ = YRho(rho_2.second,TVector3());}
    } else{
      if(pi_1.first&&pi_2.first){
        cp_channel_=1;
        lvec1 = TLorentzVector(GetGenImpactParam(*(primary_vtxs[0]),pi_1.second->vtx(), pi_1.second->vector()),0);
        lvec2 = TLorentzVector(GetGenImpactParam(*(primary_vtxs[0]),pi_2.second->vtx(), pi_2.second->vector()),0);
        lvec3 = ConvertToLorentz(pi_1.second->vector());
        lvec4 = ConvertToLorentz(pi_2.second->vector());
      } else if (pi_1.first&&rho_2.first){
        cp_channel_=2;
        lvec1 = TLorentzVector(GetGenImpactParam(*(primary_vtxs[0]),pi_1.second->vtx(), pi_1.second->vector()),0);
        lvec2 = ConvertToLorentz(rho_2.second[1]->vector());
        lvec3 = ConvertToLorentz(pi_1.second->vector());
        lvec4 = ConvertToLorentz(rho_2.second[0]->vector());
        cp_sign_1_ = YRho(rho_2.second,TVector3());
      } else if(pi_2.first&&rho_1.first){
        cp_channel_=2;
        lvec1 = TLorentzVector(GetGenImpactParam(*(primary_vtxs[0]),pi_2.second->vtx(), pi_2.second->vector()),0);
        lvec2 = ConvertToLorentz(rho_1.second[1]->vector());
        lvec3 = ConvertToLorentz(pi_2.second->vector());
        lvec4 = ConvertToLorentz(rho_1.second[0]->vector());
        cp_sign_1_ = YRho(rho_1.second,TVector3());
      } else if (rho_1.first&&rho_2.first){
        cp_channel_=3;  
        lvec1 = ConvertToLorentz(rho_1.second[1]->vector());   
        lvec2 = ConvertToLorentz(rho_2.second[1]->vector());
        lvec3 = ConvertToLorentz(rho_1.second[0]->vector());   
        lvec4 = ConvertToLorentz(rho_2.second[0]->vector());
        cp_sign_1_ = YRho(rho_1.second,TVector3())*YRho(rho_2.second,TVector3());
      }
    }
    
    if(cp_channel_!=-1){
      aco_angle_1_ = IPAcoAngle(lvec1, lvec2, lvec3, lvec4,false);    
      aco_angle_2_ = IPAcoAngle(lvec1, lvec2, lvec3, lvec4,true);
    }
    
    if(gen_tau_jets_ptr.size()>=2){
      //if(rho_1.first && rho_2.first) { 
      //  std::vector<std::pair<double,int>> angles = AcoplanarityAngles(rho_1.second,rho_2.second,true);
      //  aco_angle_2_ = angles[0].first;
      //  cp_sign_2_ = angles[0].second;
      //}
      //if(rho_1.first && a1_2.first) {
      //  cp_channel_ = 3;
      //  std::vector<std::pair<double,int>> angles = AcoplanarityAngles(rho_1.second,a1_2.second,true);
      //  aco_angle_1_ = angles[0].first;
      //  cp_sign_1_ = angles[0].second;
      //  aco_angle_2_ = angles[1].first;
      //  cp_sign_2_ = angles[1].second;
      //  aco_angle_3_ = angles[2].first;
      //  cp_sign_3_ = angles[2].second;
      //  aco_angle_4_ = angles[3].first;
      //  cp_sign_4_ = angles[3].second;
      //}
      //if(a1_1.first && rho_2.first) {
      //  cp_channel_ = 3;
      //  std::vector<std::pair<double,int>> angles = AcoplanarityAngles(rho_2.second,a1_1.second,true);
      //  aco_angle_1_ = angles[0].first;
      //  cp_sign_1_ = angles[0].second;
      //  aco_angle_2_ = angles[1].first;
      //  cp_sign_2_ = angles[1].second;
      //  aco_angle_3_ = angles[2].first;
      //  cp_sign_3_ = angles[2].second;
      //  aco_angle_4_ = angles[3].first;
      //  cp_sign_4_ = angles[3].second;
      //}

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
      mt_1_ = MT(&lep1, &met);
      mt_2_ = MT(&lep2, &met);

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
    
    std::string jets_label_ = "ak4PFJetsCHS";
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
    std::sort(jets.begin(), jets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    
    for(unsigned i=0; i<jets.size(); ++i){
      ic::PFJet *jet = jets[i];
      bool MatchedToPrompt = false;
      for(unsigned j=0; j<higgs_products.size(); ++j){
        if(DRLessThan(std::make_pair(jet, &higgs_products[j]),0.5)) MatchedToPrompt = true;
      }
      //remove jets that are matched to Higgs decay products
      if(MatchedToPrompt) jets.erase (jets.begin()+i);
    }
    
    std::vector<PFJet*> offline_bjets = jets;
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
    ic::erase_if(offline_bjets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
    
    std::string btag_label="pfCombinedInclusiveSecondaryVertexV2BJetTags";
    double btag_wp =  0.8484;
    if (event->Exists("retag_result")) {
      auto const& retag_result = event->Get<std::map<std::size_t,bool>>("retag_result"); 
      ic::erase_if(offline_bjets, !boost::bind(IsReBTagged, _1, retag_result));
    }else { 
      ic::erase_if(offline_bjets, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label) < btag_wp);
    }
    
    
    n_jets_offline_ = jets.size();
    n_bjets_offline_ = offline_bjets.size();

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
    jdphi_       = -9999;
    jdR_         = -9999;
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
      jdphi_ =  std::fabs(ROOT::Math::VectorUtil::DeltaPhi(filtered_jets[0].vector(), filtered_jets[1].vector()));
      jdR_ =  std::fabs(ROOT::Math::VectorUtil::DeltaR(filtered_jets[0].vector(), filtered_jets[1].vector()));
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
    //std::cout << "ee count = " << count_ee_ << std::endl;
    //std::cout << "em count = " << count_em_ << std::endl;
    //std::cout << "et count = " << count_et_ << std::endl;
    //std::cout << "mm count = " << count_mm_ << std::endl;
    //std::cout << "mt count = " << count_mt_ << std::endl;
    //std::cout << "tt count = " << count_tt_ << std::endl;
    return 0;
  }

  void HTTGenAnalysis::PrintInfo() {
    ;
  }

}
