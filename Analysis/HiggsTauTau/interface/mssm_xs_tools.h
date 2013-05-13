#ifndef mssm_xs_tools_h
#define mssm_xs_tools_h
#include "TH2F.h"
#include "TFile.h"
#include "TObjString.h"
#include <iostream>
#include <cmath>

class mssm_xs_tools{
 public:
   mssm_xs_tools();
  ~mssm_xs_tools();
  // specify input file, could also be moved into constructor?
  void SetInput(char const* filename);
  // help method
  void help();
  // access BRs into bbbar
  Double_t Give_BR_A_bb(Double_t mA, Double_t tanb);
  Double_t Give_BR_h_bb(Double_t mA, Double_t tanb);
  Double_t Give_BR_H_bb(Double_t mA, Double_t tanb);
  // access BRs into tautau
  Double_t Give_BR_A_tautau(Double_t mA, Double_t tanb);
  Double_t Give_BR_h_tautau(Double_t mA, Double_t tanb);
  Double_t Give_BR_H_tautau(Double_t mA, Double_t tanb);
  // access BRs into mumu
  Double_t Give_BR_A_mumu(Double_t mA, Double_t tanb);
  Double_t Give_BR_h_mumu(Double_t mA, Double_t tanb);
  Double_t Give_BR_H_mumu(Double_t mA, Double_t tanb);

  // ggF stuff
  Double_t Give_Xsec_ggFA(Double_t mA, Double_t tanb);
  Double_t Give_Xsec_ggFH(Double_t mA, Double_t tanb);
  Double_t Give_Xsec_ggFh(Double_t mA, Double_t tanb);
  
  Double_t Give_XsecUnc_muup_ggFA(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_muup_ggFH(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_muup_ggFh(Double_t mA, Double_t tanb);
  
  Double_t Give_XsecUnc_mudown_ggFA(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_mudown_ggFH(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_mudown_ggFh(Double_t mA, Double_t tanb);
  
  
  Double_t Give_XsecUnc_pdf68up_ggFA(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdf68up_ggFH(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdf68up_ggFh(Double_t mA, Double_t tanb);
  
  Double_t Give_XsecUnc_pdf68down_ggFA(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdf68down_ggFH(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdf68down_ggFh(Double_t mA, Double_t tanb);
  
  
  Double_t Give_XsecUnc_pdfalphas68up_ggFA(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdfalphas68up_ggFH(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdfalphas68up_ggFh(Double_t mA, Double_t tanb);
  
  Double_t Give_XsecUnc_pdfalphas68down_ggFA(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdfalphas68down_ggFH(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdfalphas68down_ggFh(Double_t mA, Double_t tanb);
  
  // ggF+bbH5f stuff
  //combined
  Double_t Give_Xsec_ggFplusbbA5f(Double_t mA, Double_t tanb);
  Double_t Give_Xsec_ggFplusbbH5f(Double_t mA, Double_t tanb);
  Double_t Give_Xsec_ggFplusbbh5f(Double_t mA, Double_t tanb);
  
  Double_t Give_XsecUnc_muup_ggFplusbbA5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_muup_ggFplusbbH5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_muup_ggFplusbbh5f(Double_t mA, Double_t tanb);
  
  Double_t Give_XsecUnc_mudown_ggFplusbbA5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_mudown_ggFplusbbH5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_mudown_ggFplusbbh5f(Double_t mA, Double_t tanb);
  
  
  Double_t Give_XsecUnc_pdf68up_ggFplusbbA5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdf68up_ggFplusbbH5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdf68up_ggFplusbbh5f(Double_t mA, Double_t tanb);
  
  Double_t Give_XsecUnc_pdf68down_ggFplusbbA5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdf68down_ggFplusbbH5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdf68down_ggFplusbbh5f(Double_t mA, Double_t tanb);
  
  
  Double_t Give_XsecUnc_pdfalphas68up_ggFplusbbA5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdfalphas68up_ggFplusbbH5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdfalphas68up_ggFplusbbh5f(Double_t mA, Double_t tanb);
  
  Double_t Give_XsecUnc_pdfalphas68down_ggFplusbbA5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdfalphas68down_ggFplusbbH5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdfalphas68down_ggFplusbbh5f(Double_t mA, Double_t tanb);
  



  
  // access xsec bb->phi (5-flavor scheme) in fb
  Double_t Give_Xsec_bbA5f(Double_t mA, Double_t tanb);
  Double_t Give_Xsec_bbh5f(Double_t mA, Double_t tanb);
  Double_t Give_Xsec_bbH5f(Double_t mA, Double_t tanb);
  // TNV

  // access xsec bb->phi (5-flavor scheme)
  // total scale uncertainty (quadratic addition) upwards direction in fb
  Double_t Give_XsecUnc_muup_bbA5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_muup_bbh5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_muup_bbH5f(Double_t mA, Double_t tanb);
  // access xsec bb->phi (5-flavor scheme)
  // total scale uncertainty (quadratic addition) downwards direction in fb
  Double_t Give_XsecUnc_mudown_bbA5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_mudown_bbh5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_mudown_bbH5f(Double_t mA, Double_t tanb);
  
  // access xsec bb->phi (5-flavor scheme)
  // PDF  (MSTW2008, 68%) upwards direction in fb
  Double_t Give_XsecUnc_pdf68up_bbA5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdf68up_bbh5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdf68up_bbH5f(Double_t mA, Double_t tanb);
   // access xsec bb->phi (5-flavor scheme)
  // PDF  (MSTW2008, 68%) upwards direction in fb
  Double_t Give_XsecUnc_pdf90up_bbA5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdf90up_bbh5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdf90up_bbH5f(Double_t mA, Double_t tanb);
  
  // access xsec bb->phi (5-flavor scheme)
  // PDF+alphas  (MSTW2008, 68%) upwards direction in fb
  Double_t Give_XsecUnc_pdfalphas68up_bbA5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdfalphas68up_bbh5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdfalphas68up_bbH5f(Double_t mA, Double_t tanb);
   // access xsec bb->phi (5-flavor scheme)
  // PDF+alphas  (MSTW2008, 68%) upwards direction in fb
  Double_t Give_XsecUnc_pdfalphas90up_bbA5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdfalphas90up_bbh5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdfalphas90up_bbH5f(Double_t mA, Double_t tanb);
  
  // access xsec bb->phi (5-flavor scheme)
  // PDF  (MSTW2008, 68%) downwards direction in fb
  Double_t Give_XsecUnc_pdf68down_bbA5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdf68down_bbh5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdf68down_bbH5f(Double_t mA, Double_t tanb);
   // access xsec bb->phi (5-flavor scheme)
  // PDF  (MSTW2008, 68%) downwards direction in fb
  Double_t Give_XsecUnc_pdf90down_bbA5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdf90down_bbh5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdf90down_bbH5f(Double_t mA, Double_t tanb);
  
  // access xsec bb->phi (5-flavor scheme)
  // PDF+alphas  (MSTW2008, 68%) downwards direction in fb
  Double_t Give_XsecUnc_pdfalphas68down_bbA5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdfalphas68down_bbh5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdfalphas68down_bbH5f(Double_t mA, Double_t tanb);
  // access xsec bb->phi (5-flavor scheme)
  // PDF+alphas  (MSTW2008, 68%) downwards direction in fb
  Double_t Give_XsecUnc_pdfalphas90down_bbA5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdfalphas90down_bbh5f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_pdfalphas90down_bbH5f(Double_t mA, Double_t tanb);
  
  // access xsec gg->bbphi (4-flavor scheme)
  // numbers are in fb
  Double_t Give_Xsec_bbA4f(Double_t mA, Double_t tanb);
  Double_t Give_Xsec_bbh4f(Double_t mA, Double_t tanb);
  Double_t Give_Xsec_bbH4f(Double_t mA, Double_t tanb);
  // access scale uncertainty upwards direction in fb 
  Double_t Give_XsecUnc_muup_bbA4f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_muup_bbh4f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_muup_bbH4f(Double_t mA, Double_t tanb);
  // access scale uncertainty downwards direction in fb 
  Double_t Give_XsecUnc_mudown_bbA4f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_mudown_bbh4f(Double_t mA, Double_t tanb);
  Double_t Give_XsecUnc_mudown_bbH4f(Double_t mA, Double_t tanb);
  // PDF uncertainty or 4f calculation is not available and expected to be negligible compared to scale uncertainty


  // access masses
  Double_t Give_Mass_h(Double_t mA, Double_t tanb);
  Double_t Give_Mass_H(Double_t mA, Double_t tanb);

  // Santander
  Double_t GiveXsec_Santander_h(Double_t mA, Double_t tanb);
  Double_t GiveXsec_Santander_H(Double_t mA, Double_t tanb);
  Double_t GiveXsec_Santander_A(Double_t mA, Double_t tanb);
  
  Double_t GiveXsec_UncUp_Santander_H(Double_t mA, Double_t tanb, TString PDFCL);
  Double_t GiveXsec_UncUp_Santander_h(Double_t mA, Double_t tanb, TString PDFCL);
  Double_t GiveXsec_UncUp_Santander_A(Double_t mA, Double_t tanb, TString PDFCL);
Double_t GiveXsec_UncDown_Santander_H(Double_t mA, Double_t tanb, TString PDFCL);
  Double_t GiveXsec_UncDown_Santander_h(Double_t mA, Double_t tanb, TString PDFCL);
  Double_t GiveXsec_UncDown_Santander_A(Double_t mA, Double_t tanb, TString PDFCL);


 private:
  /////////////////////////////////////////////////////////////////////
  // New out.root file contents (Yellow Report)
  TFile* m_input;
  
  TH2F* m_h_brbb_h;	 //BR(h->bb)
  TH2F*	m_h_brbb_H;	 //BR(H->bb)
  TH2F*	m_h_brbb_A;	 //BR(A->bb)
  TH2F*	m_h_brtautau_h;  //BR(h->tautau)
  TH2F*	m_h_brtautau_H;	 //BR(H->tautau)								
  TH2F*	m_h_brtautau_A;	 //BR(A->tautau)
  TH2F* m_h_brmumu_h;    //BR(h->mumu)
  TH2F*	m_h_brmumu_H;	 //BR(H->mumu)	
  TH2F* m_h_brmumu_A;	 //BR(A->mumu)
  
  // NB: ggF cross-sections are in fb
  TH2F*	m_h_ggF_xsec_h;	   //   gg->h xsection (mA,tan(beta))
  TH2F*	m_h_ggF_xsec_H;	   //   gg->H xsection (mA,tan(beta))
  TH2F*	m_h_ggF_xsec_A;	   //   gg->A xsection (mA,tan(beta))
  TH2F*	m_h_ggF_xsec05_h;  //	gg->h xsection (scale 0.5)(mA,tan(beta))
  TH2F*	m_h_ggF_xsec05_H;  //	gg->H xsection (scale 0.5)(mA,tan(beta))
  TH2F*	m_h_ggF_xsec05_A;  //	gg->A xsection (scale 0.5)(mA,tan(beta))
  TH2F*	m_h_ggF_xsec20_h;  //	gg->h xsection (scale 2.0)(mA,tan(beta))
  TH2F*	m_h_ggF_xsec20_H;  //	gg->H xsection (scale 2.0)(mA,tan(beta))
  TH2F*	m_h_ggF_xsec20_A;  //	gg->A xsection (scale 2.0)(mA,tan(beta))

  TH2F*	m_h_ggF_pdfup_h;  //	gg->h xsection PDF uncertainty+ (mA,tan(beta))
  TH2F*	m_h_ggF_pdfup_H;  //	gg->H xsection PDF uncertainty+ (mA,tan(beta))
  TH2F*	m_h_ggF_pdfup_A;  //	gg->A xsection PDF uncertainty+ (mA,tan(beta))
  TH2F*	m_h_ggF_pdfdown_h;  //	gg->h xsection PDF uncertainty- (mA,tan(beta))
  TH2F*	m_h_ggF_pdfdown_H;  //	gg->H xsection PDF uncertainty- (mA,tan(beta))
  TH2F*	m_h_ggF_pdfdown_A;  //	gg->A xsection PDF uncertainty- (mA,tan(beta))

  // NB: ggF_pdfalphas are all only pdf uncertainties; still need to add alphas unc. in quadrature
  TH2F*	m_h_ggF_alphasup_h;  //	gg->h xsection PDFALPHAS uncertainty+ (mA,tan(beta))
  TH2F*	m_h_ggF_alphasup_H;  //	gg->H xsection PDFALPHAS uncertainty+ (mA,tan(beta))
  TH2F*	m_h_ggF_alphasup_A;  //	gg->A xsection PDFALPHAS uncertainty+ (mA,tan(beta))
  TH2F*	m_h_ggF_alphasdown_h;  //	gg->h xsection PDFALPHAS uncertainty- (mA,tan(beta))
  TH2F*	m_h_ggF_alphasdown_H;  //	gg->H xsection PDFALPHAS uncertainty- (mA,tan(beta))
  TH2F*	m_h_ggF_alphasdown_A;  //	gg->A xsection PDFALPHAS uncertainty- (mA,tan(beta))

  //ggFplusbbH5f combined
 // NB: ggF cross-sections are in fb
  TH2F*	m_h_ggFplusbbH_xsec_h;	   //   gg->h xsection (mA,tan(beta))
  TH2F*	m_h_ggFplusbbH_xsec_H;	   //   gg->H xsection (mA,tan(beta))
  TH2F*	m_h_ggFplusbbH_xsec_A;	   //   gg->A xsection (mA,tan(beta))
  TH2F*	m_h_ggFplusbbH_xsec05_h;  //	gg->h xsection (scale 0.5)(mA,tan(beta))
  TH2F*	m_h_ggFplusbbH_xsec05_H;  //	gg->H xsection (scale 0.5)(mA,tan(beta))
  TH2F*	m_h_ggFplusbbH_xsec05_A;  //	gg->A xsection (scale 0.5)(mA,tan(beta))
  TH2F*	m_h_ggFplusbbH_xsec20_h;  //	gg->h xsection (scale 2.0)(mA,tan(beta))
  TH2F*	m_h_ggFplusbbH_xsec20_H;  //	gg->H xsection (scale 2.0)(mA,tan(beta))
  TH2F*	m_h_ggFplusbbH_xsec20_A;  //	gg->A xsection (scale 2.0)(mA,tan(beta))

  TH2F*	m_h_ggFplusbbH_pdfup_h;  //	gg->h xsection PDF uncertainty+ (mA,tan(beta))
  TH2F*	m_h_ggFplusbbH_pdfup_H;  //	gg->H xsection PDF uncertainty+ (mA,tan(beta))
  TH2F*	m_h_ggFplusbbH_pdfup_A;  //	gg->A xsection PDF uncertainty+ (mA,tan(beta))
  TH2F*	m_h_ggFplusbbH_pdfdown_h;  //	gg->h xsection PDF uncertainty- (mA,tan(beta))
  TH2F*	m_h_ggFplusbbH_pdfdown_H;  //	gg->H xsection PDF uncertainty- (mA,tan(beta))
  TH2F*	m_h_ggFplusbbH_pdfdown_A;  //	gg->A xsection PDF uncertainty- (mA,tan(beta))

  // NB: ggFplusbbH_pdfalphas are all only pdf uncertainties; still need to add alphas unc. in quadrature
  TH2F*	m_h_ggFplusbbH_alphasup_h;  //	gg->h xsection PDFALPHAS uncertainty+ (mA,tan(beta))
  TH2F*	m_h_ggFplusbbH_alphasup_H;  //	gg->H xsection PDFALPHAS uncertainty+ (mA,tan(beta))
  TH2F*	m_h_ggFplusbbH_alphasup_A;  //	gg->A xsection PDFALPHAS uncertainty+ (mA,tan(beta))
  TH2F*	m_h_ggFplusbbH_alphasdown_h;  //	gg->h xsection PDFALPHAS uncertainty- (mA,tan(beta))
  TH2F*	m_h_ggFplusbbH_alphasdown_H;  //	gg->H xsection PDFALPHAS uncertainty- (mA,tan(beta))
  TH2F*	m_h_ggFplusbbH_alphasdown_A;  //	gg->A xsection PDFALPHAS uncertainty- (mA,tan(beta))




  TH2F*	m_h_bbH4f_xsec_h;  //	bbh xsection (mA,tan(beta))
  TH2F*	m_h_bbH4f_xsec_H;  //	bbH xsection (mA,tan(beta))
  TH2F*	m_h_bbH4f_xsec_A;  //	bbA xsection (mA,tan(beta))
  TH2F*	m_h_bbH4f_xsec_h_low;  //	bbh xsection (mA,tan(beta))
  TH2F*	m_h_bbH4f_xsec_H_low;  //	bbH xsection (mA,tan(beta))
  TH2F*	m_h_bbH4f_xsec_A_low;  //	bbA xsection (mA,tan(beta))
  TH2F*	m_h_bbH4f_xsec_h_high;  //	bbh xsection (mA,tan(beta))
  TH2F*	m_h_bbH4f_xsec_H_high;  //	bbH xsection (mA,tan(beta))
  TH2F*	m_h_bbH4f_xsec_A_high;  //	bbA xsection (mA,tan(beta))

  TH2F*	m_h_bbH_xsec_h;  //	bb->h xsection (mA,tan(beta))
  TH2F*	m_h_bbH_xsec_H;  //	bb->H xsection (mA,tan(beta))
  TH2F*	m_h_bbH_xsec_A;  //	bb->A xsection (mA,tan(beta))

  TH2F*	m_h_bbH_mufdown_h;  //	bb->h xsection muf unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_mufdown_H;  //	bb->H xsection muf unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_mufdown_A;  //	bb->A xsection muf unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_mufup_h;  //	bb->h xsection muf unc.+ (mA,tan(beta))
  TH2F*	m_h_bbH_mufup_H;  //	bb->H xsection muf unc.+ (mA,tan(beta))
  TH2F*	m_h_bbH_mufup_A;  //	bb->A xsection muf unc.+ (mA,tan(beta))

  TH2F*	m_h_bbH_murdown_h;  //	bb->h xsection mur unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_murdown_H;  //	bb->H xsection mur unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_murdown_A;  //	bb->A xsection mur unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_murup_h;  //	bb->h xsection mur unc.+ (mA,tan(beta))
  TH2F*	m_h_bbH_murup_H;  //	bb->H xsection mur unc.+ (mA,tan(beta))
  TH2F*	m_h_bbH_murup_A;  //	bb->A xsection mur unc.+ (mA,tan(beta))

  TH2F*	m_h_bbH_mudown_h;  //	bb->h xsection mu unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_mudown_H;  //	bb->H xsection mu unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_mudown_A;  //	bb->A xsection mu unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_muup_h;  //	bb->h xsection mu unc.+ (mA,tan(beta))
  TH2F*	m_h_bbH_muup_H;  //	bb->H xsection mu unc.+ (mA,tan(beta))
  TH2F*	m_h_bbH_muup_A;  //	bb->A xsection mu unc.+ (mA,tan(beta))

  TH2F*	m_h_bbH_pdf68down_h;  //	bb->h xsection pdf68 unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_pdf68down_H;  //	bb->H xsection pdf68 unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_pdf68down_A;  //	bb->A xsection pdf68 unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_pdf68up_h;  //	bb->h xsection pdf68 unc.+ (mA,tan(beta))
  TH2F*	m_h_bbH_pdf68up_H;  //	bb->H xsection pdf68 unc.+ (mA,tan(beta))
  TH2F*	m_h_bbH_pdf68up_A;  //	bb->A xsection pdf68 unc.+ (mA,tan(beta))

  TH2F*	m_h_bbH_pdf90down_h;  //	bb->h xsection pdf90 unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_pdf90down_H;  //	bb->H xsection pdf90 unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_pdf90down_A;  //	bb->A xsection pdf90 unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_pdf90up_h;  //	bb->h xsection pdf90 unc.+ (mA,tan(beta))
  TH2F*	m_h_bbH_pdf90up_H;  //	bb->H xsection pdf90 unc.+ (mA,tan(beta))
  TH2F*	m_h_bbH_pdf90up_A;  //	bb->A xsection pdf90 unc.+ (mA,tan(beta))

  TH2F*	m_h_bbH_pdfalphas68down_h;  //	bb->h xsection pdfalphas68 unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_pdfalphas68down_H;  //	bb->H xsection pdfalphas68 unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_pdfalphas68down_A;  //	bb->A xsection pdfalphas68 unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_pdfalphas68up_h;  //	bb->h xsection pdfalphas68 unc.+ (mA,tan(beta))
  TH2F*	m_h_bbH_pdfalphas68up_H;  //	bb->H xsection pdfalphas68 unc.+ (mA,tan(beta))
  TH2F*	m_h_bbH_pdfalphas68up_A;  //	bb->A xsection pdfalphas68 unc.+ (mA,tan(beta))

  TH2F*	m_h_bbH_pdfalphas90down_h;  //	bb->h xsection pdfalphas90 unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_pdfalphas90down_H;  //	bb->H xsection pdfalphas90 unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_pdfalphas90down_A;  //	bb->A xsection pdfalphas90 unc.- (mA,tan(beta))
  TH2F*	m_h_bbH_pdfalphas90up_h;  //	bb->h xsection pdfalphas90 unc.+ (mA,tan(beta))
  TH2F*	m_h_bbH_pdfalphas90up_H;  //	bb->H xsection pdfalphas90 unc.+ (mA,tan(beta))
  TH2F*	m_h_bbH_pdfalphas90up_A;  //	bb->A xsection pdfalphas90 unc.+ (mA,tan(beta))

  TH2F*	m_h_mh;  //	mh (mA,tan(beta))
  TH2F*	m_h_mH;  //	mH (mA,tan(beta))

};

#endif // MSSM_XS_TOOLS_H
