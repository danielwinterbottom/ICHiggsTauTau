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
  std::string channel = "em";
  std::string e_or_m = "m";
  std::string extralabel = "Nov20Opt";
  bool do_tau = false;

 
 std::string tau_file = ("/vols/cms04/amd12/CMSSW_7_4_7/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/htt_"+channel+".inputs-sm-13TeV-mvis-TauOpt.root").c_str();
 std::string file = "";
 if(!do_tau){
 file = ("/vols/cms04/amd12/CMSSW_7_4_7/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/htt_"+channel+".inputs-sm-13TeV-mvis-"+extralabel+".root").c_str();
}
 
 double ztt_sig_loosem[5];
 double ztt_sig_tightm[5];
 double ztt_sig_err_loosem[5];
 double ztt_sig_err_tightm[5];

 double ztt_sig_db04[12];
// double sm_sig_db04[10];
 double ztt_sig_ea03[12];
 double ztt_sig_db04allch[12];
 double ztt_sig_db03allch[12];
 double ztt_sig_db03[12];

 double ztt_sig_err_db04[12];
// double sm_sig_db04[10];
 double ztt_sig_err_ea03[12];
 double ztt_sig_err_db04allch[12];
 double ztt_sig_err_db03allch[12];
 double ztt_sig_err_db03[12];


double x_err_tau[5]={0,0,0,0,0};
double iso_cuts_tau[5]={1.,2.,3.,4.,5.};
TH1F hempty("hempty",";Anti-E discriminator;S/#sqrt{B}",5,0.5,5.5);
hempty.SetStats(0);
if(channel=="et"){
hempty.GetYaxis()->SetRangeUser(28,36);
}else if(channel=="mt"){
hempty.GetYaxis()->SetRangeUser(50,70);
}else{
hempty.GetYaxis()->SetRangeUser(0,5);
}
hempty.GetXaxis()->SetBinLabel(1,"VL");
hempty.GetXaxis()->SetBinLabel(2,"L");
hempty.GetXaxis()->SetBinLabel(3,"M");
hempty.GetXaxis()->SetBinLabel(4,"T");
hempty.GetXaxis()->SetBinLabel(5,"VT");

double x_err[12]={0,0,0,0,0,0,0,0,0,0,0,0};
//double x_err[10]={0,0,0,0,0,0,0,0,0,0};

 //double sm_sig_trk03[10];
//  double iso_cuts[10]={0.12,0.13,0.14,0.15,0.16,0.17,0.18,0.19,0.20,0.21};
 double iso_cuts[12];
 if(channel!="em"){
   double iso_cuts_fill[12]={0.07,0.08,0.09,0.1,0.11,0.12,0.13,0.14,0.15,0.16,0.17,0.18};
   for(unsigned i=0;i<12;++i){
     iso_cuts[i]=iso_cuts_fill[i];
   }
  }
 else{
   double iso_cuts_fill[12]={0.1,0.11,0.12,0.13,0.14,0.15,0.16,0.17,0.18,0.19,0.20,0.21};
   for(unsigned i=0;i<12;++i){
     iso_cuts[i]=iso_cuts_fill[i];
   }
  }
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
  
if(do_tau){
  ztt_sig_loosem[0]=calcSoverRootB(tau_file, (channel+"_incvlelm").c_str(),true).first;
  ztt_sig_loosem[1]=calcSoverRootB(tau_file, (channel+"_inclelm").c_str(),true).first;
  ztt_sig_loosem[2]=calcSoverRootB(tau_file, (channel+"_incmelm").c_str(),true).first;
  ztt_sig_loosem[3]=calcSoverRootB(tau_file, (channel+"_inctelm").c_str(),true).first;
  ztt_sig_loosem[4]=calcSoverRootB(tau_file, (channel+"_incvtelm").c_str(),true).first;
 
  ztt_sig_tightm[0]=calcSoverRootB(tau_file, (channel+"_incvletm").c_str(),true).first;
  ztt_sig_tightm[1]=calcSoverRootB(tau_file, (channel+"_incletm").c_str(),true).first;
  ztt_sig_tightm[2]=calcSoverRootB(tau_file, (channel+"_incmetm").c_str(),true).first;
  ztt_sig_tightm[3]=calcSoverRootB(tau_file, (channel+"_inctetm").c_str(),true).first;
  ztt_sig_tightm[4]=calcSoverRootB(tau_file, (channel+"_incvtetm").c_str(),true).first;

 ztt_sig_err_loosem[0]=calcSoverRootB(tau_file, (channel+"_incvlelm").c_str(),true).second;
  ztt_sig_err_loosem[1]=calcSoverRootB(tau_file, (channel+"_inclelm").c_str(),true).second;
  ztt_sig_err_loosem[2]=calcSoverRootB(tau_file, (channel+"_incmelm").c_str(),true).second;
  ztt_sig_err_loosem[3]=calcSoverRootB(tau_file, (channel+"_inctelm").c_str(),true).second;
  ztt_sig_err_loosem[4]=calcSoverRootB(tau_file, (channel+"_incvtelm").c_str(),true).second;
 
  ztt_sig_err_tightm[0]=calcSoverRootB(tau_file, (channel+"_incvletm").c_str(),true).second;
  ztt_sig_err_tightm[1]=calcSoverRootB(tau_file, (channel+"_incletm").c_str(),true).second;
  ztt_sig_err_tightm[2]=calcSoverRootB(tau_file, (channel+"_incmetm").c_str(),true).second;
  ztt_sig_err_tightm[3]=calcSoverRootB(tau_file, (channel+"_inctetm").c_str(),true).second;
  ztt_sig_err_tightm[4]=calcSoverRootB(tau_file, (channel+"_incvtetm").c_str(),true).second;
} else {

if(channel!="em"){
  ztt_sig_db04[0] = calcSoverRootB(file, (channel+"_db04iso0p07").c_str(),true).first;
  ztt_sig_db04[1] = calcSoverRootB(file, (channel+"_db04iso0p08").c_str(),true).first;
  ztt_sig_db04[2] = calcSoverRootB(file, (channel+"_db04iso0p09").c_str(),true).first;
  ztt_sig_db04[3] = calcSoverRootB(file, (channel+"_db04iso0p1").c_str(),true).first;
  ztt_sig_db04[4] = calcSoverRootB(file, (channel+"_db04iso0p11").c_str(),true).first;
  ztt_sig_db04[5] = calcSoverRootB(file, (channel+"_db04iso0p12").c_str(),true).first;
  ztt_sig_db04[6] = calcSoverRootB(file, (channel+"_db04iso0p13").c_str(),true).first;
  ztt_sig_db04[7] = calcSoverRootB(file, (channel+"_db04iso0p14").c_str(),true).first;
  ztt_sig_db04[8] = calcSoverRootB(file, (channel+"_db04iso0p15").c_str(),true).first;
  ztt_sig_db04[9] = calcSoverRootB(file, (channel+"_db04iso0p16").c_str(),true).first;
  ztt_sig_db04[10] = calcSoverRootB(file, (channel+"_db04iso0p17").c_str(),true).first;
  ztt_sig_db04[11] = calcSoverRootB(file, (channel+"_db04iso0p18").c_str(),true).first;

  ztt_sig_err_db04[0] = calcSoverRootB(file, (channel+"_db04iso0p07").c_str(),true).second;
  ztt_sig_err_db04[1] = calcSoverRootB(file, (channel+"_db04iso0p08").c_str(),true).second;
  ztt_sig_err_db04[2] = calcSoverRootB(file, (channel+"_db04iso0p09").c_str(),true).second;
  ztt_sig_err_db04[3] = calcSoverRootB(file, (channel+"_db04iso0p1").c_str(),true).second;
  ztt_sig_err_db04[4] = calcSoverRootB(file, (channel+"_db04iso0p11").c_str(),true).second;
  ztt_sig_err_db04[5] = calcSoverRootB(file, (channel+"_db04iso0p12").c_str(),true).second;
  ztt_sig_err_db04[6] = calcSoverRootB(file, (channel+"_db04iso0p13").c_str(),true).second;
  ztt_sig_err_db04[7] = calcSoverRootB(file, (channel+"_db04iso0p14").c_str(),true).second;
  ztt_sig_err_db04[8] = calcSoverRootB(file, (channel+"_db04iso0p15").c_str(),true).second;
  ztt_sig_err_db04[9] = calcSoverRootB(file, (channel+"_db04iso0p16").c_str(),true).second;
  ztt_sig_err_db04[10] = calcSoverRootB(file, (channel+"_db04iso0p17").c_str(),true).second;
  ztt_sig_err_db04[11] = calcSoverRootB(file, (channel+"_db04iso0p18").c_str(),true).second;


 if(channel=="et"){ 
    ztt_sig_ea03[0] = calcSoverRootB(file, (channel+"_ea03iso0p07").c_str(),true).first;
    ztt_sig_ea03[1] = calcSoverRootB(file, (channel+"_ea03iso0p08").c_str(),true).first;
    ztt_sig_ea03[2] = calcSoverRootB(file, (channel+"_ea03iso0p09").c_str(),true).first;
    ztt_sig_ea03[3] = calcSoverRootB(file, (channel+"_ea03iso0p1").c_str(),true).first;
    ztt_sig_ea03[4] = calcSoverRootB(file, (channel+"_ea03iso0p11").c_str(),true).first;
    ztt_sig_ea03[5] = calcSoverRootB(file, (channel+"_ea03iso0p12").c_str(),true).first;
    ztt_sig_ea03[6] = calcSoverRootB(file, (channel+"_ea03iso0p13").c_str(),true).first;
    ztt_sig_ea03[7] = calcSoverRootB(file, (channel+"_ea03iso0p14").c_str(),true).first;
    ztt_sig_ea03[8] = calcSoverRootB(file, (channel+"_ea03iso0p15").c_str(),true).first;
    ztt_sig_ea03[9] = calcSoverRootB(file, (channel+"_ea03iso0p16").c_str(),true).first;
    ztt_sig_ea03[10] = calcSoverRootB(file, (channel+"_ea03iso0p17").c_str(),true).first;
    ztt_sig_ea03[11] = calcSoverRootB(file, (channel+"_ea03iso0p18").c_str(),true).first;

   ztt_sig_err_ea03[0] = calcSoverRootB(file, (channel+"_ea03iso0p07").c_str(),true).second;
   ztt_sig_err_ea03[1] = calcSoverRootB(file, (channel+"_ea03iso0p08").c_str(),true).second;
   ztt_sig_err_ea03[2] = calcSoverRootB(file, (channel+"_ea03iso0p09").c_str(),true).second;
   ztt_sig_err_ea03[3] = calcSoverRootB(file, (channel+"_ea03iso0p1").c_str(),true).second;
   ztt_sig_err_ea03[4] = calcSoverRootB(file, (channel+"_ea03iso0p11").c_str(),true).second;
   ztt_sig_err_ea03[5] = calcSoverRootB(file, (channel+"_ea03iso0p12").c_str(),true).second;
   ztt_sig_err_ea03[6] = calcSoverRootB(file, (channel+"_ea03iso0p13").c_str(),true).second;
   ztt_sig_err_ea03[7] = calcSoverRootB(file, (channel+"_ea03iso0p14").c_str(),true).second;
   ztt_sig_err_ea03[8] = calcSoverRootB(file, (channel+"_ea03iso0p15").c_str(),true).second;
   ztt_sig_err_ea03[9] = calcSoverRootB(file, (channel+"_ea03iso0p16").c_str(),true).second;
   ztt_sig_err_ea03[10] = calcSoverRootB(file, (channel+"_ea03iso0p17").c_str(),true).second;
   ztt_sig_err_ea03[11] = calcSoverRootB(file, (channel+"_ea03iso0p18").c_str(),true).second;
 } else if(channel=="mt"){ 
    ztt_sig_ea03[0] = calcSoverRootB(file, (channel+"_trk03iso0p07").c_str(),true).first;
    ztt_sig_ea03[1] = calcSoverRootB(file, (channel+"_trk03iso0p08").c_str(),true).first;
    ztt_sig_ea03[2] = calcSoverRootB(file, (channel+"_trk03iso0p09").c_str(),true).first;
    ztt_sig_ea03[3] = calcSoverRootB(file, (channel+"_trk03iso0p1").c_str(),true).first;
    ztt_sig_ea03[4] = calcSoverRootB(file, (channel+"_trk03iso0p11").c_str(),true).first;
    ztt_sig_ea03[5] = calcSoverRootB(file, (channel+"_trk03iso0p12").c_str(),true).first;
    ztt_sig_ea03[6] = calcSoverRootB(file, (channel+"_trk03iso0p13").c_str(),true).first;
    ztt_sig_ea03[7] = calcSoverRootB(file, (channel+"_trk03iso0p14").c_str(),true).first;
    ztt_sig_ea03[8] = calcSoverRootB(file, (channel+"_trk03iso0p15").c_str(),true).first;
    ztt_sig_ea03[9] = calcSoverRootB(file, (channel+"_trk03iso0p16").c_str(),true).first;
    ztt_sig_ea03[10] = calcSoverRootB(file, (channel+"_trk03iso0p17").c_str(),true).first;
    ztt_sig_ea03[11] = calcSoverRootB(file, (channel+"_trk03iso0p18").c_str(),true).first;

   ztt_sig_err_ea03[0] = calcSoverRootB(file, (channel+"_trk03iso0p07").c_str(),true).second;
   ztt_sig_err_ea03[1] = calcSoverRootB(file, (channel+"_trk03iso0p08").c_str(),true).second;
   ztt_sig_err_ea03[2] = calcSoverRootB(file, (channel+"_trk03iso0p09").c_str(),true).second;
   ztt_sig_err_ea03[3] = calcSoverRootB(file, (channel+"_trk03iso0p1").c_str(),true).second;
   ztt_sig_err_ea03[4] = calcSoverRootB(file, (channel+"_trk03iso0p11").c_str(),true).second;
   ztt_sig_err_ea03[5] = calcSoverRootB(file, (channel+"_trk03iso0p12").c_str(),true).second;
   ztt_sig_err_ea03[6] = calcSoverRootB(file, (channel+"_trk03iso0p13").c_str(),true).second;
   ztt_sig_err_ea03[7] = calcSoverRootB(file, (channel+"_trk03iso0p14").c_str(),true).second;
   ztt_sig_err_ea03[8] = calcSoverRootB(file, (channel+"_trk03iso0p15").c_str(),true).second;
   ztt_sig_err_ea03[9] = calcSoverRootB(file, (channel+"_trk03iso0p16").c_str(),true).second;
   ztt_sig_err_ea03[10] = calcSoverRootB(file, (channel+"_trk03iso0p17").c_str(),true).second;
   ztt_sig_err_ea03[11] = calcSoverRootB(file, (channel+"_trk03iso0p18").c_str(),true).second;
}


  ztt_sig_db04allch[0] = calcSoverRootB(file, (channel+"_db04allchiso0p07").c_str(),true).first;
  ztt_sig_db04allch[1] = calcSoverRootB(file, (channel+"_db04allchiso0p08").c_str(),true).first;
  ztt_sig_db04allch[2] = calcSoverRootB(file, (channel+"_db04allchiso0p09").c_str(),true).first;
  ztt_sig_db04allch[3] = calcSoverRootB(file, (channel+"_db04allchiso0p1").c_str(),true).first;
  ztt_sig_db04allch[4] = calcSoverRootB(file, (channel+"_db04allchiso0p11").c_str(),true).first;
  ztt_sig_db04allch[5] = calcSoverRootB(file, (channel+"_db04allchiso0p12").c_str(),true).first;
  ztt_sig_db04allch[6] = calcSoverRootB(file, (channel+"_db04allchiso0p13").c_str(),true).first;
  ztt_sig_db04allch[7] = calcSoverRootB(file, (channel+"_db04allchiso0p14").c_str(),true).first;
  ztt_sig_db04allch[8] = calcSoverRootB(file, (channel+"_db04allchiso0p15").c_str(),true).first;
  ztt_sig_db04allch[9] = calcSoverRootB(file, (channel+"_db04allchiso0p16").c_str(),true).first;
  ztt_sig_db04allch[10] = calcSoverRootB(file, (channel+"_db04allchiso0p17").c_str(),true).first;
  ztt_sig_db04allch[11] = calcSoverRootB(file, (channel+"_db04allchiso0p18").c_str(),true).first;

  ztt_sig_err_db04allch[0] = calcSoverRootB(file, (channel+"_db04allchiso0p07").c_str(),true).second;
  ztt_sig_err_db04allch[1] = calcSoverRootB(file, (channel+"_db04allchiso0p08").c_str(),true).second;
  ztt_sig_err_db04allch[2] = calcSoverRootB(file, (channel+"_db04allchiso0p09").c_str(),true).second;
  ztt_sig_err_db04allch[3] = calcSoverRootB(file, (channel+"_db04allchiso0p1").c_str(),true).second;
  ztt_sig_err_db04allch[4] = calcSoverRootB(file, (channel+"_db04allchiso0p11").c_str(),true).second;
  ztt_sig_err_db04allch[5] = calcSoverRootB(file, (channel+"_db04allchiso0p12").c_str(),true).second;
  ztt_sig_err_db04allch[6] = calcSoverRootB(file, (channel+"_db04allchiso0p13").c_str(),true).second;
  ztt_sig_err_db04allch[7] = calcSoverRootB(file, (channel+"_db04allchiso0p14").c_str(),true).second;
  ztt_sig_err_db04allch[8] = calcSoverRootB(file, (channel+"_db04allchiso0p15").c_str(),true).second;
  ztt_sig_err_db04allch[9] = calcSoverRootB(file, (channel+"_db04allchiso0p16").c_str(),true).second;
  ztt_sig_err_db04allch[10] = calcSoverRootB(file, (channel+"_db04allchiso0p17").c_str(),true).second;
  ztt_sig_err_db04allch[11] = calcSoverRootB(file, (channel+"_db04allchiso0p18").c_str(),true).second;



  ztt_sig_db03allch[0] = calcSoverRootB(file, (channel+"_db03allchiso0p07").c_str(),true).first;
  ztt_sig_db03allch[1] = calcSoverRootB(file, (channel+"_db03allchiso0p08").c_str(),true).first;
  ztt_sig_db03allch[2] = calcSoverRootB(file, (channel+"_db03allchiso0p09").c_str(),true).first;
  ztt_sig_db03allch[3] = calcSoverRootB(file, (channel+"_db03allchiso0p1").c_str(),true).first;
  ztt_sig_db03allch[4] = calcSoverRootB(file, (channel+"_db03allchiso0p11").c_str(),true).first;
  ztt_sig_db03allch[5] = calcSoverRootB(file, (channel+"_db03allchiso0p12").c_str(),true).first;
  ztt_sig_db03allch[6] = calcSoverRootB(file, (channel+"_db03allchiso0p13").c_str(),true).first;
  ztt_sig_db03allch[7] = calcSoverRootB(file, (channel+"_db03allchiso0p14").c_str(),true).first;
  ztt_sig_db03allch[8] = calcSoverRootB(file, (channel+"_db03allchiso0p15").c_str(),true).first;
  ztt_sig_db03allch[9] = calcSoverRootB(file, (channel+"_db03allchiso0p16").c_str(),true).first;
  ztt_sig_db03allch[10] = calcSoverRootB(file, (channel+"_db03allchiso0p17").c_str(),true).first;
  ztt_sig_db03allch[11] = calcSoverRootB(file, (channel+"_db03allchiso0p13").c_str(),true).first;

  ztt_sig_err_db03allch[0] = calcSoverRootB(file, (channel+"_db03allchiso0p07").c_str(),true).second;
  ztt_sig_err_db03allch[1] = calcSoverRootB(file, (channel+"_db03allchiso0p08").c_str(),true).second;
  ztt_sig_err_db03allch[2] = calcSoverRootB(file, (channel+"_db03allchiso0p09").c_str(),true).second;
  ztt_sig_err_db03allch[3] = calcSoverRootB(file, (channel+"_db03allchiso0p1").c_str(),true).second;
  ztt_sig_err_db03allch[4] = calcSoverRootB(file, (channel+"_db03allchiso0p11").c_str(),true).second;
  ztt_sig_err_db03allch[5] = calcSoverRootB(file, (channel+"_db03allchiso0p12").c_str(),true).second;
  ztt_sig_err_db03allch[6] = calcSoverRootB(file, (channel+"_db03allchiso0p13").c_str(),true).second;
  ztt_sig_err_db03allch[7] = calcSoverRootB(file, (channel+"_db03allchiso0p14").c_str(),true).second;
  ztt_sig_err_db03allch[8] = calcSoverRootB(file, (channel+"_db03allchiso0p15").c_str(),true).second;
  ztt_sig_err_db03allch[9] = calcSoverRootB(file, (channel+"_db03allchiso0p16").c_str(),true).second;
  ztt_sig_err_db03allch[10] = calcSoverRootB(file, (channel+"_db03allchiso0p17").c_str(),true).second;
  ztt_sig_err_db03allch[11] = calcSoverRootB(file, (channel+"_db03allchiso0p13").c_str(),true).second;



  ztt_sig_db03[0] = calcSoverRootB(file, (channel+"_db03iso0p07").c_str(),true).first;
  ztt_sig_db03[1] = calcSoverRootB(file, (channel+"_db03iso0p08").c_str(),true).first;
  ztt_sig_db03[2] = calcSoverRootB(file, (channel+"_db03iso0p09").c_str(),true).first;
  ztt_sig_db03[3] = calcSoverRootB(file, (channel+"_db03iso0p1").c_str(),true).first;
  ztt_sig_db03[4] = calcSoverRootB(file, (channel+"_db03iso0p11").c_str(),true).first;
  ztt_sig_db03[5] = calcSoverRootB(file, (channel+"_db03iso0p12").c_str(),true).first;
  ztt_sig_db03[6] = calcSoverRootB(file, (channel+"_db03iso0p13").c_str(),true).first;
  ztt_sig_db03[7] = calcSoverRootB(file, (channel+"_db03iso0p14").c_str(),true).first;
  ztt_sig_db03[8] = calcSoverRootB(file, (channel+"_db03iso0p15").c_str(),true).first;
  ztt_sig_db03[9] = calcSoverRootB(file, (channel+"_db03iso0p16").c_str(),true).first;
  ztt_sig_db03[10] = calcSoverRootB(file, (channel+"_db03iso0p17").c_str(),true).first;
  ztt_sig_db03[11] = calcSoverRootB(file, (channel+"_db03iso0p18").c_str(),true).first;

ztt_sig_err_db03[0] = calcSoverRootB(file, (channel+"_db03iso0p07").c_str(),true).second;
  ztt_sig_err_db03[1] = calcSoverRootB(file, (channel+"_db03iso0p08").c_str(),true).second;
  ztt_sig_err_db03[2] = calcSoverRootB(file, (channel+"_db03iso0p09").c_str(),true).second;
  ztt_sig_err_db03[3] = calcSoverRootB(file, (channel+"_db03iso0p1").c_str(),true).second;
  ztt_sig_err_db03[4] = calcSoverRootB(file, (channel+"_db03iso0p11").c_str(),true).second;
  ztt_sig_err_db03[5] = calcSoverRootB(file, (channel+"_db03iso0p12").c_str(),true).second;
  ztt_sig_err_db03[6] = calcSoverRootB(file, (channel+"_db03iso0p13").c_str(),true).second;
  ztt_sig_err_db03[7] = calcSoverRootB(file, (channel+"_db03iso0p14").c_str(),true).second;
  ztt_sig_err_db03[8] = calcSoverRootB(file, (channel+"_db03iso0p15").c_str(),true).second;
  ztt_sig_err_db03[9] = calcSoverRootB(file, (channel+"_db03iso0p16").c_str(),true).second;
  ztt_sig_err_db03[10] = calcSoverRootB(file, (channel+"_db03iso0p17").c_str(),true).second;
  ztt_sig_err_db03[11] = calcSoverRootB(file, (channel+"_db03iso0p18").c_str(),true).second;
 } else if (channel=="em"){

  ztt_sig_db04[0] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p1").c_str(),true).first;
  ztt_sig_db04[1] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p11").c_str(),true).first;
  ztt_sig_db04[2] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p12").c_str(),true).first;
  ztt_sig_db04[3] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p13").c_str(),true).first;
  ztt_sig_db04[4] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p14").c_str(),true).first;
  ztt_sig_db04[5] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p15").c_str(),true).first;
  ztt_sig_db04[6] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p16").c_str(),true).first;
  ztt_sig_db04[7] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p17").c_str(),true).first;
  ztt_sig_db04[8] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p18").c_str(),true).first;
  ztt_sig_db04[9] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p19").c_str(),true).first;
  ztt_sig_db04[10] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p20").c_str(),true).first;
  ztt_sig_db04[11] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p21").c_str(),true).first;

  ztt_sig_err_db04[0] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p1").c_str(),true).second;
  ztt_sig_err_db04[1] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p11").c_str(),true).second;
  ztt_sig_err_db04[2] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p12").c_str(),true).second;
  ztt_sig_err_db04[3] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p13").c_str(),true).second;
  ztt_sig_err_db04[4] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p14").c_str(),true).second;
  ztt_sig_err_db04[5] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p15").c_str(),true).second;
  ztt_sig_err_db04[6] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p16").c_str(),true).second;
  ztt_sig_err_db04[7] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p17").c_str(),true).second;
  ztt_sig_err_db04[8] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p18").c_str(),true).second;
  ztt_sig_err_db04[9] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p19").c_str(),true).second;
  ztt_sig_err_db04[10] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p20").c_str(),true).second;
  ztt_sig_err_db04[11] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04iso0p21").c_str(),true).second;


if(e_or_m=="e"){
  ztt_sig_ea03[0] = calcSoverRootB(file, (channel+"_eea03iso0p1").c_str(),true).first;
  ztt_sig_ea03[1] = calcSoverRootB(file, (channel+"_eea03iso0p11").c_str(),true).first;
  ztt_sig_ea03[2] = calcSoverRootB(file, (channel+"_eea03iso0p12").c_str(),true).first;
  ztt_sig_ea03[3] = calcSoverRootB(file, (channel+"_eea03iso0p13").c_str(),true).first;
  ztt_sig_ea03[4] = calcSoverRootB(file, (channel+"_eea03iso0p14").c_str(),true).first;
  ztt_sig_ea03[5] = calcSoverRootB(file, (channel+"_eea03iso0p15").c_str(),true).first;
  ztt_sig_ea03[6] = calcSoverRootB(file, (channel+"_eea03iso0p16").c_str(),true).first;
  ztt_sig_ea03[7] = calcSoverRootB(file, (channel+"_eea03iso0p17").c_str(),true).first;
  ztt_sig_ea03[8] = calcSoverRootB(file, (channel+"_eea03iso0p18").c_str(),true).first;
  ztt_sig_ea03[9] = calcSoverRootB(file, (channel+"_eea03iso0p19").c_str(),true).first;
  ztt_sig_ea03[10] = calcSoverRootB(file, (channel+"_eea03iso0p20").c_str(),true).first;
  ztt_sig_ea03[11] = calcSoverRootB(file, (channel+"_eea03iso0p21").c_str(),true).first;

ztt_sig_err_ea03[0] = calcSoverRootB(file, (channel+"_eea03iso0p1").c_str(),true).second;
ztt_sig_err_ea03[1] = calcSoverRootB(file, (channel+"_eea03iso0p11").c_str(),true).second;
ztt_sig_err_ea03[2] = calcSoverRootB(file, (channel+"_eea03iso0p12").c_str(),true).second;
  ztt_sig_err_ea03[3] = calcSoverRootB(file, (channel+"_eea03iso0p13").c_str(),true).second;
  ztt_sig_err_ea03[4] = calcSoverRootB(file, (channel+"_eea03iso0p14").c_str(),true).second;
  ztt_sig_err_ea03[5] = calcSoverRootB(file, (channel+"_eea03iso0p15").c_str(),true).second;
  ztt_sig_err_ea03[6] = calcSoverRootB(file, (channel+"_eea03iso0p16").c_str(),true).second;
  ztt_sig_err_ea03[7] = calcSoverRootB(file, (channel+"_eea03iso0p17").c_str(),true).second;
  ztt_sig_err_ea03[8] = calcSoverRootB(file, (channel+"_eea03iso0p18").c_str(),true).second;
  ztt_sig_err_ea03[9] = calcSoverRootB(file, (channel+"_eea03iso0p19").c_str(),true).second;
  ztt_sig_err_ea03[10] = calcSoverRootB(file, (channel+"_eea03iso0p20").c_str(),true).second;
  ztt_sig_err_ea03[11] = calcSoverRootB(file, (channel+"_eea03iso0p21").c_str(),true).second;
  } else if(e_or_m=="m"){
  ztt_sig_ea03[0] = calcSoverRootB(file, (channel+"_mtrk03iso0p1").c_str(),true).first;
  ztt_sig_ea03[1] = calcSoverRootB(file, (channel+"_mtrk03iso0p11").c_str(),true).first;
  ztt_sig_ea03[2] = calcSoverRootB(file, (channel+"_mtrk03iso0p12").c_str(),true).first;
  ztt_sig_ea03[3] = calcSoverRootB(file, (channel+"_mtrk03iso0p13").c_str(),true).first;
  ztt_sig_ea03[4] = calcSoverRootB(file, (channel+"_mtrk03iso0p14").c_str(),true).first;
  ztt_sig_ea03[5] = calcSoverRootB(file, (channel+"_mtrk03iso0p15").c_str(),true).first;
  ztt_sig_ea03[6] = calcSoverRootB(file, (channel+"_mtrk03iso0p16").c_str(),true).first;
  ztt_sig_ea03[7] = calcSoverRootB(file, (channel+"_mtrk03iso0p17").c_str(),true).first;
  ztt_sig_ea03[8] = calcSoverRootB(file, (channel+"_mtrk03iso0p18").c_str(),true).first;
  ztt_sig_ea03[9] = calcSoverRootB(file, (channel+"_mtrk03iso0p19").c_str(),true).first;
  ztt_sig_ea03[10] = calcSoverRootB(file, (channel+"_mtrk03iso0p20").c_str(),true).first;
  ztt_sig_ea03[11] = calcSoverRootB(file, (channel+"_mtrk03iso0p21").c_str(),true).first;

ztt_sig_err_ea03[0] = calcSoverRootB(file, (channel+"_mtrk03iso0p1").c_str(),true).second;
ztt_sig_err_ea03[1] = calcSoverRootB(file, (channel+"_mtrk03iso0p11").c_str(),true).second;
ztt_sig_err_ea03[2] = calcSoverRootB(file, (channel+"_mtrk03iso0p12").c_str(),true).second;
  ztt_sig_err_ea03[3] = calcSoverRootB(file, (channel+"_mtrk03iso0p13").c_str(),true).second;
  ztt_sig_err_ea03[4] = calcSoverRootB(file, (channel+"_mtrk03iso0p14").c_str(),true).second;
  ztt_sig_err_ea03[5] = calcSoverRootB(file, (channel+"_mtrk03iso0p15").c_str(),true).second;
  ztt_sig_err_ea03[6] = calcSoverRootB(file, (channel+"_mtrk03iso0p16").c_str(),true).second;
  ztt_sig_err_ea03[7] = calcSoverRootB(file, (channel+"_mtrk03iso0p17").c_str(),true).second;
  ztt_sig_err_ea03[8] = calcSoverRootB(file, (channel+"_mtrk03iso0p18").c_str(),true).second;
  ztt_sig_err_ea03[9] = calcSoverRootB(file, (channel+"_mtrk03iso0p19").c_str(),true).second;
  ztt_sig_err_ea03[10] = calcSoverRootB(file, (channel+"_mtrk03iso0p20").c_str(),true).second;
  ztt_sig_err_ea03[11] = calcSoverRootB(file, (channel+"_mtrk03iso0p21").c_str(),true).second;
}




  ztt_sig_db04allch[0] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p1").c_str(),true).first;
  ztt_sig_db04allch[1] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p11").c_str(),true).first;
  ztt_sig_db04allch[2] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p12").c_str(),true).first;
  ztt_sig_db04allch[3] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p13").c_str(),true).first;
  ztt_sig_db04allch[4] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p14").c_str(),true).first;
  ztt_sig_db04allch[5] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p15").c_str(),true).first;
  ztt_sig_db04allch[6] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p16").c_str(),true).first;
  ztt_sig_db04allch[7] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p17").c_str(),true).first;
  ztt_sig_db04allch[8] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p18").c_str(),true).first;
  ztt_sig_db04allch[9] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p19").c_str(),true).first;
  ztt_sig_db04allch[10] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p20").c_str(),true).first;
  ztt_sig_db04allch[11] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p21").c_str(),true).first;

  ztt_sig_err_db04allch[0] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p1").c_str(),true).second;
  ztt_sig_err_db04allch[1] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p11").c_str(),true).second;
  ztt_sig_err_db04allch[2] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p12").c_str(),true).second;
  ztt_sig_err_db04allch[3] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p13").c_str(),true).second;
  ztt_sig_err_db04allch[4] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p14").c_str(),true).second;
  ztt_sig_err_db04allch[5] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p15").c_str(),true).second;
  ztt_sig_err_db04allch[6] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p16").c_str(),true).second;
  ztt_sig_err_db04allch[7] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p17").c_str(),true).second;
  ztt_sig_err_db04allch[8] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p18").c_str(),true).second;
  ztt_sig_err_db04allch[9] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p19").c_str(),true).second;
  ztt_sig_err_db04allch[10] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p20").c_str(),true).second;
  ztt_sig_err_db04allch[11] = calcSoverRootB(file, (channel+"_"+e_or_m+"db04allchiso0p21").c_str(),true).second;



  ztt_sig_db03allch[0] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p1").c_str(),true).first;
  ztt_sig_db03allch[1] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p11").c_str(),true).first;
  ztt_sig_db03allch[2] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p12").c_str(),true).first;
  ztt_sig_db03allch[3] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p13").c_str(),true).first;
  ztt_sig_db03allch[4] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p14").c_str(),true).first;
  ztt_sig_db03allch[5] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p15").c_str(),true).first;
  ztt_sig_db03allch[6] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p16").c_str(),true).first;
  ztt_sig_db03allch[7] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p17").c_str(),true).first;
  ztt_sig_db03allch[8] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p18").c_str(),true).first;
  ztt_sig_db03allch[9] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p19").c_str(),true).first;
  ztt_sig_db03allch[10] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p20").c_str(),true).first;
  ztt_sig_db03allch[11] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p21").c_str(),true).first;

  ztt_sig_err_db03allch[0] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p1").c_str(),true).second;
  ztt_sig_err_db03allch[1] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p11").c_str(),true).second;
  ztt_sig_err_db03allch[2] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p12").c_str(),true).second;
  ztt_sig_err_db03allch[3] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p13").c_str(),true).second;
  ztt_sig_err_db03allch[4] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p14").c_str(),true).second;
  ztt_sig_err_db03allch[5] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p15").c_str(),true).second;
  ztt_sig_err_db03allch[6] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p16").c_str(),true).second;
  ztt_sig_err_db03allch[7] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p17").c_str(),true).second;
  ztt_sig_err_db03allch[8] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p18").c_str(),true).second;
  ztt_sig_err_db03allch[9] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p19").c_str(),true).second;
  ztt_sig_err_db03allch[10] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p20").c_str(),true).second;
  ztt_sig_err_db03allch[11] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03allchiso0p21").c_str(),true).second;



  ztt_sig_db03[0] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p1").c_str(),true).first;
  ztt_sig_db03[1] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p11").c_str(),true).first;
  ztt_sig_db03[2] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p12").c_str(),true).first;
  ztt_sig_db03[3] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p13").c_str(),true).first;
  ztt_sig_db03[4] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p14").c_str(),true).first;
  ztt_sig_db03[5] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p15").c_str(),true).first;
  ztt_sig_db03[6] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p16").c_str(),true).first;
  ztt_sig_db03[7] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p17").c_str(),true).first;
  ztt_sig_db03[8] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p18").c_str(),true).first;
  ztt_sig_db03[9] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p19").c_str(),true).first;
  ztt_sig_db03[10] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p20").c_str(),true).first;
  ztt_sig_db03[11] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p21").c_str(),true).first;

ztt_sig_err_db03[0] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p1").c_str(),true).second;
ztt_sig_err_db03[1] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p11").c_str(),true).second;
ztt_sig_err_db03[2] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p12").c_str(),true).second;
  ztt_sig_err_db03[3] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p13").c_str(),true).second;
  ztt_sig_err_db03[4] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p14").c_str(),true).second;
  ztt_sig_err_db03[5] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p15").c_str(),true).second;
  ztt_sig_err_db03[6] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p16").c_str(),true).second;
  ztt_sig_err_db03[7] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p17").c_str(),true).second;
  ztt_sig_err_db03[8] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p18").c_str(),true).second;
  ztt_sig_err_db03[9] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p19").c_str(),true).second;
  ztt_sig_err_db03[10] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p20").c_str(),true).second;
  ztt_sig_err_db03[11] = calcSoverRootB(file, (channel+"_"+e_or_m+"db03iso0p21").c_str(),true).second;

}

}

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

  TGraphErrors *ztt_sig_loosem_graph;
  TGraphErrors *ztt_sig_tightm_graph;
if(do_tau){
  ztt_sig_loosem_graph = new TGraphErrors(5,iso_cuts_tau,ztt_sig_loosem,x_err_tau,ztt_sig_err_loosem);
  ztt_sig_tightm_graph = new TGraphErrors(5,iso_cuts_tau,ztt_sig_tightm,x_err_tau,ztt_sig_err_tightm);

 ztt_sig_loosem_graph->SetMarkerStyle(20);
 ztt_sig_loosem_graph->SetMarkerColor(kRed);
 ztt_sig_tightm_graph->SetMarkerStyle(21);
 ztt_sig_tightm_graph->SetMarkerColor(kGreen+3);
}

/*  TGraphErrors *ztt_sig_db04_graph = new TGraphErrors(12,iso_cuts,ztt_sig_db04,x_err,ztt_sig_err_db04); 
  TGraphErrors *ztt_sig_ea03_graph = new TGraphErrors(12,iso_cuts,ztt_sig_ea03,x_err,ztt_sig_err_ea03); 
  TGraphErrors *ztt_sig_db04allch_graph = new TGraphErrors(12,iso_cuts,ztt_sig_db04allch,x_err,ztt_sig_err_db04allch); 
  TGraphErrors *ztt_sig_db03allch_graph = new TGraphErrors(12,iso_cuts,ztt_sig_db03allch,x_err,ztt_sig_err_db03allch); 
  TGraphErrors *ztt_sig_db03_graph = new TGraphErrors(12,iso_cuts,ztt_sig_db03,x_err,ztt_sig_err_db03);*/ 

  TGraphErrors *ztt_sig_db04_graph = new TGraphErrors(12,iso_cuts,ztt_sig_db04,x_err,ztt_sig_err_db04); 
  TGraphErrors *ztt_sig_ea03_graph = new TGraphErrors(12,iso_cuts,ztt_sig_ea03,x_err,ztt_sig_err_ea03); 
  TGraphErrors *ztt_sig_db04allch_graph = new TGraphErrors(12,iso_cuts,ztt_sig_db04allch,x_err,ztt_sig_err_db04allch); 
  TGraphErrors *ztt_sig_db03allch_graph = new TGraphErrors(12,iso_cuts,ztt_sig_db03allch,x_err,ztt_sig_err_db03allch); 
  TGraphErrors *ztt_sig_db03_graph = new TGraphErrors(12,iso_cuts,ztt_sig_db03,x_err,ztt_sig_err_db03); 


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
if(channel=="et"){
  ztt_sig_db04_graph->GetYaxis()->SetRangeUser(16,23);
} else if (channel=="mt"){
  ztt_sig_db04_graph->GetYaxis()->SetRangeUser(58,68);
} else {
  ztt_sig_db04_graph->GetYaxis()->SetRangeUser(55,66);
}
  ztt_sig_db04_graph->SetTitle(";Isolation cut;S/#sqrt{B}");
  
/* ztt_sig_loosem_graph->GetYaxis()->SetRangeUser(40,80);
 ztt_sig_loosem_graph->SetTitle(";AntiE discriminator;S/root(B)");*/
   
  TCanvas plot_canv("plot_canv","plot_canv",1200,800); 
  plot_canv.cd();
/*  ztt_sig_loosem_graph->Draw("AP");
  ztt_sig_tightm_graph->Draw("PSAME");*/
if(!do_tau){
  ztt_sig_db04_graph->Draw("AP"); 
  ztt_sig_ea03_graph->Draw("PSAME");
  ztt_sig_db04allch_graph->Draw("PSAME");
  ztt_sig_db03allch_graph->Draw("PSAME");
  ztt_sig_db03_graph->Draw("PSAME");

  TLegend *leg = PositionedLegend(0.3,0.2,3,0);
  leg->SetFillStyle(0);
/*  leg->AddEntry(ztt_sig_loosem_graph,"antiMuLoose","p");
  leg->AddEntry(ztt_sig_tightm_graph,"antiMuTight","p");*/
  leg->AddEntry(ztt_sig_db04_graph,"DBeta corr (cs 0.4)","p");
  if(channel=="et"||(channel=="em" && e_or_m=="e")){
    leg->AddEntry(ztt_sig_ea03_graph,"EA corrected (cs 0.3)","p");
  } else{
    leg->AddEntry(ztt_sig_ea03_graph,"Tracker iso (cs 0.3)","p");
  }

  leg->AddEntry(ztt_sig_db04allch_graph,"DBeta corr, allcharged (cs 0.4)","p");
  leg->AddEntry(ztt_sig_db03allch_graph,"DBeta corr, allcharged (cs 0.3)","p");
  leg->AddEntry(ztt_sig_db03_graph,"DBeta corr (cs 0.3)","p");


  leg->Draw("SAME");
  
 // plot_canv.SetGrid(1,1);

  plot_canv.SaveAs(("s_over_root_b_"+channel+"_"+e_or_m+".pdf").c_str());
 } else{
  hempty.Draw();
  ztt_sig_loosem_graph->Draw("PSAME"); 
  ztt_sig_tightm_graph->Draw("PSAME");

  TLegend *leg = PositionedLegend(0.3,0.2,3,0);
  leg->SetFillStyle(0);
  leg->AddEntry(ztt_sig_loosem_graph,"antiMuLoose","p");
  leg->AddEntry(ztt_sig_tightm_graph,"antiMuTight","p");

  leg->Draw("SAME");
  
 // plot_canv.SetGrid(1,1);

  plot_canv.SaveAs(("s_over_root_b_"+channel+"_tau.pdf").c_str());
}
 return 0;
}
