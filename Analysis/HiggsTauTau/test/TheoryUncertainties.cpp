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
  myStyle->SetLineWidth(3);
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
  myStyle->SetMarkerSize(0);
  myStyle->SetMarkerStyle(1);
  gROOT->SetStyle("MyStyle");
  gROOT->UseCurrentStyle();
  gROOT->ForceStyle();    
}

void DrawHist(std::vector<TH1D*> hists, std::vector<std::string> legend_entries, std::string out_name){

  SetStyle();
  
  THStack *hs = new THStack("hs","");
  TCanvas *c1 = new TCanvas();
  double ymin = 0.8 - 0.05*hists.size();
  TLegend *leg = new TLegend(0.55,ymin,0.8,0.8);
  leg->SetTextSize(0.03);
  for(unsigned i=0; i<hists.size(); ++i){
    hists[i]->SetLineColor(i+1);
    hists[i]->SetMarkerColor(i+1);
    hs->Add(hists[i]);
    std::string leg_string = "";
    if(i<legend_entries.size()) leg_string = legend_entries[i];
    leg->AddEntry(hists[i],leg_string.c_str(),"l");
  }
  hs->Draw("nostack p");
  hs->GetXaxis()->SetTitle(hists[0]->GetXaxis()->GetTitle());
  hs->GetYaxis()->SetTitle(hists[0]->GetYaxis()->GetTitle());
  c1->Update();
  leg->Draw();
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
        ("input",                  po::value<std::string>(&options.input)->default_value("/vols/cms/dw515/Offline/CMSSW_8_0_9/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/Aug11_PreSel_sv_v6/GluGluHToTauTau_M-125_mt_2016.root"))
        ("outputFilename",         po::value<std::string>(&options.outputFilename)->default_value("theory_output"))
        ("makePlots",              po::value<bool>(&options.makePlots)->default_value(false))
        ("channel",                po::value<std::string>(&options.channel)->default_value("mt"))
        ("variable",               po::value<std::string>(&options.variable)->default_value("m_vis"))
        ("doGen",                  po::value<bool>(&options.doGen)->default_value(true))
        ("doScale",                po::value<bool>(&options.doScale)->default_value(true))
        ("doPDF",                  po::value<bool>(&options.doPDF)->default_value(true))
        ("doAlphaS",               po::value<bool>(&options.doAlphaS)->default_value(true))
        ("RecreateRenorm",         po::value<bool>(&options.RecreateRenorm)->default_value(false));
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(options.cfg.c_str(), config), vm);
  po::notify(vm);
  
  std::cout << "===== program options ====="                 << std::endl;
  std::cout << "input          = " << options.input          << std::endl;
  std::cout << "outputFilename = " << options.outputFilename << std::endl;
  std::cout << "makePlots      = " << options.makePlots      << std::endl;
  std::cout << "channel        = " << options.channel        << std::endl;
  std::cout << "variable       = " << options.variable       << std::endl;
  std::cout << "doGen          = " << options.doGen          << std::endl;
  std::cout << "doScale        = " << options.doScale        << std::endl;
  std::cout << "doPDF          = " << options.doPDF          << std::endl;
  std::cout << "doAlphaS       = " << options.doAlphaS       << std::endl;
  std::cout << "RecreateRenorm = " << options.RecreateRenorm << std::endl;
  
  if(!file_exists(options.outputFilename)){
    std::cout << "Output folder does not exist! Exiting." << std::endl;
    return 0;    
  }
  
  if(options.makePlots) SetStyle();
  std::map<std::string, std::tuple<std::string,double,double>> labels_map_;
  labels_map_["m_vis"] = std::make_tuple("M_{vis} [GeV]",0,300);
  labels_map_["m_sv"] = std::make_tuple("M_{SVFit} [GeV]",0,300);
  labels_map_["mt_1"] = std::make_tuple("M_{T} [GeV]",0,200);
  if(options.channel == "em"){
    labels_map_["pt_1"] = std::make_tuple("electron p_{T} [GeV]",0,200);
    labels_map_["pt_2"] = std::make_tuple("muon p_{T} [GeV]",0,200);
    labels_map_["eta_1"] = std::make_tuple("electron #eta",-3,3);
    labels_map_["eta_2"] = std::make_tuple("muon #eta",-3,3);
  } else if(options.channel == "et"){
    labels_map_["pt_1"] = std::make_tuple("electron p_{T} [GeV]",0,200);
    labels_map_["pt_2"] = std::make_tuple("tau p_{T} [GeV]",0,200);
    labels_map_["eta_1"] = std::make_tuple("electron #eta",-3,3);
    labels_map_["eta_2"] = std::make_tuple("tau #eta",-3,3);
  } else if(options.channel == "mt"){
    labels_map_["pt_1"] = std::make_tuple("muon p_{T} [GeV]",0,200);
    labels_map_["pt_2"] = std::make_tuple("tau p_{T} [GeV]",0,200);
    labels_map_["eta_1"] = std::make_tuple("muon #eta",-3,3);
    labels_map_["eta_2"] = std::make_tuple("tau #eta",-3,3);
  } else if(options.channel == "tt"){
    labels_map_["pt_1"] = std::make_tuple("leading tau p_{T} [GeV]",0,200);
    labels_map_["pt_2"] = std::make_tuple("sub-leading tau p_{T} [GeV]",0,200);
    labels_map_["eta_1"] = std::make_tuple("leading tau #eta",-3,3);
    labels_map_["eta_2"] = std::make_tuple("sub-leading tau #eta",-3,3);
  }
  labels_map_["jpt_1"] = std::make_tuple("leading jet p_{T} [GeV]",0,200);
  labels_map_["jpt_2"] = std::make_tuple("sub-leading jet p_{T} [GeV]",0,200);
  labels_map_["jeta_1"] = std::make_tuple("leading jet #eta",-6,6);
  labels_map_["jeta_2"] = std::make_tuple("sub-leading jet #eta",-6,6);
  labels_map_["mjj"] = std::make_tuple("M_{jj} [GeV]",0,1000);
  labels_map_["jdeta"] = std::make_tuple("#Delat#eta_{jj}",0,10);
  labels_map_["n_jets"] = std::make_tuple("Number of jets",0,5);
  labels_map_["n_jetsingap"] = std::make_tuple("Number of jets in gap",0,5);
  
  std::map<std::string, std::string> alias_map_;
  
  alias_map_["inclusive"] = "(1)";
  
  if(options.doGen){
    if(options.channel == "em" || options.channel == "et" || options.channel == "mt"){
      alias_map_["baseline"] = "(mt_1<50)";
    } else if (options.channel == "tt"){
      alias_map_["baseline"] = "(passed)";
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
  
  alias_map_["vbf"] = "(n_jets>=2 && jdeta>2 && mjj>200)";
  alias_map_["1jethigh"] = "(!"+alias_map_["vbf"]+" && n_jets>=1 && pt_2>35)";
  alias_map_["1jetlow"] = "(!"+alias_map_["vbf"]+" && n_jets>=1 && pt_2<=35)";
  alias_map_["0jethigh"] = "(!"+alias_map_["vbf"]+" && !"+alias_map_["1jethigh"]+" && !"+alias_map_["1jetlow"]+" && pt_2>35)";
  alias_map_["0jetlow"] = "(!"+alias_map_["vbf"]+" && !"+alias_map_["1jethigh"]+" && !"+alias_map_["1jetlow"]+" && pt_2<=35)";
  
  TFile *f0 = new TFile(options.input.c_str());
  TTree *gen_tree = (TTree*)f0->Get("gen_ntuple");
  TH1D *h0 = new TH1D("h0","",50,std::get<1>(labels_map_[options.variable]),std::get<2>(labels_map_[options.variable]));
  gen_tree->Draw((options.variable+">>h0").c_str(),"","goff");

  double nominal_wt = h0->Integral(0,h0->GetNbinsX()+1);
  
  TH1D *htemp = new TH1D("htemp","",50,std::get<1>(labels_map_[options.variable]),std::get<2>(labels_map_[options.variable]));
  
  std::vector<double> scale_variation_addwt;
  if(options.doScale){
    std::string input_name = options.outputFilename+"/scale_variation_addwt.txt";
    bool file_check = file_exists(input_name) && check_lines(input_name,9);
    if(!file_check || options.RecreateRenorm){
      std::cout << "Creating new scale remormalization file: " << input_name << std::endl;
      std::ofstream output(input_name);
      for(unsigned i=0; i<9; ++i){
        std::string wt_name = Form("scale_variation_wts[%u]",i);
        gen_tree->Draw((options.variable+">>htemp").c_str(),wt_name.c_str(),"goff");  
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
    input_name = options.outputFilename+"/NNPDF_addwt.txt";
    file_check = file_exists(input_name) && check_lines(input_name,100);
    if(!file_check || options.RecreateRenorm){
      std::cout << "Creating new NNPDF remormalization file: " << input_name << std::endl;
      std::ofstream output(input_name);
      for(unsigned i=0; i<100; ++i){
        std::string wt_name = Form("NNPDF_wts[%u]",i);
        gen_tree->Draw((options.variable+">>htemp").c_str(),wt_name.c_str(),"goff");  
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
    input_name = options.outputFilename+"/CT10_addwt.txt";
    file_check = file_exists(input_name) && check_lines(input_name,51);
    if(!file_check || options.RecreateRenorm){
      std::cout << "Creating new CT10 remormalization file: " << input_name << std::endl;
      std::ofstream output(input_name);
      for(unsigned i=0; i<51; ++i){
        std::string wt_name = Form("CT10_wts[%u]",i);
        gen_tree->Draw((options.variable+">>htemp").c_str(),wt_name.c_str(),"goff");  
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
    input_name = options.outputFilename+"/MMHT_addwt.txt";
    file_check = file_exists(input_name) && check_lines(input_name,51);
    if(!file_check || options.RecreateRenorm){
      std::cout << "Creating new MMHT remormalization file: " << input_name << std::endl;
      std::ofstream output(input_name);
      for(unsigned i=0; i<51; ++i){
        std::string wt_name = Form("MMHT_wts[%u]",i);
        gen_tree->Draw((options.variable+">>htemp").c_str(),wt_name.c_str(),"goff");  
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
    std::string input_name = options.outputFilename+"/alpha_s_variation_addwt.txt";
    bool file_check = file_exists(input_name) && check_lines(input_name,2);
    if(!file_check || options.RecreateRenorm){
      std::cout << "Creating new Alpha_S remormalization file: " << input_name << std::endl;
      std::ofstream output(input_name);
      for(unsigned i=0; i<2; ++i){
        std::string wt_name = Form("alpha_s_wts[%u]",i);
        gen_tree->Draw((options.variable+">>htemp").c_str(),wt_name.c_str(),"goff");  
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
  
  std::string tree_name;
  if(options.doGen) tree_name = "gen_ntuple";
  else              tree_name = "ntuple"; 
  TTree *tree = (TTree*)f0->Get(tree_name.c_str());
  TH1D *h_noweight = new TH1D("h_noweight","",50,std::get<1>(labels_map_[options.variable]),std::get<2>(labels_map_[options.variable])); 
  h_noweight->GetXaxis()->SetTitle((std::get<0>(labels_map_[options.variable])).c_str());
  h_noweight->GetYaxis()->SetTitle("Entries");
  typedef std::map<std::string, std::string>::iterator it_type;
  
  std::string uncert_output_name = options.outputFilename+"/uncertainties.txt";
  std::ofstream uncert_output(uncert_output_name);
  uncert_output << "Cat";
  if(options.doScale) uncert_output << "\t\tScale";
  if(options.doPDF) uncert_output << "\t\tPDF";
  if(options.doAlphaS) uncert_output << "\t\tAlpha_S";
  uncert_output << std::endl;
  
  for(it_type iterator = alias_map_.begin(); iterator != alias_map_.end(); iterator++) {
    
    double scale_uncert;
    double pdf_uncert;
    double alpha_s_uncert;
      
    std::cout << "Category: " << iterator->first << std::endl;
    
    std::string cat;
    if(iterator->first == "inclusive") cat = "*("+iterator->second+")";
    else cat = "*("+iterator->second+" && "+alias_map_["baseline"]+")";

    tree->Draw((options.variable+">>h_noweight").c_str(),("1"+cat).c_str(),"goff");
    double A = h_noweight->Integral(0,h_noweight->GetNbinsX()+1);
    
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
        std::string out_name = options.outputFilename+"/"+options.variable+"_scale_variations_"+iterator->first;
        std::vector<std::string> legend_entries;
        legend_entries.push_back("#mu_{R}=1, #mu_{F}=1"    );
        legend_entries.push_back("#mu_{R}=1, #mu_{F}=2"    );
        legend_entries.push_back("#mu_{R}=1, #mu_{F}=0.5"  );
        legend_entries.push_back("#mu_{R}=2, #mu_{F}=1"    );
        legend_entries.push_back("#mu_{R}=2, #mu_{F}=2"    );
        legend_entries.push_back("#mu_{R}=0.5, #mu_{F}=1"  );
        legend_entries.push_back("#mu_{R}=0.5, #mu_{F}=0.5");
        DrawHist(hists, legend_entries, out_name);
      }
      
      scale_uncert = (Amax_scale-Amin_scale)/(2*A);
      std::cout << "Scale Uncertainty = " << scale_uncert*100 << " %" << std::endl;
    }
    
    if(options.doPDF){
      
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
      }
      double DeltaMax_NNPDF = A + StandDev(APlus,A);
      double DeltaMin_NNPDF = A - StandDev(AMinus,A);
      DeltaMax.push_back(DeltaMax_NNPDF);
      DeltaMin.push_back(DeltaMin_NNPDF);
      
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
      }
      std::pair<double,double> CT10_uncert = PDFUncert(A_CT10,a0_CT10, 1.645);
      double DeltaMax_CT10 = a0_CT10 + CT10_uncert.first;
      double DeltaMin_CT10 = a0_CT10 - CT10_uncert.second;
      DeltaMax.push_back(DeltaMax_CT10);
      DeltaMin.push_back(DeltaMin_CT10);
      
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
      }
      std::pair<double,double> MMHT_uncert = PDFUncert(A_MMHT,a0_MMHT, 1);
      double DeltaMax_MMHT = a0_MMHT + MMHT_uncert.first;
      double DeltaMin_MMHT = a0_MMHT - MMHT_uncert.second;
      DeltaMax.push_back(DeltaMax_MMHT);
      DeltaMin.push_back(DeltaMin_MMHT);
      
      std::sort(DeltaMax.begin(), DeltaMax.end(), greater());
      std::sort(DeltaMin.begin(), DeltaMin.end(), less_than());
      
      pdf_uncert = (DeltaMax[0] - DeltaMin[0])/(2*A);
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
        std::string out_name = options.outputFilename+"/"+options.variable+"_alpha_s_"+iterator->first;
        std::vector<std::string> legend_entries;
        legend_entries.push_back("#alpha_{s} = 0.118");
        legend_entries.push_back("#alpha_{s} = 0.117");
        legend_entries.push_back("#alpha_{s} = 0.119");
        DrawHist(hists, legend_entries, out_name);
      }
      
      alpha_s_uncert = (Amax_as-Amin_as)/(2*A);
      std::cout << "Alpha_s Uncertainty = " << alpha_s_uncert*100 << " %" << std::endl;
    }

    uncert_output << iterator->first;
    if(options.doScale) uncert_output << "\t\t" << scale_uncert;
    if(options.doPDF) uncert_output << "\t\t" << pdf_uncert;
    if(options.doAlphaS) uncert_output << "\t\t" << alpha_s_uncert;
    uncert_output << std::endl;
    
  }
  uncert_output.close();
  
  
  return 0;
}
