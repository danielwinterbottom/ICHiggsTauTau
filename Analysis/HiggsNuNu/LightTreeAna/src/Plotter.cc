#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/Plotter.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <map>
#include "CommonTools/Utils/interface/TFileDirectory.h"
#include <boost/algorithm/string.hpp>
#include "TDirectory.h"
#include "TFile.h"

namespace ic{

  Plotter::Plotter(std::string name) : LTModule(name){
  };

  Plotter::~Plotter(){ ;};

  int Plotter::Init(TFile* fs){
    fs_=fs;
    std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;
      std::cout<<"Sets are: "<<std::endl;
    for(unsigned iSets=0;iSets<sets_.size();iSets++){
      for(unsigned iSet=0;iSet<sets_[iSets].size();iSet++){
	std::cout<<"  "<<sets_[iSets][iSet]<<std::endl;
      }
    }
    std::cout<<"Base selection is: "<<basesel_<<std::endl;
    std::cout<<"Extra selection is: "<<cat_<<std::endl;
    return 0;
  };

  int Plotter::Run(LTFiles* filemanager){
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
      TCanvas *c1=new TCanvas();

      std::cout<<"  Getting sets shape"<<std::endl;
      bool first=true;
      for(unsigned iSet=0;iSet<sets_.size();iSet++){
	histos.push_back(filemanager->GetSetsShape(sets_[iSet],shapes_[ishape],basesel_,cat_,"1",true));
	histos[iSet].SetLineColor(iSet);
	if(first){
	  histos[iSet].Draw();
	  first=false;
	}
	else histos[iSet].Draw("same");
      }

      //Write to file
      dir->cd();

      //Plot and write to file
      c1->Write();
    }



    return 0;
  };

}
