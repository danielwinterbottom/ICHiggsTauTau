#ifndef ICHiggsTauTau_HiggsTauTau_RhoIDEmbedder_h
#define ICHiggsTauTau_HiggsTauTau_RhoIDEmbedder_h

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
  
class RhoIDEmbedder : public ModuleBase {
 private:
  CLASS_MEMBER(RhoIDEmbedder, fwlite::TFileService*, fs)
  CLASS_MEMBER(RhoIDEmbedder, ic::channel, channel)
  CLASS_MEMBER(RhoIDEmbedder, bool, maketrees)
  
  TTree *outtree_;
  double wt_;
  TMVA::Reader *reader_;

 public:
  RhoIDEmbedder(std::string const& name);
  virtual ~RhoIDEmbedder();
  virtual double read_mva_score(std::vector<double> vars);
  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  

  float var0, var1, var2, var3, var4, var5, var6, var7, var8, var9, var10, var11, var12, var13;

  double tauFlag1_;
  double tauFlag2_;
  double gen_match_1_;
  double gen_match_2_;
  double tau_decay_mode_1_;
  double tau_decay_mode_2_;
  double E_1_;
  double E_2_;
  double rho_dEta_1_;
  double rho_dphi_1_;
  double rho_dEta_2_;
  double rho_dphi_2_;

  // variables for training trees
  unsigned Ngammas_1_;

  double Egamma1_1_;
  double Egamma2_1_;
  double Egamma3_1_;
  double Egamma4_1_;
  double Egamma5_1_;
  double Egamma6_1_;
  double Egamma7_1_;
  double Egamma8_1_;
  double Egamma9_1_;
  double Egamma10_1_;
  double Egamma11_1_;
  double Egamma12_1_;
  
  double Epi_1_;
  double Epi0_1_;
  double Mpi0_1_;   
  double Mrho_1_;   
  double dphi_1_;   
  double dEta_1_;   
  double gammas_dEta_1_;
  double gammas_dphi_1_;
  double pt_1_;
  double eta_1_; 
  double phi_1_;

  unsigned Ngammas_2_;

  double Egamma1_2_;
  double Egamma2_2_;
  double Egamma3_2_;
  double Egamma4_2_;
  double Egamma5_2_;
  double Egamma6_2_;
  double Egamma7_2_;
  double Egamma8_2_;
  double Egamma9_2_;
  double Egamma10_2_;
  double Egamma11_2_;
  double Egamma12_2_;
  
  double Epi_2_;
  double Epi0_2_;
  double Mpi0_2_;
  double Mrho_2_;
  double dphi_2_;
  double dEta_2_;
  double gammas_dEta_2_;
  double gammas_dphi_2_;
  double pt_2_;
  double eta_2_;
  double phi_2_;

  // variables by Mohammad
  double ConeRadiusMax_2_;
  double ConeRadiusMedian_2_;
  double ConeRadiusMean_2_;
  double ConeRadiusStdDev_2_;
  double ConeRadiusMax_1_;
  double ConeRadiusMedian_1_;
  double ConeRadiusMean_1_;
  double ConeRadiusStdDev_1_;

  double ConeRadiusMaxWRTtau_2_;
  double ConeRadiusMedianWRTtau_2_;
  double ConeRadiusMeanWRTtau_2_;
  double ConeRadiusStdDevWRTtau_2_;
  double ConeRadiusMaxWRTtau_1_;
  double ConeRadiusMedianWRTtau_1_;
  double ConeRadiusMeanWRTtau_1_;
  double ConeRadiusStdDevWRTtau_1_;

  double ConeRadiusMaxWRTpi0_2_;
  double ConeRadiusMedianWRTpi0_2_;
  double ConeRadiusMeanWRTpi0_2_;
  double ConeRadiusStdDevWRTpi0_2_;
  double ConeRadiusMaxWRTpi0_1_;
  double ConeRadiusMedianWRTpi0_1_;
  double ConeRadiusMeanWRTpi0_1_;
  double ConeRadiusStdDevWRTpi0_1_;

  double NgammasModif_2_;
  double NgammasModif_1_;
    
  double Etagamma1_1_;
  double Etagamma2_1_;
  double Etagamma3_1_;
  double Etagamma4_1_;
  double Etagamma5_1_;
  double Etagamma6_1_;
  double Etagamma7_1_;
  double Etagamma8_1_;
  double Etagamma9_1_;
  double Etagamma10_1_;
  double Etagamma11_1_;
  double Etagamma12_1_;

  double Etagamma1_2_;
  double Etagamma2_2_;
  double Etagamma3_2_;
  double Etagamma4_2_;
  double Etagamma5_2_;
  double Etagamma6_2_;
  double Etagamma7_2_;
  double Etagamma8_2_;
  double Etagamma9_2_;
  double Etagamma10_2_;
  double Etagamma11_2_;
  double Etagamma12_2_;
  
  double Etapi_1_;
  double Etapi_2_;
  
  double Phigamma1_1_;
  double Phigamma2_1_;
  double Phigamma3_1_;
  double Phigamma4_1_;
  double Phigamma5_1_;
  double Phigamma6_1_;
  double Phigamma7_1_;
  double Phigamma8_1_;
  double Phigamma9_1_;
  double Phigamma10_1_;
  double Phigamma11_1_;
  double Phigamma12_1_;
  
  double Phigamma1_2_;
  double Phigamma2_2_;
  double Phigamma3_2_;
  double Phigamma4_2_;
  double Phigamma5_2_;
  double Phigamma6_2_;
  double Phigamma7_2_;
  double Phigamma8_2_;
  double Phigamma9_2_;
  double Phigamma10_2_;
  double Phigamma11_2_;
  double Phigamma12_2_;
  
  double Phipi_1_;
  double Phipi_2_;
  
  double DeltaR2WRTtau_1_;
  double DeltaR2WRTpi0_1_;
  double DeltaR2WRTtau_2_;
  double DeltaR2WRTpi0_2_;  

  double FracPtDepos_dRLessThan0p008_2_;
  double FracPtDepos_dR0p008To0p04_2_; 
  double FracPtDepos_dRMoreThan0p04_2_;
  double FracPtDepos_dRLessThan0p008_1_;
  double FracPtDepos_dR0p008To0p04_1_;
  double FracPtDepos_dRMoreThan0p04_1_;


  //temp variables
  double CenterEta_1;
  double CenterPhi_1;
  double CenterEta_2;
  double CenterPhi_2;
  double SumPt_1;
  double SumPt_2;


};

}

#endif
