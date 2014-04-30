#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataWEst.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"

namespace ic{

  DataWEst::DataWEst(std::string name) : LTModule(name){
  };

  DataWEst::~DataWEst(){ ;};

  int DataWEst::Init(){
    std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;
    std::cout<<"Signal MC set is: "<<sigmcset_<<std::endl;
    std::cout<<"Control MC set is: "<<contmcset_<<std::endl;
    std::cout<<"Control data set is: "<<contdataset_<<std::endl;
    std::cout<<"Base selection is: "<<basesel_<<std::endl;
    std::cout<<"Signal extra selection is: "<<sigcat_<<std::endl;
    std::cout<<"Control extra selection is: "<<contcat_<<std::endl;
    return 0;
  };

  int DataWEst::Run(LTFiles* filemanager){
    TH1F  sigmcshape = filemanager->GetSetShape(sigmcset_,"met(200,0.,1000.)",basesel_,sigcat_,"1",true);
    TH1F  contmcshape = filemanager->GetSetShape(contmcset_,"met(200,0.,1000.)",basesel_,contcat_,"1",true);
    TH1F  contdatashape = filemanager->GetSetShape(contdataset_,"met(200,0.,1000.)",basesel_,contcat_,"1",true);
    
    double nsmc = Integral(&sigmcshape);
    double ncmc = Integral(&contmcshape);
    double ncdata = Integral(&contdatashape);
    
    std::cout<<"nsmc: "<<nsmc<<", ncmc: "<<ncmc<<", ncdata: "<<ncdata<<std::endl;

    double nsdata=nsmc/ncmc*ncdata;

    std::cout<<module_name_<<" background in signal region is: "<<nsdata<<std::endl;

    return 0;
  };

}
