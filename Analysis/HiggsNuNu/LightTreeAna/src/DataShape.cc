#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataShape.h"
#include <iostream>
#include "TH1.h"
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
    std::cout<<"  Data sets are: "<<std::endl;
      for(unsigned iset=0;iset<dataset_.size();iset++){
	std::cout<<"    "<<dataset_[iset]<<std::endl;
      }
    std::cout<<"  Base selection is: "<<basesel_<<std::endl;
    std::cout<<"  Extra selection is: "<<cat_<<std::endl;
    std::cout<<"  Data weight is: "<<dataweight_<<std::endl;
    if((shapename_.size()!=shape_.size())&&shapename_.size()!=0){
      std::cout<<"  WARNING: different numbers of shape names and shapes expect errors!"<<std::endl;
    }
    return 0;
  };

  int DataShape::Run(LTFiles* filemanager){
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

    for(unsigned iShape=0;iShape<shape_.size();iShape++){
      std::string histname;
      if(shapename_.size()==0){
	std::vector<std::string> strs;
	boost::split(strs, shape_[iShape], boost::is_any_of("("));
	histname=strs[0];
      }
      else{
	histname=shapename_[iShape];
      }
      TH1F datashape;
      TH2F datashape2D;
      bool is2D = false;
      if (histname.find(":")!=histname.npos) {
	datashape2D=filemanager->GetSetsShape2D(dataset_,shape_[iShape],basesel_,cat_,dataweight_,false);
	is2D=true;
      }
      else datashape = filemanager->GetSetsShape(dataset_,shape_[iShape],basesel_,cat_,dataweight_,false);
      dir->cd();
      if(iShape==0){
	if (!is2D) std::cout<<"  nevents: "<<Integral(&datashape)<<"+-"<<Error(&datashape)<<std::endl;
	else std::cout<<"  nevents: "<<Integral(&datashape2D)<<"+-"<<Error(&datashape2D)<<std::endl;
      }
      if (is2D){
	datashape2D.SetName(histname.c_str());
	datashape2D.Write();
      } else {
	datashape.SetName(histname.c_str());
	datashape.Write();
      }
    }
    return 0;
  };

}
