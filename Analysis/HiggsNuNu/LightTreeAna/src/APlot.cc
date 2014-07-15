#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/APlot.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <map>
//#include "CommonTools/Utils/interface/TFileDirectory.h"
#include <boost/algorithm/string.hpp>
#include "TDirectory.h"
#include "TFile.h"

namespace ic{

  APlot::APlot(std::string name) : LTModule(name){
  };

  APlot::~APlot(){ ;};

  int APlot::Init(TFile* fs){
    fs_=fs;
    std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;
    std::cout<<"Sets are: "<<std::endl;
    for(unsigned iSets=0;iSets<sets_.size();iSets++){
      for(unsigned iSet=0;iSet<sets_[iSets].size();iSet++){
	std::cout<<"  "<<sets_[iSets][iSet]<<std::endl;
      }
    }
    std::cout<<"Shapes are:"<<std::endl;
    for(unsigned iShape=0;iShape<shapes_.size();iShape++){
      std::cout<<"  "<<shapes_[iShape]<<std::endl;
    }
    std::cout<<"Base selection is: "<<basesel_<<std::endl;
    std::cout<<"Extra selection is: "<<cat_<<std::endl;

    if(setscolours_.size()!=0){
      if(setscolours_.size()!=sets_.size()){
	std::cout<<"setscolours is the wrong size: either specify no colours or one colour per set of sets"<<std::endl;
	return 1;
      }
    }
//    if(drawopts_.size()!=0){
//       if(drawopts_.size()!=shapes_.size()){
// 	std::cout<<"drawopts is the wrong size"<<std::endl;
//      return 1;
//       }
//    }
    if(setstitle_.size()!=0){
      if(setstitle_.size()!=sets_.size()){
	std::cout<<"setstitle is the wrong size: either specify  or one title per set of sets"<<std::endl;
	return 1;
      }
    }
    if(setsnames_.size()!=0){
      if(setsnames_.size()!=sets_.size()){
	std::cout<<"setsnames is the wrong size: either specify no names or one name per set of sets"<<std::endl;
	return 1;
      }
    }
    if(setsscales_.size()!=0){
      if(setsscales_.size()!=sets_.size()){
	std::cout<<"setsscales is the wrong size: either specify no scales or one scale per set of sets"<<std::endl;
	return 1;
      }
    }

    if(axisnames_.size()!=0){
      if(axisnames_.size()!=shapes_.size()){
	std::cout<<"axisnames is the wrong size: either specify no names or one name per variable"<<std::endl;
	return 1;
      }
    }

    return 0;
  };

  int APlot::Run(LTFiles* filemanager){
    std::cout<<module_name_<<":"<<std::endl;
    TFile* file=fs_;
    for(unsigned ishape=0;ishape<shapes_.size();ishape++){
      std::vector<std::string> strs;
      boost::split(strs, shapes_[ishape], boost::is_any_of("("));
      std::string dirname=strs[0];
      std::cout<<dirname<<std::endl;
      TDirectory*  dir=file->mkdir(dirname.c_str());
      dir->cd();

      //Get Shapes
      std::vector<TH1F> histos;

      std::cout<<"  Getting sets shape"<<std::endl;
      for(unsigned iSet=0;iSet<sets_.size();iSet++){
	histos.push_back(filemanager->GetSetsShape(sets_[iSet],shapes_[ishape],basesel_,cat_,"1",true));
	if(setscolours_.size()!=0){
	  histos[iSet].SetMarkerColor(setscolours_[iSet]);
	  histos[iSet].SetLineColor(setscolours_[iSet]);
	}
	else{
	  histos[iSet].SetMarkerColor(iSet+1);
	  histos[iSet].SetLineColor(iSet+1);
	}
	if(axisnames_.size()!=0) histos[iSet].GetXaxis()->SetTitle(axisnames_[ishape].c_str());
	else histos[iSet].GetXaxis()->SetTitle(dirname.c_str());
	histos[iSet].SetMarkerStyle(2);
	if(setstitle_.size()!=0){
	  histos[iSet].SetTitle(setstitle_[iSet].c_str());
	}
	else{
	  histos[iSet].SetTitle(dirname.c_str());
	}

      }
      
      TCanvas *c1=new TCanvas();
      bool first=true;
      double ymax=0;
      for(unsigned iSet=0;iSet<sets_.size();iSet++){
	if(first){
	  histos[iSet].Draw();
	  ymax=histos[iSet].GetMaximum();
	  first=false;
	  std::cout<<"DRAWING FIRST"<<std::endl;
	         }
	else{
	  if(histos[iSet].GetMaximum()>ymax){
	    ymax=histos[iSet].GetMaximum();
	    histos[0].GetYaxis()->SetRangeUser(0,ymax+1);
	  }
	  histos[iSet].Draw("same");
	  std::cout<<"DRAWING SECOND"<<std::endl;  
	}
      }
      TLegend* leg =new TLegend();
      if(setsnames_.size()!=0){
	for(unsigned iSet=0;iSet<sets_.size();iSet++){
	  leg->AddEntry(&(histos[iSet]),(setsnames_[iSet]).c_str(),"l");
	  std::cout<<setsnames_[iSet]<<std::endl;
	  std::cout<<(setsnames_[iSet]).c_str()<<std::endl;
	}
	leg->Draw("same");
      }

      //Write to file
      dir->cd();

      //Plot and write to file
      c1->Write();
    }



    return 0;
  };

}
