#include <iostream>
#include <vector>
#include <map>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "boost/algorithm/string.hpp"
#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TMath.h"

#include "TGraphErrors.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plotting.h"

using namespace std;
using namespace ic;

std::pair<double,double> calcSoverRootB(std::string const& filepath, std::string const& directory, bool use_ztt_sig){
  TH1F ztt = GetFromTFile<TH1F>(filepath, directory, "ZTT");
  TH1F vv = GetFromTFile<TH1F>(filepath, directory, "VV"); 
  TH1F zll = GetFromTFile<TH1F>(filepath, directory, "ZLL");
  TH1F tt = GetFromTFile<TH1F>(filepath, directory, "TT");
  TH1F qcd = GetFromTFile<TH1F>(filepath, directory, "QCD");
  TH1F w = GetFromTFile<TH1F>(filepath, directory, "W");
  TH1F ggH125 = GetFromTFile<TH1F>(filepath, directory, "ggH125");
 
 double signal=0;
 double background=0;
 double sig_error=0;
 double bkg_error=0;
 if(use_ztt_sig){
  ztt.IntegralAndError(0,ztt.GetNbinsX()+1,sig_error);
  signal = ztt.Integral();

  TH1F bkglist[5]={vv,zll,tt,qcd,w};
  for(int i=0;i<5;++i){
    double dummy_bkg_err=0;
    bkglist[i].IntegralAndError(0,bkglist[i].GetNbinsX()+1,dummy_bkg_err);
    bkg_error+=dummy_bkg_err;
  }
  background = vv.Integral()+zll.Integral()+tt.Integral()+qcd.Integral()+w.Integral();
 } else {
  ggH125.IntegralAndError(0,ggH125.GetNbinsX()+1,sig_error);
  signal = ggH125.Integral();
  TH1F bkglist[6]={vv,ztt,zll,tt,qcd,w};
  for(int i=0;i<6;++i){
    double dummy_bkg_err=0;
    bkglist[i].IntegralAndError(0,bkglist[i].GetNbinsX()+1,dummy_bkg_err);
    bkg_error+=dummy_bkg_err;
  }
  background = ztt.Integral()+vv.Integral()+zll.Integral()+tt.Integral()+qcd.Integral()+w.Integral();
 }
 double sorb_err = TMath::Sqrt((sig_error/signal)*(sig_error/signal)+(0.5*bkg_error/background)*(0.5*bkg_error/background))*signal/TMath::Sqrt(background);
  return std::make_pair(signal/TMath::Sqrt(background),sorb_err); 
}

 int main(int /*argc*/, char* /*argv*/[]){
  using namespace ic;
  using namespace std;

 
// std::string file = "/vols/cms04/amd12/CMSSW_7_4_7/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/htt_mt.inputs-sm-13TeV-mvis-tauOPT.root";
 std::string file = "/vols/cms04/amd12/CMSSW_7_4_7/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/htt_em.inputs-sm-13TeV-mvis-EMFullDS.root";
 
 /*double ztt_sig_loosem[5];
 double ztt_sig_tightm[5];
 double ztt_sig_err_loosem[5];
 double ztt_sig_err_tightm[5];*/

 double ztt_sig_db04[10];
// double sm_sig_db04[10];
 double ztt_sig_ea03[10];
 double ztt_sig_db04allch[10];
 double ztt_sig_db03allch[10];
 double ztt_sig_db03[10];

 double ztt_sig_err_db04[10];
// double sm_sig_db04[10];
 double ztt_sig_err_ea03[10];
 double ztt_sig_err_db04allch[10];
 double ztt_sig_err_db03allch[10];
 double ztt_sig_err_db03[10];


//double x_err[5]={0,0,0,0,0};
//double x_err[12]={0,0,0,0,0,0,0,0,0,0,0,0};
double x_err[10]={0,0,0,0,0,0,0,0,0,0};

 //double sm_sig_trk03[10];
  double iso_cuts[10]={0.12,0.13,0.14,0.15,0.16,0.17,0.18,0.19,0.20,0.21};
/*  iso_cuts[0] = 0.07;
  iso_cuts[1] = 0.08;
  iso_cuts[2] = 0.09;
  iso_cuts[3] = 0.1;
  iso_cuts[4] = 0.11;
  iso_cuts[5] = 0.12;
  iso_cuts[6] = 0.13;
  iso_cuts[7] = 0.14;
  iso_cuts[8] = 0.15;
  iso_cuts[9] = 0.16;
  iso_cuts[10] = 0.17;
  iso_cuts[11] = 0.18;*/


// double antie[5]={1.,2.,3.,4.,5.};
  
 /* ztt_sig_loosem[0]=calcSoverRootB(file, "em_incvlelm",true).first;
  ztt_sig_loosem[1]=calcSoverRootB(file, "em_inclelm",true).first;
  ztt_sig_loosem[2]=calcSoverRootB(file, "em_incmelm",true).first;
  ztt_sig_loosem[3]=calcSoverRootB(file, "em_inctelm",true).first;
  ztt_sig_loosem[4]=calcSoverRootB(file, "em_incvtelm",true).first;
 
  ztt_sig_tightm[0]=calcSoverRootB(file, "em_incvletm",true).first;
  ztt_sig_tightm[1]=calcSoverRootB(file, "em_incletm",true).first;
  ztt_sig_tightm[2]=calcSoverRootB(file, "em_incmetm",true).first;
  ztt_sig_tightm[3]=calcSoverRootB(file, "em_inctetm",true).first;
  ztt_sig_tightm[4]=calcSoverRootB(file, "em_incvtetm",true).first;

 ztt_sig_err_loosem[0]=calcSoverRootB(file, "em_incvlelm",true).second;
  ztt_sig_err_loosem[1]=calcSoverRootB(file, "em_inclelm",true).second;
  ztt_sig_err_loosem[2]=calcSoverRootB(file, "em_incmelm",true).second;
  ztt_sig_err_loosem[3]=calcSoverRootB(file, "em_inctelm",true).second;
  ztt_sig_err_loosem[4]=calcSoverRootB(file, "em_incvtelm",true).second;
 
  ztt_sig_err_tightm[0]=calcSoverRootB(file, "em_incvletm",true).second;
  ztt_sig_err_tightm[1]=calcSoverRootB(file, "em_incletm",true).second;
  ztt_sig_err_tightm[2]=calcSoverRootB(file, "em_incmetm",true).second;
  ztt_sig_err_tightm[3]=calcSoverRootB(file, "em_inctetm",true).second;
  ztt_sig_err_tightm[4]=calcSoverRootB(file, "em_incvtetm",true).second;
*/

/*
  ztt_sig_db04[0] = calcSoverRootB(file, "em_db04iso0p07",true).first;
  ztt_sig_db04[1] = calcSoverRootB(file, "em_db04iso0p08",true).first;
  ztt_sig_db04[2] = calcSoverRootB(file, "em_db04iso0p09",true).first;
  ztt_sig_db04[3] = calcSoverRootB(file, "em_db04iso0p1",true).first;
  ztt_sig_db04[4] = calcSoverRootB(file, "em_db04iso0p11",true).first;
  ztt_sig_db04[5] = calcSoverRootB(file, "em_db04iso0p12",true).first;
  ztt_sig_db04[6] = calcSoverRootB(file, "em_db04iso0p13",true).first;
  ztt_sig_db04[7] = calcSoverRootB(file, "em_db04iso0p14",true).first;
  ztt_sig_db04[8] = calcSoverRootB(file, "em_db04iso0p15",true).first;
  ztt_sig_db04[9] = calcSoverRootB(file, "em_db04iso0p16",true).first;
  ztt_sig_db04[10] = calcSoverRootB(file, "em_db04iso0p17",true).first;
  ztt_sig_db04[11] = calcSoverRootB(file, "em_db04iso0p18",true).first;

  ztt_sig_err_db04[0] = calcSoverRootB(file, "em_db04iso0p07",true).second;
  ztt_sig_err_db04[1] = calcSoverRootB(file, "em_db04iso0p08",true).second;
  ztt_sig_err_db04[2] = calcSoverRootB(file, "em_db04iso0p09",true).second;
  ztt_sig_err_db04[3] = calcSoverRootB(file, "em_db04iso0p1",true).second;
  ztt_sig_err_db04[4] = calcSoverRootB(file, "em_db04iso0p11",true).second;
  ztt_sig_err_db04[5] = calcSoverRootB(file, "em_db04iso0p12",true).second;
  ztt_sig_err_db04[6] = calcSoverRootB(file, "em_db04iso0p13",true).second;
  ztt_sig_err_db04[7] = calcSoverRootB(file, "em_db04iso0p14",true).second;
  ztt_sig_err_db04[8] = calcSoverRootB(file, "em_db04iso0p15",true).second;
  ztt_sig_err_db04[9] = calcSoverRootB(file, "em_db04iso0p16",true).second;
  ztt_sig_err_db04[10] = calcSoverRootB(file, "em_db04iso0p17",true).second;
  ztt_sig_err_db04[11] = calcSoverRootB(file, "em_db04iso0p18",true).second;


  
  ztt_sig_ea03[0] = calcSoverRootB(file, "em_ea03iso0p07",true).first;
  ztt_sig_ea03[1] = calcSoverRootB(file, "em_ea03iso0p08",true).first;
  ztt_sig_ea03[2] = calcSoverRootB(file, "em_ea03iso0p09",true).first;
  ztt_sig_ea03[3] = calcSoverRootB(file, "em_ea03iso0p1",true).first;
  ztt_sig_ea03[4] = calcSoverRootB(file, "em_ea03iso0p11",true).first;
  ztt_sig_ea03[5] = calcSoverRootB(file, "em_ea03iso0p12",true).first;
  ztt_sig_ea03[6] = calcSoverRootB(file, "em_ea03iso0p13",true).first;
  ztt_sig_ea03[7] = calcSoverRootB(file, "em_ea03iso0p14",true).first;
  ztt_sig_ea03[8] = calcSoverRootB(file, "em_ea03iso0p15",true).first;
  ztt_sig_ea03[9] = calcSoverRootB(file, "em_ea03iso0p16",true).first;
  ztt_sig_ea03[10] = calcSoverRootB(file, "em_ea03iso0p17",true).first;
  ztt_sig_ea03[11] = calcSoverRootB(file, "em_ea03iso0p18",true).first;

ztt_sig_err_ea03[0] = calcSoverRootB(file, "em_ea03iso0p07",true).second;
  ztt_sig_err_ea03[1] = calcSoverRootB(file, "em_ea03iso0p08",true).second;
  ztt_sig_err_ea03[2] = calcSoverRootB(file, "em_ea03iso0p09",true).second;
  ztt_sig_err_ea03[3] = calcSoverRootB(file, "em_ea03iso0p1",true).second;
  ztt_sig_err_ea03[4] = calcSoverRootB(file, "em_ea03iso0p11",true).second;
  ztt_sig_err_ea03[5] = calcSoverRootB(file, "em_ea03iso0p12",true).second;
  ztt_sig_err_ea03[6] = calcSoverRootB(file, "em_ea03iso0p13",true).second;
  ztt_sig_err_ea03[7] = calcSoverRootB(file, "em_ea03iso0p14",true).second;
  ztt_sig_err_ea03[8] = calcSoverRootB(file, "em_ea03iso0p15",true).second;
  ztt_sig_err_ea03[9] = calcSoverRootB(file, "em_ea03iso0p16",true).second;
  ztt_sig_err_ea03[10] = calcSoverRootB(file, "em_ea03iso0p17",true).second;
  ztt_sig_err_ea03[11] = calcSoverRootB(file, "em_ea03iso0p18",true).second;



  ztt_sig_db04allch[0] = calcSoverRootB(file, "em_db04allchiso0p07",true).first;
  ztt_sig_db04allch[1] = calcSoverRootB(file, "em_db04allchiso0p08",true).first;
  ztt_sig_db04allch[2] = calcSoverRootB(file, "em_db04allchiso0p09",true).first;
  ztt_sig_db04allch[3] = calcSoverRootB(file, "em_db04allchiso0p1",true).first;
  ztt_sig_db04allch[4] = calcSoverRootB(file, "em_db04allchiso0p11",true).first;
  ztt_sig_db04allch[5] = calcSoverRootB(file, "em_db04allchiso0p12",true).first;
  ztt_sig_db04allch[6] = calcSoverRootB(file, "em_db04allchiso0p13",true).first;
  ztt_sig_db04allch[7] = calcSoverRootB(file, "em_db04allchiso0p14",true).first;
  ztt_sig_db04allch[8] = calcSoverRootB(file, "em_db04allchiso0p15",true).first;
  ztt_sig_db04allch[9] = calcSoverRootB(file, "em_db04allchiso0p16",true).first;
  ztt_sig_db04allch[10] = calcSoverRootB(file, "em_db04allchiso0p17",true).first;
  ztt_sig_db04allch[11] = calcSoverRootB(file, "em_db04allchiso0p18",true).first;

  ztt_sig_err_db04allch[0] = calcSoverRootB(file, "em_db04allchiso0p07",true).second;
  ztt_sig_err_db04allch[1] = calcSoverRootB(file, "em_db04allchiso0p08",true).second;
  ztt_sig_err_db04allch[2] = calcSoverRootB(file, "em_db04allchiso0p09",true).second;
  ztt_sig_err_db04allch[3] = calcSoverRootB(file, "em_db04allchiso0p1",true).second;
  ztt_sig_err_db04allch[4] = calcSoverRootB(file, "em_db04allchiso0p11",true).second;
  ztt_sig_err_db04allch[5] = calcSoverRootB(file, "em_db04allchiso0p12",true).second;
  ztt_sig_err_db04allch[6] = calcSoverRootB(file, "em_db04allchiso0p13",true).second;
  ztt_sig_err_db04allch[7] = calcSoverRootB(file, "em_db04allchiso0p14",true).second;
  ztt_sig_err_db04allch[8] = calcSoverRootB(file, "em_db04allchiso0p15",true).second;
  ztt_sig_err_db04allch[9] = calcSoverRootB(file, "em_db04allchiso0p16",true).second;
  ztt_sig_err_db04allch[10] = calcSoverRootB(file, "em_db04allchiso0p17",true).second;
  ztt_sig_err_db04allch[11] = calcSoverRootB(file, "em_db04allchiso0p18",true).second;



  ztt_sig_db03allch[0] = calcSoverRootB(file, "em_db03allchiso0p07",true).first;
  ztt_sig_db03allch[1] = calcSoverRootB(file, "em_db03allchiso0p08",true).first;
  ztt_sig_db03allch[2] = calcSoverRootB(file, "em_db03allchiso0p09",true).first;
  ztt_sig_db03allch[3] = calcSoverRootB(file, "em_db03allchiso0p1",true).first;
  ztt_sig_db03allch[4] = calcSoverRootB(file, "em_db03allchiso0p11",true).first;
  ztt_sig_db03allch[5] = calcSoverRootB(file, "em_db03allchiso0p12",true).first;
  ztt_sig_db03allch[6] = calcSoverRootB(file, "em_db03allchiso0p13",true).first;
  ztt_sig_db03allch[7] = calcSoverRootB(file, "em_db03allchiso0p14",true).first;
  ztt_sig_db03allch[8] = calcSoverRootB(file, "em_db03allchiso0p15",true).first;
  ztt_sig_db03allch[9] = calcSoverRootB(file, "em_db03allchiso0p16",true).first;
  ztt_sig_db03allch[10] = calcSoverRootB(file, "em_db03allchiso0p17",true).first;
  ztt_sig_db03allch[11] = calcSoverRootB(file, "em_db03allchiso0p13",true).first;

  ztt_sig_err_db03allch[0] = calcSoverRootB(file, "em_db03allchiso0p07",true).second;
  ztt_sig_err_db03allch[1] = calcSoverRootB(file, "em_db03allchiso0p08",true).second;
  ztt_sig_err_db03allch[2] = calcSoverRootB(file, "em_db03allchiso0p09",true).second;
  ztt_sig_err_db03allch[3] = calcSoverRootB(file, "em_db03allchiso0p1",true).second;
  ztt_sig_err_db03allch[4] = calcSoverRootB(file, "em_db03allchiso0p11",true).second;
  ztt_sig_err_db03allch[5] = calcSoverRootB(file, "em_db03allchiso0p12",true).second;
  ztt_sig_err_db03allch[6] = calcSoverRootB(file, "em_db03allchiso0p13",true).second;
  ztt_sig_err_db03allch[7] = calcSoverRootB(file, "em_db03allchiso0p14",true).second;
  ztt_sig_err_db03allch[8] = calcSoverRootB(file, "em_db03allchiso0p15",true).second;
  ztt_sig_err_db03allch[9] = calcSoverRootB(file, "em_db03allchiso0p16",true).second;
  ztt_sig_err_db03allch[10] = calcSoverRootB(file, "em_db03allchiso0p17",true).second;
  ztt_sig_err_db03allch[11] = calcSoverRootB(file, "em_db03allchiso0p13",true).second;



  ztt_sig_db03[0] = calcSoverRootB(file, "em_db03iso0p07",true).first;
  ztt_sig_db03[1] = calcSoverRootB(file, "em_db03iso0p08",true).first;
  ztt_sig_db03[2] = calcSoverRootB(file, "em_db03iso0p09",true).first;
  ztt_sig_db03[3] = calcSoverRootB(file, "em_db03iso0p1",true).first;
  ztt_sig_db03[4] = calcSoverRootB(file, "em_db03iso0p11",true).first;
  ztt_sig_db03[5] = calcSoverRootB(file, "em_db03iso0p12",true).first;
  ztt_sig_db03[6] = calcSoverRootB(file, "em_db03iso0p13",true).first;
  ztt_sig_db03[7] = calcSoverRootB(file, "em_db03iso0p14",true).first;
  ztt_sig_db03[8] = calcSoverRootB(file, "em_db03iso0p15",true).first;
  ztt_sig_db03[9] = calcSoverRootB(file, "em_db03iso0p16",true).first;
  ztt_sig_db03[10] = calcSoverRootB(file, "em_db03iso0p17",true).first;
  ztt_sig_db03[11] = calcSoverRootB(file, "em_db03iso0p18",true).first;

ztt_sig_err_db03[0] = calcSoverRootB(file, "em_db03iso0p07",true).second;
  ztt_sig_err_db03[1] = calcSoverRootB(file, "em_db03iso0p08",true).second;
  ztt_sig_err_db03[2] = calcSoverRootB(file, "em_db03iso0p09",true).second;
  ztt_sig_err_db03[3] = calcSoverRootB(file, "em_db03iso0p1",true).second;
  ztt_sig_err_db03[4] = calcSoverRootB(file, "em_db03iso0p11",true).second;
  ztt_sig_err_db03[5] = calcSoverRootB(file, "em_db03iso0p12",true).second;
  ztt_sig_err_db03[6] = calcSoverRootB(file, "em_db03iso0p13",true).second;
  ztt_sig_err_db03[7] = calcSoverRootB(file, "em_db03iso0p14",true).second;
  ztt_sig_err_db03[8] = calcSoverRootB(file, "em_db03iso0p15",true).second;
  ztt_sig_err_db03[9] = calcSoverRootB(file, "em_db03iso0p16",true).second;
  ztt_sig_err_db03[10] = calcSoverRootB(file, "em_db03iso0p17",true).second;
  ztt_sig_err_db03[11] = calcSoverRootB(file, "em_db03iso0p18",true).second;

*/

  ztt_sig_db04[0] = calcSoverRootB(file, "em_mdb04iso0p12",true).first;
  ztt_sig_db04[1] = calcSoverRootB(file, "em_mdb04iso0p13",true).first;
  ztt_sig_db04[2] = calcSoverRootB(file, "em_mdb04iso0p14",true).first;
  ztt_sig_db04[3] = calcSoverRootB(file, "em_mdb04iso0p15",true).first;
  ztt_sig_db04[4] = calcSoverRootB(file, "em_mdb04iso0p16",true).first;
  ztt_sig_db04[5] = calcSoverRootB(file, "em_mdb04iso0p17",true).first;
  ztt_sig_db04[6] = calcSoverRootB(file, "em_mdb04iso0p18",true).first;
  ztt_sig_db04[7] = calcSoverRootB(file, "em_mdb04iso0p19",true).first;
  ztt_sig_db04[8] = calcSoverRootB(file, "em_mdb04iso0p20",true).first;
  ztt_sig_db04[9] = calcSoverRootB(file, "em_mdb04iso0p21",true).first;

  ztt_sig_err_db04[0] = calcSoverRootB(file, "em_mdb04iso0p12",true).second;
  ztt_sig_err_db04[1] = calcSoverRootB(file, "em_mdb04iso0p13",true).second;
  ztt_sig_err_db04[2] = calcSoverRootB(file, "em_mdb04iso0p14",true).second;
  ztt_sig_err_db04[3] = calcSoverRootB(file, "em_mdb04iso0p15",true).second;
  ztt_sig_err_db04[4] = calcSoverRootB(file, "em_mdb04iso0p16",true).second;
  ztt_sig_err_db04[5] = calcSoverRootB(file, "em_mdb04iso0p17",true).second;
  ztt_sig_err_db04[6] = calcSoverRootB(file, "em_mdb04iso0p18",true).second;
  ztt_sig_err_db04[7] = calcSoverRootB(file, "em_mdb04iso0p19",true).second;
  ztt_sig_err_db04[8] = calcSoverRootB(file, "em_mdb04iso0p20",true).second;
  ztt_sig_err_db04[9] = calcSoverRootB(file, "em_mdb04iso0p21",true).second;



  ztt_sig_ea03[0] = calcSoverRootB(file, "em_eea03iso0p12",true).first;
  ztt_sig_ea03[1] = calcSoverRootB(file, "em_eea03iso0p13",true).first;
  ztt_sig_ea03[2] = calcSoverRootB(file, "em_eea03iso0p14",true).first;
  ztt_sig_ea03[3] = calcSoverRootB(file, "em_eea03iso0p15",true).first;
  ztt_sig_ea03[4] = calcSoverRootB(file, "em_eea03iso0p16",true).first;
  ztt_sig_ea03[5] = calcSoverRootB(file, "em_eea03iso0p17",true).first;
  ztt_sig_ea03[6] = calcSoverRootB(file, "em_eea03iso0p18",true).first;
  ztt_sig_ea03[7] = calcSoverRootB(file, "em_eea03iso0p19",true).first;
  ztt_sig_ea03[8] = calcSoverRootB(file, "em_eea03iso0p20",true).first;
  ztt_sig_ea03[9] = calcSoverRootB(file, "em_eea03iso0p21",true).first;

ztt_sig_err_ea03[0] = calcSoverRootB(file, "em_eea03iso0p12",true).second;
  ztt_sig_err_ea03[1] = calcSoverRootB(file, "em_eea03iso0p13",true).second;
  ztt_sig_err_ea03[2] = calcSoverRootB(file, "em_eea03iso0p14",true).second;
  ztt_sig_err_ea03[3] = calcSoverRootB(file, "em_eea03iso0p15",true).second;
  ztt_sig_err_ea03[4] = calcSoverRootB(file, "em_eea03iso0p16",true).second;
  ztt_sig_err_ea03[5] = calcSoverRootB(file, "em_eea03iso0p17",true).second;
  ztt_sig_err_ea03[6] = calcSoverRootB(file, "em_eea03iso0p18",true).second;
  ztt_sig_err_ea03[7] = calcSoverRootB(file, "em_eea03iso0p19",true).second;
  ztt_sig_err_ea03[8] = calcSoverRootB(file, "em_eea03iso0p20",true).second;
  ztt_sig_err_ea03[9] = calcSoverRootB(file, "em_eea03iso0p21",true).second;



  ztt_sig_db04allch[0] = calcSoverRootB(file, "em_mdb04allchiso0p12",true).first;
  ztt_sig_db04allch[1] = calcSoverRootB(file, "em_mdb04allchiso0p13",true).first;
  ztt_sig_db04allch[2] = calcSoverRootB(file, "em_mdb04allchiso0p14",true).first;
  ztt_sig_db04allch[3] = calcSoverRootB(file, "em_mdb04allchiso0p15",true).first;
  ztt_sig_db04allch[4] = calcSoverRootB(file, "em_mdb04allchiso0p16",true).first;
  ztt_sig_db04allch[5] = calcSoverRootB(file, "em_mdb04allchiso0p17",true).first;
  ztt_sig_db04allch[6] = calcSoverRootB(file, "em_mdb04allchiso0p18",true).first;
  ztt_sig_db04allch[7] = calcSoverRootB(file, "em_mdb04allchiso0p19",true).first;
  ztt_sig_db04allch[8] = calcSoverRootB(file, "em_mdb04allchiso0p20",true).first;
  ztt_sig_db04allch[9] = calcSoverRootB(file, "em_mdb04allchiso0p21",true).first;

  ztt_sig_err_db04allch[0] = calcSoverRootB(file, "em_mdb04allchiso0p12",true).second;
  ztt_sig_err_db04allch[1] = calcSoverRootB(file, "em_mdb04allchiso0p13",true).second;
  ztt_sig_err_db04allch[2] = calcSoverRootB(file, "em_mdb04allchiso0p14",true).second;
  ztt_sig_err_db04allch[3] = calcSoverRootB(file, "em_mdb04allchiso0p15",true).second;
  ztt_sig_err_db04allch[4] = calcSoverRootB(file, "em_mdb04allchiso0p16",true).second;
  ztt_sig_err_db04allch[5] = calcSoverRootB(file, "em_mdb04allchiso0p17",true).second;
  ztt_sig_err_db04allch[6] = calcSoverRootB(file, "em_mdb04allchiso0p18",true).second;
  ztt_sig_err_db04allch[7] = calcSoverRootB(file, "em_mdb04allchiso0p19",true).second;
  ztt_sig_err_db04allch[8] = calcSoverRootB(file, "em_mdb04allchiso0p20",true).second;
  ztt_sig_err_db04allch[9] = calcSoverRootB(file, "em_mdb04allchiso0p21",true).second;



  ztt_sig_db03allch[0] = calcSoverRootB(file, "em_mdb03allchiso0p12",true).first;
  ztt_sig_db03allch[1] = calcSoverRootB(file, "em_mdb03allchiso0p13",true).first;
  ztt_sig_db03allch[2] = calcSoverRootB(file, "em_mdb03allchiso0p14",true).first;
  ztt_sig_db03allch[3] = calcSoverRootB(file, "em_mdb03allchiso0p15",true).first;
  ztt_sig_db03allch[4] = calcSoverRootB(file, "em_mdb03allchiso0p16",true).first;
  ztt_sig_db03allch[5] = calcSoverRootB(file, "em_mdb03allchiso0p17",true).first;
  ztt_sig_db03allch[6] = calcSoverRootB(file, "em_mdb03allchiso0p18",true).first;
  ztt_sig_db03allch[7] = calcSoverRootB(file, "em_mdb03allchiso0p19",true).first;
  ztt_sig_db03allch[8] = calcSoverRootB(file, "em_mdb03allchiso0p20",true).first;
  ztt_sig_db03allch[9] = calcSoverRootB(file, "em_mdb03allchiso0p21",true).first;

  ztt_sig_err_db03allch[0] = calcSoverRootB(file, "em_mdb03allchiso0p12",true).second;
  ztt_sig_err_db03allch[1] = calcSoverRootB(file, "em_mdb03allchiso0p13",true).second;
  ztt_sig_err_db03allch[2] = calcSoverRootB(file, "em_mdb03allchiso0p14",true).second;
  ztt_sig_err_db03allch[3] = calcSoverRootB(file, "em_mdb03allchiso0p15",true).second;
  ztt_sig_err_db03allch[4] = calcSoverRootB(file, "em_mdb03allchiso0p16",true).second;
  ztt_sig_err_db03allch[5] = calcSoverRootB(file, "em_mdb03allchiso0p17",true).second;
  ztt_sig_err_db03allch[6] = calcSoverRootB(file, "em_mdb03allchiso0p18",true).second;
  ztt_sig_err_db03allch[7] = calcSoverRootB(file, "em_mdb03allchiso0p19",true).second;
  ztt_sig_err_db03allch[8] = calcSoverRootB(file, "em_mdb03allchiso0p20",true).second;
  ztt_sig_err_db03allch[9] = calcSoverRootB(file, "em_mdb03allchiso0p21",true).second;



  ztt_sig_db03[0] = calcSoverRootB(file, "em_mdb03iso0p12",true).first;
  ztt_sig_db03[1] = calcSoverRootB(file, "em_mdb03iso0p13",true).first;
  ztt_sig_db03[2] = calcSoverRootB(file, "em_mdb03iso0p14",true).first;
  ztt_sig_db03[3] = calcSoverRootB(file, "em_mdb03iso0p15",true).first;
  ztt_sig_db03[4] = calcSoverRootB(file, "em_mdb03iso0p16",true).first;
  ztt_sig_db03[5] = calcSoverRootB(file, "em_mdb03iso0p17",true).first;
  ztt_sig_db03[6] = calcSoverRootB(file, "em_mdb03iso0p18",true).first;
  ztt_sig_db03[7] = calcSoverRootB(file, "em_mdb03iso0p19",true).first;
  ztt_sig_db03[8] = calcSoverRootB(file, "em_mdb03iso0p20",true).first;
  ztt_sig_db03[9] = calcSoverRootB(file, "em_mdb03iso0p21",true).first;

ztt_sig_err_db03[0] = calcSoverRootB(file, "em_mdb03iso0p12",true).second;
  ztt_sig_err_db03[1] = calcSoverRootB(file, "em_mdb03iso0p13",true).second;
  ztt_sig_err_db03[2] = calcSoverRootB(file, "em_mdb03iso0p14",true).second;
  ztt_sig_err_db03[3] = calcSoverRootB(file, "em_mdb03iso0p15",true).second;
  ztt_sig_err_db03[4] = calcSoverRootB(file, "em_mdb03iso0p16",true).second;
  ztt_sig_err_db03[5] = calcSoverRootB(file, "em_mdb03iso0p17",true).second;
  ztt_sig_err_db03[6] = calcSoverRootB(file, "em_mdb03iso0p18",true).second;
  ztt_sig_err_db03[7] = calcSoverRootB(file, "em_mdb03iso0p19",true).second;
  ztt_sig_err_db03[8] = calcSoverRootB(file, "em_mdb03iso0p20",true).second;
  ztt_sig_err_db03[9] = calcSoverRootB(file, "em_mdb03iso0p21",true).second;



/*  sm_sig_trk03[0] = calcSoverRootB(file, "em_trk03iso0p09",false);
  sm_sig_trk03[1] = calcSoverRootB(file, "em_trk03iso0p1",false);
  sm_sig_trk03[2] = calcSoverRootB(file, "em_trk03iso0p11",false);
  sm_sig_trk03[3] = calcSoverRootB(file, "em_trk03iso0p12",false);
  sm_sig_trk03[4] = calcSoverRootB(file, "em_trk03iso0p13",false);
  sm_sig_trk03[5] = calcSoverRootB(file, "em_trk03iso0p14",false);
  sm_sig_trk03[6] = calcSoverRootB(file, "em_trk03iso0p15",false);
  sm_sig_trk03[7] = calcSoverRootB(file, "em_trk03iso0p16",false);
  sm_sig_trk03[8] = calcSoverRootB(file, "em_trk03iso0p17",false);
  sm_sig_trk03[9] = calcSoverRootB(file, "em_trk03iso0p18",false);*/

  //TGraphErrors *ztt_sig_loosem_graph = new TGraphErrors(5,antie,ztt_sig_loosem,x_err,ztt_sig_err_loosem);
  //TGraphErrors *ztt_sig_tightm_graph = new TGraphErrors(5,antie,ztt_sig_tightm,x_err,ztt_sig_err_tightm);

 /*ztt_sig_loosem_graph->SetMarkerStyle(20);
 ztt_sig_loosem_graph->SetMarkerColor(kRed);
 ztt_sig_tightm_graph->SetMarkerStyle(21);
 ztt_sig_tightm_graph->SetMarkerColor(kGreen+3);
*/

/*  TGraphErrors *ztt_sig_db04_graph = new TGraphErrors(12,iso_cuts,ztt_sig_db04,x_err,ztt_sig_err_db04); 
  TGraphErrors *ztt_sig_ea03_graph = new TGraphErrors(12,iso_cuts,ztt_sig_ea03,x_err,ztt_sig_err_ea03); 
  TGraphErrors *ztt_sig_db04allch_graph = new TGraphErrors(12,iso_cuts,ztt_sig_db04allch,x_err,ztt_sig_err_db04allch); 
  TGraphErrors *ztt_sig_db03allch_graph = new TGraphErrors(12,iso_cuts,ztt_sig_db03allch,x_err,ztt_sig_err_db03allch); 
  TGraphErrors *ztt_sig_db03_graph = new TGraphErrors(12,iso_cuts,ztt_sig_db03,x_err,ztt_sig_err_db03);*/ 

  TGraphErrors *ztt_sig_db04_graph = new TGraphErrors(10,iso_cuts,ztt_sig_db04,x_err,ztt_sig_err_db04); 
  TGraphErrors *ztt_sig_ea03_graph = new TGraphErrors(10,iso_cuts,ztt_sig_ea03,x_err,ztt_sig_err_ea03); 
  TGraphErrors *ztt_sig_db04allch_graph = new TGraphErrors(10,iso_cuts,ztt_sig_db04allch,x_err,ztt_sig_err_db04allch); 
  TGraphErrors *ztt_sig_db03allch_graph = new TGraphErrors(10,iso_cuts,ztt_sig_db03allch,x_err,ztt_sig_err_db03allch); 
  TGraphErrors *ztt_sig_db03_graph = new TGraphErrors(10,iso_cuts,ztt_sig_db03,x_err,ztt_sig_err_db03); 


/*  TGraph *sm_sig_db04_graph = new TGraph(10,iso_cuts,sm_sig_db04); 
  TGraph *sm_sig_trk03_graph = new TGraph(10,iso_cuts,sm_sig_trk03); */


  ztt_sig_db04_graph->SetMarkerStyle(20);
  ztt_sig_db04_graph->SetMarkerColor(kRed);
  ztt_sig_ea03_graph->SetMarkerStyle(21);
  ztt_sig_ea03_graph->SetMarkerColor(kGreen+3);
  ztt_sig_db04allch_graph->SetMarkerStyle(22);
  ztt_sig_db04allch_graph->SetMarkerColor(kBlue);
  ztt_sig_db03allch_graph->SetMarkerStyle(23);
  ztt_sig_db03allch_graph->SetMarkerColor(kBlue+3);
  ztt_sig_db03_graph->SetMarkerStyle(24);
  ztt_sig_db03_graph->SetMarkerColor(kRed+2);


//:  ztt_sig_db04_graph->GetXaxis()->SetRangeUser(0.065,0.185);
  ztt_sig_db04_graph->GetYaxis()->SetRangeUser(40,80);
  ztt_sig_db04_graph->SetTitle(";Isolation cut;S/root(B)");
  
/* ztt_sig_loosem_graph->GetYaxis()->SetRangeUser(40,80);
 ztt_sig_loosem_graph->SetTitle(";AntiE discriminator;S/root(B)");*/
   
  TCanvas plot_canv("plot_canv","plot_canv",1200,800); 
  plot_canv.cd();
/*  ztt_sig_loosem_graph->Draw("AP");
  ztt_sig_tightm_graph->Draw("PSAME");*/
  ztt_sig_db04_graph->Draw("AP"); 
  //ztt_sig_ea03_graph->Draw("PSAME");
 // ztt_sig_db04allch_graph->Draw("PSAME");
//  ztt_sig_db03allch_graph->Draw("PSAME");
  ztt_sig_db03_graph->Draw("PSAME");

  TLegend *leg = PositionedLegend(0.3,0.3,6,0);
  leg->SetFillStyle(0);
/*  leg->AddEntry(ztt_sig_loosem_graph,"antiMuLoose","p");
  leg->AddEntry(ztt_sig_tightm_graph,"antiMuTight","p");*/
  leg->AddEntry(ztt_sig_db04_graph,"DBeta corr (cs 0.4)","p");
  leg->AddEntry(ztt_sig_ea03_graph,"Tracker iso (cs 0.3)","p");
  leg->AddEntry(ztt_sig_db04allch_graph,"DBeta corr, allcharged (cs 0.4)","p");
  leg->AddEntry(ztt_sig_db03allch_graph,"DBeta corr, allcharged (cs 0.3)","p");
  leg->AddEntry(ztt_sig_db03_graph,"DBeta corr (cs 0.3)","p");


  leg->Draw("SAME");
  
 // plot_canv.SetGrid(1,1);

  plot_canv.SaveAs("s_over_root_b_em_m.pdf");

 return 0;
}
