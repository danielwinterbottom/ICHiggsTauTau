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

double calcSoverRootB(std::string const& filepath, std::string const& directory, bool use_ztt_sig){
  TH1F ztt = GetFromTFile<TH1F>(filepath, directory, "ZTT");
  TH1F vv = GetFromTFile<TH1F>(filepath, directory, "VV"); 
  TH1F zll = GetFromTFile<TH1F>(filepath, directory, "ZLL");
  TH1F tt = GetFromTFile<TH1F>(filepath, directory, "TT");
  TH1F qcd = GetFromTFile<TH1F>(filepath, directory, "QCD");
  TH1F w = GetFromTFile<TH1F>(filepath, directory, "W");
  TH1F ggH125 = GetFromTFile<TH1F>(filepath, directory, "ggH125");
 
 double signal=0;
 double background=0;
 if(use_ztt_sig){
  signal = ztt.Integral();
  background = vv.Integral()+zll.Integral()+tt.Integral()+qcd.Integral()+w.Integral();
 } else {
  signal = ggH125.Integral();
  background = ztt.Integral()+vv.Integral()+zll.Integral()+tt.Integral()+qcd.Integral()+w.Integral();
 }
  return signal/TMath::Sqrt(background); 
}

 int main(int /*argc*/, char* /*argv*/[]){
  using namespace ic;
  using namespace std;

 
 std::string file = "/vols/cms04/amd12/CMSSW_7_4_7/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/htt_mt.inputs-sm-13TeV-mvis-MuonFullDS.root";
 
 double ztt_sig_db04[12];
// double sm_sig_db04[10];
 double ztt_sig_trk03[12];
 double ztt_sig_db04allch[12];
 double ztt_sig_db03allch[7];
 double ztt_sig_db03[7];
 //double sm_sig_trk03[10];
 double iso_cuts[12];
  iso_cuts[0] = 0.07;
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
  iso_cuts[11] = 0.18;
 
 double iso_cuts_short[7]={0.07,0.08,0.09,0.1,0.11,0.12,0.13};

  ztt_sig_db04[0] = calcSoverRootB(file, "mt_db04iso0p07",true);
  ztt_sig_db04[1] = calcSoverRootB(file, "mt_db04iso0p08",true);
  ztt_sig_db04[2] = calcSoverRootB(file, "mt_db04iso0p09",true);
  ztt_sig_db04[3] = calcSoverRootB(file, "mt_db04iso0p1",true);
  ztt_sig_db04[4] = calcSoverRootB(file, "mt_db04iso0p11",true);
  ztt_sig_db04[5] = calcSoverRootB(file, "mt_db04iso0p12",true);
  ztt_sig_db04[6] = calcSoverRootB(file, "mt_db04iso0p13",true);
  ztt_sig_db04[7] = calcSoverRootB(file, "mt_db04iso0p14",true);
  ztt_sig_db04[8] = calcSoverRootB(file, "mt_db04iso0p15",true);
  ztt_sig_db04[9] = calcSoverRootB(file, "mt_db04iso0p16",true);
  ztt_sig_db04[10] = calcSoverRootB(file, "mt_db04iso0p17",true);
  ztt_sig_db04[11] = calcSoverRootB(file, "mt_db04iso0p18",true);

  /*sm_sig_db04[0] = calcSoverRootB(file, "mt_db04iso0p09",false);
  sm_sig_db04[1] = calcSoverRootB(file, "mt_db04iso0p1",false);
  sm_sig_db04[2] = calcSoverRootB(file, "mt_db04iso0p11",false);
  sm_sig_db04[3] = calcSoverRootB(file, "mt_db04iso0p12",false);
  sm_sig_db04[4] = calcSoverRootB(file, "mt_db04iso0p13",false);
  sm_sig_db04[5] = calcSoverRootB(file, "mt_db04iso0p14",false);
  sm_sig_db04[6] = calcSoverRootB(file, "mt_db04iso0p15",false);
  sm_sig_db04[7] = calcSoverRootB(file, "mt_db04iso0p16",false);
  sm_sig_db04[8] = calcSoverRootB(file, "mt_db04iso0p17",false);
  sm_sig_db04[9] = calcSoverRootB(file, "mt_db04iso0p18",false);*/

 
  ztt_sig_trk03[0] = calcSoverRootB(file, "mt_trk03iso0p07",true);
  ztt_sig_trk03[1] = calcSoverRootB(file, "mt_trk03iso0p08",true);
  ztt_sig_trk03[2] = calcSoverRootB(file, "mt_trk03iso0p09",true);
  ztt_sig_trk03[3] = calcSoverRootB(file, "mt_trk03iso0p1",true);
  ztt_sig_trk03[4] = calcSoverRootB(file, "mt_trk03iso0p11",true);
  ztt_sig_trk03[5] = calcSoverRootB(file, "mt_trk03iso0p12",true);
  ztt_sig_trk03[6] = calcSoverRootB(file, "mt_trk03iso0p13",true);
  ztt_sig_trk03[7] = calcSoverRootB(file, "mt_trk03iso0p14",true);
  ztt_sig_trk03[8] = calcSoverRootB(file, "mt_trk03iso0p15",true);
  ztt_sig_trk03[9] = calcSoverRootB(file, "mt_trk03iso0p16",true);
  ztt_sig_trk03[10] = calcSoverRootB(file, "mt_trk03iso0p17",true);
  ztt_sig_trk03[11] = calcSoverRootB(file, "mt_trk03iso0p18",true);

  ztt_sig_db04allch[0] = calcSoverRootB(file, "mt_db04allchiso0p07",true);
  ztt_sig_db04allch[1] = calcSoverRootB(file, "mt_db04allchiso0p08",true);
  ztt_sig_db04allch[2] = calcSoverRootB(file, "mt_db04allchiso0p09",true);
  ztt_sig_db04allch[3] = calcSoverRootB(file, "mt_db04allchiso0p1",true);
  ztt_sig_db04allch[4] = calcSoverRootB(file, "mt_db04allchiso0p11",true);
  ztt_sig_db04allch[5] = calcSoverRootB(file, "mt_db04allchiso0p12",true);
  ztt_sig_db04allch[6] = calcSoverRootB(file, "mt_db04allchiso0p13",true);
  ztt_sig_db04allch[7] = calcSoverRootB(file, "mt_db04allchiso0p14",true);
  ztt_sig_db04allch[8] = calcSoverRootB(file, "mt_db04allchiso0p15",true);
  ztt_sig_db04allch[9] = calcSoverRootB(file, "mt_db04allchiso0p16",true);
  ztt_sig_db04allch[10] = calcSoverRootB(file, "mt_db04allchiso0p17",true);
  ztt_sig_db04allch[11] = calcSoverRootB(file, "mt_db04allchiso0p18",true);

  ztt_sig_db03allch[0] = calcSoverRootB(file, "mt_db03allchiso0p07",true);
  ztt_sig_db03allch[1] = calcSoverRootB(file, "mt_db03allchiso0p08",true);
  ztt_sig_db03allch[2] = calcSoverRootB(file, "mt_db03allchiso0p09",true);
  ztt_sig_db03allch[3] = calcSoverRootB(file, "mt_db03allchiso0p1",true);
  ztt_sig_db03allch[4] = calcSoverRootB(file, "mt_db03allchiso0p11",true);
  ztt_sig_db03allch[5] = calcSoverRootB(file, "mt_db03allchiso0p12",true);
  ztt_sig_db03allch[6] = calcSoverRootB(file, "mt_db03allchiso0p13",true);

  ztt_sig_db03[0] = calcSoverRootB(file, "mt_db03iso0p07",true);
  ztt_sig_db03[1] = calcSoverRootB(file, "mt_db03iso0p08",true);
  ztt_sig_db03[2] = calcSoverRootB(file, "mt_db03iso0p09",true);
  ztt_sig_db03[3] = calcSoverRootB(file, "mt_db03iso0p1",true);
  ztt_sig_db03[4] = calcSoverRootB(file, "mt_db03iso0p11",true);
  ztt_sig_db03[5] = calcSoverRootB(file, "mt_db03iso0p12",true);
  ztt_sig_db03[6] = calcSoverRootB(file, "mt_db03iso0p13",true);


/*  sm_sig_trk03[0] = calcSoverRootB(file, "mt_trk03iso0p09",false);
  sm_sig_trk03[1] = calcSoverRootB(file, "mt_trk03iso0p1",false);
  sm_sig_trk03[2] = calcSoverRootB(file, "mt_trk03iso0p11",false);
  sm_sig_trk03[3] = calcSoverRootB(file, "mt_trk03iso0p12",false);
  sm_sig_trk03[4] = calcSoverRootB(file, "mt_trk03iso0p13",false);
  sm_sig_trk03[5] = calcSoverRootB(file, "mt_trk03iso0p14",false);
  sm_sig_trk03[6] = calcSoverRootB(file, "mt_trk03iso0p15",false);
  sm_sig_trk03[7] = calcSoverRootB(file, "mt_trk03iso0p16",false);
  sm_sig_trk03[8] = calcSoverRootB(file, "mt_trk03iso0p17",false);
  sm_sig_trk03[9] = calcSoverRootB(file, "mt_trk03iso0p18",false);*/

  TGraph *ztt_sig_db04_graph = new TGraph(12,iso_cuts,ztt_sig_db04); 
  TGraph *ztt_sig_trk03_graph = new TGraph(12,iso_cuts,ztt_sig_trk03); 
  TGraph *ztt_sig_db04allch_graph = new TGraph(12,iso_cuts,ztt_sig_db04allch); 
  TGraph *ztt_sig_db03allch_graph = new TGraph(7,iso_cuts_short,ztt_sig_db03allch); 
  TGraph *ztt_sig_db03_graph = new TGraph(7,iso_cuts_short,ztt_sig_db03); 

/*  TGraph *sm_sig_db04_graph = new TGraph(10,iso_cuts,sm_sig_db04); 
  TGraph *sm_sig_trk03_graph = new TGraph(10,iso_cuts,sm_sig_trk03); */


  ztt_sig_db04_graph->SetMarkerStyle(20);
  ztt_sig_db04_graph->SetMarkerColor(kRed);
  ztt_sig_trk03_graph->SetMarkerStyle(21);
  ztt_sig_trk03_graph->SetMarkerColor(kGreen+3);
  ztt_sig_db04allch_graph->SetMarkerStyle(22);
  ztt_sig_db04allch_graph->SetMarkerColor(kBlue);
  ztt_sig_db03allch_graph->SetMarkerStyle(23);
  ztt_sig_db03allch_graph->SetMarkerColor(kBlue+3);
  ztt_sig_db03_graph->SetMarkerStyle(24);
  ztt_sig_db03_graph->SetMarkerColor(kRed+2);

  ztt_sig_db04_graph->GetXaxis()->SetRangeUser(0.085,0.185);
  ztt_sig_db04_graph->GetYaxis()->SetRangeUser(40,80);
  ztt_sig_db04_graph->SetTitle(";Isolation cut;S/root(B)");
  
   
  TCanvas plot_canv("plot_canv","plot_canv",1200,800); 
  plot_canv.cd();
  ztt_sig_db04_graph->Draw("AP"); 
  ztt_sig_trk03_graph->Draw("PSAME");
  ztt_sig_db04allch_graph->Draw("PSAME");
  ztt_sig_db03allch_graph->Draw("PSAME");
  ztt_sig_db03_graph->Draw("PSAME");
  TLegend *leg = PositionedLegend(0.3,0.3,6,0);
  leg->SetFillStyle(0);
  leg->AddEntry(ztt_sig_db04_graph,"DBeta corr (cs 0.4)","p");
  leg->AddEntry(ztt_sig_trk03_graph,"Tracker iso (cs 0.3)","p");
  leg->AddEntry(ztt_sig_db04allch_graph,"DBeta corr, allcharged (cs 0.4)","p");
  leg->AddEntry(ztt_sig_db03allch_graph,"DBeta corr, allcharged (cs 0.3)","p");
  leg->AddEntry(ztt_sig_db03_graph,"DBeta corr (cs 0.3)","p");

  leg->Draw("SAME");
  
 // plot_canv.SetGrid(1,1);

  plot_canv.SaveAs("s_over_root_b_mt.pdf");

 return 0;
}
