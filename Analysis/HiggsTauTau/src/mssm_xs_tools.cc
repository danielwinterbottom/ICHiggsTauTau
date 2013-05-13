// tool to access MSSM neutral Higgs quantities 
// Version 0.1 by  Trevor Vickey    (Trevor.Vickey@cern.ch)
//             and Markus Warsinsky (Markus.Warsinsky@cern.ch
// to do:
//        - implement a help method to list all accessor methods
//        - ... 
#define mssm_xs_tools_cxx
#include "TH2F.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/mssm_xs_tools.h"
#include <iostream>
mssm_xs_tools::mssm_xs_tools(){
   std::cout<<"Welcome to the MSSM neutral cross section tool"<<std::endl;
   std::cout<<" Version 0.4 \n by  Monica Vazquez Acosta (Monica.Vazquez.Acosta@cern.ch),\n     Trevor Vickey         (Trevor.Vickey@cern.ch)\n     Markus Warsinsky      (Markus.Warsinsky@cern.ch)\n"<<std::endl;
   std::cout<<"please chose your input file with the SetInput method"<<std::endl;
}

mssm_xs_tools::~mssm_xs_tools() {}

void mssm_xs_tools::help(){
  std::cout<<"this is the MSSM neutral Higgs xsec tool"<<std::endl;
  std::cout<<"it takes prefabricated 2D ROOT histograms to obtain xsections"<<std::endl;
  std::cout<<"and BRs of h/H/A in dependence of mA and tan(beta)"<<std::endl;
  std::cout<<"to use it, first open an input file with the SetInput(filename) method"<<std::endl<<std::endl;
  std::cout<<"there are accessor classes for different quantities:"<<std::endl;
  std::cout<<"-  Give_Mass_h(Double_t mA, Double_t tanb) and\n   Give_Mass_H(Double_t mA, Double_t tanb)"<<std::endl;
  std::cout<<"   give the masses of the scalar bosons"<<std::endl;
  std::cout<<"-  Give_BR_X_YYY(Double_t mA, Double_t tanb) give the BRs,\n   where X can be h,H or A, and YYY can be bb, tautau or mumu"<<std::endl;
  std::cout<<"-  Give_Xsec_bbX5f(Double_t mA, Double_t tanb) gives the xsection in fb "<<std::endl;
  std::cout<<"   for b-associated production in the 5 flavour scheme (X can be h, H or A)"<<std::endl;
  std::cout<<"-  Give_Xsec_bbX4f(Double_t mA, Double_t tanb) gives the xsection in fb "<<std::endl;
  std::cout<<"   for b-associated production in the 5 flavour scheme (X can be h, H or A)"<<std::endl;
  std::cout<<"-  Give_Xsec_ggFX(Double_t mA, Double_t tanb) gives the xsection in fb "<<std::endl;
  std::cout<<"   for gluon-gluon fusion(X can be h, H or A)"<<std::endl;
  std::cout<<"-  for all xsections there are also accessor methods for the"<<std::endl;
  std::cout<<"       - scale uncertainties in fb "<<std::endl;
  std::cout<<"          (Give_XsecUnc_muup_YYY, Give_Xsec_Unc_mudown_YYY "<<std::endl;
  std::cout<<"             with YYY either bbX5f, bbX4f or ggFX with X h, H or A)"<<std::endl;
  std::cout<<"       - PDF uncertainties in fb"<<std::endl;
  std::cout<<"          (Give_XsecUnc_pdf68up/pdf90up/pdf68down/pdf90down)"<<std::endl;
  std::cout<<"          - Currently not available for b-assoc. prod. in 4 flavour scheme"<<std::endl;
  std::cout<<"          - 90% CL only for b-assoc. prod. in 5 flavour scheme"<<std::endl;
  std::cout<<"       - PDF+alphas uncertainties in fb"<<std::endl;
  std::cout<<"          (Give_XsecUnc_pdfalphas68up/pdfalphas90up/\n           pdfalphas68down/pdfalphas90down)"<<std::endl;
  std::cout<<"          - Currently not available for b-assoc. prod. in 4 flavour scheme"<<std::endl;
  std::cout<<"          - 90% CL only for b-assoc. prod. in 5 flavour scheme"<<std::endl;
  std::cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<std::endl;
  std::cout<<"-  Give_Xsec_ggFplusbbX5f(Double_t mA, Double_t tanb) gives the xsection in fb "<<std::endl;
  std::cout<<"   for gluon-gluon fusion+ b-associated production in the 5 flavour scheme (X can be h, H or A)"<<std::endl;
  std::cout<<"       - scale uncertainties in fb "<<std::endl;
  std::cout<<"          Give_XsecUnc_muup/mudown_ggFplusbbX5f"<<std::endl;
  std::cout<<"       - PDF uncertainties in fb"<<std::endl;
  std::cout<<"          Give_XsecUnc_pdf68up/pdf68down_ggFplusbbX5f"<<std::endl;
  std::cout<<"       - PDF+alphas uncertainties in fb"<<std::endl;
  std::cout<<"          Give_XsecUnc_pdfalphas68up/pdfalphas68down_ggFplusbbX5f"<<std::endl;
  std::cout<<" please look into the header file for more detail"<<std::endl;

  return;
}


void mssm_xs_tools::SetInput(char const* filename){
  m_input=new TFile(filename);
  m_h_brbb_h = (TH2F*) m_input->Get("h_brbb_h");	 
  m_h_brbb_H = (TH2F*) m_input->Get("h_brbb_H");	 
  m_h_brbb_A = (TH2F*) m_input->Get("h_brbb_A");	 
  m_h_brtautau_h = (TH2F*) m_input->Get("h_brtautau_h");	 
  m_h_brtautau_H = (TH2F*) m_input->Get("h_brtautau_H");	 
  m_h_brtautau_A = (TH2F*) m_input->Get("h_brtautau_A");	 
  m_h_brmumu_h = (TH2F*) m_input->Get("h_brmumu_h");	 
  m_h_brmumu_H = (TH2F*) m_input->Get("h_brmumu_H");	 
  m_h_brmumu_A = (TH2F*) m_input->Get("h_brmumu_A");	 

  
  
  m_h_ggF_xsec_h = (TH2F*) m_input->Get("h_ggF_xsec_h");	 
  m_h_ggF_xsec_H = (TH2F*) m_input->Get("h_ggF_xsec_H");	 
  m_h_ggF_xsec_A = (TH2F*) m_input->Get("h_ggF_xsec_A");	 
  m_h_ggF_xsec05_h = (TH2F*) m_input->Get("h_ggF_xsec05_h");
  m_h_ggF_xsec05_H = (TH2F*) m_input->Get("h_ggF_xsec05_H");
  m_h_ggF_xsec05_A = (TH2F*) m_input->Get("h_ggF_xsec05_A");
  m_h_ggF_xsec20_h = (TH2F*) m_input->Get("h_ggF_xsec20_h");
  m_h_ggF_xsec20_H = (TH2F*) m_input->Get("h_ggF_xsec20_H");
  m_h_ggF_xsec20_A = (TH2F*) m_input->Get("h_ggF_xsec20_A");
  
  m_h_ggF_pdfup_h = (TH2F*) m_input->Get("h_ggF_pdfup_h"); 
  m_h_ggF_pdfup_H = (TH2F*) m_input->Get("h_ggF_pdfup_H"); 
  m_h_ggF_pdfup_A = (TH2F*) m_input->Get("h_ggF_pdfup_A"); 
  m_h_ggF_pdfdown_h = (TH2F*) m_input->Get("h_ggF_pdfdown_h");
  m_h_ggF_pdfdown_H = (TH2F*) m_input->Get("h_ggF_pdfdown_H");
  m_h_ggF_pdfdown_A = (TH2F*) m_input->Get("h_ggF_pdfdown_A");
  


  m_h_ggF_alphasup_h = (TH2F*) m_input->Get("h_ggF_alphasup_h");
  m_h_ggF_alphasup_H = (TH2F*) m_input->Get("h_ggF_alphasup_H");
  m_h_ggF_alphasup_A = (TH2F*) m_input->Get("h_ggF_alphasup_A");
  m_h_ggF_alphasdown_h = (TH2F*) m_input->Get("h_ggF_alphasdown_h");
  m_h_ggF_alphasdown_H = (TH2F*) m_input->Get("h_ggF_alphasdown_H");
  m_h_ggF_alphasdown_A = (TH2F*) m_input->Get("h_ggF_alphasdown_A");
  

  //COMBINED ggF + bbH
  //&&&&&&&&&&&&&&&&&&&

  m_h_ggFplusbbH_xsec_h = (TH2F*) m_input->Get("h_ggFplusbbH_xsec_h");	 
  m_h_ggFplusbbH_xsec_H = (TH2F*) m_input->Get("h_ggFplusbbH_xsec_H");	 
  m_h_ggFplusbbH_xsec_A = (TH2F*) m_input->Get("h_ggFplusbbH_xsec_A");	 
  m_h_ggFplusbbH_xsec05_h = (TH2F*) m_input->Get("h_ggFplusbbH_xsec05_h");
  m_h_ggFplusbbH_xsec05_H = (TH2F*) m_input->Get("h_ggFplusbbH_xsec05_H");
  m_h_ggFplusbbH_xsec05_A = (TH2F*) m_input->Get("h_ggFplusbbH_xsec05_A");
  m_h_ggFplusbbH_xsec20_h = (TH2F*) m_input->Get("h_ggFplusbbH_xsec20_h");
  m_h_ggFplusbbH_xsec20_H = (TH2F*) m_input->Get("h_ggFplusbbH_xsec20_H");
  m_h_ggFplusbbH_xsec20_A = (TH2F*) m_input->Get("h_ggFplusbbH_xsec20_A");
  
  m_h_ggFplusbbH_pdfup_h = (TH2F*) m_input->Get("h_ggFplusbbH_pdfup_h"); 
  m_h_ggFplusbbH_pdfup_H = (TH2F*) m_input->Get("h_ggFplusbbH_pdfup_H"); 
  m_h_ggFplusbbH_pdfup_A = (TH2F*) m_input->Get("h_ggFplusbbH_pdfup_A"); 
  m_h_ggFplusbbH_pdfdown_h = (TH2F*) m_input->Get("h_ggFplusbbH_pdfdown_h");
  m_h_ggFplusbbH_pdfdown_H = (TH2F*) m_input->Get("h_ggFplusbbH_pdfdown_H");
  m_h_ggFplusbbH_pdfdown_A = (TH2F*) m_input->Get("h_ggFplusbbH_pdfdown_A");

  m_h_ggFplusbbH_alphasup_h = (TH2F*) m_input->Get("h_ggFplusbbH_alphasup_h");
  m_h_ggFplusbbH_alphasup_H = (TH2F*) m_input->Get("h_ggFplusbbH_alphasup_H");
  m_h_ggFplusbbH_alphasup_A = (TH2F*) m_input->Get("h_ggFplusbbH_alphasup_A");
  m_h_ggFplusbbH_alphasdown_h = (TH2F*) m_input->Get("h_ggFplusbbH_alphasdown_h");
  m_h_ggFplusbbH_alphasdown_H = (TH2F*) m_input->Get("h_ggFplusbbH_alphasdown_H");
  m_h_ggFplusbbH_alphasdown_A = (TH2F*) m_input->Get("h_ggFplusbbH_alphasdown_A");

  //&&&&&&&&&&&&&&&&&&&
  //END COMBINED
  
  m_h_bbH4f_xsec_h = (TH2F*) m_input->Get("h_bbH4f_xsec_h"); 
  m_h_bbH4f_xsec_H = (TH2F*) m_input->Get("h_bbH4f_xsec_H"); 
  m_h_bbH4f_xsec_A = (TH2F*) m_input->Get("h_bbH4f_xsec_A"); 
  m_h_bbH4f_xsec_h_low = (TH2F*) m_input->Get("h_bbH4f_xsec_h_low"); 
  m_h_bbH4f_xsec_H_low = (TH2F*) m_input->Get("h_bbH4f_xsec_H_low"); 
  m_h_bbH4f_xsec_A_low = (TH2F*) m_input->Get("h_bbH4f_xsec_A_low"); 
  m_h_bbH4f_xsec_h_high = (TH2F*) m_input->Get("h_bbH4f_xsec_h_high");
  m_h_bbH4f_xsec_H_high = (TH2F*) m_input->Get("h_bbH4f_xsec_H_high"); 
  m_h_bbH4f_xsec_A_high = (TH2F*) m_input->Get("h_bbH4f_xsec_A_high"); 
  

  
  m_h_bbH_xsec_h = (TH2F*) m_input->Get("h_bbH_xsec_h"); 
  m_h_bbH_xsec_H = (TH2F*) m_input->Get("h_bbH_xsec_H"); 
  m_h_bbH_xsec_A = (TH2F*) m_input->Get("h_bbH_xsec_A"); 

  m_h_bbH_mufdown_h = (TH2F*) m_input->Get("h_bbH_mufdown_h"); 
  m_h_bbH_mufdown_H = (TH2F*) m_input->Get("h_bbH_mufdown_H"); 
  m_h_bbH_mufdown_A = (TH2F*) m_input->Get("h_bbH_mufdown_A"); 
  m_h_bbH_mufup_h = (TH2F*) m_input->Get("h_bbH_mufup_h"); 
  m_h_bbH_mufup_H = (TH2F*) m_input->Get("h_bbH_mufup_H"); 
  m_h_bbH_mufup_A = (TH2F*) m_input->Get("h_bbH_mufup_A"); 

  m_h_bbH_murdown_h = (TH2F*) m_input->Get("h_bbH_murdown_h");
  m_h_bbH_murdown_H = (TH2F*) m_input->Get("h_bbH_murdown_H");
  m_h_bbH_murdown_A = (TH2F*) m_input->Get("h_bbH_murdown_A");
  m_h_bbH_murup_h = (TH2F*) m_input->Get("h_bbH_murup_h");  
  m_h_bbH_murup_H = (TH2F*) m_input->Get("h_bbH_murup_H");  
  m_h_bbH_murup_A = (TH2F*) m_input->Get("h_bbH_murup_A");  
  
  m_h_bbH_mudown_h = (TH2F*) m_input->Get("h_bbH_mudown_h"); 
  m_h_bbH_mudown_H = (TH2F*) m_input->Get("h_bbH_mudown_H"); 
  m_h_bbH_mudown_A = (TH2F*) m_input->Get("h_bbH_mudown_A"); 
  m_h_bbH_muup_h = (TH2F*) m_input->Get("h_bbH_muup_h"); 
  m_h_bbH_muup_H = (TH2F*) m_input->Get("h_bbH_muup_H"); 
  m_h_bbH_muup_A = (TH2F*) m_input->Get("h_bbH_muup_A"); 

  m_h_bbH_pdf68down_h = (TH2F*) m_input->Get("h_bbH_pdf68down_h"); 
  m_h_bbH_pdf68down_H = (TH2F*) m_input->Get("h_bbH_pdf68down_H"); 
  m_h_bbH_pdf68down_A = (TH2F*) m_input->Get("h_bbH_pdf68down_A"); 
  m_h_bbH_pdf68up_h = (TH2F*) m_input->Get("h_bbH_pdf68up_h"); 
  m_h_bbH_pdf68up_H = (TH2F*) m_input->Get("h_bbH_pdf68up_H"); 
  m_h_bbH_pdf68up_A = (TH2F*) m_input->Get("h_bbH_pdf68up_A"); 

  m_h_bbH_pdf90down_h = (TH2F*) m_input->Get("h_bbH_pdf90down_h"); 
  m_h_bbH_pdf90down_H = (TH2F*) m_input->Get("h_bbH_pdf90down_H"); 
  m_h_bbH_pdf90down_A = (TH2F*) m_input->Get("h_bbH_pdf90down_A"); 
  m_h_bbH_pdf90up_h = (TH2F*) m_input->Get("h_bbH_pdf90up_h"); 
  m_h_bbH_pdf90up_H = (TH2F*) m_input->Get("h_bbH_pdf90up_H"); 
  m_h_bbH_pdf90up_A = (TH2F*) m_input->Get("h_bbH_pdf90up_A"); 

  m_h_bbH_pdfalphas68down_h = (TH2F*) m_input->Get("h_bbH_pdfalphas68down_h"); 
  m_h_bbH_pdfalphas68down_H = (TH2F*) m_input->Get("h_bbH_pdfalphas68down_H"); 
  m_h_bbH_pdfalphas68down_A = (TH2F*) m_input->Get("h_bbH_pdfalphas68down_A"); 
  m_h_bbH_pdfalphas68up_h = (TH2F*) m_input->Get("h_bbH_pdfalphas68up_h"); 
  m_h_bbH_pdfalphas68up_H = (TH2F*) m_input->Get("h_bbH_pdfalphas68up_H"); 
  m_h_bbH_pdfalphas68up_A = (TH2F*) m_input->Get("h_bbH_pdfalphas68up_A"); 

  m_h_bbH_pdfalphas90down_h = (TH2F*) m_input->Get("h_bbH_pdfalphas90down_h"); 
  m_h_bbH_pdfalphas90down_H = (TH2F*) m_input->Get("h_bbH_pdfalphas90down_H"); 
  m_h_bbH_pdfalphas90down_A = (TH2F*) m_input->Get("h_bbH_pdfalphas90down_A"); 
  m_h_bbH_pdfalphas90up_h = (TH2F*) m_input->Get("h_bbH_pdfalphas90up_h"); 
  m_h_bbH_pdfalphas90up_H = (TH2F*) m_input->Get("h_bbH_pdfalphas90up_H"); 
  m_h_bbH_pdfalphas90up_A = (TH2F*) m_input->Get("h_bbH_pdfalphas90up_A"); 
  
  m_h_mh = (TH2F*) m_input->Get("h_mh"); 
  m_h_mH = (TH2F*) m_input->Get("h_mH"); 

 
  std::cout<<"some info about the setup used for your chosen output file:"<<std::endl;
  TObjString* description=(TObjString*) m_input->Get("description");
  std::cout<<description->String()<<std::endl;

  return;
}
// New Stuff:
Double_t mssm_xs_tools::Give_BR_A_bb(Double_t mA, Double_t tanb){
  int gbin=m_h_brbb_A->FindBin(mA,tanb);
  return m_h_brbb_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_BR_H_bb(Double_t mA, Double_t tanb){
  int gbin=m_h_brbb_H->FindBin(mA,tanb);
  return m_h_brbb_H->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_BR_h_bb(Double_t mA, Double_t tanb){
  int gbin=m_h_brbb_h->FindBin(mA,tanb);
  return m_h_brbb_h->GetBinContent(gbin);
}

Double_t mssm_xs_tools::Give_BR_A_tautau(Double_t mA, Double_t tanb){
  int gbin=m_h_brtautau_A->FindBin(mA,tanb);
  return m_h_brtautau_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_BR_H_tautau(Double_t mA, Double_t tanb){
  int gbin=m_h_brtautau_H->FindBin(mA,tanb);
  return m_h_brtautau_H->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_BR_h_tautau(Double_t mA, Double_t tanb){
  int gbin=m_h_brtautau_h->FindBin(mA,tanb);
  return m_h_brtautau_h->GetBinContent(gbin);
}

Double_t mssm_xs_tools::Give_BR_A_mumu(Double_t mA, Double_t tanb){
  int gbin=m_h_brmumu_A->FindBin(mA,tanb);
  return m_h_brmumu_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_BR_H_mumu(Double_t mA, Double_t tanb){
  int gbin=m_h_brmumu_H->FindBin(mA,tanb);
  return m_h_brmumu_H->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_BR_h_mumu(Double_t mA, Double_t tanb){
  int gbin=m_h_brmumu_h->FindBin(mA,tanb);
  return m_h_brmumu_h->GetBinContent(gbin);
}






Double_t mssm_xs_tools::Give_Mass_h(Double_t mA, Double_t tanb){
  int gbin=m_h_mh->FindBin(mA,tanb);
  return m_h_mh->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_Mass_H(Double_t mA, Double_t tanb){
  int gbin=m_h_mH->FindBin(mA,tanb);
  return m_h_mH->GetBinContent(gbin);
}

  
Double_t mssm_xs_tools::Give_Xsec_bbA5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_xsec_A->FindBin(mA,tanb);
  return m_h_bbH_xsec_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_Xsec_bbh5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_xsec_h->FindBin(mA,tanb);
  return m_h_bbH_xsec_h->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_Xsec_bbH5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_xsec_H->FindBin(mA,tanb);
  return m_h_bbH_xsec_H->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_muup_bbA5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_muup_A->FindBin(mA,tanb);
  return m_h_bbH_muup_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_muup_bbh5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_muup_h->FindBin(mA,tanb);
  return m_h_bbH_muup_h->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_muup_bbH5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_muup_H->FindBin(mA,tanb);
  return m_h_bbH_muup_H->GetBinContent(gbin);
}


Double_t mssm_xs_tools::Give_XsecUnc_mudown_bbA5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_mudown_A->FindBin(mA,tanb);
  return m_h_bbH_mudown_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_mudown_bbh5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_mudown_h->FindBin(mA,tanb);
  return m_h_bbH_mudown_h->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_mudown_bbH5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_mudown_H->FindBin(mA,tanb);
  return m_h_bbH_mudown_H->GetBinContent(gbin);
}


Double_t mssm_xs_tools::Give_XsecUnc_pdf68up_bbA5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdf68up_A->FindBin(mA,tanb);
  return m_h_bbH_pdf68up_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_pdf68up_bbh5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdf68up_h->FindBin(mA,tanb);
  return m_h_bbH_pdf68up_h->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_pdf68up_bbH5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdf68up_H->FindBin(mA,tanb);
  return m_h_bbH_pdf68up_H->GetBinContent(gbin);
}

Double_t mssm_xs_tools::Give_XsecUnc_pdf68down_bbA5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdf68down_A->FindBin(mA,tanb);
  return m_h_bbH_pdf68down_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_pdf68down_bbh5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdf68down_h->FindBin(mA,tanb);
  return m_h_bbH_pdf68down_h->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_pdf68down_bbH5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdf68down_H->FindBin(mA,tanb);
  return m_h_bbH_pdf68down_H->GetBinContent(gbin);
}



Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas68up_bbA5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdfalphas68up_A->FindBin(mA,tanb);
  return m_h_bbH_pdfalphas68up_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas68up_bbh5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdfalphas68up_h->FindBin(mA,tanb);
  return m_h_bbH_pdfalphas68up_h->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas68up_bbH5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdfalphas68up_H->FindBin(mA,tanb);
  return m_h_bbH_pdfalphas68up_H->GetBinContent(gbin);
}

Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas68down_bbA5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdfalphas68down_A->FindBin(mA,tanb);
  return m_h_bbH_pdfalphas68down_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas68down_bbh5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdfalphas68down_h->FindBin(mA,tanb);
  return m_h_bbH_pdfalphas68down_h->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas68down_bbH5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdfalphas68down_H->FindBin(mA,tanb);
  return m_h_bbH_pdfalphas68down_H->GetBinContent(gbin);
}





Double_t mssm_xs_tools::Give_XsecUnc_pdf90up_bbA5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdf90up_A->FindBin(mA,tanb);
  return m_h_bbH_pdf90up_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_pdf90up_bbh5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdf90up_h->FindBin(mA,tanb);
  return m_h_bbH_pdf90up_h->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_pdf90up_bbH5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdf90up_H->FindBin(mA,tanb);
  return m_h_bbH_pdf90up_H->GetBinContent(gbin);
}

Double_t mssm_xs_tools::Give_XsecUnc_pdf90down_bbA5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdf90down_A->FindBin(mA,tanb);
  return m_h_bbH_pdf90down_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_pdf90down_bbh5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdf90down_h->FindBin(mA,tanb);
  return m_h_bbH_pdf90down_h->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_pdf90down_bbH5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdf90down_H->FindBin(mA,tanb);
  return m_h_bbH_pdf90down_H->GetBinContent(gbin);
}



Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas90up_bbA5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdfalphas90up_A->FindBin(mA,tanb);
  return m_h_bbH_pdfalphas90up_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas90up_bbh5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdfalphas90up_h->FindBin(mA,tanb);
  return m_h_bbH_pdfalphas90up_h->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas90up_bbH5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdfalphas90up_H->FindBin(mA,tanb);
  return m_h_bbH_pdfalphas90up_H->GetBinContent(gbin);
}

Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas90down_bbA5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdfalphas90down_A->FindBin(mA,tanb);
  return m_h_bbH_pdfalphas90down_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas90down_bbh5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdfalphas90down_h->FindBin(mA,tanb);
  return m_h_bbH_pdfalphas90down_h->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas90down_bbH5f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH_pdfalphas90down_H->FindBin(mA,tanb);
  return m_h_bbH_pdfalphas90down_H->GetBinContent(gbin);
}

Double_t mssm_xs_tools::Give_Xsec_bbA4f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH4f_xsec_A->FindBin(mA,tanb);
  return m_h_bbH4f_xsec_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_Xsec_bbH4f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH4f_xsec_H->FindBin(mA,tanb);
  return m_h_bbH4f_xsec_H->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_Xsec_bbh4f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH4f_xsec_h->FindBin(mA,tanb);
  return m_h_bbH4f_xsec_h->GetBinContent(gbin);
}


Double_t mssm_xs_tools::Give_XsecUnc_mudown_bbA4f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH4f_xsec_A_low->FindBin(mA,tanb);
  double lowval=m_h_bbH4f_xsec_A_low->GetBinContent(gbin);
  gbin=m_h_bbH4f_xsec_A->FindBin(mA,tanb);
  double centval=m_h_bbH4f_xsec_A->GetBinContent(gbin);
  return lowval-centval;
}

Double_t mssm_xs_tools::Give_XsecUnc_mudown_bbh4f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH4f_xsec_h_low->FindBin(mA,tanb);
  double lowval=m_h_bbH4f_xsec_h_low->GetBinContent(gbin);
  gbin=m_h_bbH4f_xsec_h->FindBin(mA,tanb);
  double centval=m_h_bbH4f_xsec_h->GetBinContent(gbin);
  return lowval-centval;
}

Double_t mssm_xs_tools::Give_XsecUnc_mudown_bbH4f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH4f_xsec_H_low->FindBin(mA,tanb);
  double lowval=m_h_bbH4f_xsec_H_low->GetBinContent(gbin);
  gbin=m_h_bbH4f_xsec_H->FindBin(mA,tanb);
  double centval=m_h_bbH4f_xsec_H->GetBinContent(gbin);
  return lowval-centval;
}


Double_t mssm_xs_tools::Give_XsecUnc_muup_bbA4f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH4f_xsec_A_high->FindBin(mA,tanb);
  double highval=m_h_bbH4f_xsec_A_high->GetBinContent(gbin);
  gbin=m_h_bbH4f_xsec_A->FindBin(mA,tanb);
  double centval=m_h_bbH4f_xsec_A->GetBinContent(gbin);
  return highval-centval;
}

Double_t mssm_xs_tools::Give_XsecUnc_muup_bbh4f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH4f_xsec_h_high->FindBin(mA,tanb);
  double highval=m_h_bbH4f_xsec_h_high->GetBinContent(gbin);
  gbin=m_h_bbH4f_xsec_h->FindBin(mA,tanb);
  double centval=m_h_bbH4f_xsec_h->GetBinContent(gbin);
  return highval-centval;
}

Double_t mssm_xs_tools::Give_XsecUnc_muup_bbH4f(Double_t mA, Double_t tanb){
  int gbin=m_h_bbH4f_xsec_H_high->FindBin(mA,tanb);
  double highval=m_h_bbH4f_xsec_H_high->GetBinContent(gbin);
  gbin=m_h_bbH4f_xsec_H->FindBin(mA,tanb);
  double centval=m_h_bbH4f_xsec_H->GetBinContent(gbin);
  return highval-centval;
}

Double_t mssm_xs_tools::Give_Xsec_ggFA(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_xsec_A->FindBin(mA,tanb);
  return 1000.*m_h_ggF_xsec_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_Xsec_ggFH(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_xsec_H->FindBin(mA,tanb);
  return 1000.*m_h_ggF_xsec_H->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_Xsec_ggFh(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_xsec_h->FindBin(mA,tanb);
  return 1000.*m_h_ggF_xsec_h->GetBinContent(gbin);
}

Double_t mssm_xs_tools::Give_XsecUnc_muup_ggFA(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_xsec05_A->FindBin(mA,tanb);
  double highval=1000.*m_h_ggF_xsec05_A->GetBinContent(gbin);
  gbin=m_h_ggF_xsec_A->FindBin(mA,tanb);
  double centval=1000.*m_h_ggF_xsec_A->GetBinContent(gbin);
  return (highval-centval);
}

Double_t mssm_xs_tools::Give_XsecUnc_muup_ggFH(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_xsec05_H->FindBin(mA,tanb);
  double highval=1000.*m_h_ggF_xsec05_H->GetBinContent(gbin);
  gbin=m_h_ggF_xsec_H->FindBin(mA,tanb);
  double centval=1000.*m_h_ggF_xsec_H->GetBinContent(gbin);
  return (highval-centval);
}

Double_t mssm_xs_tools::Give_XsecUnc_muup_ggFh(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_xsec05_h->FindBin(mA,tanb);
  double highval=1000.*m_h_ggF_xsec05_h->GetBinContent(gbin);
  gbin=m_h_ggF_xsec_h->FindBin(mA,tanb);
  double centval=1000.*m_h_ggF_xsec_h->GetBinContent(gbin);
  return (highval-centval);
}


Double_t mssm_xs_tools::Give_XsecUnc_mudown_ggFA(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_xsec20_A->FindBin(mA,tanb);
  double lowval=1000.*m_h_ggF_xsec20_A->GetBinContent(gbin);
  gbin=m_h_ggF_xsec_A->FindBin(mA,tanb);
  double centval=1000.*m_h_ggF_xsec_A->GetBinContent(gbin);
  return (lowval-centval);
}

Double_t mssm_xs_tools::Give_XsecUnc_mudown_ggFH(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_xsec20_H->FindBin(mA,tanb);
  double lowval=1000.*m_h_ggF_xsec20_H->GetBinContent(gbin);
  gbin=m_h_ggF_xsec_H->FindBin(mA,tanb);
  double centval=1000.*m_h_ggF_xsec_H->GetBinContent(gbin);
  return (lowval-centval);
}

Double_t mssm_xs_tools::Give_XsecUnc_mudown_ggFh(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_xsec20_h->FindBin(mA,tanb);
  double lowval=1000.*m_h_ggF_xsec20_h->GetBinContent(gbin);
  gbin=m_h_ggF_xsec_h->FindBin(mA,tanb);
  double centval=1000.*m_h_ggF_xsec_h->GetBinContent(gbin);
  return (lowval-centval);
}


Double_t mssm_xs_tools::Give_XsecUnc_pdf68up_ggFA(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_pdfup_A->FindBin(mA,tanb);
  return 1000.*m_h_ggF_pdfup_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_pdf68up_ggFH(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_pdfup_H->FindBin(mA,tanb);
  return 1000.*m_h_ggF_pdfup_H->GetBinContent(gbin);
}

Double_t mssm_xs_tools::Give_XsecUnc_pdf68up_ggFh(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_pdfup_h->FindBin(mA,tanb);
  return 1000.*m_h_ggF_pdfup_h->GetBinContent(gbin);
}


Double_t mssm_xs_tools::Give_XsecUnc_pdf68down_ggFA(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_pdfdown_A->FindBin(mA,tanb);
  return (-1000.)*m_h_ggF_pdfdown_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_pdf68down_ggFH(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_pdfdown_H->FindBin(mA,tanb);
  return (-1000.)*m_h_ggF_pdfdown_H->GetBinContent(gbin);
}

Double_t mssm_xs_tools::Give_XsecUnc_pdf68down_ggFh(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_pdfdown_h->FindBin(mA,tanb);
  return (-1000.)*m_h_ggF_pdfdown_h->GetBinContent(gbin);
}

Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas68up_ggFA(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_alphasup_A->FindBin(mA,tanb);
  double alphasval=1000.*m_h_ggF_alphasup_A->GetBinContent(gbin);
  gbin=m_h_ggF_pdfup_A->FindBin(mA,tanb);
  double pdfval=1000.*m_h_ggF_pdfup_A->GetBinContent(gbin);
  return sqrt(alphasval*alphasval+pdfval*pdfval);
}


Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas68up_ggFH(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_alphasup_H->FindBin(mA,tanb);
  double alphasval=1000.*m_h_ggF_alphasup_H->GetBinContent(gbin);
  gbin=m_h_ggF_pdfup_H->FindBin(mA,tanb);
  double pdfval=1000.*m_h_ggF_pdfup_H->GetBinContent(gbin);
  return sqrt(alphasval*alphasval+pdfval*pdfval);
}

Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas68up_ggFh(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_alphasup_h->FindBin(mA,tanb);
  double alphasval=1000.*m_h_ggF_alphasup_h->GetBinContent(gbin);
  gbin=m_h_ggF_pdfup_h->FindBin(mA,tanb);
  double pdfval=1000.*m_h_ggF_pdfup_h->GetBinContent(gbin);
  return sqrt(alphasval*alphasval+pdfval*pdfval);
}

Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas68down_ggFA(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_alphasdown_A->FindBin(mA,tanb);
  double alphasval=1000.*m_h_ggF_alphasdown_A->GetBinContent(gbin);
  gbin=m_h_ggF_pdfdown_A->FindBin(mA,tanb);
  double pdfval=1000.*m_h_ggF_pdfdown_A->GetBinContent(gbin);
  return -sqrt(alphasval*alphasval+pdfval*pdfval);
}


Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas68down_ggFH(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_alphasdown_H->FindBin(mA,tanb);
  double alphasval=1000.*m_h_ggF_alphasdown_H->GetBinContent(gbin);
  gbin=m_h_ggF_pdfdown_H->FindBin(mA,tanb);
  double pdfval=1000.*m_h_ggF_pdfdown_H->GetBinContent(gbin);
  return -sqrt(alphasval*alphasval+pdfval*pdfval);
}

Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas68down_ggFh(Double_t mA, Double_t tanb){
  int gbin=m_h_ggF_alphasdown_h->FindBin(mA,tanb);
  double alphasval=1000.*m_h_ggF_alphasdown_h->GetBinContent(gbin);
  gbin=m_h_ggF_pdfdown_h->FindBin(mA,tanb);
  double pdfval=1000.*m_h_ggF_pdfdown_h->GetBinContent(gbin);
  return -sqrt(alphasval*alphasval+pdfval*pdfval);
}

//COMBINED ggF + bbH
//&&&&&&&&&&&&&&&&&&&


Double_t mssm_xs_tools::Give_Xsec_ggFplusbbA5f(Double_t mA, Double_t tanb){
  int gbin=m_h_ggFplusbbH_xsec_A->FindBin(mA,tanb);
  return 1000.*m_h_ggFplusbbH_xsec_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_Xsec_ggFplusbbH5f(Double_t mA, Double_t tanb){
  int gbin=m_h_ggFplusbbH_xsec_H->FindBin(mA,tanb);
  return 1000.*m_h_ggFplusbbH_xsec_H->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_Xsec_ggFplusbbh5f(Double_t mA, Double_t tanb){
  int gbin=m_h_ggFplusbbH_xsec_h->FindBin(mA,tanb);
  return 1000.*m_h_ggFplusbbH_xsec_h->GetBinContent(gbin);
}

//SCALE UNCERTAINTY
Double_t mssm_xs_tools::Give_XsecUnc_muup_ggFplusbbA5f(Double_t mA, Double_t tanb){
  int gbin1=m_h_ggF_xsec05_A->FindBin(mA,tanb);
  int gbin2=m_h_bbH_muup_A->FindBin(mA,tanb);
  double highval=0.;
  double highval1=1000.*m_h_ggF_xsec05_A->GetBinContent(gbin1);
  double highval2=1.*m_h_bbH_muup_A->GetBinContent(gbin2);
  highval=highval1+highval2;
  gbin1=m_h_ggF_xsec_A->FindBin(mA,tanb);
  double centval=1000.*m_h_ggF_xsec_A->GetBinContent(gbin1);
  return (highval-centval);
}

Double_t mssm_xs_tools::Give_XsecUnc_muup_ggFplusbbh5f(Double_t mA, Double_t tanb){
  int gbin1=m_h_ggF_xsec05_h->FindBin(mA,tanb);
  int gbin2=m_h_bbH_muup_h->FindBin(mA,tanb);
  double highval=0.;
  double highval1=1000.*m_h_ggF_xsec05_h->GetBinContent(gbin1);
  double highval2=1.*m_h_bbH_muup_h->GetBinContent(gbin2);
  highval=highval1+highval2;
  gbin1=m_h_ggF_xsec_h->FindBin(mA,tanb);
  double centval=1000.*m_h_ggF_xsec_h->GetBinContent(gbin1);
  return (highval-centval);
}

Double_t mssm_xs_tools::Give_XsecUnc_muup_ggFplusbbH5f(Double_t mA, Double_t tanb){
  int gbin1=m_h_ggF_xsec05_H->FindBin(mA,tanb);
  int gbin2=m_h_bbH_muup_H->FindBin(mA,tanb);
  double highval=0.;
  double highval1=1000.*m_h_ggF_xsec05_H->GetBinContent(gbin1);
  double highval2=1.*m_h_bbH_muup_H->GetBinContent(gbin2);
  highval=highval1+highval2;
  gbin1=m_h_ggF_xsec_H->FindBin(mA,tanb);
  double centval=1000.*m_h_ggF_xsec_H->GetBinContent(gbin1);
  return (highval-centval);
}

Double_t mssm_xs_tools::Give_XsecUnc_mudown_ggFplusbbA5f(Double_t mA, Double_t tanb){
  int gbin1=m_h_ggF_xsec20_A->FindBin(mA,tanb);
  int gbin2=m_h_bbH_mudown_A->FindBin(mA,tanb);
  double lowval=0.;
  double lowval1=1000.*m_h_ggF_xsec20_A->GetBinContent(gbin1);
  double lowval2=1.*m_h_bbH_mudown_A->GetBinContent(gbin2);
  lowval=lowval1+lowval2;
  gbin1=m_h_ggF_xsec_A->FindBin(mA,tanb);
  double centval=1000.*m_h_ggF_xsec_A->GetBinContent(gbin1);
  return (lowval-centval);
}

Double_t mssm_xs_tools::Give_XsecUnc_mudown_ggFplusbbh5f(Double_t mA, Double_t tanb){
  int gbin1=m_h_ggF_xsec20_h->FindBin(mA,tanb);
  int gbin2=m_h_bbH_mudown_h->FindBin(mA,tanb);
  double lowval=0.;
  double lowval1=1000.*m_h_ggF_xsec20_h->GetBinContent(gbin1);
  double lowval2=1.*m_h_bbH_mudown_h->GetBinContent(gbin2);
  lowval=lowval1+lowval2;
  gbin1=m_h_ggF_xsec_h->FindBin(mA,tanb);
  double centval=1000.*m_h_ggF_xsec_h->GetBinContent(gbin1);
  return (lowval-centval);
}

Double_t mssm_xs_tools::Give_XsecUnc_mudown_ggFplusbbH5f(Double_t mA, Double_t tanb){
  int gbin1=m_h_ggF_xsec20_H->FindBin(mA,tanb);
  int gbin2=m_h_bbH_mudown_H->FindBin(mA,tanb);
  double lowval=0.;
  double lowval1=1000.*m_h_ggF_xsec20_H->GetBinContent(gbin1);
  double lowval2=1.*m_h_bbH_mudown_H->GetBinContent(gbin2);
  lowval=lowval1+lowval2;
  gbin1=m_h_ggF_xsec_H->FindBin(mA,tanb);
  double centval=1000.*m_h_ggF_xsec_H->GetBinContent(gbin1);
  return (lowval-centval);
}


//

Double_t mssm_xs_tools::Give_XsecUnc_pdf68up_ggFplusbbA5f(Double_t mA, Double_t tanb){
  int gbin=m_h_ggFplusbbH_pdfup_A->FindBin(mA,tanb);
  return 1000.*m_h_ggFplusbbH_pdfup_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_pdf68up_ggFplusbbH5f(Double_t mA, Double_t tanb){
  int gbin=m_h_ggFplusbbH_pdfup_H->FindBin(mA,tanb);
  return 1000.*m_h_ggFplusbbH_pdfup_H->GetBinContent(gbin);
}

Double_t mssm_xs_tools::Give_XsecUnc_pdf68up_ggFplusbbh5f(Double_t mA, Double_t tanb){
  int gbin=m_h_ggFplusbbH_pdfup_h->FindBin(mA,tanb);
  return 1000.*m_h_ggFplusbbH_pdfup_h->GetBinContent(gbin);
}


Double_t mssm_xs_tools::Give_XsecUnc_pdf68down_ggFplusbbA5f(Double_t mA, Double_t tanb){
  int gbin=m_h_ggFplusbbH_pdfdown_A->FindBin(mA,tanb);
  return (-1000.)*m_h_ggFplusbbH_pdfdown_A->GetBinContent(gbin);
}
Double_t mssm_xs_tools::Give_XsecUnc_pdf68down_ggFplusbbH5f(Double_t mA, Double_t tanb){
  int gbin=m_h_ggFplusbbH_pdfdown_H->FindBin(mA,tanb);
  return (-1000.)*m_h_ggFplusbbH_pdfdown_H->GetBinContent(gbin);
}

Double_t mssm_xs_tools::Give_XsecUnc_pdf68down_ggFplusbbh5f(Double_t mA, Double_t tanb){
  int gbin=m_h_ggFplusbbH_pdfdown_h->FindBin(mA,tanb);
  return (-1000.)*m_h_ggFplusbbH_pdfdown_h->GetBinContent(gbin);
}

Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas68up_ggFplusbbA5f(Double_t mA, Double_t tanb){
  int gbin=m_h_ggFplusbbH_alphasup_A->FindBin(mA,tanb);
  double alphasval=1000.*m_h_ggFplusbbH_alphasup_A->GetBinContent(gbin);
  gbin=m_h_ggFplusbbH_pdfup_A->FindBin(mA,tanb);
  double pdfval=1000.*m_h_ggFplusbbH_pdfup_A->GetBinContent(gbin);
  return sqrt(alphasval*alphasval+pdfval*pdfval);
}


Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas68up_ggFplusbbH5f(Double_t mA, Double_t tanb){
  int gbin=m_h_ggFplusbbH_alphasup_H->FindBin(mA,tanb);
  double alphasval=1000.*m_h_ggFplusbbH_alphasup_H->GetBinContent(gbin);
  gbin=m_h_ggFplusbbH_pdfup_H->FindBin(mA,tanb);
  double pdfval=1000.*m_h_ggFplusbbH_pdfup_H->GetBinContent(gbin);
  return sqrt(alphasval*alphasval+pdfval*pdfval);
}

Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas68up_ggFplusbbh5f(Double_t mA, Double_t tanb){
  int gbin=m_h_ggFplusbbH_alphasup_h->FindBin(mA,tanb);
  double alphasval=1000.*m_h_ggFplusbbH_alphasup_h->GetBinContent(gbin);
  gbin=m_h_ggFplusbbH_pdfup_h->FindBin(mA,tanb);
  double pdfval=1000.*m_h_ggFplusbbH_pdfup_h->GetBinContent(gbin);
  return sqrt(alphasval*alphasval+pdfval*pdfval);
}

Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas68down_ggFplusbbA5f(Double_t mA, Double_t tanb){
  int gbin=m_h_ggFplusbbH_alphasdown_A->FindBin(mA,tanb);
  double alphasval=1000.*m_h_ggFplusbbH_alphasdown_A->GetBinContent(gbin);
  gbin=m_h_ggFplusbbH_pdfdown_A->FindBin(mA,tanb);
  double pdfval=1000.*m_h_ggFplusbbH_pdfdown_A->GetBinContent(gbin);
  return -sqrt(alphasval*alphasval+pdfval*pdfval);
}


Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas68down_ggFplusbbH5f(Double_t mA, Double_t tanb){
  int gbin=m_h_ggFplusbbH_alphasdown_H->FindBin(mA,tanb);
  double alphasval=1000.*m_h_ggFplusbbH_alphasdown_H->GetBinContent(gbin);
  gbin=m_h_ggFplusbbH_pdfdown_H->FindBin(mA,tanb);
  double pdfval=1000.*m_h_ggFplusbbH_pdfdown_H->GetBinContent(gbin);
  return -sqrt(alphasval*alphasval+pdfval*pdfval);
}

Double_t mssm_xs_tools::Give_XsecUnc_pdfalphas68down_ggFplusbbh5f(Double_t mA, Double_t tanb){
  int gbin=m_h_ggFplusbbH_alphasdown_h->FindBin(mA,tanb);
  double alphasval=1000.*m_h_ggFplusbbH_alphasdown_h->GetBinContent(gbin);
  gbin=m_h_ggFplusbbH_pdfdown_h->FindBin(mA,tanb);
  double pdfval=1000.*m_h_ggFplusbbH_pdfdown_h->GetBinContent(gbin);
  return -sqrt(alphasval*alphasval+pdfval*pdfval);
}


Double_t mssm_xs_tools::GiveXsec_Santander_h(Double_t mA, Double_t tanb){
  double mh=this->Give_Mass_h(mA,tanb);
  double t=log(mh/4.75)-2.;
  double fourflav=this->Give_Xsec_bbh4f(mA,tanb);
  double fiveflav=this->Give_Xsec_bbh5f(mA,tanb);
  double sigma=(1./(1.+t))*(fourflav+t*fiveflav);
  return sigma;
}
Double_t mssm_xs_tools::GiveXsec_Santander_H(Double_t mA, Double_t tanb){
  double mh=this->Give_Mass_H(mA,tanb);
  double t=log(mh/4.75)-2.;
  double fourflav=this->Give_Xsec_bbH4f(mA,tanb);
  double fiveflav=this->Give_Xsec_bbH5f(mA,tanb);
  double sigma=(1./(1.+t))*(fourflav+t*fiveflav);
  return sigma;
}

Double_t mssm_xs_tools::GiveXsec_Santander_A(Double_t mA, Double_t tanb){
  double mh=mA;
  double t=log(mh/4.75)-2.;
  double fourflav=this->Give_Xsec_bbA4f(mA,tanb);
  double fiveflav=this->Give_Xsec_bbA5f(mA,tanb);
  double sigma=(1./(1.+t))*(fourflav+t*fiveflav);
  return sigma;
}

Double_t mssm_xs_tools::GiveXsec_UncUp_Santander_H(Double_t mA, Double_t tanb, TString PDFCL){
   double mh=this->Give_Mass_H(mA,tanb);
   double t=log(mh/4.75)-2.;
   // scale unc. for 4f:
   double dscale4f=this->Give_XsecUnc_muup_bbH4f(mA,tanb);
   double dscale5f=this->Give_XsecUnc_muup_bbH5f(mA,tanb);
   double pdf5f=0.;
   if (PDFCL=="68"){
     pdf5f=Give_XsecUnc_pdfalphas68up_bbH5f(mA,tanb);
   }
   else if (PDFCL=="90"){
     pdf5f=Give_XsecUnc_pdfalphas90up_bbH5f(mA,tanb);
   }
   else {
     return -99.;
   }
   dscale5f=dscale5f+pdf5f;
   //double err=1./(1+t)*sqrt(dscale4f*dscale4f+(t*t*dscale5f*dscale5f));
   double err=1./(1+t)*(dscale4f+(t*dscale5f));
   return err;
}
   
  
Double_t mssm_xs_tools::GiveXsec_UncUp_Santander_h(Double_t mA, Double_t tanb, TString PDFCL){
   double mh=this->Give_Mass_h(mA,tanb);
   double t=log(mh/4.75)-2.;
   // scale unc. for 4f:
   double dscale4f=this->Give_XsecUnc_muup_bbh4f(mA,tanb);
   double dscale5f=this->Give_XsecUnc_muup_bbh5f(mA,tanb);
   double pdf5f=0.;
   if (PDFCL=="68"){
     pdf5f=Give_XsecUnc_pdfalphas68up_bbh5f(mA,tanb);
   }
   else if (PDFCL=="90"){
     pdf5f=Give_XsecUnc_pdfalphas90up_bbh5f(mA,tanb);
   }
   else {
     return -99.;
   }
   dscale5f=dscale5f+pdf5f;
   //double err=1./(1+t)*sqrt(dscale4f*dscale4f+(t*t*dscale5f*dscale5f));
   double err=1./(1+t)*(dscale4f+(t*dscale5f));
   return err;
}
   
  
Double_t mssm_xs_tools::GiveXsec_UncUp_Santander_A(Double_t mA, Double_t tanb, TString PDFCL){
  double mh=mA;
   double t=log(mh/4.75)-2.;
   // scale unc. for 4f:
   double dscale4f=this->Give_XsecUnc_muup_bbA4f(mA,tanb);
   double dscale5f=this->Give_XsecUnc_muup_bbA5f(mA,tanb);
   double pdf5f=0.;
   if (PDFCL=="68"){
     pdf5f=Give_XsecUnc_pdfalphas68up_bbA5f(mA,tanb);
   }
   else if (PDFCL=="90"){
     pdf5f=Give_XsecUnc_pdfalphas90up_bbA5f(mA,tanb);
   }
   else {
     return -99.;
   }
   dscale5f=dscale5f+pdf5f;
   //double err=1./(1+t)*sqrt(dscale4f*dscale4f+(t*t*dscale5f*dscale5f));
   double err=1./(1+t)*(dscale4f+(t*dscale5f));
   return err;
}
   
  
Double_t mssm_xs_tools::GiveXsec_UncDown_Santander_H(Double_t mA, Double_t tanb, TString PDFCL){
   double mh=this->Give_Mass_H(mA,tanb);
   double t=log(mh/4.75)-2.;
   // scale unc. for 4f:
   double dscale4f=this->Give_XsecUnc_mudown_bbH4f(mA,tanb);
   double dscale5f=this->Give_XsecUnc_mudown_bbH5f(mA,tanb);
   double pdf5f=0.;
   if (PDFCL=="68"){
     pdf5f=Give_XsecUnc_pdfalphas68down_bbH5f(mA,tanb);
   }
   else if (PDFCL=="90"){
     pdf5f=Give_XsecUnc_pdfalphas90down_bbH5f(mA,tanb);
   }
   else {
     return -99.;
   }
   dscale5f=dscale5f+pdf5f;
   //double err=-1./(1+t)*sqrt(dscale4f*dscale4f+(t*t*dscale5f*dscale5f));
   double err=+1./(1+t)*(dscale4f+(t*dscale5f));
   return err;
}
   


  
Double_t mssm_xs_tools::GiveXsec_UncDown_Santander_h(Double_t mA, Double_t tanb, TString PDFCL){
   double mh=this->Give_Mass_h(mA,tanb);
   double t=log(mh/4.75)-2.;
   // scale unc. for 4f:
   double dscale4f=this->Give_XsecUnc_mudown_bbh4f(mA,tanb);
   double dscale5f=this->Give_XsecUnc_mudown_bbh5f(mA,tanb);
   double pdf5f=0.;
   if (PDFCL=="68"){
     pdf5f=Give_XsecUnc_pdfalphas68down_bbh5f(mA,tanb);
   }
   else if (PDFCL=="90"){
     pdf5f=Give_XsecUnc_pdfalphas90down_bbh5f(mA,tanb);
   }
   else {
     return -99.;
   }
   dscale5f=dscale5f+pdf5f;
   //double err=-1./(1+t)*sqrt(dscale4f*dscale4f+(t*t*dscale5f*dscale5f));
   double err=+1./(1+t)*(dscale4f+(t*dscale5f));
   return err;
}
   
  
Double_t mssm_xs_tools::GiveXsec_UncDown_Santander_A(Double_t mA, Double_t tanb, TString PDFCL){
  double mh=mA;
   double t=log(mh/4.75)-2.;
   // scale unc. for 4f:
   double dscale4f=this->Give_XsecUnc_mudown_bbA4f(mA,tanb);
   double dscale5f=this->Give_XsecUnc_mudown_bbA5f(mA,tanb);
   double pdf5f=0.;
   if (PDFCL=="68"){
     pdf5f=Give_XsecUnc_pdfalphas68down_bbA5f(mA,tanb);
   }
   else if (PDFCL=="90"){
     pdf5f=Give_XsecUnc_pdfalphas90down_bbA5f(mA,tanb);
   }
   else {
     return -99.;
   }
   dscale5f=dscale5f+pdf5f;
   //double err=-1./(1+t)*sqrt(dscale4f*dscale4f+(t*t*dscale5f*dscale5f));
   double err=+1./(1+t)*(dscale4f+(t*dscale5f));
   return err;
}
   
  
