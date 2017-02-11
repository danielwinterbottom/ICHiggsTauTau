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
#include <sstream> 
#include <cstdlib>
#include <math.h> 

//boost
#include "boost/program_options.hpp"

namespace po = boost::program_options;

struct greater {
    template<class T>
    bool operator()(T const &a, T const &b) const { return a > b; }
};

std::vector<std::string> SeperateString(std::string str){
  std::vector<std::string> string_vec;
  std::string word;
  std::stringstream stream(str);
  while( getline(stream, word, ',') ){
    string_vec.push_back(word);
  }
  return string_vec;
}

std::pair<TH1D*, TH1D*> StandDev(std::vector<TH1D*> hist_vec) {
   
  TH1D* hist_up = (TH1D*)hist_vec[0]->Clone();
  TH1D* hist_down = (TH1D*)hist_vec[0]->Clone();
  
  for(unsigned j=1; j<=(unsigned)hist_vec[0]->GetNbinsX();++j){
    std::vector<double> a;
    for(unsigned k=0; k<hist_vec.size(); ++k){
      double bin_content = hist_vec[k]->GetBinContent(j);
      a.push_back(bin_content);    
    }
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
    double sd = sqrt(sum);
    
    hist_up->SetBinContent(j,hist_up->GetBinContent(j)+sd);
    hist_down->SetBinContent(j,hist_down->GetBinContent(j)-sd);
    
  }
  return std::make_pair(hist_up,hist_down);
}

std::pair<TH1D*, TH1D*> ScaleError(std::vector<TH1D*> hist_vec) {
   
  TH1D* hist_up = (TH1D*)hist_vec[0]->Clone();
  TH1D* hist_down = (TH1D*)hist_vec[0]->Clone();
  
  for(unsigned j=1; j<=(unsigned)hist_vec[0]->GetNbinsX();++j){
    std::vector<double> a;
    for(unsigned k=0; k<hist_vec.size(); ++k){
      double bin_content = hist_vec[k]->GetBinContent(j);
      a.push_back(bin_content);    
    }
    std::sort(a.begin(), a.end(), greater());

    
    hist_up->SetBinContent(j,a.front());
    hist_down->SetBinContent(j,a.back());
    
  }
  return std::make_pair(hist_up,hist_down);
}

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
  SetStyle();
  std::vector<TH1D*> error_hists;
  std::string error_name = "";
  for(unsigned i=0; i<legend_entries.size(); ++i){
    double norm = 1./hists[0]->Integral();
    if(legend_entries[i] == "scale_up") error_name = "QCD Scale uncertainty";
    if(legend_entries[i] == "pdf_up") error_name = "PDF uncertainty";
    if(legend_entries[i] == "scale_up" || legend_entries[i] == "pdf_up"){
      //hists[i]->SetLineColor(10);
      hists[i]->SetLineWidth(0);
      hists[i]->SetMarkerStyle(0);
      hists[i]->SetFillStyle(1001);
      hists[i]->SetFillColor(16);
      if(Normalize) hists[i]->Scale(norm);
      error_hists.insert(error_hists.begin(),(TH1D*)hists[i]->Clone());
      hists.erase (hists.begin()+i);
      legend_entries.erase (legend_entries.begin()+i);
    }
    if(legend_entries[i] == "scale_down" || legend_entries[i] == "pdf_down"){
      hists[i]->SetLineColor(10);
      hists[i]->SetLineWidth(1);
      hists[i]->SetMarkerStyle(0);
      hists[i]->SetFillStyle(1001);
      hists[i]->SetFillColor(10);
      if(Normalize) hists[i]->Scale(norm);
      error_hists.push_back((TH1D*)hists[i]->Clone()); 
      hists.erase (hists.begin()+i);
      legend_entries.erase (legend_entries.begin()+i);
    }
  }
  bool doLegend = legend_entries.size() > 0;
  TGaxis::SetMaxDigits(3);
  THStack *hs = new THStack("hs","");
  if(error_hists.size()==2){
    hs->Add((TH1D*)error_hists[0]->Clone());
    hs->Add((TH1D*)error_hists[1]->Clone());
  }
  
  TCanvas *c1 = new TCanvas();
  if(log) c1->SetLogy();
  double ymin = 0.87 - 0.05*hists.size();
  TLegend *leg = new TLegend(0.60,ymin,0.87,0.87);
  if(bottomLeg){
    double ymax = 0.1 + 0.05*hists.size();
    leg = new TLegend(0.75,0.1,0.87,ymax);    
  }
  leg->SetTextSize(0.025);
  leg->SetFillStyle(0);
  for(unsigned i=0; i<hists.size(); ++i){
    int col_num = i+1;
    if(col_num==7) col_num = 9;
    hists[i]->SetLineColor(col_num);
    hists[i]->SetMarkerColor(col_num);
    //for(unsigned j=0; j<(unsigned)hists[i]->GetNbinsX()+1; ++j){
    //  hists[i]->SetBinError(j,0.00001);    
    //} //get rid of vartical xerror bars by setting them to small value - purley for aesthetic reasons
    TH1D *htemp = (TH1D*)hists[i]->Clone();
    if(Normalize){
      double norm = 1./hists[0]->Integral();
      htemp->Scale(norm);
      hists[i]->GetYaxis()->SetTitle("Normalized Entries");
    }
    hs->Add(htemp);
    std::string leg_string = "";
    if(i<legend_entries.size()) leg_string = legend_entries[i];
    leg->AddEntry(hists[i],leg_string.c_str(),"lpe");
  }
  
  if(error_hists.size()==2){
    leg->AddEntry(error_hists[0],error_name.c_str(),"f");      
  }
  
  if(!ratio){
    hs->Draw("nostack pe");
    
    if(error_hists.size()==2){
      error_hists[0]->Draw("HISTsame");
      error_hists[1]->Draw("HISTsame");
    }
    hs->Draw("nostack pe same");
    
    hs->GetXaxis()->SetTitle(hists[0]->GetXaxis()->GetTitle());
    hs->GetYaxis()->SetTitle(hists[0]->GetYaxis()->GetTitle());
    if(intLabels){
     TGaxis *axis = (TGaxis*)hs->GetXaxis();
     axis->SetOption("I");
    }
    hs->Draw("nostack pe same");
    c1->Update();
    if(doLegend) leg->Draw();
  } else{
   TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
   pad1->SetBottomMargin(0);
   pad1->SetFillStyle(4000);
   pad1->SetFrameFillStyle(4000);       
   pad1->Draw();             
   pad1->cd();
   //hs->Draw("nostack pe");
   hs->Draw("nostack pe");
   
   if(error_hists.size()==2){
     error_hists[0]->Draw("HISTsame");
     error_hists[1]->Draw("HISTsame");
   }
   hs->Draw("nostack pe same");
   
   float scale1 = pad1->GetAbsHNDC();
   hs->GetYaxis()->SetLabelSize(hs->GetYaxis()->GetLabelSize()/scale1);
   hs->GetYaxis()->SetTitleSize(hs->GetYaxis()->GetTitleSize()/scale1);
   hs->GetYaxis()->SetTitle(hists[0]->GetYaxis()->GetTitle());
   hs->GetYaxis()->SetTitleOffset(1.1);
   hs->SetMinimum(hs->GetMaximum()/100000);
   //hs->Draw("nostack pe");
   hs->Draw("nostack pe same");
   gPad->RedrawAxis();
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
   hs_ratio->Draw("nostack pe");
   
   if(error_hists.size()==2){
     std::vector<TH1D*> ratio_error_hists;
     ratio_error_hists.push_back((TH1D*)error_hists[0]->Clone());
     ratio_error_hists.push_back((TH1D*)error_hists[1]->Clone());
     ratio_error_hists[0]->Divide(h_denum);
     ratio_error_hists[1]->Divide(h_denum);
     ratio_error_hists[0]->Draw("HISTsame");
     ratio_error_hists[1]->Draw("HISTsame");
   }
   hs_ratio->Draw("nostack pe same");
   
   
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
   hs_ratio->Draw("nostack pe same"); 
   
   gPad->RedrawAxis();
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
    
  SetStyle();
    
  std::string variable;
  std::string cfg;
  std::string channel;
  std::string cat;
  std::string outputDirname;
  bool doGen;
  bool do2DBinning;
  bool doPDF;
  bool doScale;
  bool NormScale;
  bool RecreateRenorm;
  double ggRatioSize;
  double qqRatioSize;
  std::string SamplesToSkip;
  
  po::options_description preconfig("Pre-Configuration");
  preconfig.add_options()("cfg", po::value<std::string>(&cfg)->default_value("config.cfg"));
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);
  po::options_description config("Configuration");
  config.add_options()
        ("outputDirname",          po::value<std::string>(&outputDirname)->default_value("generator_comparisons"))
        ("cat",                    po::value<std::string>(&cat)->default_value("inclusive"))
        ("channel",                po::value<std::string>(&channel)->default_value("mt"))
        ("doGen",                  po::value<bool>(&doGen)->default_value(true))
        ("doPDF",                  po::value<bool>(&doPDF)->default_value(false))
        ("doScale",                po::value<bool>(&doScale)->default_value(false))
        ("NormScale",              po::value<bool>(&NormScale)->default_value(true))
        ("RecreateRenorm",         po::value<bool>(&RecreateRenorm)->default_value(false))
        ("do2DBinning",            po::value<bool>(&do2DBinning)->default_value(false))
        ("ggRatioSize",            po::value<double>(&ggRatioSize)->default_value(0.3))
        ("qqRatioSize",            po::value<double>(&qqRatioSize)->default_value(0.03))
        ("SamplesToSkip",          po::value<std::string>(&SamplesToSkip)->default_value(""))
        ("variable",               po::value<std::string>(&variable)->default_value("m_vis"));
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
  po::notify(vm);
  
  std::cout << "===== program options ====="         << std::endl;
  std::cout << "outputDirname  = " << outputDirname  << std::endl;
  std::cout << "cat            = " << cat            << std::endl;
  std::cout << "channel        = " << channel        << std::endl;
  std::cout << "variable       = " << variable       << std::endl;
  std::cout << "doGen          = " << doGen          << std::endl;
  std::cout << "doPDF          = " << doPDF          << std::endl;
  std::cout << "doScale        = " << doScale        << std::endl;
  std::cout << "NormScale      = " << NormScale      << std::endl;
  std::cout << "RecreateRenorm = " << RecreateRenorm << std::endl;
  std::cout << "ggRatioSize    = " << ggRatioSize    << std::endl;
  std::cout << "qqRatioSize    = " << qqRatioSize    << std::endl;
  std::cout << "SamplesToSkip  = " << SamplesToSkip  << std::endl;
  std::cout << "do2DBinning    = " << do2DBinning    << std::endl;
 
  std::map<std::string, std::tuple<std::string,int,double,double>> labels_map_;
  labels_map_["m_vis"] = std::make_tuple("M_{vis} [GeV]",25,0,200); //change back to 50!
  labels_map_["m_sv"] = std::make_tuple("M_{SVFit} [GeV]",50,0,200);
  labels_map_["mt_1"] = std::make_tuple("M_{T} [GeV]",30,0,200);
  labels_map_["m_vis_res"]= std::make_tuple("(M_{vis}_{reco}-M_{vis}_{gen})/M_{vis}_{gen}",25,-1,1);
  labels_map_["m_vis_diff"]= std::make_tuple("M_{vis}_{reco}-M_{vis}_{gen} [GeV]",25,-100,100);
  labels_map_["lep1_matched"]= std::make_tuple("lep 1 matched to gen",2,0,2);
  labels_map_["lep2_matched"]= std::make_tuple("lep 1 matched to gen",2,0,2);
  labels_map_["met"] = std::make_tuple("MET [GeV]",30,0,150);
  labels_map_["mva_olddm_tight_2"] = std::make_tuple("Tau Isolation Tight WP",2,0,2);
  labels_map_["mva_olddm_medium_2"] = std::make_tuple("Tau Isolation Medium WP",2,0,2);
  labels_map_["mva_olddm_loose_2"] = std::make_tuple("Tau Isolation Loose WP",2,0,2);
  labels_map_["mva_olddm_vloose_2"] = std::make_tuple("Tau Isolation V.Loose WP",2,0,2);
  labels_map_["mva_olddm_tight_1"] = std::make_tuple("Tau Isolation Tight WP",2,0,2);
  labels_map_["mva_olddm_medium_1"] = std::make_tuple("Tau Isolation Medium WP",2,0,2);
  labels_map_["mva_olddm_loose_1"] = std::make_tuple("Tau Isolation Loose WP",2,0,2);
  labels_map_["mva_olddm_vloose_1"] = std::make_tuple("Tau Isolation V.Loose WP",2,0,2);
  if(channel == "em"){
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
  } else if(channel == "et"){
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
  } else if(channel == "mt"){
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
  } else if(channel == "tt"){
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
  labels_map_["pt_tt"] = std::make_tuple("Higgs p_{T} [GeV]",10,0,300);
  labels_map_["HiggsPt"] = std::make_tuple("Higgs p_{T} [GeV]",10,0,300);
  labels_map_["FirstHiggsPt"] = std::make_tuple("FirstHiggsPt p_{T} [GeV]",10,0,300);
  labels_map_["jpt_1"] = std::make_tuple("leading jet p_{T} [GeV]",30,0,200);
  labels_map_["jpt_2"] = std::make_tuple("sub-leading jet p_{T} [GeV]",30,0,200);
  labels_map_["jeta_1"] = std::make_tuple("leading jet #eta",30,-6,6);
  labels_map_["jeta_2"] = std::make_tuple("sub-leading jet #eta",30,-6,6);
  labels_map_["mjj"] = std::make_tuple("M_{jj} [GeV]",30,0,2000);
  labels_map_["jdeta"] = std::make_tuple("#Delta#eta_{jj}",25,0,10);
  labels_map_["n_jets"] = std::make_tuple("Number of jets",5,0,5);
  labels_map_["n_jets_nofilter"] = std::make_tuple("Number of jets [No Filtering]",10,0,10);
  labels_map_["n_jetsingap"] = std::make_tuple("Number of jets in gap",5,0,5);
  labels_map_["n_bjets"] = std::make_tuple("Number of b-jets",5,0,5);
  
  labels_map_["jdphi"] = std::make_tuple("#Delta#phi_{jj}",50,-8,8);
  std::map<std::string, std::string> alias_map_;
  
  alias_map_["inclusive"] = "(1)";
  alias_map_["passed"] = "(passed)";
  //alias_map_["baseline"] = "(1)";
  //alias_map_["0jet"] = "(n_jets==0)";
  //alias_map_["1jet"] = "(n_jets==1)";
  //alias_map_["2jet"] = "(n_jets==2)";
  alias_map_["njetge2"] = "(n_jets>=2)";
  //alias_map_["njetge2_CJV"] = "(n_jets>=2 && n_jetsingap==0)";
  
  if(channel == "em"){
    alias_map_["baseline"] = "(iso_1<0.15 && iso_2<0.2 && !leptonveto)";
  } else if (channel == "et" || channel == "mt"){
    alias_map_["baseline"] = "(iso_1<0.1 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && !leptonveto && mt_1<50)";    
  } else if (channel == "mt"){
    alias_map_["baseline"] = "(iso_1<0.15 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && !leptonveto && mt_1<50)";    
  } else if (channel == "tt"){
    alias_map_["baseline"] = "(mva_olddm_tight_1>0.5 && mva_olddm_tight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)";
  }
  
  if(doGen){
    if(channel == "em" || channel == "et" || channel == "mt"){
      if(channel == "em") alias_map_["baseline"] = "(passed && pt_1>13 && pt_2>10 && eta_1<2.5 && eta_2<2.4)";
      else if(channel == "et") alias_map_["baseline"] = "(mt_1<50 && passed && pt_1>26 && pt_2>20 && eta_1<2.1 && eta_2<2.3)";
      else if(channel == "mt") alias_map_["baseline"] = "(mt_1<50 && passed && pt_1>23 && pt_2>20 && eta_1<2.4 && eta_2<2.3)";
    } else if (channel == "tt"){
      alias_map_["baseline"] = "(passed && pt_1>40 && pt_2>40 && eta_1<2.1 && eta_2<2.1)";
    }
  }
  std::string iso_cut_string;
  if(channel == "et" || channel == "em") iso_cut_string = "0.1";
  else iso_cut_string = "0.15";
  //alias_map_["0jet_high"] = "(pt_2>50 && n_jets==0 && antiele_2 && antimu_2 && !leptonveto && mt_1<50)"; 
  alias_map_["0jet_high_vloose"] = "(pt_2>50 && n_jets==0)*(iso_1<"+iso_cut_string+" && mva_olddm_vloose_2>0.5 && antiele_2 && antimu_2 && !leptonveto && mt_1<50)";
  alias_map_["0jet_high_loose"] = "(pt_2>50 && n_jets==0)*(iso_1<"+iso_cut_string+" && mva_olddm_loose_2>0.5 && antiele_2 && antimu_2 && !leptonveto && mt_1<50)";
  alias_map_["0jet_high_medium"] = "(pt_2>50 && n_jets==0)*(iso_1<"+iso_cut_string+" && mva_olddm_medium_2>0.5 && antiele_2 && antimu_2 && !leptonveto && mt_1<50)";
  alias_map_["0jet_high_vtight"] = "(pt_2>50 && n_jets==0)*(iso_1<"+iso_cut_string+" && mva_olddm_vtight_2>0.5 && antiele_2 && antimu_2 && !leptonveto && mt_1<50)";
  //alias_map_["0jet_high"] = "(pt_2>50 && n_jets==0)*(iso_1<"+iso_cut_string+" && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && !leptonveto && mt_1<50)";
  alias_map_["0jet_high"] = "(pt_2>50 && n_jets==0)*(iso_1<"+iso_cut_string+" && mva_olddm_medium_2>0.5)";
  alias_map_["0jet_high_nopt"] = "(n_jets==0)*(iso_1<"+iso_cut_string+" && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && !leptonveto && mt_1<50)";
  alias_map_["0jet_high_noiso1"] = "(pt_2>50 && n_jets==0)*(mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && !leptonveto && mt_1<50 &&pt_1>26 &&eta_1<2.1)";
  alias_map_["0jet_high_notauiso"] = "(pt_2>50 && n_jets==0)*(iso_1<"+iso_cut_string+" && antiele_2 && antimu_2 && !leptonveto && mt_1<50)";
  alias_map_["0jet_high_noantiele"] = "(pt_2>50 && n_jets==0)*(iso_1<"+iso_cut_string+" && mva_olddm_tight_2>0.5 && antimu_2 && !leptonveto && mt_1<50)";
  alias_map_["0jet_high_noantimu"] = "(pt_2>50 && n_jets==0)*(iso_1<"+iso_cut_string+" && mva_olddm_tight_2>0.5 && antiele_2 && !leptonveto && mt_1<50)";
  alias_map_["0jet_high_noleptonveto"] = "(pt_2>50 && n_jets==0)*(iso_1<"+iso_cut_string+" && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && mt_1<50)";
  alias_map_["0jet_high_nomt"] = "(pt_2>50 && n_jets==0)*(iso_1<"+iso_cut_string+" && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && !leptonveto)";
  
      if (channel == "em" || channel == "et" || channel == "mt"){   
      if(channel == "et"){
        alias_map_["vbf_high"] = "(n_jets==2 && mjj>800 && pt_tt>100)*"+alias_map_["baseline"]; 
        alias_map_["vbf_low"] = "(n_jets==2 && mjj>500 && (mjj<800 || pt_tt<100))*"+alias_map_["baseline"]; 
        alias_map_["1jet_high"] = "((n_jets==1 || (n_jets==2 && mjj<500)) && pt_tt>140)*"+alias_map_["baseline"]; 
        alias_map_["1jet_low"] = "(n_jets==1 || (n_jets==2 && mjj<500)) && ((pt_2>30 && pt_2<40) || (pt_2>40 && pt_tt<140))*"+alias_map_["baseline"]; 
        alias_map_["0jet_high"] = "(pt_2>50 && n_jets==0)*"+alias_map_["baseline"]; 
        alias_map_["0jet_low"] = "(pt_2>20 && pt_2<50 && n_jets==0)*"+alias_map_["baseline"]; 
      } else if(channel == "mt"){
        alias_map_["vbf_high"] = "(n_jets==2 && mjj>800 && pt_tt>100)*"+alias_map_["baseline"]; 
        alias_map_["vbf_low"] = "(n_jets==2 && mjj>500 && (mjj<800 || pt_tt<100))*"+alias_map_["baseline"]; 
        alias_map_["1jet_high"] = "((n_jets==1 || (n_jets==2 && mjj<500)) && pt_tt>140)*"+alias_map_["baseline"]; 
        alias_map_["1jet_low"] = "(n_jets==1 || (n_jets==2 && mjj<500)) && ((pt_2>30 && pt_2<40) || (pt_2>40 && pt_tt<140))*"+alias_map_["baseline"]; 
        alias_map_["0jet_high"] = "(pt_2>50 && n_jets==0)*"+alias_map_["baseline"]; 
        alias_map_["0jet_low"] = "(pt_2>20 && pt_2<50 && n_jets==0)*"+alias_map_["baseline"]; 
      } else if(channel == "em"){
        alias_map_["vbf_high"] = "(pt_2>15 && n_jets==2 && mjj>800 && pzeta>-10)*"+alias_map_["baseline"]; 
        alias_map_["vbf_low"] = "(pt_2>15 && n_jets==2 && mjj>500 && mjj<800 && pzeta>-10)*"+alias_map_["baseline"]; 
        alias_map_["1jet_high"] = "(pt_2>35 && (n_jets==1 || (n_jets==2 && mjj < 500)) && pzeta>-35)*"+alias_map_["baseline"]; 
        alias_map_["1jet_low"] = "(pt_2>15 && pt_2<35 && pzeta>-35 && (n_jets==1 || (n_jets==2 && mjj < 500)))*"+alias_map_["baseline"]; 
        alias_map_["0jet_high"] = "(pt_2>35 && pzeta>-35 && n_jets==0)*"+alias_map_["baseline"]; 
        alias_map_["0jet_low"] = "(pt_2>15 && pt_2<35 && pzeta>-35 && n_jets==0)*"+alias_map_["baseline"]; 
      }
    }
    else if (channel == "tt"){
      alias_map_["vbf_high"] = "(pt_1>50 && n_jets>=2 && pt_tt>100 && mjj>500 && jdeta>2.5 && n_jetsingap<1)*"+alias_map_["baseline"]; 
      alias_map_["vbf_low"] = "(pt_1>50 && n_jets>=2 && jdeta>2.5 && n_jetsingap < 1)*((pt_tt<100 && mjj>300) || (pt_tt>100 && mjj>300 && mjj<500))*"+alias_map_["baseline"]; 
      alias_map_["1jet_high"] = "(pt_1>50 && pt_tt>170)*(n_jets==1 || (n_jets>=2 && !(mjj>300 && jdeta>2.5 && n_jetsingap<1)))*"+alias_map_["baseline"]; 
      alias_map_["1jet_low"] = "(pt_1>50 && pt_tt>100 && pt_tt<170)*(n_jets==1 || (n_jets>=2 && !(mjj>300 && jdeta>2.5 && n_jetsingap<1)))*"+alias_map_["baseline"]; 
      alias_map_["0jet"] = "(pt_1>50 && n_jets==0)*"+alias_map_["baseline"]; 
    }
  
  
  if(channel == "et" || channel == "mt"){
      alias_map_["0jet"] = "(pt_2>30 && n_jets==0)*"+alias_map_["baseline"];
      alias_map_["boosted"] = "(pt_2>30 && (n_jets==1 || (n_jets==2 && mjj<300) || n_jets>2))*"+alias_map_["baseline"];
      alias_map_["vbf"]  = "(pt_2>30 && n_jets==2 && mjj>300)*"+alias_map_["baseline"]; 
    } else if(channel == "em"){
      alias_map_["0jet"] = "(pt_2>15 && pzeta>-35 && n_jets==0)*"+alias_map_["baseline"];
      alias_map_["boosted"] = "(pt_2>15 && pzeta>-35 && (n_jets==1 || (n_jets==2 && mjj<300) || n_jets>2))*"+alias_map_["baseline"];
      alias_map_["vbf"]  = "(pt_2>15 && pzeta>-10 && n_jets==2 && mjj>300)*"+alias_map_["baseline"]; 
    } else if(channel == "tt"){
      alias_map_["0jet"] = "(pt_1>50 && n_jets==0)*"+alias_map_["baseline"];
      alias_map_["boosted"] = "((pt_1>50 && n_jets==1 || ((n_jets>=2)* !(jdeta>2.5 && n_jetsingap<1 && pt_tt>100))))*"+alias_map_["baseline"];
      alias_map_["vbf"]  = "((pt_1>50 && n_jets>=2)*(jdeta>2.5 && n_jetsingap<1 && pt_tt>100))*"+alias_map_["baseline"]; 
    }
    
  alias_map_["central_jets"] = "(fabs(jeta_1)<2 && fabs(jeta_2)<2)";
    
  alias_map_["tau_iso!=tight"] = "(mva_olddm_tight_2<0.5)";   
  if(channel == "tt") alias_map_["tau_iso!=tight"] = "(mva_olddm_tight_1<0.5 && mva_olddm_tight_2<0.5)";
    
  //std::vector<std::string> sig = {"qqH"};
  std::vector<std::string> sig = {"ggH", "qqH"};
  std::string file_prefix = "/vols/cms/dw515/Offline/output/PartonShower9/";
  
  TH1D *htemp;
  
  std::vector<std::string> skip;
  skip=SeperateString(SamplesToSkip);
  
  for(unsigned i=0; i<sig.size(); ++i){
      
    std::map<std::string, std::string>  filenames_;
    std::string sig_string;
    if(sig[i] == "ggH") sig_string = "GluGlu";
    if(sig[i] == "qqH") sig_string = "VBF";
    filenames_["aMC@NLO"]             = file_prefix+sig_string+"HToTauTau_amcatnlo_M-125_"+channel+"_2016.root"; 
    filenames_["PowHeg_Pythia"]              = file_prefix+sig_string+"HToTauTau_M-125_"+channel+"_2016.root";
    filenames_["PowHeg_PythiaFragDown"] = file_prefix+sig_string+"HToTauTau_PythiaFragment_Down_M-125_"+channel+"_2016.root";
    filenames_["PowHeg_PythiaFragUp"]   = file_prefix+sig_string+"HToTauTau_PythiaFragment_Up_M-125_"+channel+"_2016.root";
    filenames_["PowHeg_Herwig++"]             = file_prefix+sig_string+"HToTauTau_herwigpp_M-125_"+channel+"_2016.root";
    
    typedef std::map<std::string, std::string>::iterator it_type;
    
    std::vector<TH1D*> hists;
    std::vector<std::string> legend_entries;
    
    for(it_type iterator = filenames_.begin(); iterator != filenames_.end(); iterator++){
        
      bool skip_sample = false;
      for(unsigned i=0; i<skip.size(); ++i){
        if (iterator->first.find(skip[i]) != std::string::npos){
          skip_sample = true;
        }
      }
      if(skip_sample) continue;
        
      TFile *f0 = new TFile(iterator->second.c_str());
      TTree *gen_tree = (TTree*)f0->Get("gen_ntuple");
      std::string tree_name;
      if(doGen) tree_name = "gen_ntuple";
      else              tree_name = "ntuple"; 
      TTree *tree = (TTree*)f0->Get(tree_name.c_str());
      
      double jdphi_ = -9999;
      TBranch *b1 = tree->Branch("jdphi",&jdphi_);
      
      if(variable == "jdphi"){
        double j1phi_; double j2phi_;
        tree->SetBranchAddress("jphi_1",&j1phi_);
        tree->SetBranchAddress("jphi_2",&j2phi_); 
        
        for(unsigned j=0; j<tree->GetEntries();++j){
          tree->GetEntry(j);
          
          if(std::fabs(j1phi_)<4 && std::fabs(j2phi_)<4){
            if(j1phi_<0) j1phi_+=2*M_PI;
            if(j2phi_<0) j2phi_+=2*M_PI;
            jdphi_ = j1phi_-j2phi_;
          }
          b1->Fill();
        }
      }
  
      
      std::vector<double> bins;
      
      if(channel == "et" || channel == "mt" || channel == "em"){
        if(variable == "mjj") bins = {300,700,1100,1500,3000};
        if(variable == "pt_tt") bins = {0,100,150,200,250,300};
        if(channel == "em"){ if(variable == "pt_2") bins = {15,20,25,30,35};}
        else if(variable == "pt_2") bins = {30,35,40,45,50,55}; 
      } else if (channel == "tt"){
        if(variable == "mjj") bins = {0,300,500,800};
        if(variable == "pt_tt") bins = {0,100,170,300};
        if(variable == "pt_1") bins = {50};  
      }
      
      double* bins_array = &bins[0];
      
      int  binnum = bins.size()-1;
      if(do2DBinning){
        htemp = new TH1D("htemp","",binnum,bins_array);
      } else{
        htemp = new TH1D("htemp","",std::get<1>(labels_map_[variable]),std::get<2>(labels_map_[variable]),std::get<3>(labels_map_[variable]));
      }
      htemp->Sumw2();
      std::string default_wt = "wt";
      gen_tree->Draw("event>>htemp",default_wt.c_str(),"goff");
      
      double total_inclusive = htemp->Integral(0,htemp->GetNbinsX()+1);
      std::string wt_string = default_wt+="*"+alias_map_[cat];
      tree->Draw((variable+">>htemp").c_str(),wt_string.c_str(),"goff");
      htemp->Scale(1/total_inclusive);
    
      htemp->GetXaxis()->SetTitle((std::get<0>(labels_map_[variable])).c_str());
      htemp->GetYaxis()->SetTitle("Normalized Entries");
      if(!doGen && iterator->first == "aMC@NLO") continue;
      hists.push_back((TH1D*)htemp->Clone());
      legend_entries.push_back(iterator->first);
      if(iterator->first == "PowHeg_Pythia"){
        std::reverse( hists.begin(), hists.end() );
        std::reverse( legend_entries.begin(), legend_entries.end() );
      }
      
      if(iterator->first == "PowHeg_Pythia"){
        if(doPDF){
          double nominal_wt = total_inclusive;
          std::vector<double> NNPDF_addwt;
            
          std::string input_name;
          bool file_check;
          input_name = outputDirname+"/NNPDF_addwt_"+sig[i]+".txt";
          file_check = file_exists(input_name) && check_lines(input_name,100);
          if(!file_check || RecreateRenorm){
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
          
          std::vector<TH1D*> hists_vec;
          
          tree->Draw((variable+">>htemp").c_str(),wt_string.c_str(),"goff");
          htemp->Scale(1/total_inclusive);
          hists_vec.push_back((TH1D*) htemp->Clone());

          for(unsigned i=0; i<100; ++i){
            std::string add_wt = Form("%f",NNPDF_addwt[i]);
            std::string wt_name = Form("%s*NNPDF_wts[%u]",default_wt.c_str(),i);
            wt_name+="*"+add_wt;
            wt_name+="*"+alias_map_[cat];
            tree->Draw((variable+">>htemp").c_str(),wt_name.c_str(),"goff"); 
            htemp->Scale(1/total_inclusive);
            hists_vec.push_back((TH1D*) htemp->Clone());

          }

          std::pair<TH1D*, TH1D*> pdf_pair = StandDev(hists_vec);
          
          std::vector<TH1D*> hists_pdf;
          hists_pdf.push_back(pdf_pair.first);
          hists_pdf.push_back(pdf_pair.second);
          
          hists.push_back(pdf_pair.first);
          legend_entries.push_back("pdf_up");
          hists.push_back(pdf_pair.second);
          legend_entries.push_back("pdf_down");
  
        } 
        
        if(doScale){
          double nominal_wt = total_inclusive;
          std::vector<double> scale_variation_addwt;
          std::string input_name = outputDirname+"/scale_variation_addwt_"+sig[i]+".txt";
          bool file_check = file_exists(input_name) && check_lines(input_name,9);
          if(!file_check || RecreateRenorm){
            std::cout << "Creating new scale remormalization file: " << input_name << std::endl;
            std::ofstream output(input_name);
            for(unsigned i=0; i<9; ++i){
              if(!NormScale) scale_variation_addwt.push_back(1.0);
              else{
                std::string wt_name = Form("%s*scale_variation_wts[%u]",default_wt.c_str(),i);
                gen_tree->Draw("event>>htemp",wt_name.c_str(),"goff");  
                double wt_sum = htemp->Integral(0,htemp->GetNbinsX()+1);
                scale_variation_addwt.push_back(nominal_wt/wt_sum);
                output << nominal_wt/wt_sum << std::endl;
              }
            }
            output.close();
          } else{
            std::cout << "Using existing scale remormalization weights from file: " << input_name << ". Use option --RecreateRenorm to force new remormalization." << std::endl;
            std::ifstream input(input_name);
            double temp;
            while(input >> temp){
              if(!NormScale) scale_variation_addwt.push_back(1.0);
              else scale_variation_addwt.push_back(temp);
            }
            input.close();
          }
          
          std::vector<TH1D*> scale_hists;
          
          for(unsigned i=0; i<9; ++i){
            if(i==5 || i==7) continue;
            std::string add_wt = Form("%f",scale_variation_addwt[i]);
            std::string wt_name = Form("%s*scale_variation_wts[%u]",default_wt.c_str(),i);

            wt_name+="*"+add_wt;
            wt_name+="*"+alias_map_[cat];
            tree->Draw((variable+">>htemp").c_str(),wt_name.c_str(),"goff");  
            htemp->Scale(1/total_inclusive);
            scale_hists.push_back((TH1D*)htemp->Clone());
          }
          
          std::pair<TH1D*, TH1D*> scale_errors = ScaleError(scale_hists);
          hists.push_back((TH1D*) scale_errors.first->Clone());
          hists.push_back((TH1D*) scale_errors.second->Clone());
          legend_entries.push_back("scale_up");
          legend_entries.push_back("scale_down");

        }
      }
    }
    
    std::string out_name = outputDirname+"/"+variable+"_"+channel+"_"+cat+"_"+sig[i]+"_generator_compare";
    if(doPDF) out_name = outputDirname+"/"+variable+"_"+channel+"_"+cat+"_"+sig[i]+"_generator_compare_withpdf";
    if(doScale) out_name = outputDirname+"/"+variable+"_"+channel+"_"+cat+"_"+sig[i]+"_generator_compare_withscale";
    bool intLabels = variable=="n_jets" || variable=="n_jets_nofilter" || variable=="n_jetsingap" || variable=="n_bjets" || variable=="mva_olddm_vloose_2" || variable=="mva_olddm_tight_2";
    std::string title = cat;
    title = "";
    bool bottomLeg = variable.find("mva_olddm_")!=std::string::npos;
    double RatioSize = 0.3;
    if(sig[i] == "ggH") RatioSize = ggRatioSize;
    if(sig[i] == "qqH") RatioSize = qqRatioSize;
    DrawHist(hists, legend_entries, out_name, true,false,title,intLabels,false,RatioSize, bottomLeg);
  }
  
  return 0;
}
