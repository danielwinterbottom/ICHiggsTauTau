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

std::pair<double,double> calcSoverRootB(std::string const& filepath, std::string const& directory, bool use_ztt_sig,std::string signalsample, std::string mass){
  TH1F ztt = GetFromTFile<TH1F>(filepath, directory, "ZTT");
  TH1F vv = GetFromTFile<TH1F>(filepath, directory, "VV"); 
  TH1F zll = GetFromTFile<TH1F>(filepath, directory, "ZLL");
  TH1F tt = GetFromTFile<TH1F>(filepath, directory, "TT");
  TH1F qcd = GetFromTFile<TH1F>(filepath, directory, "QCD");
  TH1F w = GetFromTFile<TH1F>(filepath, directory, "W");
  TH1F sigsample  = GetFromTFile<TH1F>(filepath, directory, (signalsample+mass).c_str());
 
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
  sigsample.IntegralAndError(0,sigsample.GetNbinsX()+1,sig_error);
  signal = sigsample.Integral();
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

 int main(int argc, char* argv[]){
  using namespace ic;
  using namespace std;
  if(argc <= 5){
    std::cout<<"Usage [CHANNEL] [ExtraDCLabel] [MASSPOINT] [SIGNALSAMPLE] [CATEGORY]"<<std::endl;
    exit(1);
  }
  std::string channel = argv[1];//"mt";
  std::string extralabel = argv[2];//"MTOpt";
  std::string massp = argv[3];//"2900";
  std::string signalsample = argv[4];//"ggH";
  std::string category = argv[5];//"+category+"
  bool use_ztt = false;

 
 std::string file = ("/vols/cms04/amd12/CMSSW_7_4_7/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/htt_"+channel+".inputs-mssm-13TeV-mvis-"+extralabel+".root").c_str();
 
 double sig_pfmt[14];
 double sig_mvamt[14];
 double sig_err_pfmt[14];
 double sig_err_mvamt[14];


double x_err[14]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
double mt_cuts[14]={10.,15.,20.,25.,30.,35.,40.,45.,50.,60.,70.,80.,90.,100.};

  sig_pfmt[0] = calcSoverRootB(file, (channel+"_"+category+"_pfmt10").c_str(),use_ztt,signalsample,massp).first;
  sig_pfmt[1] = calcSoverRootB(file, (channel+"_"+category+"_pfmt15").c_str(),use_ztt,signalsample,massp).first;
  sig_pfmt[2] = calcSoverRootB(file, (channel+"_"+category+"_pfmt20").c_str(),use_ztt,signalsample,massp).first;
  sig_pfmt[3] = calcSoverRootB(file, (channel+"_"+category+"_pfmt25").c_str(),use_ztt,signalsample,massp).first;
  sig_pfmt[4] = calcSoverRootB(file, (channel+"_"+category+"_pfmt30").c_str(),use_ztt,signalsample,massp).first;
  sig_pfmt[5] = calcSoverRootB(file, (channel+"_"+category+"_pfmt35").c_str(),use_ztt,signalsample,massp).first;
  sig_pfmt[6] = calcSoverRootB(file, (channel+"_"+category+"_pfmt40").c_str(),use_ztt,signalsample,massp).first;
  sig_pfmt[7] = calcSoverRootB(file, (channel+"_"+category+"_pfmt45").c_str(),use_ztt,signalsample,massp).first;
  sig_pfmt[8] = calcSoverRootB(file, (channel+"_"+category+"_pfmt50").c_str(),use_ztt,signalsample,massp).first;
  sig_pfmt[9] = calcSoverRootB(file, (channel+"_"+category+"_pfmt60").c_str(),use_ztt,signalsample,massp).first;
  sig_pfmt[10] = calcSoverRootB(file, (channel+"_"+category+"_pfmt70").c_str(),use_ztt,signalsample,massp).first;
  sig_pfmt[11] = calcSoverRootB(file, (channel+"_"+category+"_pfmt80").c_str(),use_ztt,signalsample,massp).first;
  sig_pfmt[12] = calcSoverRootB(file, (channel+"_"+category+"_pfmt90").c_str(),use_ztt,signalsample,massp).first;
  sig_pfmt[13] = calcSoverRootB(file, (channel+"_"+category+"_pfmt100").c_str(),use_ztt,signalsample,massp).first;

  sig_err_pfmt[0] = calcSoverRootB(file, (channel+"_"+category+"_pfmt10").c_str(),use_ztt,signalsample,massp).second;
  sig_err_pfmt[1] = calcSoverRootB(file, (channel+"_"+category+"_pfmt15").c_str(),use_ztt,signalsample,massp).second;
  sig_err_pfmt[2] = calcSoverRootB(file, (channel+"_"+category+"_pfmt20").c_str(),use_ztt,signalsample,massp).second;
  sig_err_pfmt[3] = calcSoverRootB(file, (channel+"_"+category+"_pfmt25").c_str(),use_ztt,signalsample,massp).second;
  sig_err_pfmt[4] = calcSoverRootB(file, (channel+"_"+category+"_pfmt30").c_str(),use_ztt,signalsample,massp).second;
  sig_err_pfmt[5] = calcSoverRootB(file, (channel+"_"+category+"_pfmt35").c_str(),use_ztt,signalsample,massp).second;
  sig_err_pfmt[6] = calcSoverRootB(file, (channel+"_"+category+"_pfmt40").c_str(),use_ztt,signalsample,massp).second;
  sig_err_pfmt[7] = calcSoverRootB(file, (channel+"_"+category+"_pfmt45").c_str(),use_ztt,signalsample,massp).second;
  sig_err_pfmt[8] = calcSoverRootB(file, (channel+"_"+category+"_pfmt50").c_str(),use_ztt,signalsample,massp).second;
  sig_err_pfmt[9] = calcSoverRootB(file, (channel+"_"+category+"_pfmt60").c_str(),use_ztt,signalsample,massp).second;
  sig_err_pfmt[10] = calcSoverRootB(file, (channel+"_"+category+"_pfmt70").c_str(),use_ztt,signalsample,massp).second;
  sig_err_pfmt[11] = calcSoverRootB(file, (channel+"_"+category+"_pfmt80").c_str(),use_ztt,signalsample,massp).second;
  sig_err_pfmt[12] = calcSoverRootB(file, (channel+"_"+category+"_pfmt90").c_str(),use_ztt,signalsample,massp).second;
  sig_err_pfmt[13] = calcSoverRootB(file, (channel+"_"+category+"_pfmt100").c_str(),use_ztt,signalsample,massp).second;

  sig_mvamt[0] = calcSoverRootB(file, (channel+"_"+category+"_mt10").c_str(),use_ztt,signalsample,massp).first;
  sig_mvamt[1] = calcSoverRootB(file, (channel+"_"+category+"_mt15").c_str(),use_ztt,signalsample,massp).first;
  sig_mvamt[2] = calcSoverRootB(file, (channel+"_"+category+"_mt20").c_str(),use_ztt,signalsample,massp).first;
  sig_mvamt[3] = calcSoverRootB(file, (channel+"_"+category+"_mt25").c_str(),use_ztt,signalsample,massp).first;
  sig_mvamt[4] = calcSoverRootB(file, (channel+"_"+category+"_mt30").c_str(),use_ztt,signalsample,massp).first;
  sig_mvamt[5] = calcSoverRootB(file, (channel+"_"+category+"_mt35").c_str(),use_ztt,signalsample,massp).first;
  sig_mvamt[6] = calcSoverRootB(file, (channel+"_"+category+"_mt40").c_str(),use_ztt,signalsample,massp).first;
  sig_mvamt[7] = calcSoverRootB(file, (channel+"_"+category+"_mt45").c_str(),use_ztt,signalsample,massp).first;
  sig_mvamt[8] = calcSoverRootB(file, (channel+"_"+category+"_mt50").c_str(),use_ztt,signalsample,massp).first;
  sig_mvamt[9] = calcSoverRootB(file, (channel+"_"+category+"_mt60").c_str(),use_ztt,signalsample,massp).first;
  sig_mvamt[10] = calcSoverRootB(file, (channel+"_"+category+"_mt70").c_str(),use_ztt,signalsample,massp).first;
  sig_mvamt[11] = calcSoverRootB(file, (channel+"_"+category+"_mt80").c_str(),use_ztt,signalsample,massp).first;
  sig_mvamt[12] = calcSoverRootB(file, (channel+"_"+category+"_mt90").c_str(),use_ztt,signalsample,massp).first;
  sig_mvamt[13] = calcSoverRootB(file, (channel+"_"+category+"_mt100").c_str(),use_ztt,signalsample,massp).first;

  sig_err_mvamt[0] = calcSoverRootB(file, (channel+"_"+category+"_mt10").c_str(),use_ztt,signalsample,massp).second;
  sig_err_mvamt[1] = calcSoverRootB(file, (channel+"_"+category+"_mt15").c_str(),use_ztt,signalsample,massp).second;
  sig_err_mvamt[2] = calcSoverRootB(file, (channel+"_"+category+"_mt20").c_str(),use_ztt,signalsample,massp).second;
  sig_err_mvamt[3] = calcSoverRootB(file, (channel+"_"+category+"_mt25").c_str(),use_ztt,signalsample,massp).second;
  sig_err_mvamt[4] = calcSoverRootB(file, (channel+"_"+category+"_mt30").c_str(),use_ztt,signalsample,massp).second;
  sig_err_mvamt[5] = calcSoverRootB(file, (channel+"_"+category+"_mt35").c_str(),use_ztt,signalsample,massp).second;
  sig_err_mvamt[6] = calcSoverRootB(file, (channel+"_"+category+"_mt40").c_str(),use_ztt,signalsample,massp).second;
  sig_err_mvamt[7] = calcSoverRootB(file, (channel+"_"+category+"_mt45").c_str(),use_ztt,signalsample,massp).second;
  sig_err_mvamt[8] = calcSoverRootB(file, (channel+"_"+category+"_mt50").c_str(),use_ztt,signalsample,massp).second;
  sig_err_mvamt[9] = calcSoverRootB(file, (channel+"_"+category+"_mt60").c_str(),use_ztt,signalsample,massp).second;
  sig_err_mvamt[10] = calcSoverRootB(file, (channel+"_"+category+"_mt70").c_str(),use_ztt,signalsample,massp).second;
  sig_err_mvamt[11] = calcSoverRootB(file, (channel+"_"+category+"_mt80").c_str(),use_ztt,signalsample,massp).second;
  sig_err_mvamt[12] = calcSoverRootB(file, (channel+"_"+category+"_mt90").c_str(),use_ztt,signalsample,massp).second;
  sig_err_mvamt[13] = calcSoverRootB(file, (channel+"_"+category+"_mt100").c_str(),use_ztt,signalsample,massp).second;



   TGraphErrors *sig_pfmt_graph = new TGraphErrors(14,mt_cuts,sig_pfmt,x_err,sig_err_pfmt);
   TGraphErrors *sig_mvamt_graph = new TGraphErrors(14,mt_cuts,sig_mvamt,x_err,sig_err_mvamt);

 sig_pfmt_graph->SetMarkerStyle(20);
 sig_pfmt_graph->SetMarkerColor(kRed);
 sig_mvamt_graph->SetMarkerStyle(21);
 sig_mvamt_graph->SetMarkerColor(kGreen+3);

  TCanvas plot_canv("plot_canv","plot_canv",1200,800); 
  plot_canv.cd();
  sig_mvamt_graph->SetTitle(";m_{T} cut [GeV];S/#sqrt{B}");
  double maxmva = TMath::MaxElement(sig_mvamt_graph->GetN(),sig_mvamt_graph->GetY());
  double maxpf = TMath::MaxElement(sig_pfmt_graph->GetN(),sig_pfmt_graph->GetY());
  double minmva = TMath::MinElement(sig_mvamt_graph->GetN(),sig_mvamt_graph->GetY());
  double minpf = TMath::MinElement(sig_pfmt_graph->GetN(),sig_pfmt_graph->GetY());

  double max = (maxmva > maxpf) ? maxmva:maxpf;
  double min = (minmva < minpf) ? minmva:minpf;

  sig_mvamt_graph->GetYaxis()->SetRangeUser(0.5*min,1.5*max);
/*  ztt_sig_loosem_graph->Draw("AP");
  ztt_sig_tightm_graph->Draw("PSAME");*/
  sig_mvamt_graph->Draw("AP"); 
  sig_pfmt_graph->Draw("PSAME");

  TLegend *leg = PositionedLegend(0.3,0.1,3,0);
  leg->SetFillStyle(0);
  leg->AddEntry(sig_mvamt_graph,"m_{T} (mvamet)","p");
  leg->AddEntry(sig_pfmt_graph,"m_{T} (pfmet)","p");


  leg->Draw("SAME");
  
if(use_ztt){
  plot_canv.SaveAs(("s_over_root_b_"+channel+"_mtcut_ztt.pdf").c_str());
 } else{

  plot_canv.SaveAs(("s_over_root_b_"+channel+"_"+category+"_mtcut_"+signalsample+"_"+massp+".pdf").c_str());
}
 return 0;
}
