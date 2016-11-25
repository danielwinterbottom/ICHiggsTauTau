#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/NormPlots.h"
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

  NormPlots::NormPlots(std::string name) : LTModule(name){
  };

  NormPlots::~NormPlots(){ ;};

  int NormPlots::Init(TFile* fs){
    fs_=fs;
    std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;
    std::cout<<"Signal MC set is: "<<sigset_<<std::endl;
    std::cout<<"QCD MC set is: "<<qcdset_<<std::endl;
    std::cout<<"Base selection is: "<<basesel_<<std::endl;
    std::cout<<"Extra selection is: "<<cat_<<std::endl;
    return 0;
  };

  int NormPlots::Run(LTFiles* filemanager){
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
      std::cout<<"  Getting signal MC shape"<<std::endl;
      TH1F  sigshape = filemanager->GetSetShape(sigset_,shapes_[ishape],basesel_,cat_,"1",true);
      std::cout<<"  Getting QCD MC shape"<<std::endl;
      TH1F  qcdshape = filemanager->GetSetShape(qcdset_,shapes_[ishape],basesel_,cat_,"1",true);
      std::cout<<"  Getting EWK MC shape"<<std::endl;
      TH1F  ewkshape = filemanager->GetSetsShape(ewkset_,shapes_[ishape],basesel_,cat_,"1",true);
      sigshape.SetName("signal");
      qcdshape.SetName("qcd");
      ewkshape.SetName("ewk");

      //Write to file
      dir->cd();
      sigshape.Write();
      qcdshape.Write();
      ewkshape.Write();

      //Plot and write to file
      TCanvas *c1=new TCanvas();
      sigshape.SetLineColor(1);
      qcdshape.SetLineColor(2);
      ewkshape.SetLineColor(3);
      TLegend* leg =new TLegend(0.7,0.89,1,1);
      leg->AddEntry(&sigshape,"Signal","l");
      leg->AddEntry(&qcdshape,"QCD","l");
      leg->AddEntry(&ewkshape,"EWK","l");
      std::cout<<"  draw sig normalized"<<std::endl;
      sigshape.DrawNormalized();
      std::cout<<"  draw qcd normalized"<<std::endl;
      qcdshape.DrawNormalized("same");
      std::cout<<"  draw ewk normalized"<<std::endl;
      ewkshape.DrawNormalized("same");
      leg->Draw();
      c1->Write();
    }



    return 0;
  };

}
