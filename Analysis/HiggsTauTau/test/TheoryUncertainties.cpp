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
  bool doGen;
  bool doScale;
  bool doPDF;
  bool doAlphaS;
  std::string cfg; 
  
};

double StandDev(std::vector<double> a, double a0) {
  unsigned N = a.size();
  double sum=0;
  for(unsigned i=0; i<N; ++i){
    sum+=pow(a[i]-a0,2);  
  }
  sum = sum/N;
  return sqrt(sum);
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
        ("outputFilename",         po::value<std::string>(&options.outputFilename)->default_value("output"))
        ("makePlots",              po::value<bool>(&options.makePlots)->default_value(false))
        ("channel",                po::value<std::string>(&options.channel)->default_value("mt"))
        ("doGen",                  po::value<bool>(&options.doGen)->default_value(true))
        ("doScale",                po::value<bool>(&options.doScale)->default_value(true))
        ("doPDF",                  po::value<bool>(&options.doPDF)->default_value(true))
        ("doAlphaS",               po::value<bool>(&options.doAlphaS)->default_value(true));
  po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
  po::store(po::parse_config_file<char>(options.cfg.c_str(), config), vm);
  po::notify(vm);
  
  std::cout << "===== program options ====="                 << std::endl;
  std::cout << "input          = " << options.input          << std::endl;
  std::cout << "outputFilename = " << options.outputFilename << std::endl;
  std::cout << "makePlots      = " << options.makePlots      << std::endl;
  std::cout << "channel        = " << options.channel        << std::endl;
  std::cout << "doGen          = " << options.doGen          << std::endl;
  std::cout << "doScale        = " << options.doScale        << std::endl;
  std::cout << "doPDF          = " << options.doPDF        << std::endl;
  std::cout << "doAlphaS       = " << options.doAlphaS       << std::endl;
  
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
  TH1D *h0 = new TH1D("h0","",100,0,300);
  gen_tree->Draw("m_vis>>h0","","goff");

  double nominal_wt = h0->Integral(0,h0->GetNbinsX()+1);
  
  TH1D *htemp = new TH1D("htemp","",100,0,300);
  std::vector<double> scale_variation_addwt;
  if(options.doScale){
    for(unsigned i=0; i<9; ++i){
      std::string wt_name = Form("scale_variation_wts[%u]",i);
      gen_tree->Draw("m_vis>>htemp",wt_name.c_str(),"goff");  
      double wt_sum = htemp->Integral(0,htemp->GetNbinsX()+1);
      scale_variation_addwt.push_back(nominal_wt/wt_sum);
    }
  }
  std::vector<double> NNPDF_addwt;
  if(options.doPDF){
    for(unsigned i=0; i<100; ++i){
      std::string wt_name = Form("NNPDF_wts[%u]",i);
      gen_tree->Draw("m_vis>>htemp",wt_name.c_str(),"goff");  
      double wt_sum = htemp->Integral(0,htemp->GetNbinsX()+1);
      NNPDF_addwt.push_back(nominal_wt/wt_sum);
    }
  }
  std::vector<double> alpha_s_variation_addwt;
  if(options.doAlphaS){
    for(unsigned i=0; i<2; ++i){
      std::string wt_name = Form("alpha_s_wts[%u]",i);
      gen_tree->Draw("m_vis>>htemp",wt_name.c_str(),"goff");  
      double wt_sum = htemp->Integral(0,htemp->GetNbinsX()+1);
      alpha_s_variation_addwt.push_back(nominal_wt/wt_sum);
    }
  }
  
  std::string tree_name;
  if(options.doGen) tree_name = "gen_ntuple";
  else              tree_name = "ntuple"; 
  TTree *tree = (TTree*)f0->Get(tree_name.c_str());
  TH1D *h_noweight = new TH1D("h_noweight","",100,0,300);  
  typedef std::map<std::string, std::string>::iterator it_type;
  
  for(it_type iterator = alias_map_.begin(); iterator != alias_map_.end(); iterator++) {
      
    std::cout << "Category: " << iterator->first << std::endl;
    
    std::string cat;
    if(iterator->first == "inclusive") cat = "*("+iterator->second+")";
    else cat = "*("+iterator->second+" && "+alias_map_["baseline"]+")";
    std::cout << iterator->first << std::endl;

    tree->Draw("m_vis>>h_noweight",("1"+cat).c_str(),"goff");
    double A = h_noweight->Integral(0,h_noweight->GetNbinsX()+1);
    
    std::map<std::string, double> scale_uncert_map_;
    if(options.doScale){
      double Amin_scale = A;
      double Amax_scale = A;
      
      for(unsigned i=0; i<9; ++i){
        if(i==0 || i==5 || i==7) continue;
        std::string add_wt = Form("%f",scale_variation_addwt[i]);
        std::string wt_name = Form("scale_variation_wts[%u]",i);
        wt_name+="*"+add_wt;
        wt_name+=cat;
        tree->Draw("m_vis>>htemp",wt_name.c_str(),"goff");  
        double wt_sum = htemp->Integral(0,htemp->GetNbinsX()+1);
        Amin_scale = std::min(wt_sum,Amin_scale);
        Amax_scale = std::max(wt_sum,Amax_scale);
      }
      
      double scale_uncert = (Amax_scale-Amin_scale)/(2*A);
      scale_uncert_map_[iterator->first] = scale_uncert;
      std::cout << "Scale Uncertainty = " << scale_uncert*100 << " %" << std::endl;
    }
    
    std::map<std::string, std::pair<double,double>> pdf_uncert_map_;
    if(options.doPDF){
      std::vector<double> AMinus;
      std::vector<double> APlus;
      for(unsigned i=0; i<100; ++i){
        std::string add_wt = Form("%f",NNPDF_addwt[i]);
        std::string wt_name = Form("NNPDF_wts[%u]",i);
        wt_name+="*"+add_wt;
        wt_name+=cat;
        tree->Draw("m_vis>>htemp",wt_name.c_str(),"goff");  
        double wt_sum = htemp->Integral(0,htemp->GetNbinsX()+1);
        if(wt_sum > A) APlus.push_back(wt_sum);
        else if(wt_sum < A) AMinus.push_back(wt_sum);
      }
      
      double pdf_uncert_plus = StandDev(APlus,A)/A;
      double pdf_uncert_minus = StandDev(AMinus,A)/A;
      pdf_uncert_map_[iterator->first] = std::make_pair(pdf_uncert_plus,pdf_uncert_minus);
      std::cout << "PDF Uncertainty = +" << pdf_uncert_plus*100 << " % -" << pdf_uncert_minus*100 << " %"<< std::endl;
    }
    
    std::map<std::string, double> alpha_s_uncert_map_;
    if(options.doAlphaS){
      double Amin_as = A;
      double Amax_as = A;
      
      for(unsigned i=0; i<2; ++i){
        std::string add_wt = Form("%f",alpha_s_variation_addwt[i]);
        std::string wt_name = Form("alpha_s_wts[%u]",i);
        wt_name+="*"+add_wt;
        wt_name+=cat;
        tree->Draw("m_vis>>htemp",wt_name.c_str(),"goff");  
        double wt_sum = htemp->Integral(0,htemp->GetNbinsX()+1);
        Amin_as = std::min(wt_sum,Amin_as);
        Amax_as = std::max(wt_sum,Amax_as);
      }
    
      std::cout << A << std::endl;
      std::cout << Amax_as << std::endl;
      std::cout << Amin_as << std::endl;
      double alpha_s_uncert = (Amax_as-Amin_as)/(2*A);
      scale_uncert_map_[iterator->first] = alpha_s_uncert;
      std::cout << "Alpha_s Uncertainty = " << alpha_s_uncert*100 << " %" << std::endl;
    }
  }  
  
  return 0;
}
