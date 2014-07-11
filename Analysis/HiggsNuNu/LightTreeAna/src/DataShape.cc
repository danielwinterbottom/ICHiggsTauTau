#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataShape.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include <map>

namespace ic{

  DataShape::DataShape(std::string name) : LTModule(name){
    dataweight_="weight_nolep";
    std::vector<std::string> shapes;
    shapes.push_back("jet2_pt(200,0.,1000.)");
    shape_=shapes;
    dirname_="";
  };

  DataShape::~DataShape(){ ;};

  int DataShape::Init(TFile* fs){
    fs_=fs;
    std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;
    std::cout<<"Data set is: "<<dataset_<<std::endl;
    std::cout<<"Base selection is: "<<basesel_<<std::endl;
    std::cout<<"Extra selection is: "<<cat_<<std::endl;
    std::cout<<"Data weight is: "<<dataweight_<<std::endl;
    if((shapename_.size()!=shape_.size())&&shapename_.size()!=0){
      std::cout<<"WARNING: different numbers of shape names and shapes expect errors!"<<std::endl;
    }
    return 0;
  };

  int DataShape::Run(LTFiles* filemanager){
    std::cout<<module_name_<<":"<<std::endl;

    TFile *file=fs_;
    TDirectory* dir;
    if(dirname_==""){
      std::cout<<"1"<<std::endl;
      std::cout<<dataset_.c_str();
      dir=file->mkdir(dataset_.c_str());
    }
    else if(!fs_->GetDirectory(dirname_.c_str())){
      std::cout<<"2"<<std::endl;
      dir=file->mkdir(dirname_.c_str());
    }
    else{
      std::cout<<"3"<<std::endl;
      dir=fs_->GetDirectory(dirname_.c_str());
    }
    std::cout<<"4"<<std::endl;
    dir->cd();
    //Get Shapes for Data
    std::cout<<"  Getting Data shape"<<std::endl;

    for(unsigned iShape=0;iShape<shape_.size();iShape++){
      TH1F  datashape = filemanager->GetSetShape(dataset_,shape_[iShape],basesel_,cat_,dataweight_,false);
      dir->cd();
      std::string histname;
      if(shapename_.size()==0){
	std::vector<std::string> strs;
	boost::split(strs, shape_[iShape], boost::is_any_of("("));
	histname=strs[0];
      }
      else{
	histname=shapename_[iShape];
      }
      datashape.SetName(histname.c_str());
      datashape.Write();
    }
    return 0;
  };

}
