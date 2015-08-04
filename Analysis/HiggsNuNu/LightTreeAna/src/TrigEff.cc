#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/TrigEff.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include <map>

namespace ic{

  TrigEff::TrigEff(std::string name) : LTModule(name){
    trigger_="pass_sigtrigger==1";
    dataweight_="weight_nolep";
    shape_=("jet2_pt(200,0.,1000.)");
    shapename_="";
    dirname_="";
  };

  TrigEff::~TrigEff(){ ;};

  int TrigEff::Init(TFile* fs){
    fs_=fs;
    std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;
    std::cout<<"  Data sets are: "<<std::endl;
      for(unsigned iset=0;iset<dataset_.size();iset++){
	std::cout<<"    "<<dataset_[iset]<<std::endl;
      }
    std::cout<<"  Base selection is: "<<basesel_<<std::endl;
    std::cout<<"  Extra selection is: "<<cat_<<std::endl;
    std::cout<<"  Data weight is: "<<dataweight_<<std::endl;
    return 0;
  };

  int TrigEff::Run(LTFiles* filemanager){
    std::cout<<module_name_<<":"<<std::endl;

    TFile *file=fs_;
    TDirectory* dir;
    if(dirname_==""){
      dir=file->mkdir(this->module_name().c_str());
    }
    else if(!fs_->GetDirectory(dirname_.c_str())){
      dir=file->mkdir(dirname_.c_str());
    }
    else{
      dir=file->GetDirectory(dirname_.c_str());
    }
    dir->cd();
    //Get Shapes for Data
    std::cout<<"  Getting shape"<<std::endl;

    TH1F  datashapenotrig = filemanager->GetSetsShape(dataset_,shape_,basesel_,cat_,dataweight_,false);
    TH1F  datashapewithtrig = filemanager->GetSetsShape(dataset_,shape_,basesel_,cat_+"&&"+trigger_,dataweight_,false);
    datashapenotrig.Sumw2();
    datashapewithtrig.Sumw2();
    TH1F* efficiency=(TH1F*)datashapenotrig.Clone();
    efficiency->Divide(&datashapewithtrig,&datashapenotrig);
    dir->cd();
    std::string histname;
    if(shapename_==""){
      std::vector<std::string> strs;
      boost::split(strs, shape_, boost::is_any_of("("));
      histname=strs[0];
    }
    else{
      histname=shapename_;
    }
    std::cout<<"  nevents before trigger: "<<Integral(&datashapenotrig)<<"+-"<<Error(&datashapenotrig)<<std::endl;
    std::cout<<"  nevents after trigger: "<<Integral(&datashapewithtrig)<<"+-"<<Error(&datashapewithtrig)<<std::endl;

    efficiency->SetName(histname.c_str());
    datashapenotrig.SetName((histname+"notrig").c_str());
    datashapewithtrig.SetName((histname+"withtrig").c_str());
    datashapenotrig.Write();
    datashapewithtrig.Write();
    efficiency->Write();
    return 0;
  };

}
