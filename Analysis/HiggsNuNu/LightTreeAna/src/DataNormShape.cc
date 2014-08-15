#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataNormShape.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include "TVectorD.h"
#include <map>

namespace ic{

  DataNormShape::DataNormShape(std::string name) : LTModule(name){
    sigmcweight_="total_weight_lepveto";
    contmcweight_="total_weight_leptight";
    contdataweight_="weight_nolep";
    contdataextrasel_="";
    sigcontextrafactor_=1;
    std::vector<std::string> shapes;
    shapes.push_back("jet2_pt(200,0.,1000.)");
    shape_=shapes;
    dirname_="";
  };

  DataNormShape::~DataNormShape(){ ;};

  int DataNormShape::Init(TFile* fs){
    fs_=fs;
    std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;
    std::cout<<"  Signal MC set is: "<<sigmcset_<<std::endl;
    std::cout<<"  Control MC set is: "<<contmcset_<<std::endl;
    std::cout<<"  Control data set is: "<<contdataset_<<std::endl;
    std::cout<<"  Base selection is: "<<basesel_<<std::endl;
    std::cout<<"  Signal extra selection is: "<<sigcat_<<std::endl;
    std::cout<<"  Control extra selection is: "<<contcat_<<std::endl;
    std::cout<<"  Signal MC weight is: "<<sigmcweight_<<std::endl;
    std::cout<<"  Control MC weight is: "<<contmcweight_<<std::endl;
    std::cout<<"  Control data weight is: "<<contdataweight_<<std::endl;
    if((shapename_.size()!=shape_.size())&&shapename_.size()!=0){
      std::cout<<"WARNING: different numbers of shape names and shapes expect errors!"<<std::endl;
    }
    return 0;
  };

  int DataNormShape::Run(LTFiles* filemanager){
    std::cout<<module_name_<<":"<<std::endl;

    TFile *file=fs_;
    TDirectory* dir;
    if(dirname_==""){
      dir=file->mkdir(sigmcset_.c_str());
    }
    else if(!fs_->GetDirectory(dirname_.c_str())){
      dir=file->mkdir(dirname_.c_str());
    }
    else{
      dir=fs_->GetDirectory(dirname_.c_str());
    }
    dir->cd();
    //Get Shapes for NSMC, NCMC, NCData and NCBkg
    std::cout<<"  Getting control MC shape"<<std::endl;
    TH1F  contmcshape = filemanager->GetSetShape(contmcset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_,contmcweight_,false);
   std::cout<<"  Getting control MC Backgrounds shape"<<std::endl;
    TH1F  contbkgshape = filemanager->GetSetsShape(contbkgset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_,contmcweight_,false);
    std::cout<<"  Getting control Data shape"<<std::endl;
    TH1F  contdatashape = filemanager->GetSetShape(contdataset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_+contdataextrasel_,contdataweight_,false);

    
    //Integrate over shape to get number in each region
    double ncmc = Integral(&contmcshape);
    double ncdata = Integral(&contdatashape);
    double ncbkg = Integral(&contbkgshape);

    std::cout<<"  ncmc: "<<ncmc<<", ncdata: "<<ncdata<<", ncbkg: "<<ncbkg<<std::endl;

    //Calculate weight
    double weight=(ncdata-ncbkg)/ncmc;
    TVectorD weightvec(1);
    weightvec[0]=weight*sigcontextrafactor_;
    std::cout<<"  Getting signal MC shapes"<<std::endl;
    for(unsigned iShape=0;iShape<shape_.size();iShape++){
      TH1F  sigmcshape = filemanager->GetSetShape(sigmcset_,shape_[iShape],basesel_,sigcat_,sigmcweight_,false);
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
      sigmcshape.SetName(histname.c_str());
      sigmcshape.Scale(weight*sigcontextrafactor_);
      sigmcshape.Write();
    }
    dir->cd();
    weightvec.Write("ddweight");
  
    return 0;
  };

}
