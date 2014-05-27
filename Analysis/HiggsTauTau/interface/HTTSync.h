#ifndef ICHiggsTauTau_Module_HTTSync_h
#define ICHiggsTauTau_Module_HTTSync_h

#include <string>
#include "TTree.h"
#include "TFile.h"
#include "TMVA/Reader.h"
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "Utilities/interface/SVFitService.h"
#include "HiggsTauTau/interface/HTTConfig.h"

namespace ic {

class HTTSync : public ModuleBase {
 private:
  std::string output_name_;

  SVFitService sv_service_;
  CLASS_MEMBER(HTTSync, bool, is_embedded)
  CLASS_MEMBER(HTTSync, std::string, select_category)
  CLASS_MEMBER(HTTSync, std::string, met_label)
  CLASS_MEMBER(HTTSync, int, select_sel_mode)
  CLASS_MEMBER(HTTSync, double, jet_eta)

  TFile *lOFile;
  TTree *lOTree;

  // Sync Tree Variables
  // Definitions in HTTSync.cc
  int   lRun;
  int   lLumi;
  int   lEvt;
  int   lNPV;
  int   lNPU;
  float lRho;
  float lMCWeight;
  float lPUWeight;
  float lFakeWeight;
  float lTrigweight_1;
  float lTrigweight_2;
  float lIdweight_1;
  float lIdweight_2;
  float lIsoweight_1;
  float lIsoweight_2;
  float lEffWeight;
  float lWeight;
  float lEmbeddedWeight;
  float lSignalWeight;
  float lMSV;
  float lPtSV;
  float lEtaSV;
  float lPhiSV;
  float lMVis;
  float lMSVUp;
  float lMSVDown;
  float lPt1;
  float lPhi1;
  float lEta1;
  float lM1;
  int   lq1;
  float lIso1;
  float lMVA1;
  float lD01;
  float lDZ1;
  bool  lPassId1;
  bool  lPassIso1;
  float lMt1;
  float lPtTT;
  float lPt2;
  float lPhi2;
  float lEta2;
  float lM2;
  int   lq2;
  float lIso2;
  float lD02;
  float lDZ2;
  float l3Hits_2;
  float lagainstElectronMVA3raw_2;
  float lbyIsolationMVA2raw_2;
  float lagainstMuonLoose2_2;
  float lagainstMuonMedium2_2;
  float lagainstMuonTight2_2;
  float lMVA2;
  bool  lPassId2;
  bool  lPassIso2;
  float lMt2;
  float lMet;
  float lMetPhi;
  float lL1Met;
  float lL1MetPhi;
  float lL1MetCorr;
  float lCaloMet;
  float lCaloMetPhi;
  float lCaloMetCorr;
  float lCaloMetPhiCorr;
  float lMVAMet;
  float lMVAMetPhi;
  float lPZetaVis;
  float lPZetaMiss;
  float lMetCov00;
  float lMetCov01;
  float lMetCov10;
  float lMetCov11;
  float lMVACov00;
  float lMVACov01;
  float lMVACov10;
  float lMVACov11;
  float lJPt1;
  float lJEta1;
  float lJPhi1;
  float lJPtRaw1;
  float lJPtUnc1;
  float lJMVA1;
  float lLRM1;
  int lCTM1;
  bool  lJPass1;
  float lJPt2;
  float lJEta2;
  float lJPhi2;
  float lJPtRaw2;
  float lJPtUnc2;
  float lJMVA2;
  float lLRM2;
  int lCTM2;
  bool  lJPass2;
  float lBTagPt;
  float lBTagEta;
  float lBTagPhi;
  float lMJJ;
  float lJDEta;
  int   lNJetInGap;
  float lMVA;
  float lJDPhi;
  float lDiJetPt;
  float lDiJetPhi;
  float lHDJetPhi;
  float lVisJetEta;
  float lPtVis;
  int   lNBTag;
  int   lNJets;
  int   lNJetsPt20;
  float em_gf_mva_;
  float em_vbf_mva_;

  ic::channel channel_;

 public:
  HTTSync(std::string const &name, std::string const &output_name,
          ic::channel channel);
  virtual ~HTTSync();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};
}

#endif
