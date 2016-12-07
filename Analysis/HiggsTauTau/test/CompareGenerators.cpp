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
  double ymin = 0.87 - 0.05*hists.size();
  TLegend *leg = new TLegend(0.65,ymin,0.87,0.87);
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
  
  if(!ratio){
    hs->Draw("nostack pe");
    hs->GetXaxis()->SetTitle(hists[0]->GetXaxis()->GetTitle());
    hs->GetYaxis()->SetTitle(hists[0]->GetYaxis()->GetTitle());
    if(intLabels){
     TGaxis *axis = (TGaxis*)hs->GetXaxis();
     axis->SetOption("I");
    }
    hs->Draw("nostack pe");
    c1->Update();
    if(doLegend) leg->Draw();
  } else{
   TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
   pad1->SetBottomMargin(0);
   pad1->SetFillStyle(4000);
   pad1->SetFrameFillStyle(4000);       
   pad1->Draw();             
   pad1->cd();
   hs->Draw("nostack pe");
   float scale1 = pad1->GetAbsHNDC();
   hs->GetYaxis()->SetLabelSize(hs->GetYaxis()->GetLabelSize()/scale1);
   hs->GetYaxis()->SetTitleSize(hs->GetYaxis()->GetTitleSize()/scale1);
   hs->GetYaxis()->SetTitle(hists[0]->GetYaxis()->GetTitle());
   hs->GetYaxis()->SetTitleOffset(1.1);
   hs->SetMinimum(hs->GetMaximum()/100000);
   hs->Draw("nostack pe");
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
   hs_ratio->Draw("nostack pe"); 
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
        ("do2DBinning",            po::value<bool>(&do2DBinning)->default_value(false))
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
  //alias_map_["njetge2"] = "(n_jets>=2)";
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
    
  alias_map_["tau_iso!=tight"] = "(mva_olddm_tight_2<0.5)";   
  if(channel == "tt") alias_map_["tau_iso!=tight"] = "(mva_olddm_tight_1<0.5 && mva_olddm_tight_2<0.5)";
    
  std::vector<std::string> sig = {"ggH", "qqH"};
  std::string file_prefix = "/vols/cms/dw515/Offline/output/GenComps/";
  
  TH1D *htemp;
  
  for(unsigned i=0; i<sig.size(); ++i){
      
    std::map<std::string, std::string>  filenames_;
    std::string sig_string;
    if(sig[i] == "ggH") sig_string = "GluGlu";
    if(sig[i] == "qqH") sig_string = "VBF";
    filenames_["aMC@NLO"]             = file_prefix+sig_string+"HToTauTau_amcatnlo_M-125_"+channel+"_2016.root"; 
    filenames_["PowHeg"]              = file_prefix+sig_string+"HToTauTau_M-125_"+channel+"_2016.root";
    filenames_["PythiaFragmentDown"] = file_prefix+sig_string+"HToTauTau_PythiaFragment_Down_M-125_"+channel+"_2016.root";
    filenames_["PythiaFragmentUp"]   = file_prefix+sig_string+"HToTauTau_PythiaFragment_Up_M-125_"+channel+"_2016.root";
    filenames_["Herwig++"]             = file_prefix+sig_string+"HToTauTau_herwigpp_M-125_"+channel+"_2016.root";
    
    typedef std::map<std::string, std::string>::iterator it_type;
    
    std::vector<TH1D*> hists;
    std::vector<std::string> legend_entries;
    
    for(it_type iterator = filenames_.begin(); iterator != filenames_.end(); iterator++){
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
      
      //bins={300,3000};
      double* bins_array = &bins[0];
      
      int  binnum = bins.size()-1;
      if(do2DBinning){
        htemp = new TH1D("htemp","",binnum,bins_array);
      } else{
        htemp = new TH1D("htemp","",std::get<1>(labels_map_[variable]),std::get<2>(labels_map_[variable]),std::get<3>(labels_map_[variable]));
      }
      htemp->Sumw2();
      //std::string wt_string = "wt";
      std::string wt_string = "wt";
      gen_tree->Draw("event>>htemp",wt_string.c_str(),"goff");
      
      double total_inclusive = htemp->Integral(0,htemp->GetNbinsX()+1);
      wt_string = "wt*"+alias_map_[cat];
      std::cout << alias_map_[cat] << std::endl;
      tree->Draw((variable+">>htemp").c_str(),wt_string.c_str(),"goff");
      htemp->Scale(1/total_inclusive);
      //std::cout << iterator->first << std::endl;
      //for(int k=0; k<= htemp->GetNbinsX();++k){
      //  std::cout << "bin " << k << std::endl;
      //  std::cout << "bin error " << htemp->GetBinError(k) << std::endl;
      //  std::cout << "bin value " << htemp->GetBinContent(k) << std::endl;
      //}
    
      htemp->GetXaxis()->SetTitle((std::get<0>(labels_map_[variable])).c_str());
      htemp->GetYaxis()->SetTitle("Normalized Entries");
      if(!doGen && iterator->first == "aMC@NLO") continue;
      hists.push_back((TH1D*)htemp->Clone());
      legend_entries.push_back(iterator->first);
      if(iterator->first == "PowHeg"){
        std::reverse( hists.begin(), hists.end() );
        std::reverse( legend_entries.begin(), legend_entries.end() );
      }
    }
    
    //SetStyle();
    std::string out_name = outputDirname+"/"+variable+"_"+channel+"_"+cat+"_"+sig[i]+"_generator_compare";
    bool intLabels = variable=="n_jets" || variable=="n_jets_nofilter" || variable=="n_jetsingap" || variable=="n_bjets" || variable=="mva_olddm_vloose_2" || variable=="mva_olddm_tight_2";
    std::string title = cat;
    title = "";
    bool bottomLeg = variable.find("mva_olddm_")!=std::string::npos;
    DrawHist(hists, legend_entries, out_name, true,false,title,intLabels,false,0.15, bottomLeg);
  }
  
  return 0;
}
