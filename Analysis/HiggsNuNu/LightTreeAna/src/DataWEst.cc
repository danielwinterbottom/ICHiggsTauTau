#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/DataWEst.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"
#include <map>

namespace ic{

  DataWEst::DataWEst(std::string name) : LTModule(name){
  };

  DataWEst::~DataWEst(){ ;};

  int DataWEst::Init(fwlite::TFileService* fs){
    fs_=fs;
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
    std::map<std::string,TH1F> sigmcshapes;
    std::map<std::string,TH1F> contmcshapes;
    std::map<std::string,TH1F> contdatashapes;
    std::map<std::string,TH1F> contbkgshapes;

    std::vector<std::string> sigmcnames=filemanager->GetSetNames(sigmcset_);
    std::vector<std::string> contmcnames=filemanager->GetSetNames(contmcset_);
    std::vector<std::string> contdatanames=filemanager->GetSetNames(contdataset_);
    std::vector<std::string> contbkgnames=contbkgset_;

    for(unsigned iname=0;iname<sigmcnames.size();iname++){
      sigmcshapes[sigmcnames[iname]]=filemanager->GetShape(sigmcnames[iname],"jet2_pt(200,0.,1000.)",basesel_,sigcat_,"");
      std::cout<<sigmcnames[iname]<<": "<<Integral(&sigmcshapes[sigmcnames[iname]])<<std::endl;
    }

    for(unsigned iname=0;iname<contmcnames.size();iname++){
      contmcshapes[contmcnames[iname]]=filemanager->GetShape(contmcnames[iname],"jet2_pt(200,0.,1000.)",basesel_,contcat_,"");
      std::cout<<contmcnames[iname]<<": "<<Integral(&contmcshapes[contmcnames[iname]])<<std::endl;
    }

    for(unsigned iname=0;iname<contdatanames.size();iname++){
      contdatashapes[contdatanames[iname]]=filemanager->GetShape(contdatanames[iname],"jet2_pt(200,0.,1000.)",basesel_,contcat_,"");
      std::cout<<contdatanames[iname]<<": "<<Integral(&contdatashapes[contdatanames[iname]])<<std::endl;
    }

    for(unsigned iname=0;iname<contbkgnames.size();iname++){
      contbkgshapes[contbkgnames[iname]]=filemanager->GetSetShape(contbkgnames[iname],"jet2_pt(200,0.,1000.)",basesel_,contcat_,"1",false);
      std::cout<<contbkgnames[iname]<<": "<<Integral(&contbkgshapes[contbkgnames[iname]])<<std::endl;

    }
    
    


    //Get Shapes for NSMC, NCMC, NCData and NCBkg
    TH1F  sigmcshape = filemanager->GetSetShape(sigmcset_,"jet2_pt(200,0.,1000.)",basesel_,sigcat_,"total_weight",false);
    TH1F  contmcshape = filemanager->GetSetShape(contmcset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_,"total_weight",false);
    TH1F  contbkgshape = filemanager->GetSetsShape(contbkgset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_,"total_weight",false);
    TH1F  contdatashape = filemanager->GetSetShape(contdataset_,"jet2_pt(200,0.,1000.)",basesel_,contcat_,"total_weight",false);
    
    //Integrate over shape to get number in each region
    double nsmc = Integral(&sigmcshape);
    double ncmc = Integral(&contmcshape);
    double ncdata = Integral(&contdatashape);
    double ncbkg = Integral(&contbkgshape);

    std::cout<<"nsmc: "<<nsmc<<", ncmc: "<<ncmc<<", ncdata: "<<ncdata<<", ncbkg: "<<ncbkg<<std::endl;

    //Calculate background
    double nsdata=nsmc/ncmc*(ncdata-ncbkg);

    std::cout<<module_name_<<" background in signal region is: "<<nsdata<<std::endl;

    return 0;
  };

}
