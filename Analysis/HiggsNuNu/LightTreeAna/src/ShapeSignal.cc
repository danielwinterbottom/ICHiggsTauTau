#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/ShapeSignal.h"
#include "TMath.h"
#include "TSystem.h"
#include "TFile.h"
#include <iostream>
#include "fstream"
#include <map>
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TDirectory.h"
#include <boost/algorithm/string.hpp>
#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include <string>
#include <vector>

namespace ic{

  ShapeSignal::ShapeSignal(std::string name) : LTModule(name){
    sigmcweight_="total_weight_lepveto";
    contmcweight_="total_weight_leptight";
    contdataweight_="weight_nolep";
  };

 ShapeSignal::~ShapeSignal(){ ;};

  int ShapeSignal::Init(TFile* fs){
  fs_=fs;
  std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;
  std::cout<<"Signal MC set is: "<<sigmcset_<<std::endl;
  std::cout<<"Control MC set is: "<<contmcset_<<std::endl;
  std::cout<<"Control data set is: "<<contdataset_<<std::endl;
  std::cout<<"Base selection is: "<<basesel_<<std::endl;
  std::cout<<"Signal extra selection is: "<<sigcat_<<std::endl;
  std::cout<<"Control extra selection is: "<<contcat_<<std::endl;
  std::cout<<"Signal MC weight is: "<<sigmcweight_<<std::endl;
  std::cout<<"Control MC weight is: "<<contmcweight_<<std::endl;
  std::cout<<"Control data weight is: "<<contdataweight_<<std::endl;
  return 0;
};

  int ShapeSignal::Run(LTFiles* filemanager){
    std::cout<<module_name_<<":"<<std::endl;
    TFile* file=fs_;

  for(unsigned iSet=0;iSet<sets_.size();iSet++){
   std::vector<std::string> strs;
   //   boost::split(strs, shapes_[ishape], boost::is_any_of("("));
   std::string dirname=strs[0];
   std::cout<<dirname<<std::endl;
   TDirectory*  dir=file->mkdir(dirname.c_str());
   dir->cd();

  //Get Shapes for NSMC, NCMC, NCData and NCBkg                                             
  std::cout<<"  Getting signal MC shape"<<std::endl;
  TH1F  sigmcshape = filemanager->GetSetShape(sigmcset_,"jet2_pt(200,0.,1000.)",basesel_,sigcat_,sigmcweight_,false);
  std::cout<<"  Getting control MC shape"<<std::endl;
  TH1F  contmcshape = filemanager->GetSetShape(contmcset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_,contmcweight_,false);
  std::cout<<"  Getting control MC Backgrounds shape"<<std::endl;
  TH1F  contbkgshape = filemanager->GetSetsShape(contbkgset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_,contmcweight_,false);
  std::cout<<"  Getting control Data shape"<<std::endl;
  TH1F  contdatashape = filemanager->GetSetShape(contdataset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_,contdataweight_,false);

  //  double nsmc = Integral(&sigmcshape);
  double ncmc = Integral(&contmcshape);
  double ncdata = Integral(&contdatashape);
  double ncbkg = Integral(&contbkgshape);                                        
  double scale= (ncdata-ncbkg)/(ncmc);

  //Get shape for scaled signal
  sigmcshape.Scale(scale);
  
  
  TCanvas *c1=new TCanvas(); 
  TLegend* leg =new TLegend();
  leg->Draw();
  sigmcshape.Draw();
  dir->cd();
  c1->Write();
  }
   return 0;
};

}
