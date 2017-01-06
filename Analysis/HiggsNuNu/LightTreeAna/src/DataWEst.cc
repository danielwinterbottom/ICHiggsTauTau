#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataWEst.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"
#include <map>

namespace ic{

  DataWEst::DataWEst(std::string name) : LTModule(name){
    sigmcweight_="total_weight_lepveto";
    contmcweight_="total_weight_leptight";
    contdataweight_="weight_nolepnotrig*weight_trig_0";
  };

  DataWEst::~DataWEst(){ ;};

  int DataWEst::Init(TFile* fs){
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

  int DataWEst::Run(LTFiles* filemanager){
    std::cout<<module_name_<<":"<<std::endl;

    //Get Shapes for NSMC, NCMC, NCData and NCBkg
    std::cout<<"  Getting signal MC shape"<<std::endl;
    TH1F  sigmcshape = filemanager->GetSetShape(sigmcset_,"jet2_pt(200,0.,1000.)",basesel_,sigcat_,sigmcweight_,false);
    std::cout<<"  Getting control MC shape"<<std::endl;
    TH1F  contmcshape = filemanager->GetSetShape(contmcset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_,contmcweight_,false);
   std::cout<<"  Getting control MC Backgrounds shape"<<std::endl;
    TH1F  contbkgshape = filemanager->GetSetsShape(contbkgset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_,contmcweight_,false);
    std::cout<<"  Getting control Data shape"<<std::endl;
    TH1F  contdatashape = filemanager->GetSetShape(contdataset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_,contdataweight_,false);
    
    //Integrate over shape to get number in each region
    double nsmc = Integral(&sigmcshape);
    double ncmc = Integral(&contmcshape);
    double ncdata = Integral(&contdatashape);
    double ncbkg = Integral(&contbkgshape);

    std::cout<<"  nsmc: "<<nsmc<<", ncmc: "<<ncmc<<", ncdata: "<<ncdata<<", ncbkg: "<<ncbkg<<std::endl;

    //Calculate background
    double nsdata=nsmc/ncmc*(ncdata-ncbkg);

    std::cout<<"  "<<module_name_<<" background in signal region is: "<<nsdata<<std::endl;

    return 0;
  };

}
