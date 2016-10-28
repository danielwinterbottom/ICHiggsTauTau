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

double StandDev(std::vector<double> a, double a0) {
  unsigned N = a.size();
  double sum=0;
  if(N>0){ 
    for(unsigned i=0; i<N; ++i){
      sum+=pow(a[i]-a0,2);  
    }
    sum = sum/N;
  }
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

void DrawHist(std::vector<TH1D*> hists, std::vector<std::string> legend_entries, std::string out_name, bool ratio, bool Normalize, std::string title, bool intLabels, bool log){
  bool doLegend = legend_entries.size() > 0;
  SetStyle();
  TGaxis::SetMaxDigits(3);
  THStack *hs = new THStack("hs","");
  TCanvas *c1 = new TCanvas();
  if(log) c1->SetLogy();
  double ymin = 0.8 - 0.05*hists.size();
  TLegend *leg = new TLegend(0.65,ymin,0.8,0.8);
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
   hs_ratio->SetMinimum(0.7);
   hs_ratio->SetMaximum(1.3);
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
        ("input",                  po::value<std::string>(&options.input)->default_value("/vols/cms/dw515/Offline/CMSSW_8_0_9/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/Aug11_PreSel_sv_v6/VBFHToTauTau_M-125_mt_2016.root"))
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
        ("doAcceptance",           po::value<bool>(&options.doAcceptance)->default_value(false));
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
  
  if(!file_exists(options.outputDirname)){
    std::cout << "Output folder does not exist! Exiting." << std::endl;
    return 0;    
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
      if(options.channel == "em") alias_map_["baseline"] = "(mt_1<50 && passed && pt_1>13 && pt_2>10 && eta_1<2.5 && eta_2<2.4)";
      else if(options.channel == "et") alias_map_["baseline"] = "(mt_1<50 && passed && pt_1>26 && pt_2>20 && eta_1<2.1 && eta_2<2.3)";
      else if(options.channel == "mt") alias_map_["baseline"] = "(mt_1<50 && passed && pt_1>23 && pt_2>20 && eta_1<2.4 && eta_2<2.3)";
    } else if (options.channel == "tt"){
      alias_map_["baseline"] = "(passed && pt_1>40 && pt_2>40 && eta_1<2.1 && eta_2<2.1)";
    }
  } else{
    if(options.channel == "em"){
      alias_map_["baseline"] = "(iso_1<0.15 && iso_2<0.15 && !leptonveto && mt_1<50)";
    } else if (options.channel == "et" || options.channel == "mt"){
      alias_map_["baseline"] = "(iso_1<0.15 && mva_olddm_medium_2>0.5 && antiele_2 && antimu_2 && !leptonveto && mt_1<50)";    
    } else if (options.channel == "tt"){
      alias_map_["baseline"] = "(mva_olddm_tight_1>0.5 && mva_olddm_tight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)";
    }
  }
  
  if(!options.doGen){
    alias_map_["vbf_vtight"] = "(n_jets>=2 && jdeta>4. && mjj>800 && n_bjets==0 && n_jetsingap==0 && pt_tt>100)";
    alias_map_["vbf_vvtight"] = "(n_jets>=2 && jdeta>4. && mjj>900 && n_bjets==0 && n_jetsingap==0 && pt_tt>100)";
    if (options.channel == "em" || options.channel == "et" || options.channel == "mt"){
      alias_map_["vbf_run1"] = "(n_jets>=2 && jdeta>3.5 && mjj>500 && n_bjets==0 && n_jetsingap==0)";
      alias_map_["vbf_tight"] = "(n_jets>=2 && jdeta>4. && mjj>700 && n_bjets==0 && n_jetsingap==0 && pt_tt>100)";
      alias_map_["vbf_loose"] = "(!"+alias_map_["vbf_tight"]+" && n_jets>=2 && jdeta>3.5 && mjj>500 && n_bjets==0 && n_jetsingap==0)";    
      if(options.channel == "et"){
        alias_map_["1jet_low"] = "(!"+alias_map_["vbf_run1"]+" && n_jets>=1 && n_bjets==0 && met>30 && pt_2<=45)";
        alias_map_["1jet_high_boosted"] = "(!"+alias_map_["vbf_run1"]+" && n_jets>=1 && n_bjets==0 && met>30 && pt_tt>100 && pt_2>45)";
      } else if(options.channel == "mt"){
        alias_map_["1jet_low"] = "(!"+alias_map_["vbf_run1"]+" && n_jets>=1 && n_bjets==0 && pt_2<=45)";
        alias_map_["1jet_high"] = "(!"+alias_map_["vbf_run1"]+" && n_jets>=1 && n_bjets==0 && pt_tt<=100 && pt_2>45)";
        alias_map_["1jet_high_boosted"] = "(!"+alias_map_["vbf_run1"]+" && n_jets>=1 && n_bjets==0 && pt_tt>100 && pt_2>45)";
      } else if(options.channel == "em"){
        alias_map_["1jet_low"] = "(!"+alias_map_["vbf_run1"]+" && n_jets>=1 && n_bjets==0 && pt_2<=45)";
        alias_map_["1jet_high"] = "(!"+alias_map_["vbf_run1"]+" && n_jets>=1 && n_bjets==0 && pt_2>45)";
      }
      if(options.channel == "em"){
        alias_map_["0jet_low"] = "(n_jets==0 && pt_2<=35)";
        alias_map_["0jet_high"] = "(n_jets==0 && pt_2>35)";
      } else{
        alias_map_["0jet_low"] = "(n_jets==0 && pt_2<=45)";
        alias_map_["0jet_high"] = "(n_jets==0 && pt_2>45)";
      }
    }
    else if (options.channel == "tt"){
      alias_map_["vbf"] = "(n_jets>=2 && jdeta>3.5 && mjj>500 && n_bjets==0 && pt_tt>100 && n_jetsingap==0)";
      alias_map_["1jet_run1_tt"] = "(!"+alias_map_["vbf"]+" && n_jets>=1 && n_bjets==0 && pt_tt>100)";
      alias_map_["1jet_lowboost"] = "(!"+alias_map_["vbf"]+" && n_jets>=1 && n_bjets==0 && pt_tt>100 && pt_tt<=170)";
      alias_map_["1jet_highboost"] = "(!"+alias_map_["vbf"]+" && n_jets>=1 && n_bjets==0 && pt_tt>170 )";
      alias_map_["0jet"] = "(!"+alias_map_["vbf"]+" && !"+alias_map_["1jet_run1_tt"]+" && n_jets>=0 && n_bjets==0)";
    }
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
  TH1D *h_ct10 = new TH1D("h_ct10","",50,0,50);
  h_ct10->Sumw2();
  TH1D *h_mmht = new TH1D("h_mmht","",50,0,50);
  h_mmht->Sumw2();
  
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
  
  double total_inclusive = 0;
  if(options.doAcceptance){
    gen_tree->Draw("event>>htemp","","goff");  
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
        std::string wt_name = Form("scale_variation_wts[%u]",i);
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
  }
  std::vector<double> NNPDF_addwt;
  std::vector<double> CT10_addwt;
  std::vector<double> MMHT_addwt;
  if(options.doPDF){
    std::string input_name;
    bool file_check;
    input_name = options.outputDirname+"/NNPDF_addwt.txt";
    file_check = file_exists(input_name) && check_lines(input_name,100);
    if(!file_check || options.RecreateRenorm){
      std::cout << "Creating new NNPDF remormalization file: " << input_name << std::endl;
      std::ofstream output(input_name);
      for(unsigned i=0; i<100; ++i){
        std::string wt_name = Form("NNPDF_wts[%u]",i);
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
    input_name = options.outputDirname+"/CT10_addwt.txt";
    file_check = file_exists(input_name) && check_lines(input_name,51);
    if(!file_check || options.RecreateRenorm){
      std::cout << "Creating new CT10 remormalization file: " << input_name << std::endl;
      std::ofstream output(input_name);
      for(unsigned i=0; i<51; ++i){
        std::string wt_name = Form("CT10_wts[%u]",i);
        gen_tree->Draw("event>>htemp",wt_name.c_str(),"goff");  
        double wt_sum = htemp->Integral(0,htemp->GetNbinsX()+1);
        CT10_addwt.push_back(nominal_wt/wt_sum);
        output << nominal_wt/wt_sum << std::endl;
      }
      output.close();
    } else{
      std::cout << "Using existing CT10 remormalization weights from file: " << input_name << ". Use option --RecreateRenorm to force new remormalization." << std::endl;
      std::ifstream input(input_name);
      double temp;
      while(input >> temp){
        CT10_addwt.push_back(temp);
      }
      input.close();
    }
    input_name = options.outputDirname+"/MMHT_addwt.txt";
    file_check = file_exists(input_name) && check_lines(input_name,51);
    if(!file_check || options.RecreateRenorm){
      std::cout << "Creating new MMHT remormalization file: " << input_name << std::endl;
      std::ofstream output(input_name);
      for(unsigned i=0; i<51; ++i){
        std::string wt_name = Form("MMHT_wts[%u]",i);
        gen_tree->Draw("event>>htemp",wt_name.c_str(),"goff");  
        double wt_sum = htemp->Integral(0,htemp->GetNbinsX()+1);
        MMHT_addwt.push_back(nominal_wt/wt_sum);
        output << nominal_wt/wt_sum << std::endl;
      }
      output.close();
    } else{
      std::cout << "Using existing MMHT remormalization weights from file: " << input_name << ". Use option --RecreateRenorm to force new remormalization." << std::endl;
      std::ifstream input(input_name);
      double temp;
      while(input >> temp){
        MMHT_addwt.push_back(temp);
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
        std::string wt_name = Form("alpha_s_wts[%u]",i);
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
  if(options.doScale) uncert_output << "\tScale";
  if(options.doPDF) uncert_output << "\tNNPDF";
  if(options.doPDF) uncert_output << "\tPDF";
  if(options.doAlphaS) uncert_output << "\tAlpha_S";
  if(options.doAcceptance) uncert_output << "\tAcceptance";
  uncert_output << std::endl;
  
  for(it_type iterator = alias_map_.begin(); iterator != alias_map_.end(); iterator++) {
    
    double scale_uncert;
    double pdf_uncert;
    double nnpdf_uncert;
    double alpha_s_uncert;
    
    bool intLabels = options.variable=="n_jets" || options.variable=="n_jetsingap" || options.variable=="n_bjets";
      
    std::cout << "Category: " << iterator->first << std::endl;
    std::string title = options.channel+" "+iterator->first;
    
    std::string cat;
    if(iterator->first == "inclusive") cat = "*("+iterator->second+")";
    else cat = "*("+iterator->second+" && "+alias_map_["baseline"]+")";

    tree->Draw((options.variable+">>h_noweight").c_str(),("1"+cat).c_str(),"goff");
    double A = h_noweight->Integral(0,h_noweight->GetNbinsX()+1);
    
    double fract_acceptance=0;
    if(options.doAcceptance){
      fract_acceptance  = A/total_inclusive;
      std::cout << "Category Acceptance = " << fract_acceptance*100 << " %" << std::endl;
    }
    
    if(options.doScale){
      double Amin_scale = A;
      double Amax_scale = A;
      
      std::vector<TH1D*> hists;
      if(options.makePlots) hists.push_back((TH1D*)h_noweight->Clone());
      
      for(unsigned i=0; i<9; ++i){
        if(i==0 || i==5 || i==7) continue;
        std::string add_wt = Form("%f",scale_variation_addwt[i]);
        std::string wt_name = Form("scale_variation_wts[%u]",i);
        wt_name+="*"+add_wt;
        wt_name+=cat;
        tree->Draw((options.variable+">>htemp").c_str(),wt_name.c_str(),"goff");  
        double wt_sum = htemp->Integral(0,htemp->GetNbinsX()+1);
        Amin_scale = std::min(wt_sum,Amin_scale);
        Amax_scale = std::max(wt_sum,Amax_scale);
        if(options.makePlots){
          hists.push_back((TH1D*)htemp->Clone());
        }
      }
      
      if(options.makePlots){
        std::string out_name = options.outputDirname+"/"+options.variable+"_"+options.channel+"_scale_variations_"+iterator->first;
        std::vector<std::string> legend_entries;
        legend_entries.push_back("#mu_{R}=1, #mu_{F}=1"    );
        legend_entries.push_back("#mu_{R}=1, #mu_{F}=2"    );
        legend_entries.push_back("#mu_{R}=1, #mu_{F}=0.5"  );
        legend_entries.push_back("#mu_{R}=2, #mu_{F}=1"    );
        legend_entries.push_back("#mu_{R}=2, #mu_{F}=2"    );
        legend_entries.push_back("#mu_{R}=0.5, #mu_{F}=1"  );
        legend_entries.push_back("#mu_{R}=0.5, #mu_{F}=0.5");
        DrawHist(hists, legend_entries, out_name, true,true,title,intLabels,false);
      }
      
      scale_uncert = (Amax_scale-Amin_scale)/(2*A);
      std::cout << "Scale Uncertainty = " << scale_uncert*100 << " %" <<std::endl;
    }
    
    if(options.doPDF){
        
      std::vector<TH1D*> hists;
      
      std::vector<double> DeltaMax;
      std::vector<double> DeltaMin;
        
      std::vector<double> AMinus;
      std::vector<double> APlus;
      for(unsigned i=0; i<100; ++i){
        std::string add_wt = Form("%f",NNPDF_addwt[i]);
        std::string wt_name = Form("NNPDF_wts[%u]",i);
        wt_name+="*"+add_wt;
        wt_name+=cat;
        tree->Draw((options.variable+">>htemp").c_str(),wt_name.c_str(),"goff");  
        double wt_sum = htemp->Integral(0,htemp->GetNbinsX()+1);
        if(wt_sum > A) APlus.push_back(wt_sum);
        else if(wt_sum < A) AMinus.push_back(wt_sum);
        if(options.makePlots){
          h_nnpdf->SetBinContent(i,wt_sum);    
        }
      }
      if(options.makePlots) hists.push_back(h_nnpdf);
      double DeltaMax_NNPDF = A + StandDev(APlus,A);
      double DeltaMin_NNPDF = A - StandDev(AMinus,A);
      DeltaMax.push_back(DeltaMax_NNPDF);
      DeltaMin.push_back(DeltaMin_NNPDF);
      
      if(options.makePlots){
        std::vector<double> A_all;
        A_all.push_back(A);
        A_all.insert(A_all.end(), AMinus.begin(), AMinus.end());
        A_all.insert(A_all.end(), APlus.begin(), APlus.end());
        TH1D *h = new TH1D("h","",20,0.95,1.05);
        for(unsigned i=0; i<A_all.size(); ++i){
          h->Fill(A_all[i]/A);    
        }
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
        std::string out_name = options.outputDirname+"/"+options.channel+"_nnpdf_spread_"+iterator->first;
        c1->Print((out_name+".pdf").c_str());
        delete h;
      }
      
      
      nnpdf_uncert = (DeltaMax_NNPDF - DeltaMin_NNPDF)/(2*A);
      std::cout << "A:" << A << std::endl;
      std::cout << "nnpdf + = " << (A + StandDev(APlus,A)) << std::endl;
      std::cout << "nnpdf - = " << (A - StandDev(AMinus,A)) << std::endl;
      
      std::vector<double> A_CT10;
      double a0_CT10=0;
      for(unsigned i=0; i<51; ++i){
        std::string add_wt = Form("%f",CT10_addwt[i]);
        std::string wt_name = Form("CT10_wts[%u]",i);
        wt_name+="*"+add_wt;
        wt_name+=cat;
        tree->Draw((options.variable+">>htemp").c_str(),wt_name.c_str(),"goff");  
        double wt_sum = htemp->Integral(0,htemp->GetNbinsX()+1);
        if(i == 0) a0_CT10 = wt_sum;
        else A_CT10.push_back(wt_sum);
        if(options.makePlots){
          h_ct10->SetBinContent(i,wt_sum);    
        }
      }
      if(options.makePlots) hists.push_back(h_ct10);
      std::pair<double,double> CT10_uncert = PDFUncert(A_CT10,a0_CT10, 1.645);
      double DeltaMax_CT10 = a0_CT10 + CT10_uncert.first;
      double DeltaMin_CT10 = a0_CT10 - CT10_uncert.second;
      DeltaMax.push_back(DeltaMax_CT10);
      DeltaMin.push_back(DeltaMin_CT10);
      
      std::cout << "ct10 + = " << (DeltaMax_CT10) << std::endl;
      std::cout << "ct10 - = " << (DeltaMin_CT10) << std::endl;
      
      std::vector<double> A_MMHT;
      double a0_MMHT=0;
      for(unsigned i=0; i<51; ++i){
        std::string add_wt = Form("%f",MMHT_addwt[i]);
        std::string wt_name = Form("MMHT_wts[%u]",i);
        wt_name+="*"+add_wt;
        wt_name+=cat;
        tree->Draw((options.variable+">>htemp").c_str(),wt_name.c_str(),"goff");  
        double wt_sum = htemp->Integral(0,htemp->GetNbinsX()+1);
        if(i == 0) a0_MMHT = wt_sum;
        else A_MMHT.push_back(wt_sum);
        if(options.makePlots){
          h_mmht->SetBinContent(i,wt_sum);    
        }
      }
      if(options.makePlots) hists.push_back(h_mmht);
      std::pair<double,double> MMHT_uncert = PDFUncert(A_MMHT,a0_MMHT, 1);
      double DeltaMax_MMHT = a0_MMHT + MMHT_uncert.first;
      double DeltaMin_MMHT = a0_MMHT - MMHT_uncert.second;
      DeltaMax.push_back(DeltaMax_MMHT);
      DeltaMin.push_back(DeltaMin_MMHT);
      
      std::cout << "mmht10 + = " << (DeltaMax_MMHT) << std::endl;
      std::cout << "mmht10 - = " << (DeltaMin_MMHT) << std::endl;
      
      std::sort(DeltaMax.begin(), DeltaMax.end(), greater());
      std::sort(DeltaMin.begin(), DeltaMin.end(), less_than());
      
      if(options.makePlots){
        std::string out_name = options.outputDirname+"/"+options.variable+"_"+options.channel+"_pdf_acceptance_"+iterator->first;
        std::vector<std::string> legend_entries;
        legend_entries.push_back("NNPDF");
        legend_entries.push_back("CT10");
        legend_entries.push_back("MMHT");
        DrawHist(hists, legend_entries, out_name, false,false, title, intLabels,false);
      }
      
      pdf_uncert = (DeltaMax[0] - DeltaMin[0])/(2*A);
      std::cout << "NNPDF Uncertainty = " << nnpdf_uncert*100 << std::endl;
      std::cout << "PDF Uncertainty = " << pdf_uncert*100 << " %" << std::endl;
    }
    
    if(options.doAlphaS){
      double Amin_as = A;
      double Amax_as = A;
      
      std::vector<TH1D*> hists;
      if(options.makePlots) hists.push_back((TH1D*)h_noweight->Clone());
      
      for(unsigned i=0; i<2; ++i){
        std::string add_wt = Form("%f",alpha_s_variation_addwt[i]);
        std::string wt_name = Form("alpha_s_wts[%u]",i);
        wt_name+="*"+add_wt;
        wt_name+=cat;
        tree->Draw((options.variable+">>htemp").c_str(),wt_name.c_str(),"goff");  
        double wt_sum = htemp->Integral(0,htemp->GetNbinsX()+1);
        Amin_as = std::min(wt_sum,Amin_as);
        Amax_as = std::max(wt_sum,Amax_as);
        if(options.makePlots){
          hists.push_back((TH1D*)htemp->Clone());
        }
      }
      if(options.makePlots){
        std::string out_name = options.outputDirname+"/"+options.variable+"_"+options.channel+"_alpha_s_"+iterator->first;
        std::vector<std::string> legend_entries;
        legend_entries.push_back("#alpha_{s} = 0.118");
        legend_entries.push_back("#alpha_{s} = 0.117");
        legend_entries.push_back("#alpha_{s} = 0.119");
        DrawHist(hists, legend_entries, out_name, true,true, title, intLabels,false);
      }
      
      alpha_s_uncert = 1.5*(Amax_as-Amin_as)/(2*A);
      std::cout << "Alpha_s Uncertainty = " << alpha_s_uncert*100 << " %" << std::endl;
    }

    uncert_output << iterator->first;
    if(options.doScale) uncert_output << "\t" << scale_uncert;
    if(options.doPDF) uncert_output << "\t" << nnpdf_uncert;
    if(options.doPDF) uncert_output << "\t" << pdf_uncert;
    if(options.doAlphaS) uncert_output << "\t" << alpha_s_uncert;
    if(options.doAcceptance) uncert_output << "\t" << fract_acceptance;
    uncert_output << std::endl;
    
  }
  uncert_output.close();
  
  
  return 0;
}
