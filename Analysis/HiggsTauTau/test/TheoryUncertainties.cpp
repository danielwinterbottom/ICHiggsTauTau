// ROOT includes
#include "TChain.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TBranch.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TRint.h"
#include "TGaxis.h"
#include "TLatex.h"
#include "TEventList.h"
#include "TProfile.h"

// C++ includes
#include <iostream>
#include <fstream>
#include <memory>
#include <map>
#include <string>
#include <cstdlib>
#include <math.h> 

//boost
#include "boost/program_options.hpp"

namespace po = boost::program_options;

struct ProgramOptions{

  std::string input;
  std::string outputDirname;
  std::string outputFilename;
  bool        makePlots;
  std::string channel;
  std::string variable;
  bool        doGen;
  bool        doScale;
  bool        doPDF;
  bool        doAlphaS;
  std::string cfg;
  bool        RecreateRenorm;
  bool        doAcceptance;
  bool        do2D;
  std::string sample;
  std::string cat;
  bool relaxIso;
  bool doPtWeight;
  bool scaleUp;
  bool scaleDown;
  bool doST;
  
};

inline bool file_exists(const std::string& name){
  std::ifstream f(name.c_str());
  return f.good();
}
   
inline bool check_lines(const std::string& name, unsigned lines){
  std::ifstream file(name); 
  std::size_t lines_count =0;
   std::string line;
   while (std::getline(file , line)) ++lines_count;
   file.close();
   return lines_count == lines;
}

double StandDev(std::vector<double> a) {
  unsigned N = a.size();
  double mean=0;
  for(unsigned i=0; i<N; ++i){
    mean+=a[i];
  }
  mean=mean/N;
  double sum=0; 
  for(unsigned i=0; i<N; ++i){
    sum+=pow(a[i]-mean,2);  
  }
  sum = sum/N;
  return sqrt(sum);
}

std::pair<double,double> PDFUncert(std::vector<double> a, double a0, double b) {
  unsigned N = a.size();
  double dAplus=0;
  double dAminus=0;
  for(unsigned i=0; i<N; i+=2){
     double aplus = a[i];
     double aminus = a[i+1];
     double temp;
     temp = std::max(aplus-a0, aminus-a0);
     dAplus += pow(std::max(0.,temp),2);
     temp = std::max(a0-aplus, a0-aminus);
     dAminus += pow(std::max(0.,temp),2);
  }
  dAplus = sqrt(dAplus)/b;
  dAminus = sqrt(dAminus)/b;
  return std::make_pair(dAplus,dAminus);
}

struct greater {
    template<class T>
    bool operator()(T const &a, T const &b) const { return a > b; }
};
struct less_than {
    template<class T>
    bool operator()(T const &a, T const &b) const { return a < b; }
};

std::pair<double, double> ReturnMax (std::pair<double,double> a, std::pair<double,double> b){
  if(a.first > b.first) return a;
  else return b;
}
std::pair<double, double> ReturnMin (std::pair<double,double> a, std::pair<double,double> b){
  if(a.first < b.first) return a;
  else return b;
}

double EnergyCalc (double pt, double eta){
  double theta = 2*atan(exp(-eta));
  double E = pt/sin(theta);
  return E;    
}

bool DeltaRMatch(double eta1, double eta2, double phi1, double phi2, double dR_max){
  if(phi1<0) phi1 += 2*M_PI;
  if(phi2<0) phi2 += 2*M_PI;
  double dR = sqrt(pow(eta1-eta2,2)+pow(phi1-phi2,2));
  return dR < dR_max;
}

void SetStyle(){
  TStyle *myStyle  = new TStyle("MyStyle","");
  myStyle->SetCanvasBorderMode(0);
  myStyle->SetPadBorderMode(0);
  myStyle->SetCanvasColor(0);
  myStyle->SetStatColor(0);
  myStyle->SetLegendBorderSize(0);
  myStyle->SetCanvasDefX(900);
  myStyle->SetCanvasDefY(900);
  myStyle->SetCanvasDefH(850);
  myStyle->SetCanvasDefW(850);
  myStyle->SetPadBottomMargin(0.1);
  myStyle->SetPadTopMargin(0.1);
  myStyle->SetPadLeftMargin(0.125);
  myStyle->SetPadRightMargin(0.125);
  myStyle->SetPadTickX(1);
  myStyle->SetPadTickY(1);
  myStyle->SetFrameBorderMode(0);
  myStyle->SetLineWidth(1);
  myStyle->SetOptStat(1110);
  myStyle->SetStatX(0.9);
  myStyle->SetStatY(0.93);
  myStyle->SetStatW(0.22);
  myStyle->SetStatH(0.15);
  myStyle->SetTitleYOffset(1.5);
  myStyle->SetTitleFont(62);
  myStyle->SetTitleFont(62,"Y");
  myStyle->SetLabelFont(62);
  myStyle->SetLabelFont(62,"Y");
  myStyle->SetOptTitle(0);
  myStyle->SetHistLineStyle(0);
  myStyle->SetHistLineWidth(3);
  myStyle->SetMarkerSize(0.9);
  myStyle->SetMarkerStyle(20);
  gROOT->SetStyle("MyStyle");
  gROOT->UseCurrentStyle();
  gROOT->ForceStyle();    
}

void DrawHist(std::vector<TH1D*> hists, std::vector<std::string> legend_entries, std::string out_name, bool ratio, bool Normalize, std::string title, bool intLabels, bool log, double ratio_range, bool bottomLeg){
  bool doLegend = legend_entries.size() > 0;
  SetStyle();
  TGaxis::SetMaxDigits(3);
  THStack *hs = new THStack("hs","");
  TCanvas *c1 = new TCanvas();
  if(log) c1->SetLogy();
  double ymin = 0.8 - 0.05*hists.size();
  TLegend *leg = new TLegend(0.65,ymin,0.8,0.8);
  if(bottomLeg){
    double ymax = 0.1 + 0.05*hists.size();
    leg = new TLegend(0.60,0.1,0.8,ymax);    
  }
  leg->SetTextSize(0.03);
  leg->SetFillStyle(0);
  for(unsigned i=0; i<hists.size(); ++i){
    int col_num = i+1;
    if(col_num==7) col_num = 9;
    hists[i]->SetLineColor(col_num);
    hists[i]->SetMarkerColor(col_num);
    for(unsigned j=0; j<(unsigned)hists[i]->GetNbinsX()+1; ++j){
      hists[i]->SetBinError(j,0.00001);    
    } //get rid of vartical xerror bars by setting them to small value - purley for aesthetic reasons
    TH1D *htemp = (TH1D*)hists[i]->Clone();
    if(Normalize){
      double norm = 1./hists[0]->Integral();
      htemp->Scale(norm);
      hists[i]->GetYaxis()->SetTitle("Normalized Entries");
    }
    hs->Add(htemp);
    std::string leg_string = "";
    if(i<legend_entries.size()) leg_string = legend_entries[i];
    leg->AddEntry(hists[i],leg_string.c_str(),"lp");
  }
  
  if(!ratio){
    hs->Draw("nostack p");
    hs->GetXaxis()->SetTitle(hists[0]->GetXaxis()->GetTitle());
    hs->GetYaxis()->SetTitle(hists[0]->GetYaxis()->GetTitle());
    if(intLabels){
     TGaxis *axis = (TGaxis*)hs->GetXaxis();
     axis->SetOption("I");
    }
    hs->Draw("nostack p");
    c1->Update();
    if(doLegend) leg->Draw();
  } else{
   TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
   pad1->SetBottomMargin(0);
   pad1->SetFillStyle(4000);
   pad1->SetFrameFillStyle(4000);       
   pad1->Draw();             
   pad1->cd();
   hs->Draw("nostack p");
   float scale1 = pad1->GetAbsHNDC();
   hs->GetYaxis()->SetLabelSize(hs->GetYaxis()->GetLabelSize()/scale1);
   hs->GetYaxis()->SetTitleSize(hs->GetYaxis()->GetTitleSize()/scale1);
   hs->GetYaxis()->SetTitle(hists[0]->GetYaxis()->GetTitle());
   hs->GetYaxis()->SetTitleOffset(1.1);
   hs->SetMinimum(hs->GetMaximum()/100000);
   hs->Draw("nostack p");
   c1->Update();
   if(doLegend) leg->Draw();
    
   THStack *hs_ratio = new THStack("hs_ratio","");
   TH1D *h_denum = (TH1D*)hists[0]->Clone();
   for(unsigned i=0; i<hists.size(); ++i){
     TH1D *htemp = (TH1D*)hists[i]->Clone();
     htemp->Divide(h_denum);
     hs_ratio->Add((TH1D*)htemp->Clone());
   }
   c1->cd(); 
   TPad *pad2 = new TPad("pad2", "pad2", 0, 0.02, 1, 0.3);
   pad2->SetFillStyle(4000);
   pad2->SetFrameFillStyle(4000); 
   pad2->SetTopMargin(0);
   pad2->SetBottomMargin(0.2);
   pad2->Draw();
   pad2->cd();
   hs_ratio->Draw("nostack p"); 
   float scale = pad2->GetAbsHNDC();
   pad2->SetBottomMargin(0.3); 
   hs_ratio->GetXaxis()->SetTitle(hists[0]->GetXaxis()->GetTitle());
   if(intLabels){
     hs_ratio->GetXaxis()->SetNdivisions(hists[0]->GetNbinsX()+1,0,0);
   }
   hs_ratio->GetYaxis()->SetTitle("Ratio");
   hs_ratio->GetYaxis()->SetTitleOffset(0.4);
   hs_ratio->GetYaxis()->CenterTitle();
   hs_ratio->GetXaxis()->SetLabelSize(hs_ratio->GetXaxis()->GetLabelSize()/scale);
   hs_ratio->GetYaxis()->SetLabelSize(hs_ratio->GetYaxis()->GetLabelSize()/scale);
   hs_ratio->GetXaxis()->SetTitleSize(hs_ratio->GetXaxis()->GetTitleSize()/scale);
   hs_ratio->GetYaxis()->SetTitleSize(hs_ratio->GetYaxis()->GetTitleSize()/scale);
   double ratio_max = 1+ratio_range;
   double ratio_min = 1-ratio_range;
   hs_ratio->SetMinimum(ratio_min);
   hs_ratio->SetMaximum(ratio_max);
   hs_ratio->GetYaxis()->SetNdivisions(3,5,0);
   hs_ratio->Draw("nostack p"); 
   pad2->Update();
   c1->Update();
   c1->cd();  
  }
  
  TLatex *tex = new TLatex(0.7,0.95,(title).c_str());
  tex->SetNDC();
  tex->SetTextFont(44);
  tex->SetTextSize(23);
  tex->SetLineWidth(2);
  tex->Draw();
  
  c1->Print((out_name+".pdf").c_str());
  
}

int main(int argc, char* argv[]){
    
  ProgramOptions options;
  
  po::options_description preconfig("Pre-Configuration");
  preconfig.add_options()("cfg", po::value<std::string>(&options.cfg)->default_value("config.cfg"));
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);
  po::options_description config("Configuration");
  config.add_options()
        ("input",                  po::value<std::string>(&options.input)->default_value("/vols/cms/dw515/Offline/output/GenComps/GluGluHToTauTau_M-125_mt_2016.root"))
        ("outputDirname",          po::value<std::string>(&options.outputDirname)->default_value("theory_output"))
        ("outputFilename",         po::value<std::string>(&options.outputFilename)->default_value("uncertanties.txt"))
        ("makePlots",              po::value<bool>(&options.makePlots)->default_value(false))
        ("channel",                po::value<std::string>(&options.channel)->default_value("mt"))
        ("variable",               po::value<std::string>(&options.variable)->default_value("m_vis"))
        ("doGen",                  po::value<bool>(&options.doGen)->default_value(true))
        ("doScale",                po::value<bool>(&options.doScale)->default_value(true))
        ("doPDF",                  po::value<bool>(&options.doPDF)->default_value(true))
        ("doAlphaS",               po::value<bool>(&options.doAlphaS)->default_value(true))
        ("RecreateRenorm",         po::value<bool>(&options.RecreateRenorm)->default_value(false))
        ("doAcceptance",           po::value<bool>(&options.doAcceptance)->default_value(false))
        ("do2D",                   po::value<bool>(&options.do2D)->default_value(false))
        ("sample",                 po::value<std::string>(&options.sample)->default_value("default"))
        ("cat",                    po::value<std::string>(&options.cat)->default_value(""))
        ("relaxIso",               po::value<bool>(&options.relaxIso)->default_value(false))
        ("doPtWeight",             po::value<bool>(&options.doPtWeight)->default_value(false))
        ("scaleDown",              po::value<bool>(&options.scaleDown)->default_value(false))
        ("scaleUp",                po::value<bool>(&options.scaleUp)->default_value(false))
        ("doST",                   po::value<bool>(&options.doST)->default_value(false));
        
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(options.cfg.c_str(), config), vm);
  po::notify(vm);
  
  std::cout << "===== program options ====="                 << std::endl;
  std::cout << "input          = " << options.input          << std::endl;
  std::cout << "outputDirname  = " << options.outputDirname  << std::endl;
  std::cout << "outputFilename = " << options.outputFilename << std::endl;
  std::cout << "makePlots      = " << options.makePlots      << std::endl;
  std::cout << "channel        = " << options.channel        << std::endl;
  std::cout << "variable       = " << options.variable       << std::endl;
  std::cout << "doGen          = " << options.doGen          << std::endl;
  std::cout << "doScale        = " << options.doScale        << std::endl;
  std::cout << "doPDF          = " << options.doPDF          << std::endl;
  std::cout << "doAlphaS       = " << options.doAlphaS       << std::endl;
  std::cout << "RecreateRenorm = " << options.RecreateRenorm << std::endl;
  std::cout << "doAcceptance   = " << options.doAcceptance   << std::endl;
  std::cout << "do2D           = " << options.do2D           << std::endl;
  std::cout << "sample         = " << options.sample         << std::endl;
  std::cout << "cat            = " << options.cat            << std::endl;
  std::cout << "relaxIso       = " << options.relaxIso       << std::endl;
  std::cout << "doPtWeight     = " << options.doPtWeight     << std::endl;
  std::cout << "scaleDown      = " << options.scaleDown      << std::endl;
  std::cout << "scaleUp        = " << options.scaleUp        << std::endl;
  std::cout << "doST           = " << options.doST           << std::endl;
  
  if(!file_exists(options.outputDirname)){
    std::cout << "Output folder does not exist! Exiting." << std::endl;
    return 0;    
  }
  
  std::string higgs_pt_weight= "1";
  if(options.doPtWeight){
    if(options.sample == "default" || options.sample == "powheg"){
      higgs_pt_weight = "wt_ggh_pt";
      if(options.scaleUp) higgs_pt_weight = "wt_ggh_pt_up";
      if(options.scaleDown) higgs_pt_weight = "wt_ggh_pt_down";
    }
    if(options.sample == "pythiaup") higgs_pt_weight = "wt_ggh_pt_pythiaup";
    if(options.sample == "pythiadown") higgs_pt_weight = "wt_ggh_pt_pythiadown";
    if(options.sample == "herwig") higgs_pt_weight = "wt_ggh_pt_herwig";
    if(options.sample == "amc") higgs_pt_weight = "wt_ggh_pt_amc";
    if(options.sample == "default_up" || options.sample == "powheg_up") higgs_pt_weight = "wt_ggh_pt_up";
    if(options.sample == "default_down" || options.sample == "powheg_down") higgs_pt_weight = "wt_ggh_pt_down";
  }
  
  if(options.makePlots) SetStyle();
  std::map<std::string, std::tuple<std::string,int,double,double>> labels_map_;
  labels_map_["m_vis"] = std::make_tuple("M_{vis} [GeV]",25,0,200); //change back to 50!
  labels_map_["m_sv"] = std::make_tuple("M_{SVFit} [GeV]",50,0,200);
  labels_map_["mt_1"] = std::make_tuple("M_{T} [GeV]",30,0,200);
  labels_map_["m_vis_res"]= std::make_tuple("(M_{vis}_{reco}-M_{vis}_{gen})/M_{vis}_{gen}",25,-1,1);
  labels_map_["m_vis_diff"]= std::make_tuple("M_{vis}_{reco}-M_{vis}_{gen} [GeV]",25,-100,100);
  labels_map_["lep1_matched"]= std::make_tuple("lep 1 matched to gen",2,0,2);
  labels_map_["lep2_matched"]= std::make_tuple("lep 1 matched to gen",2,0,2);
  labels_map_["mva_olddm_tight_2"] = std::make_tuple("Tau isolation Tight WP",2,0,2);
  labels_map_["mva_olddm_medium_2"] = std::make_tuple("Tau Isolation Medium WP",2,0,2);
  labels_map_["mva_olddm_loose_2"] = std::make_tuple("Tau Isolation Loose WP",2,0,2);
  labels_map_["mva_olddm_vloose_2"] = std::make_tuple("Tau Isolation V.Loose WP",2,0,2);
  labels_map_["mva_olddm_tight_1"] = std::make_tuple("Tau isolation Tight WP",2,0,2);
  labels_map_["mva_olddm_medium_1"] = std::make_tuple("Tau Isolation Medium WP",2,0,2);
  labels_map_["mva_olddm_loose_1"] = std::make_tuple("Tau Isolation Loose WP",2,0,2);
  labels_map_["mva_olddm_vloose_1"] = std::make_tuple("Tau Isolation V.Loose WP",2,0,2);
  if(options.channel == "em"){
    labels_map_["pt_1"] = std::make_tuple("electron p_{T} [GeV]",30,0,150);
    labels_map_["pt_2"] = std::make_tuple("muon p_{T} [GeV]",30,0,150);
    labels_map_["eta_1"] = std::make_tuple("electron #eta",25,-3,3);
    labels_map_["eta_2"] = std::make_tuple("muon #eta",25,-3,3);
    labels_map_["E_diff1"] = std::make_tuple("electron E_{reco}-E_{gen} [GeV]",25,-30,30);
    labels_map_["E_diff2"] = std::make_tuple("muon E_{reco}-E_{gen} [GeV]",25,-20,20);
    labels_map_["pt_diff1"]= std::make_tuple("electron p_{T}_{reco}-p_{T}_{gen} [GeV]",25,-30,30);
    labels_map_["pt_diff2"]= std::make_tuple("muon p_{T}_{reco}-p_{T}_{gen} [GeV]",25,-20,20);
    labels_map_["E_res1"] = std::make_tuple("electron (E_{reco}-E_{gen})/E_{gen}",25,-0.3,0.3);
    labels_map_["E_res2"] = std::make_tuple("muon (E_{reco}-E_{gen})/E_{gen}",25,-0.2,0.2);
    labels_map_["pt_res1"]= std::make_tuple("electron (p_{T}_{reco}-p_{T}_{gen})/p_{T}_{gen}",25,-0.3,0.3);
    labels_map_["pt_res2"]= std::make_tuple("muon (p_{T}_{reco}-p_{T}_{gen})/p_{T}_{gen}",25,-0.2,0.2);
    labels_map_["iso_1"] = std::make_tuple("electron isolation",10,0,0.3);
    labels_map_["iso_2"] = std::make_tuple("muon isolation",10,0,0.3);
  } else if(options.channel == "et"){
    labels_map_["pt_1"] = std::make_tuple("electron p_{T} [GeV]",30,0,150);
    labels_map_["pt_2"] = std::make_tuple("tau p_{T} [GeV]",30,0,150);
    labels_map_["eta_1"] = std::make_tuple("electron #eta",25,-3,3);
    labels_map_["eta_2"] = std::make_tuple("tau #eta",25,-3,3);
    labels_map_["E_diff1"] = std::make_tuple("electron E_{reco}-E_{gen} [GeV]",25,-30,30);
    labels_map_["E_diff2"] = std::make_tuple("tau E_{reco}-E_{gen} [GeV]",25,-50,50);
    labels_map_["pt_diff1"]= std::make_tuple("electron p_{T}_{reco}-p_{T}_{gen} [GeV]",25,-30,30);
    labels_map_["pt_diff2"]= std::make_tuple("tau p_{T}_{reco}-p_{T}_{gen} [GeV]",25,-50,50);
    labels_map_["E_res1"] = std::make_tuple("electron (E_{reco}-E_{gen})/E_{gen}",25,-0.3,0.3);
    labels_map_["E_res2"] = std::make_tuple("tau (E_{reco}-E_{gen})/E_{gen}",25,-0.5,0.5);
    labels_map_["pt_res1"]= std::make_tuple("electron (p_{T}_{reco}-p_{T}_{gen})/p_{T}_{gen}",25,-0.3,0.3);
    labels_map_["pt_res2"]= std::make_tuple("tau (p_{T}_{reco}-p_{T}_{gen})/p_{T}_{gen}",25,-0.5,0.5);
    labels_map_["iso_1"] = std::make_tuple("electron isolation",10,0,0.3);
  } else if(options.channel == "mt"){
    labels_map_["pt_1"] = std::make_tuple("muon p_{T} [GeV]",30,0,150);
    labels_map_["pt_2"] = std::make_tuple("tau p_{T} [GeV]",30,0,150);
    labels_map_["eta_1"] = std::make_tuple("muon #eta",25,-3,3);
    labels_map_["eta_2"] = std::make_tuple("tau #eta",25,-3,3);
    labels_map_["E_diff1"] = std::make_tuple("muon E_{reco}-E_{gen} [GeV]",25,-20,20);
    labels_map_["E_diff2"] = std::make_tuple("tau E_{reco}-E_{gen} [GeV]",25,-50,50);
    labels_map_["pt_diff1"]= std::make_tuple("muon p_{T}_{reco}-p_{T}_{gen} [GeV]",25,-20,20);
    labels_map_["pt_diff2"]= std::make_tuple("tau p_{T}_{reco}-p_{T}_{gen} [GeV]",25,-50,50);
    labels_map_["E_res1"] = std::make_tuple("muon (E_{reco}-E_{gen})/E_{gen}",25,-0.2,0.2);
    labels_map_["E_res2"] = std::make_tuple("tau (E_{reco}-E_{gen})/E_{gen}",25,-0.5,0.5);
    labels_map_["pt_res1"]= std::make_tuple("muon (p_{T}_{reco}-p_{T}_{gen})/p_{T}_{gen}",25,-0.2,0.2);
    labels_map_["pt_res2"]= std::make_tuple("tau (p_{T}_{reco}-p_{T}_{gen})/p_{T}_{gen}",25,-0.5,0.5);
    labels_map_["iso_1"] = std::make_tuple("muon isolation",10,0,0.3);
  } else if(options.channel == "tt"){
    labels_map_["pt_1"] = std::make_tuple("leading tau p_{T} [GeV]",30,0,150);
    labels_map_["pt_2"] = std::make_tuple("sub-leading tau p_{T} [GeV]",30,0,150);
    labels_map_["eta_1"] = std::make_tuple("leading tau #eta",25,-3,3);
    labels_map_["eta_2"] = std::make_tuple("sub-leading tau #eta",25,-3,3);
    labels_map_["E_diff1"] = std::make_tuple("leading tau E_{reco}-E_{gen} [GeV]",25,-50,50);
    labels_map_["E_diff2"] = std::make_tuple("sub-leading tau E_{reco}-E_{gen} [GeV]",25,-50,50);
    labels_map_["pt_diff1"]= std::make_tuple("leading tau p_{T}_{reco}-p_{T}_{gen} [GeV]",25,-50,50);
    labels_map_["pt_diff2"]= std::make_tuple("sub-leading tau p_{T}_{reco}-p_{T}_{gen} [GeV]",25,-50,50);
    labels_map_["E_res1"] = std::make_tuple("leading tau (E_{reco}-E_{gen})/E_{gen}",25,-0.5,0.5);
    labels_map_["E_res2"] = std::make_tuple("sub-leading tau (E_{reco}-E_{gen})/E_{gen}",25,-0.5,0.5);
    labels_map_["pt_res1"]= std::make_tuple("leading tau (p_{T}_{reco}-p_{T}_{gen})/p_{T}_{gen}",25,-0.5,0.5);
    labels_map_["pt_res2"]= std::make_tuple("sub-leading tau (p_{T}_{reco}-p_{T}_{gen})/p_{T}_{gen}",25,-0.5,0.5);
    labels_map_["E_res1"] = std::make_tuple("leading tau (E_{reco}-E_{gen})/E_{gen}",25,-0.3,0.3);
    labels_map_["E_res2"] = std::make_tuple("sub-leading tau (E_{reco}-E_{gen})/E_{gen}",25,-0.5,0.5);
    labels_map_["pt_res1"]= std::make_tuple("leading tau (p_{T}_{reco}-p_{T}_{gen})/p_{T}_{gen}",25,-0.3,0.3);
    labels_map_["pt_res2"]= std::make_tuple("sub-leading tau (p_{T}_{reco}-p_{T}_{gen})/p_{T}_{gen}",25,-0.5,0.5);
  }
  labels_map_["pt_tt"] = std::make_tuple("Higgs p_{T} [GeV]",30,0,200);
  labels_map_["jpt_1"] = std::make_tuple("leading jet p_{T} [GeV]",30,0,200);
  labels_map_["jpt_2"] = std::make_tuple("sub-leading jet p_{T} [GeV]",30,0,200);
  labels_map_["jeta_1"] = std::make_tuple("leading jet #eta",30,-6,6);
  labels_map_["jeta_2"] = std::make_tuple("sub-leading jet #eta",30,-6,6);
  labels_map_["mjj"] = std::make_tuple("M_{jj} [GeV]",30,0,1000);
  labels_map_["jdeta"] = std::make_tuple("#Delta#eta_{jj}",25,0,10);
  labels_map_["n_jets"] = std::make_tuple("Number of jets",5,0,5);
  labels_map_["n_jetsingap"] = std::make_tuple("Number of jets in gap",5,0,5);
  if(!options.doGen) labels_map_["n_bjets"] = std::make_tuple("Number of b-jets",5,0,5);
  std::map<std::string, std::string> alias_map_;
  
  alias_map_["inclusive"] = "(1)";
  alias_map_["baseline"] = "(1)";  
  if(options.doGen){
    if(options.channel == "em" || options.channel == "et" || options.channel == "mt"){
      if(options.channel == "em") alias_map_["baseline"] = "(passed && pt_1>13 && pt_2>10 && eta_1<2.5 && eta_2<2.4)";
      else if(options.channel == "et") alias_map_["baseline"] = "(mt_1<50 && passed && pt_1>26 && pt_2>20 && eta_1<2.1 && eta_2<2.3)";
      else if(options.channel == "mt") alias_map_["baseline"] = "(mt_1<50 && passed && pt_1>23 && pt_2>20 && eta_1<2.4 && eta_2<2.3)";
    } else if (options.channel == "tt"){
      alias_map_["baseline"] = "(passed && pt_1>40 && pt_2>40 && eta_1<2.1 && eta_2<2.1)";
    }
  } else{
    std::string iso_string = "mva_olddm_tight";
    if(options.relaxIso) iso_string = "mva_olddm_vloose";
    if(options.channel == "em"){
      alias_map_["baseline"] = "(iso_1<0.15 && iso_2<0.2 && !leptonveto)";
    } else if (options.channel == "et" || options.channel == "mt"){
      alias_map_["baseline"] = "(iso_1<0.1 && "+iso_string+"_2>0.5 && antiele_2 && antimu_2 && !leptonveto && mt_1<50)";    
    } else if (options.channel == "mt"){
      alias_map_["baseline"] = "(iso_1<0.15 && "+iso_string+"_2>0.5 && antiele_2 && antimu_2 && !leptonveto && mt_1<50)";    
    } else if (options.channel == "tt"){
      alias_map_["baseline"] = "("+iso_string+"_1>0.5 && "+iso_string+"_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)";
    }
  }
  
  //alias_map_["tau_iso!=tight"] = "(mva_olddm_tight_2<0.5)";   
  //if(options.channel == "tt") alias_map_["tau_iso!=tight"] = "(mva_olddm_tight_1<0.5 && mva_olddm_tight_2<0.5)";
  
  if(!options.do2D && !options.doST){
    if (options.channel == "em" || options.channel == "et" || options.channel == "mt"){   
      if(options.channel == "et"){
        alias_map_["vbf_high"] = "(n_jets==2 && mjj>800 && pt_tt>100)";
        alias_map_["vbf_low"] = "(n_jets==2 && mjj>500 && (mjj<800 || pt_tt<100))";
        alias_map_["1jet_high"] = "((n_jets==1 || (n_jets==2 && mjj<500)) && pt_tt>140)";
        alias_map_["1jet_low"] = "(n_jets==1 || (n_jets==2 && mjj<500)) && ((pt_2>20 && pt_2<40) || (pt_2>40 && pt_tt<140))";
        alias_map_["0jet_high"] = "(pt_2>50 && n_jets==0)";
        alias_map_["0jet_low"] = "(pt_2>20 && pt_2<50 && n_jets==0)";
      } else if(options.channel == "mt"){
        alias_map_["vbf_high"] = "(n_jets==2 && mjj>800 && pt_tt>100)";
        alias_map_["vbf_low"] = "(n_jets==2 && mjj>500 && (mjj<800 || pt_tt<100))";
        alias_map_["1jet_high"] = "((n_jets==1 || (n_jets==2 && mjj<500)) && pt_tt>140)";
        alias_map_["1jet_low"] = "(n_jets==1 || (n_jets==2 && mjj<500)) && ((pt_2>20 && pt_2<40) || (pt_2>40 && pt_tt<140))";
        alias_map_["0jet_high"] = "(pt_2>50 && n_jets==0)";
        alias_map_["0jet_low"] = "(pt_2>20 && pt_2<50 && n_jets==0)";
      } else if(options.channel == "em"){
        alias_map_["vbf_high"] = "(pt_2>15 && n_jets==2 && mjj>800 && pzeta>-10)";
        alias_map_["vbf_low"] = "(pt_2>15 && n_jets==2 && mjj>500 && mjj<800 && pzeta>-10)";
        alias_map_["1jet_high"] = "(pt_2>35 && (n_jets==1 || (n_jets==2 && mjj < 500)) && pzeta>-35)";
        alias_map_["1jet_low"] = "(pt_2>15 && pt_2<35 && pzeta>-35 && (n_jets==1 || (n_jets==2 && mjj < 500)))";
        alias_map_["0jet_high"] = "(pt_2>35 && pzeta>-35 && n_jets==0)";
        alias_map_["0jet_low"] = "(pt_2>15 && pt_2<35 && pzeta>-35 && n_jets==0)";
      }
    }
    else if (options.channel == "tt"){
      alias_map_["vbf_high"] = "(pt_1>50 && n_jets>=2 && pt_tt>100 && mjj>500 && jdeta>2.5 && n_jetsingap<1)";
      alias_map_["vbf_low"] = "(pt_1>50 && n_jets>=2 && jdeta>2.5 && n_jetsingap < 1)*((pt_tt<100 && mjj>300) || (pt_tt>100 && mjj>300 && mjj<500))";
      alias_map_["1jet_high"] = "(pt_1>50 && pt_tt>170)*(n_jets==1 || (n_jets>=2 && !(mjj>300 && jdeta>2.5 && n_jetsingap<1)))";
      alias_map_["1jet_low"] = "(pt_1>50 && pt_tt>100 && pt_tt<170)*(n_jets==1 || (n_jets>=2 && !(mjj>300 && jdeta>2.5 && n_jetsingap<1)))";
      alias_map_["0jet"] = "(pt_1>50 && n_jets==0)";
    }
  }
  
  if(options.do2D){
      
    std::vector<double> mjj_2D;
    std::vector<double> pt_tt_2D;
    std::vector<double> pt_2D;
      
    if(options.channel == "et" || options.channel == "mt" || options.channel == "em"){
      mjj_2D = {300,700,1100,1500,3000};
      pt_tt_2D = {0,100,150,200,250,300,500};
      if(options.channel == "em") pt_2D = {15,20,25,30,35,300};
      else pt_2D = {30,35,40,45,50,55,300}; 
    } else if (options.channel == "tt"){
      mjj_2D = {0,300,500,800};
      pt_tt_2D = {0,100,170,300};
      pt_2D = {50};  
    }
    
    std::string cat_0jet;
    std::string cat_1jet;
    std::string cat_vbf; 
      
    if(options.channel == "et" || options.channel == "mt"){
      cat_0jet = "(pt_2>30 && n_jets==0)";
      cat_1jet = "(pt_2>30 && (n_jets==1 || (n_jets==2 && mjj<300) || n_jets>2)) ";
      cat_vbf  = "(pt_2>30 && n_jets==2 && mjj>300)"; 
    } else if(options.channel == "em"){
      cat_0jet = "(pt_2>15 && pzeta>-35 && n_jets==0)";
      cat_1jet = "(pt_2>15 && pzeta>-35 && (n_jets==1 || (n_jets==2 && mjj<300) || n_jets>2)) ";
      cat_vbf  = "(pt_2>15 && pzeta>-10 && n_jets==2 && mjj>300)"; 
    } else if(options.channel == "tt"){
      cat_0jet = "(pt_1>50 && n_jets==0)";
      cat_1jet = "((pt_1>50 && n_jets==1 || ((n_jets>=2)* !(jdeta>2.5 && n_jetsingap<1 && pt_tt>100)))) ";
      cat_vbf  = "((pt_1>50 && n_jets>=2)*(jdeta>2.5 && n_jetsingap<1 && pt_tt>100))"; 
    }
    
    unsigned count_cats=10;
    
    for(unsigned i=0; i<pt_2D.size(); ++i){
       double min = pt_2D[i];
       double max;
       std::string alias_cut;
       std::string alias_string;
       unsigned lep_num=2;
       if(options.channel == "tt") lep_num=1;
       if(i==0){
         if(options.channel == "tt") alias_map_[Form("%u0jet_pt=%.0f:inf",count_cats,min)] = cat_0jet+Form("*(pt_%u>%.0f)",lep_num,min);
         else alias_map_[Form("%u0jet_pt=%.0f:%.0f",count_cats,min,pt_2D.back())] = cat_0jet+Form("*(pt_%u>%.0f && pt_%u<=%.0f)",lep_num,min,lep_num,pt_2D.back());
         count_cats++;
       }
       if(i!=pt_2D.size()-1){
         max = pt_2D[i+1];
         alias_cut = Form("*(pt_%u>%.0f && pt_%u<=%.0f)",lep_num,min,lep_num,max);
         alias_string = Form("%u0jet_pt=%.0f:%.0f",count_cats,min,max);
       } else {
         alias_cut = Form("*(pt_%u>%.0f)",lep_num,min);  
         alias_string = Form("%u0jet_pt=%.0f:inf",count_cats,min);
       } 
       alias_map_[alias_string] = cat_0jet+alias_cut;
       count_cats++;
    }
    
    
    for(unsigned i=0; i<pt_tt_2D.size(); ++i){
       double min = pt_tt_2D[i];
       double max;
       std::string alias_cut;
       std::string alias_string;
       if(i==0){
         if(options.channel == "tt") alias_map_[Form("%uboosted_ptH=%.0f:inf",count_cats,min)] = cat_1jet+Form("*(pt_tt>%.0f)",min);
         else alias_map_[Form("%uboosted_ptH=%.0f:%.0f",count_cats,min,pt_tt_2D.back())] = cat_1jet+Form("*(pt_tt>%.0f && pt_tt<=%.0f)",min,pt_tt_2D.back());
         count_cats++;
       }
       if(i!=pt_tt_2D.size()-1){
         max = pt_tt_2D[i+1];
         alias_cut = Form("*(pt_tt>%.0f && pt_tt<=%.0f)",min,max);
         alias_string = Form("%uboosted_ptH=%.0f:%.0f",count_cats,min,max);
       } else {
         alias_cut = Form("*(pt_tt>%.0f)",min);  
         alias_string = Form("%uboosted_ptH=%.0f:inf",count_cats,min);
       } 
       alias_map_[alias_string] = cat_1jet+alias_cut;
       count_cats++;
    }
    
    
    for(unsigned i=0; i<mjj_2D.size(); ++i){
       double min = mjj_2D[i];
       double max;
       std::string alias_cut;
       std::string alias_string;
       if(i==0){
         if(options.channel == "tt") alias_map_[Form("%uvbf_mjj=%.0f:inf",count_cats,min)] = cat_vbf+Form("*(mjj>%.0f)",min);
         else alias_map_[Form("%uvbf_mjj=%.0f:%.0f",count_cats,min,mjj_2D.back())] = cat_vbf+Form("*(mjj>%.0f && mjj<=%.0f)",min,mjj_2D.back());
         count_cats++;
       }
       if(i!=mjj_2D.size()-1){
         max = mjj_2D[i+1];
         alias_cut = Form("*(mjj>%.0f && mjj<=%.0f)",min,max);
         alias_string = Form("%uvbf_mjj=%.0f:%.0f",count_cats,min,max);
       } else {
         alias_cut = Form("*(mjj>%.0f)",min);  
         alias_string = Form("%uvbf_mjj=%.0f:inf",count_cats,min);
       } 
       alias_map_[alias_string] = cat_vbf+alias_cut;
       count_cats++;
    }
  }
  
  if(options.doST){

    alias_map_.clear();
    alias_map_["baseline"] = "(mt_1<50 && genpt_1>60 && genpt_2>60 && geneta_1<2.3 && geneta_2<2.3)";
    alias_map_["0jet"]    = "(n_jets==0)";
    alias_map_["boosted"] = "((n_jets==1 || (n_jets==2 && mjj<300) || n_jets>2)) ";
    alias_map_["vbf"]     = "(n_jets==2 && mjj>300)"; 

  }
  
  
  TFile *f0 = new TFile(options.input.c_str());
  TTree *gen_tree = (TTree*)f0->Get("gen_ntuple");
  std::string tree_name;
  if(options.doGen) tree_name = "gen_ntuple";
  else              tree_name = "ntuple"; 
  TTree *tree = (TTree*)f0->Get(tree_name.c_str());

  double E_res1_ = -9999;
  double E_res2_ = -9999;
  double pt_res1_ = -9999;
  double pt_res2_ = -9999;
  double m_vis_res_ = -9999;
  double E_diff1_ = -9999;
  double E_diff2_ = -9999;
  double pt_diff1_ = -9999;
  double pt_diff2_ = -9999;
  double m_vis_diff_ = -9999;

  TBranch *b1 = tree->Branch("E_res1",&E_res1_);
  TBranch *b2 = tree->Branch("E_res2",&E_res2_);
  TBranch *b3 = tree->Branch("pt_res1",&pt_res1_);
  TBranch *b4 = tree->Branch("pt_res2",&pt_res2_);
  TBranch *b5 = tree->Branch("m_vis_res",&m_vis_res_);
  TBranch *b6 = tree->Branch("E_diff1",&E_diff1_);
  TBranch *b7 = tree->Branch("E_diff2",&E_diff2_);
  TBranch *b8 = tree->Branch("pt_diff1",&pt_diff1_);
  TBranch *b9 = tree->Branch("pt_diff2",&pt_diff2_);
  TBranch *b10 = tree->Branch("m_vis_diff",&m_vis_diff_);
  
  TH1D *h0 = new TH1D("h0","",std::get<1>(labels_map_[options.variable]),std::get<2>(labels_map_[options.variable]),std::get<3>(labels_map_[options.variable]));
  h0->Sumw2();
  gen_tree->Draw("event>>h0","","goff");

  double nominal_wt = h0->Integral(0,h0->GetNbinsX()+1);
  
  TH1D *htemp = new TH1D("htemp","",std::get<1>(labels_map_[options.variable]),std::get<2>(labels_map_[options.variable]),std::get<3>(labels_map_[options.variable]));
  htemp->Sumw2();
  TH1D *h_nnpdf = new TH1D("h_nnpdf","",100,0,100);
  h_nnpdf->Sumw2();
  h_nnpdf->GetYaxis()->SetTitle("Acceptance");
  h_nnpdf->GetYaxis()->SetTitle("PDF Member");
  
  if(options.variable == "E_res1" || options.variable == "E_res2" || options.variable == "pt_res1" || options.variable == "pt_res2" || options.variable == "m_vis_res" || options.variable == "E_diff1" || options.variable == "E_diff2" || options.variable == "pt_diff1" || options.variable == "pt_diff2" || options.variable == "m_vis_diff"){
      
    std::cout << "Calculating " << options.variable << " and adding to tree.." << std::endl;
    
    unsigned long long event_; double pt_1_; double pt_2_; double eta_1_; double eta_2_; double phi_1_; double phi_2_; double m_vis_;
    
    tree->SetBranchAddress("event",&event_);
    tree->SetBranchAddress("pt_1",&pt_1_);
    tree->SetBranchAddress("pt_2",&pt_2_);
    tree->SetBranchAddress("eta_1",&eta_1_);
    tree->SetBranchAddress("eta_2",&eta_2_);
    tree->SetBranchAddress("phi_1",&phi_1_);
    tree->SetBranchAddress("phi_2",&phi_2_);
    tree->SetBranchAddress("m_vis",&m_vis_);
    
    unsigned long long gen_event_; double gen_pt_1_; double gen_pt_2_; double gen_eta_1_; double gen_eta_2_; double gen_phi_1_; double gen_phi_2_; double gen_m_vis_; bool passed_;

    gen_tree->SetBranchAddress("event",&gen_event_);
    gen_tree->SetBranchAddress("pt_1",&gen_pt_1_);
    gen_tree->SetBranchAddress("pt_2",&gen_pt_2_);
    gen_tree->SetBranchAddress("eta_1",&gen_eta_1_);
    gen_tree->SetBranchAddress("eta_2",&gen_eta_2_);
    gen_tree->SetBranchAddress("phi_1",&gen_phi_1_);
    gen_tree->SetBranchAddress("phi_2",&gen_phi_2_);
    gen_tree->SetBranchAddress("m_vis",&gen_m_vis_);
    gen_tree->SetBranchAddress("passed",&passed_);
    
    unsigned min_j=0;
    for(unsigned i=0; i<tree->GetEntries();++i){  
      tree->GetEntry(i);
      for(unsigned j=min_j; j<gen_tree->GetEntries();++j){
        gen_tree->GetEntry(j);
 
        if(event_==gen_event_){
            
          
          m_vis_res_ = (m_vis_-gen_m_vis_)/gen_m_vis_;
          m_vis_diff_ = (m_vis_-gen_m_vis_);
          E_diff1_ = (EnergyCalc(pt_1_,eta_1_) - EnergyCalc(gen_pt_1_,eta_1_));
          E_diff2_ = (EnergyCalc(pt_2_,eta_2_) - EnergyCalc(gen_pt_2_,eta_2_));
          pt_diff1_ = (pt_1_ - gen_pt_1_);
          pt_diff2_ = (pt_2_ - gen_pt_2_);
          E_res1_ = (EnergyCalc(pt_1_,eta_1_) - EnergyCalc(gen_pt_1_,eta_1_))/EnergyCalc(gen_pt_1_,eta_1_);
          E_res2_ = (EnergyCalc(pt_2_,eta_2_) - EnergyCalc(gen_pt_2_,eta_2_))/EnergyCalc(gen_pt_2_,eta_2_);
          pt_res1_ = (pt_1_ - gen_pt_1_)/gen_pt_1_;
          pt_res2_ = (pt_2_ - gen_pt_2_)/gen_pt_2_;
          min_j = j;
          break;
        } else if(j == gen_tree->GetEntries()-1) std::cout << "Reached end of gen tree with no matching event!" << std::endl;
      }
      b1->Fill();
      b2->Fill();
      b3->Fill();
      b4->Fill();
      b5->Fill();
      b6->Fill();
      b7->Fill();
      b8->Fill();
      b9->Fill();
      b10->Fill();
    }
  }
  
  std::string default_wt = "wt*"+higgs_pt_weight;
  
  double total_inclusive = 0;
  if(options.doAcceptance){
    if(options.doGen) gen_tree->Draw("event>>htemp",(default_wt).c_str(),"goff");
    else gen_tree->Draw("event>>htemp",default_wt.c_str(),"goff");  
    total_inclusive = htemp->Integral(0,htemp->GetNbinsX()+1);
  }
  
  std::vector<double> scale_variation_addwt;
  if(options.doScale){
    std::string input_name = options.outputDirname+"/scale_variation_addwt.txt";
    bool file_check = file_exists(input_name) && check_lines(input_name,9);
    if(!file_check || options.RecreateRenorm){
      std::cout << "Creating new scale remormalization file: " << input_name << std::endl;
      std::ofstream output(input_name);
      for(unsigned i=0; i<9; ++i){
        std::string wt_name = Form("%s*scale_variation_wts[%u]",default_wt.c_str(),i);
        if(options.doPtWeight){
          if(i == 4) wt_name = Form("wt*wt_ggh_pt_scalehigh*scale_variation_wts[%u]",i);
          if(i == 8) wt_name = Form("wt*wt_ggh_pt_scalelow*scale_variation_wts[%u]",i);
        }
        gen_tree->Draw("event>>htemp",wt_name.c_str(),"goff");  
        double wt_sum = htemp->Integral(0,htemp->GetNbinsX()+1);
        scale_variation_addwt.push_back(nominal_wt/wt_sum);
        output << nominal_wt/wt_sum << std::endl;
      }
      output.close();
    } else{
      std::cout << "Using existing scale remormalization weights from file: " << input_name << ". Use option --RecreateRenorm to force new remormalization." << std::endl;
      std::ifstream input(input_name);
      double temp;
      while(input >> temp){
        scale_variation_addwt.push_back(temp);
      }
      input.close();
    }
  //"wt_ggh_pt_scalehigh",
  //"wt_ggh_pt_scalelow", 
  }
  std::vector<double> NNPDF_addwt;

  if(options.doPDF){
    std::string input_name;
    bool file_check;
    input_name = options.outputDirname+"/NNPDF_addwt.txt";
    file_check = file_exists(input_name) && check_lines(input_name,100);
    if(!file_check || options.RecreateRenorm){
      std::cout << "Creating new NNPDF remormalization file: " << input_name << std::endl;
      std::ofstream output(input_name);
      for(unsigned i=0; i<100; ++i){
        std::string wt_name = Form("%s*NNPDF_wts[%u]",default_wt.c_str(),i);
        gen_tree->Draw("event>>htemp",wt_name.c_str(),"goff");  
        double wt_sum = htemp->Integral(0,htemp->GetNbinsX()+1);
        NNPDF_addwt.push_back(nominal_wt/wt_sum);
        output << nominal_wt/wt_sum << std::endl;
      }
      output.close();
    } else{
      std::cout << "Using existing NNPDF remormalization weights from file: " << input_name << ". Use option --RecreateRenorm to force new remormalization." << std::endl;
      std::ifstream input(input_name);
      double temp;
      while(input >> temp){
        NNPDF_addwt.push_back(temp);
      }
      input.close();  
    } 
  }
  std::vector<double> alpha_s_variation_addwt;
  if(options.doAlphaS){
    std::string input_name = options.outputDirname+"/alpha_s_variation_addwt.txt";
    bool file_check = file_exists(input_name) && check_lines(input_name,2);
    if(!file_check || options.RecreateRenorm){
      std::cout << "Creating new Alpha_S remormalization file: " << input_name << std::endl;
      std::ofstream output(input_name);
      for(unsigned i=0; i<2; ++i){
        std::string wt_name = Form("%s*alpha_s_wts[%u]",default_wt.c_str(),i);
        gen_tree->Draw("event>>htemp",wt_name.c_str(),"goff");  
        double wt_sum = htemp->Integral(0,htemp->GetNbinsX()+1);
        alpha_s_variation_addwt.push_back(nominal_wt/wt_sum);
        output << nominal_wt/wt_sum << std::endl;
      }
      output.close();
    } else{
      std::cout << "Using existing Alpha_S remormalization weights from file: " << input_name << ". Use option --RecreateRenorm to force new remormalization." << std::endl;
      std::ifstream input(input_name);
      double temp;
      while(input >> temp){
        alpha_s_variation_addwt.push_back(temp);
      }
      input.close();
    }
  }
  
  TH1D *h_noweight = new TH1D("h_noweight","",std::get<1>(labels_map_[options.variable]),std::get<2>(labels_map_[options.variable]),std::get<3>(labels_map_[options.variable])); 
  h_noweight->Sumw2();
  h_noweight->GetXaxis()->SetTitle((std::get<0>(labels_map_[options.variable])).c_str());
  h_noweight->GetYaxis()->SetTitle("Entries");
  typedef std::map<std::string, std::string>::iterator it_type;
  
  std::string uncert_output_name = options.outputDirname+"/"+options.outputFilename;
  std::ofstream uncert_output(uncert_output_name);
  
  uncert_output << "Cat";
  if(options.doScale) uncert_output << "\tScale\tScale_error";
  if(options.doPDF) uncert_output << "\tNNPDF\tNNPDF_error";
  if(options.doPDF) uncert_output << "\tPDF";
  if(options.doAlphaS) uncert_output << "\tAlpha_S\tAlpha_S_error";
  if(options.doAcceptance) uncert_output << "\tAcceptance\tAcceptance_error";
  uncert_output << std::endl;
  
  for(it_type iterator = alias_map_.begin(); iterator != alias_map_.end(); iterator++) {
    
    if(options.cat != "" && options.cat != iterator->first) continue;
    
    double scale_uncert;
    double scale_uncert_error;
    double pdf_uncert;
    double nnpdf_uncert;
    double nnpdf_uncert_error;
    double alpha_s_uncert;
    double alpha_s_uncert_error;
    
    bool intLabels = options.variable=="n_jets" || options.variable=="n_jetsingap" || options.variable=="n_bjets" || options.variable=="mva_olddm_vloose_2" || options.variable=="mva_olddm_tight_2";
    
    std::string cat_string = iterator->first;
    if(options.do2D && cat_string!="inclusive" && cat_string !="baseline"){
      cat_string.erase(cat_string.begin(),cat_string.begin()+2);
    }

    std::cout << "Category: " << cat_string << std::endl;
    std::string title = options.channel+" "+cat_string;
    
    std::string cat;
    if(cat_string == "inclusive") cat = "*("+iterator->second+")";
    else cat = "*("+iterator->second+" && "+alias_map_["baseline"]+")";
    std::cout << cat << std::endl;
    tree->Draw((options.variable+">>h_noweight").c_str(),(default_wt+cat).c_str(),"goff");
    
    if(options.doGen) tree->Draw("event>>htemp",(default_wt+cat).c_str(),"goff");  
    else tree->Draw("event>>htemp",(default_wt+cat).c_str(),"goff"); 
    double A = htemp->Integral(0,htemp->GetNbinsX()+1);
    bool check_non_zero = A>0;
    double A_error;
    htemp->IntegralAndError(0,htemp->GetNbinsX()+1,A_error);
    
    double fract_acceptance=0;
    double fract_acceptance_error=0;
    if(options.doAcceptance){
      fract_acceptance  = A/total_inclusive;
      std::cout << A << "   " << total_inclusive << std::endl;
      fract_acceptance_error = fract_acceptance * A_error/A;
      std::cout << "Category Acceptance = " << fract_acceptance*100 << " % +/- " << fract_acceptance_error*100 << " %" << std::endl;
      
      if(options.makePlots && cat_string=="baseline"){
        TFile *f = new TFile((options.outputDirname+"/AcceptancePlots.root").c_str(),"UPDATE");
        std::string new_name = options.variable+"_"+"_"+options.channel+"_"+options.sample;
        TH1D *h_to_write = new TH1D(new_name.c_str(),"",std::get<1>(labels_map_[options.variable]),std::get<2>(labels_map_[options.variable]),std::get<3>(labels_map_[options.variable])); 
        tree->Draw((options.variable+">>"+new_name).c_str(),(default_wt+cat).c_str(),"goff");
        h_to_write->Scale(1/total_inclusive);
        h_to_write->Write();
        delete h_to_write;
        f->Close();
        delete f;
      }
    }
    
    if(options.doScale){
      double Amin_scale = A;
      double Amax_scale = A;
      unsigned max_index = 0;
      unsigned min_index = 0;
      
      std::vector<TH1D*> hists;
      hists.push_back((TH1D*)h_noweight->Clone());
      
      for(unsigned i=0; i<9; ++i){
        if(i==0 || i==5 || i==7) continue;
        std::string add_wt = Form("%f",scale_variation_addwt[i]);
        std::string wt_name = Form("%s*scale_variation_wts[%u]",default_wt.c_str(),i);
        if(options.doPtWeight){
          if(i == 4) wt_name = Form("wt*wt_ggh_pt_scalehigh*scale_variation_wts[%u]",i);
          if(i == 8) wt_name = Form("wt*wt_ggh_pt_scalelow*scale_variation_wts[%u]",i);
        }
//         add_wt= "1.0";//delete this!!
        wt_name+="*"+add_wt;
        wt_name+=cat;
        tree->Draw((options.variable+">>htemp").c_str(),wt_name.c_str(),"goff");  
        double wt_sum = htemp->Integral(0,htemp->GetNbinsX()+1);
        if(i == 4){
          max_index = i;
          Amax_scale = wt_sum;
        }
        if(i == 8){
          min_index = i;
          Amin_scale = wt_sum;
        }
        if(i == 4 || i == 8) hists.push_back((TH1D*)htemp->Clone());
      }
      
      scale_uncert = (Amax_scale-Amin_scale)/(2*A);
      
      std::string wt_name = Form("%s*(scale_variation_wts[%u]*%f - scale_variation_wts[%u]*%f)",default_wt.c_str(),max_index,scale_variation_addwt[max_index],min_index,scale_variation_addwt[min_index]);
      wt_name+=cat;
      tree->Draw((options.variable+">>htemp").c_str(),wt_name.c_str(),"goff");
      double MaxDiff_error;
      htemp->IntegralAndError(0,htemp->GetNbinsX()+1,MaxDiff_error);
      
      scale_uncert_error = scale_uncert*sqrt(pow(MaxDiff_error/(Amax_scale-Amin_scale),2) + pow(A_error/A,2));
      scale_uncert_error=std::fabs(scale_uncert_error);
      std::cout << Amax_scale << "   " << Amin_scale << std::endl;
      std::cout << "A = " << A << std::endl;
      std::cout << "Scale Uncertainty = " << scale_uncert*100 << " % +/- " << scale_uncert_error*100 << " %" << std::endl;
      
      if(options.makePlots){
        std::string out_name = options.outputDirname+"/"+options.variable+"_"+options.channel+"_scale_variations_"+cat_string;
        if(options.doGen) out_name = options.outputDirname+"/gen_"+options.variable+"_"+options.channel+"_scale_variations_"+cat_string;
        std::vector<std::string> legend_entries;
        legend_entries.push_back("#mu_{R}=1, #mu_{F}=1"    );
        //legend_entries.push_back("#mu_{R}=1, #mu_{F}=2"    );
        //legend_entries.push_back("#mu_{R}=1, #mu_{F}=0.5"  );
        //legend_entries.push_back("#mu_{R}=2, #mu_{F}=1"    );
        legend_entries.push_back("#mu_{R}=2, #mu_{F}=2"    );
        //legend_entries.push_back("#mu_{R}=0.5, #mu_{F}=1"  );
        legend_entries.push_back("#mu_{R}=0.5, #mu_{F}=0.5");
        bool bottomLeg = options.variable.find("mva_olddm_")!=std::string::npos;
        DrawHist(hists, legend_entries, out_name, true,true,title,intLabels,false,0.3,bottomLeg);
      }
    }
    
    if(options.doPDF){
        
      std::vector<TH1D*> hists;
        
      std::vector<double> AMinus;
      std::vector<double> APlus;
      for(unsigned i=0; i<100; ++i){
        std::string add_wt = Form("%f",NNPDF_addwt[i]);
        std::string wt_name = Form("%s*NNPDF_wts[%u]",default_wt.c_str(),i);
        wt_name+="*"+add_wt;
        wt_name+=cat;
        tree->Draw((options.variable+">>htemp").c_str(),wt_name.c_str(),"goff"); 
        double wt_sum = htemp->Integral(0,htemp->GetNbinsX()+1);
        if(wt_sum >= A) APlus.push_back(wt_sum);
        else if(wt_sum < A) AMinus.push_back(wt_sum);
        if(options.makePlots){
          h_nnpdf->SetBinContent(i,wt_sum);    
        }
      }
      if(options.makePlots) hists.push_back(h_nnpdf);
      
      std::vector<double> A_all;
      A_all.push_back(A);
      A_all.insert(A_all.end(), AMinus.begin(), AMinus.end());
      A_all.insert(A_all.end(), APlus.begin(), APlus.end());
      
      std::sort(A_all.begin(), A_all.end(), greater());
      
      double sd = StandDev(A_all);

      TH1D *h = new TH1D("h","",20,A_all.back()*0.5,A_all.front()*1.5);
      for(unsigned i=0; i<A_all.size(); ++i){
        h->Fill(A_all[i]);    
      }
      nnpdf_uncert = sd/A;//= h->GetRMS()/A;
      nnpdf_uncert_error = nnpdf_uncert*sqrt(pow(h->GetRMSError()/h->GetRMS(),2) + pow(A_error/A,2));
      
      std::cout << "NNPDF Uncertainty = " << nnpdf_uncert*100 << " % +/- " << nnpdf_uncert_error*100 << " %" << std::endl;
      
      if(options.makePlots){
        SetStyle();
        h->SetStats(100001100);
        h->SetLineColor(4);
        h->GetYaxis()->SetTitle("Entries");
        h->GetXaxis()->SetTitle("A_{pdf-member}/A_{default}");
        TCanvas *c1 = new TCanvas();
        h->Draw();
        TLatex *tex = new TLatex(0.7,0.95,(title).c_str());
        tex->SetNDC();
        tex->SetTextFont(44);
        tex->SetTextSize(23);
        tex->SetLineWidth(2);
        tex->Draw();
        std::string out_name = options.outputDirname+"/"+options.channel+"_nnpdf_spread_"+cat_string;
        c1->Print((out_name+".pdf").c_str());
      }
      delete h;
    }
    
    if(options.doAlphaS){
      double Amin_as = A;
      double Amax_as = A;
      int min_index = -1;
      int max_index = -1;
      
      std::vector<TH1D*> hists;
      if(options.makePlots) hists.push_back((TH1D*)h_noweight->Clone());
      
      for(unsigned i=0; i<2; ++i){
        std::string add_wt = Form("%f",alpha_s_variation_addwt[i]);
        std::string wt_name = Form("%s*alpha_s_wts[%u]",default_wt.c_str(),i);
        wt_name+="*"+add_wt;
        wt_name+=cat;
        tree->Draw((options.variable+">>htemp").c_str(),wt_name.c_str(),"goff");  
        double wt_sum = htemp->Integral(0,htemp->GetNbinsX()+1);
        
        if(i == 1){
          max_index = i;
          Amax_as = wt_sum;
        }
        if(i == 0){
          min_index = i;
          Amin_as = wt_sum;
        }
        if(options.makePlots){
          hists.push_back((TH1D*)htemp->Clone());
        }
      }
      if(options.makePlots){
        std::string out_name = options.outputDirname+"/"+options.variable+"_"+options.channel+"_alpha_s_"+cat_string;
        if(options.doGen) out_name = options.outputDirname+"/gen_"+options.variable+"_"+options.channel+"_alpha_s_"+cat_string;
        std::vector<std::string> legend_entries;
        legend_entries.push_back("#alpha_{s} = 0.118");
        legend_entries.push_back("#alpha_{s} = 0.117");
        legend_entries.push_back("#alpha_{s} = 0.119");
        bool bottomLeg = options.variable.find("mva_olddm_")!=std::string::npos;
        DrawHist(hists, legend_entries, out_name, true,true, title, intLabels,false,0.006, bottomLeg);
      }
      
      alpha_s_uncert = 1.5*(Amax_as-Amin_as)/(2*A);
      
      std::string max_weight_string;
      if(max_index != -1) max_weight_string = Form("alpha_s_wts[%u]*%f",max_index,alpha_s_variation_addwt[max_index]);
      else max_weight_string = "1";
      std::string min_weight_string;
      if(min_index != -1) min_weight_string = Form("alpha_s_wts[%u]*%f",min_index,alpha_s_variation_addwt[min_index]);
      else min_weight_string = "1";
      std::string wt_name = default_wt+"*("+max_weight_string+"-"+min_weight_string+")";
      wt_name+=cat;
      tree->Draw((options.variable+">>htemp").c_str(),wt_name.c_str(),"goff");
      double MaxDiff_error;
      htemp->IntegralAndError(0,htemp->GetNbinsX()+1,MaxDiff_error);
      
      alpha_s_uncert_error = alpha_s_uncert*sqrt(pow(MaxDiff_error/(Amax_as-Amin_as),2) + pow(A_error/A,2));
      alpha_s_uncert_error=std::fabs(alpha_s_uncert_error);
      
      std::cout << "Alpha_s Uncertainty = " << alpha_s_uncert*100 << " % +/- " << alpha_s_uncert_error*100 << " %" << std::endl;
    }
    
    if(check_non_zero){
      uncert_output << cat_string;
      if(options.doScale) uncert_output << "\t" << scale_uncert << "\t" << scale_uncert_error;
      if(options.doPDF) uncert_output << "\t" << nnpdf_uncert << "\t" << nnpdf_uncert_error;
      if(options.doPDF) uncert_output << "\t" << pdf_uncert;
      if(options.doAlphaS) uncert_output << "\t" << alpha_s_uncert << "\t" << alpha_s_uncert_error;
      if(options.doAcceptance) uncert_output << "\t" << fract_acceptance << "\t" << fract_acceptance_error;
      uncert_output << std::endl;
    }
    
  }
  uncert_output.close();
  
  //f->Close();
  f0->Close();
  
  return 0;
}
