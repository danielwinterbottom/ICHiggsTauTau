#ifndef ICHiggsTauTau_HiggsTauTau_Pi0MVA_h
#define ICHiggsTauTau_HiggsTauTau_Pi0MVA_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/PFCandidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include <string>
#include "Utilities/interface/FnRootTools.h"
#include "TMVA/Reader.h"

namespace ic {

  template<class T>
  std::vector<std::pair<T*,std::vector<T*>>> ClusterGammas(std::vector<T*> gammas, int p) {
    //cluster gammas into 2 objects where distance measure is annalogous to jet algorithms anti-KT: p=-1, KT p=+1, C/A: p=0. 
    //The eta and phi of a clustered object is set as the energy weighted average   
    std::vector<std::pair<T*,std::vector<T*>>> clusters;

    // to start each gamma gives one cluster 
    for (auto g : gammas) {
      std::vector<T*> g_vec = {g};
      clusters.push_back(std::make_pair(g,g_vec));
    }
    while(clusters.size()>2) {
      int min_i=-1, min_j=-1;
      double min_d=0;
      for(unsigned i=0; i<clusters.size()-1;++i) {
        for(unsigned j=i+1; j<clusters.size();++j) {
          double d = pow(ROOT::Math::VectorUtil::DeltaR(clusters[i].first->vector(),clusters[j].first->vector()),2)*std::min(pow(clusters[i].first->pt(),2*p),pow(clusters[j].first->pt(),2*p));
          if(min_i==-1 || d< min_d) {
            min_d = d;
            min_i=i;
            min_j=j;
          }
        }
      }
      // merge pair with smallest d, remove old elements from vector and start again
      std::vector<T*> new_vec = clusters[min_i].second;
      new_vec.insert(new_vec.end(), clusters[min_j].second.begin(), clusters[min_j].second.end());
      double E=clusters[min_i].first->energy() + clusters[min_j].first->energy();
      double phi = (clusters[min_i].first->energy()*clusters[min_i].first->phi() + clusters[min_j].first->energy()*clusters[min_j].first->phi())/E;
      double eta = (clusters[min_i].first->energy()*clusters[min_i].first->eta() + clusters[min_j].first->energy()*clusters[min_j].first->eta())/E;
      double theta = atan(exp(-eta))*2;
      double pt = E*sin(theta);
      T* new_cluster = new T();
      ROOT::Math::PtEtaPhiEVector vec = ROOT::Math::PtEtaPhiEVector(pt, eta, phi, E);
      new_cluster->set_vector(vec);
      clusters.erase(clusters.begin()+std::max(min_j,min_i));
      clusters.erase(clusters.begin()+std::min(min_j,min_i));
      std::sort(new_vec.begin(), new_vec.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
      clusters.push_back(std::make_pair(new_cluster,new_vec));
    }
    if(clusters.size()==2) {
      if(clusters[0].first->pt() < clusters[1].first->pt()) {
        auto temp = clusters[0];
        clusters[0] = clusters[1];
        clusters[1] = temp; 
      }
    } 
    return clusters;
  }

  template<class T>
  std::vector<std::pair<T*,std::vector<T*>>> ClusterGammasNew(std::vector<T*> gammas, int p) {
    //cluster gammas into 2 objects where distance measure is annalogous to jet algorithms anti-KT: p=-1, KT p=+1, C/A: p=0. 
    //The eta and phi of a clustered object is set as the energy weighted average   
    std::vector<std::pair<T*,std::vector<T*>>> clusters;

    // to start each gamma gives one cluster 
    for (auto g : gammas) {
      std::vector<T*> g_vec = {g};
      clusters.push_back(std::make_pair(g,g_vec));
    }
    while(clusters.size()>2) {
      int min_i=-1, min_j=-1;
      double min_d=0;
      for(unsigned i=0; i<clusters.size()-1;++i) {
        for(unsigned j=i+1; j<clusters.size();++j) {
          double etaAssociationDistance = std::max(std::min(0.20*pow(clusters[i].first->pt(),-0.66) + 0.20*pow(clusters[j].first->pt(),-0.66),0.15),0.05);
          double phiAssociationDistance = std::max(std::min(0.35*pow(clusters[i].first->pt(),-0.71) + 0.35*pow(clusters[j].first->pt(),-0.71),0.30),0.05);

          double d = (pow(ROOT::Math::VectorUtil::DeltaPhi(clusters[i].first->vector(),clusters[j].first->vector())/phiAssociationDistance,2) + pow((clusters[i].first->eta()-clusters[j].first->eta())/etaAssociationDistance,2))*std::min(pow(clusters[i].first->pt(),2*p),pow(clusters[j].first->pt(),2*p));
 
          //double d = pow(ROOT::Math::VectorUtil::DeltaR(clusters[i].first->vector(),clusters[j].first->vector()),2)*std::min(pow(clusters[i].first->pt(),2*p),pow(clusters[j].first->pt(),2*p));
          if(min_i==-1 || d< min_d) {
            min_d = d;
            min_i=i;
            min_j=j;
          }
        }
      }
      // merge pair with smallest d, remove old elements from vector and start again
      std::vector<T*> new_vec = clusters[min_i].second;
      new_vec.insert(new_vec.end(), clusters[min_j].second.begin(), clusters[min_j].second.end());
      double E=clusters[min_i].first->energy() + clusters[min_j].first->energy();
      double phi = (clusters[min_i].first->energy()*clusters[min_i].first->phi() + clusters[min_j].first->energy()*clusters[min_j].first->phi())/E;
      double eta = (clusters[min_i].first->energy()*clusters[min_i].first->eta() + clusters[min_j].first->energy()*clusters[min_j].first->eta())/E;
      double theta = atan(exp(-eta))*2;
      double pt = E*sin(theta);
      T* new_cluster = new T();
      ROOT::Math::PtEtaPhiEVector vec = ROOT::Math::PtEtaPhiEVector(pt, eta, phi, E);
      new_cluster->set_vector(vec);
      clusters.erase(clusters.begin()+std::max(min_j,min_i));
      clusters.erase(clusters.begin()+std::min(min_j,min_i));
      std::sort(new_vec.begin(), new_vec.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
      clusters.push_back(std::make_pair(new_cluster,new_vec));
    }
    if(clusters.size()==2) {
      if(clusters[0].first->pt() < clusters[1].first->pt()) {
        auto temp = clusters[0];
        clusters[0] = clusters[1];
        clusters[1] = temp;
      }
    }
    return clusters;
  }

  
class Pi0MVA : public ModuleBase {
 private:
  CLASS_MEMBER(Pi0MVA, fwlite::TFileService*, fs)
  CLASS_MEMBER(Pi0MVA, ic::channel, channel)
  CLASS_MEMBER(Pi0MVA, bool, maketrees)
  CLASS_MEMBER(Pi0MVA, ic::strategy, strategy)
  
  TTree *outtree_;
  double wt_;
  TMVA::Reader *reader_;

 public:
  Pi0MVA(std::string const& name);
  virtual ~Pi0MVA();
  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

  unsigned gen_match_1_;

  unsigned event_;
  double wt_cp_sm_, wt_cp_ps_, wt_cp_mm_;
  int n_pu_;

  float pi0_2gammas_E_, pi0_3gammas_E_, pi0_4gammas_E_;
  float pi0_2gammas_eta_, pi0_3gammas_eta_, pi0_4gammas_eta_;
  float pi0_2gammas_phi_, pi0_3gammas_phi_, pi0_4gammas_phi_;

  float pi0_2gammas_M_, pi0_3gammas_M_, pi0_4gammas_M_, pi0_1gammas_M_;
  float Egammas_4ormore_;

  double tauFlag1_;
  double tauFlag2_;
  double gen_match_2_;
  double tau_decay_mode_1_;
  double tau_decay_mode_2_;
  unsigned Ngammas_1_, NgammaspT1_1_, NelecspT1_1_;
  float n_vtx_, rho_;
  float Egentau_1_, Phigentau_1_, Etagentau_1_, Mgentau_1_, Pxgentau_1_, Pygentau_1_, Pzgentau_1_;
  float Egenpi0_1_, Phigenpi0_1_, Etagenpi0_1_, Mgenpi0_1_, Pxgenpi0_1_, Pygenpi0_1_, Pzgenpi0_1_;
  float Epi_1_, Phipi_1_, Etapi_1_, Mpi_1_, Pxpi_1_, Pypi_1_, Pzpi_1_;
  float Epi0_1_, Phipi0_1_, Etapi0_1_, Mpi0_1_, Pxpi0_1_, Pypi0_1_, Pzpi0_1_;
  float Etau_1_, Phitau_1_, Etatau_1_, Mtau_1_, Pxtau_1_, Pytau_1_, Pztau_1_;
  float Egammas_1_, Phigammas_1_, Etagammas_1_, Mgammas_1_, Pxgammas_1_, Pygammas_1_, Pzgammas_1_;
  float Egammas_2_, Phigammas_2_, Etagammas_2_, Mgammas_2_, Pxgammas_2_, Pygammas_2_, Pzgammas_2_;
  float Egammas_3_, Phigammas_3_, Etagammas_3_, Mgammas_3_, Pxgammas_3_, Pygammas_3_, Pzgammas_3_;
  float Egammas_4_, Phigammas_4_, Etagammas_4_, Mgammas_4_, Pxgammas_4_, Pygammas_4_, Pzgammas_4_;
  float Egammas_5_, Phigammas_5_, Etagammas_5_, Mgammas_5_, Pxgammas_5_, Pygammas_5_, Pzgammas_5_;
  float Egammas_6_, Phigammas_6_, Etagammas_6_, Mgammas_6_, Pxgammas_6_, Pygammas_6_, Pzgammas_6_;
  float Egammas_7_, Phigammas_7_, Etagammas_7_, Mgammas_7_, Pxgammas_7_, Pygammas_7_, Pzgammas_7_;

  float eta_, pt_;
  float new_pi0_E_1_ = 0., new_pi0_E_2_ = 0., new_pi0_E_3_ =0., new_pi0_E_4_=0.;
  float new_M_1_, new_M_2_, new_M_3_, new_M_4_;
  float new_eta_1_, new_eta_2_, new_eta_3_, new_eta_4_;
  float new_phi_1_, new_phi_2_, new_phi_3_, new_phi_4_;
  float mod_M_;
  unsigned Nseeds_;
  unsigned Ngams_;

  float Egenpi0_1_1_, Egenpi0_1_2_;
  float Egengamma_1_, Egengamma_2_;
  float max_gengamma_dR_;
  float gengamma_dR_;
  float gen_tau_pt_; 
  float a1_rho_mass_1_, a1_rho_mass_2_;
  float pi0_pt_;

  float new_pi0_pt_1_, new_pi0_pt_2_, new_pi0_pt_3_, new_pi0_pt_4_;

  float new_pi0_phi_1_, new_pi0_phi_2_, new_pi0_phi_3_, new_pi0_phi_4_;
  float new_pi0_eta_1_, new_pi0_eta_2_, new_pi0_eta_3_, new_pi0_eta_4_;
  float genpi0_dR_;
  float dR_match_gamma1_, dR_match_gamma2_, dR_match_pi01_, dR_match_pi02_, dR_match_gamma1_wrong_, dR_match_gamma2_wrong_, dR_match_pi01_wrong_, dR_match_pi02_wrong_;
  unsigned Nstrips_, Nstrips_inciso_;
  float lead_strip_pt_, sublead_strip_pt_,  lead_strip_dR_, closest_strip_pt_, closest_strip_dR_, lead_strip_pt_inciso_, lead_strip_dR_inciso_, closest_strip_pt_inciso_, closest_strip_dR_inciso_;
  unsigned Nstrips_dm0_; 
  float new_strips_lead_gamma_pt_, new_strips_pt_, new_tau_pt_;
  float pt_genpi0_1_, pt_genpi0_2_, pt_gengammas_1_, pt_gengammas_2_;

  float lead_cluster_pt_, sublead_cluster_pt_;
  float lead_cluster_E_, sublead_cluster_E_;

  float lead_cluster_pt_new_, sublead_cluster_pt_new_;
  float lead_cluster_E_new_, sublead_cluster_E_new_;

  float genpi0_deta_, genpi0_dphi_,gengamma_deta_, gengamma_dphi_;

  float new_pi0_phi_, new_pi0_eta_, new_pi0_pt_, new_pi0_E_;
};

}

#endif
