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
#include "TGraphAsymmErrors.h"

#include "TGraphErrors.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plot.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/Plotting.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"

using namespace std;

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

void DrawGraph(TGraph *graph, std::string print_name, std::string title){
  SetStyle();
  TCanvas *c1 = new TCanvas();
  graph->GetXaxis()->SetTitle("Real electron efficiency");
  graph->GetYaxis()->SetTitle("Fake electron rejection");
  graph->SetMarkerColor(2);
  graph->SetLineColor(2);
  graph->Draw("AL");
  
  TLatex *tex = new TLatex(0.7,0.95,(title).c_str());
  tex->SetNDC();
  tex->SetTextFont(44);
  tex->SetTextSize(23);
  tex->SetLineWidth(2);
  tex->Draw();
  c1->Print((print_name+".pdf").c_str());
}

TGraph DivideGraphs(TGraph* gr1, TGraph *gr2){
  TGraph gr_out;
  for(unsigned i=0; i<(unsigned)gr1->GetN(); ++i){
    double x1; double x2; double y1; double y2;
    gr1->GetPoint(i,x1,y1);  
    gr2->GetPoint(i,x2,y2);
    double ratio = y1/y2;
    gr_out.SetPoint(i, x1, ratio);      
  }
  return gr_out;
}

void DrawMultiGraph(std::vector<TGraph*> graphs, std::string print_name, std::vector<std::string> legend_entries, std::string title){
  SetStyle();
  TCanvas *c1 = new TCanvas();
  TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  pad1->SetBottomMargin(0);
  pad1->SetFillStyle(4000);
  pad1->SetFrameFillStyle(4000);       
  pad1->Draw();             
  pad1->cd();
  double ymax = 0.15 + 0.05*graphs.size();
  TLegend *leg = new TLegend(0.15,0.15,0.5,ymax);
  leg->SetTextSize(0.03);
  leg->SetFillStyle(0);
  TMultiGraph mg;
  for(unsigned i=0; i<graphs.size();++i){
    graphs[i]->SetMarkerColor((i+1)*2);
    graphs[i]->SetLineColor((i+1)*2);
    graphs[i]->SetName(Form("g%u",i));
    graphs[i]->Draw("AL");
    mg.Add(graphs[i],"L");
    if(legend_entries.size() > i) leg->AddEntry(Form("g%u",i),legend_entries[i].c_str(),"PL");
  }
  mg.Draw("AL");
  float scale1 = pad1->GetAbsHNDC();
  mg.GetYaxis()->SetLabelSize(mg.GetYaxis()->GetLabelSize()/scale1);
  mg.GetYaxis()->SetTitleSize(mg.GetYaxis()->GetTitleSize()/scale1);
  mg.SetMinimum(0.001);
  mg.Draw("AL");
  mg.GetXaxis()->SetTitle("Real electron efficiency");
  mg.GetYaxis()->SetTitle("Fake electron rejection");
  mg.GetYaxis()->SetTitleOffset(1.1);
  if(legend_entries.size()>0) leg->Draw();
  c1->Update();
  
  TGraph ratio;
  if(graphs.size()>1) ratio = DivideGraphs(graphs[0], graphs[1]);
  
  c1->cd();
  TPad *pad2 = new TPad("pad2", "pad2", 0, 0.02, 1, 0.3);
  pad2->SetFillStyle(4000);
  pad2->SetFrameFillStyle(4000); 
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.27);
  pad2->Draw();
  pad2->cd();
  ratio.GetYaxis()->SetTitle("Ratio");
  ratio.GetYaxis()->SetTitleOffset(0.4);
  ratio.GetYaxis()->CenterTitle();
  ratio.GetXaxis()->SetTitle("Real electron efficiency");
  float scale = pad2->GetAbsHNDC();
  ratio.GetXaxis()->SetLabelSize(ratio.GetXaxis()->GetLabelSize()/scale);
  ratio.GetYaxis()->SetLabelSize(ratio.GetYaxis()->GetLabelSize()/scale);
  ratio.GetXaxis()->SetTitleSize(ratio.GetXaxis()->GetTitleSize()/scale);
  ratio.GetYaxis()->SetTitleSize(ratio.GetYaxis()->GetTitleSize()/scale);
  double ratio_max = 1+0.3;
  double ratio_min = 1-0.1;
  ratio.SetMinimum(ratio_min);
  ratio.SetMaximum(ratio_max);
  ratio.GetYaxis()->SetNdivisions(3,5,0);
  ratio.Draw("AL");
  c1->cd();
  
  TLatex *tex = new TLatex(0.7,0.95,(title).c_str());
  tex->SetNDC();
  tex->SetTextFont(44);
  tex->SetTextSize(23);
  tex->SetLineWidth(2);
  tex->Draw();
  
  c1->Print((print_name+".pdf").c_str());
  for(unsigned i=0; i<graphs.size();++i) delete graphs[i];
}

TGraph* ROCCurve(TTree *tree, std::string gen_sel, std::string sel, std::string variable){
    
  TH1D real = TH1D("real", "",500,-1,1);
  TH1D fake = TH1D("fake", "",500,-1,1);
  
  std::string real_sel = sel+"*"+gen_sel;
  std::string fake_sel = sel+"*(gen_match==6)";

  tree->Draw((variable+">>real").c_str(),real_sel.c_str(),"goff");  
  tree->Draw((variable+">>fake").c_str(),fake_sel.c_str(),"goff");
  
  real.Scale(1./real.Integral());
  fake.Scale(1./fake.Integral());
    
  std::vector<double> mva_real_eff;
  std::vector<double> mva_fake_rej;  
    
  for(int i=1;i<=500;i++){
    mva_real_eff.push_back(1-real.Integral(1,i));     
    mva_fake_rej.push_back(fake.Integral(1,i));  
  }
  
  TGraph *roc = new TGraph(500,&(mva_real_eff[0]),&(mva_fake_rej[0]));
  return roc;
}

int main(int /*argc*/, char* /*argv*/[]){
  
  std::string file_name = "/vols/cms/dw515/Offline/output/VBFTest/VBFHToTauTau_M-125_2016_et_0.root";  
  std::string tree_name = "tree";
  TFile *file = TFile::Open(file_name.c_str());
  TTree *tree = (TTree*)file->Get(tree_name.c_str());

  std::string promptE_sel = "(gen_match==1)";
  std::string tauE_sel = "(gen_match==3)";
  
  std::map<std::string, std::string> det_region_;
  det_region_[""] = "(pt > 10)";
  det_region_["barrel1"] = "(fabs(eta) <= 0.8 && pt > 10)";
  det_region_["barrel2"] = "(fabs(eta) > 0.8 && fabs(eta) <= 1.479 && pt > 10)";
  det_region_["endcap"] = "(fabs(eta) > 1.479 && pt > 10)";
  
  //TGraph *promptE_roc_2015 = ROCCurve(tree, promptE_sel, "(pt>10)","electronmvaID");
  //TGraph *tauE_roc_2015 = ROCCurve(tree, tauE_sel, "(pt>10)","electronmvaID");
  //TGraph *promptE_roc_2016 = ROCCurve(tree, promptE_sel, "(pt>10)","id_2016");
  //TGraph *tauE_roc_2016 = ROCCurve(tree, tauE_sel, "(pt>10)","id_2016");
  //
  //std::vector<TGraph*> tauE_comp;
  //std::vector<std::string> tauE_comp_leg;
  //tauE_comp.push_back((TGraph*)tauE_roc_2016->Clone());
  //tauE_comp_leg.push_back("GeneralPurpose MVA 2016");
  //tauE_comp.push_back((TGraph*)tauE_roc_2015->Clone());
  //tauE_comp_leg.push_back("Non-Triggering MVA 2015");
  //
  //std::vector<TGraph*> promptE_comp;
  //std::vector<std::string> promptE_comp_leg;
  //promptE_comp.push_back((TGraph*)promptE_roc_2016->Clone());
  //promptE_comp_leg.push_back("General Purpose MVA 2016");
  //promptE_comp.push_back((TGraph*)promptE_roc_2015->Clone());
  //promptE_comp_leg.push_back("Non-Triggering MVA 2015");
  //
  //DrawMultiGraph(tauE_comp, "tauE_comp",tauE_comp_leg,"");
  //DrawMultiGraph(promptE_comp, "promptE_comp",promptE_comp_leg,"");
  //
  //DrawGraph(promptE_roc_2015, "promptE_2015");
  //DrawGraph(tauE_roc_2015, "tauE_2015");
  //DrawGraph(promptE_roc_2016, "promptE_2016");
  //DrawGraph(tauE_roc_2016, "tauE_2016");
  
  typedef std::map<std::string, std::string>::iterator it_type;
  for(it_type iterator = det_region_.begin(); iterator != det_region_.end(); iterator++){
    std::string title = iterator->first;
    std::string sel = iterator->second;
    
    TGraph *promptE_roc_2015 = ROCCurve(tree, promptE_sel, sel,"electronmvaID");
    TGraph *tauE_roc_2015 = ROCCurve(tree, tauE_sel, sel,"electronmvaID");
    TGraph *promptE_roc_2016 = ROCCurve(tree, promptE_sel, sel,"id_2016");
    TGraph *tauE_roc_2016 = ROCCurve(tree, tauE_sel, sel,"id_2016");
    
    std::vector<TGraph*> tauE_comp;
    std::vector<std::string> tauE_comp_leg;
    tauE_comp.push_back((TGraph*)tauE_roc_2016->Clone());
    tauE_comp_leg.push_back("GeneralPurpose MVA 2016");
    tauE_comp.push_back((TGraph*)tauE_roc_2015->Clone());
    tauE_comp_leg.push_back("Non-Triggering MVA 2015");
    
    std::vector<TGraph*> promptE_comp;
    std::vector<std::string> promptE_comp_leg;
    promptE_comp.push_back((TGraph*)promptE_roc_2016->Clone());
    promptE_comp_leg.push_back("General Purpose MVA 2016");
    promptE_comp.push_back((TGraph*)promptE_roc_2015->Clone());
    promptE_comp_leg.push_back("Non-Triggering MVA 2015");
    
    DrawMultiGraph(tauE_comp, "tauE_comp_"+title,tauE_comp_leg,title);
    DrawMultiGraph(promptE_comp, "promptE_comp_"+title,promptE_comp_leg,title);
    
    DrawGraph(promptE_roc_2015, "promptE_2015_"+title,title);
    DrawGraph(tauE_roc_2015, "tauE_2015_"+title,title);
    DrawGraph(promptE_roc_2016, "promptE_2016_"+title,title);
    DrawGraph(tauE_roc_2016, "tauE_2016_"+title,title);
  }
  
  return 0;
}








