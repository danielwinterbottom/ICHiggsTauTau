#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/SummaryTable.h"
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include <map>

namespace ic{

  SummaryTable::SummaryTable(std::string name) : LTModule(name){
    std::vector<std::string> shapes;
    shapes.push_back("jet2_pt(200,0.,1000.)");
    shape_=shapes;
  };

  SummaryTable::~SummaryTable(){ ;};

  int SummaryTable::Init(TFile* fs){
    fs_=fs;
    std::cout<<"Initialisation info for "<<module_name_<<":"<<std::endl;
    std::cout<<"  Processes are: "<<std::endl;
    for(unsigned idir=0;idir<dirs_.size();idir++){
      std::cout<<"    "<<dirs_[idir]<<std::endl;
    }
    return 0;
  };

  int SummaryTable::Run(LTFiles*){
    std::cout<<module_name_<<":"<<std::endl;

    TFile *file=fs_;
    TDirectory* dir;
    for(unsigned idir=0;idir<dirs_.size();idir++){
      if(dirs_[idir]==""){
	std::cout<<"Empty dir name given skipping"<<std::endl;
	continue;
      }
      else if(!fs_->GetDirectory(dirs_[idir].c_str())){
	std::cout<<"No directory called: "<<dirs_[idir]<<"continuing"<<std::endl;
	continue;
      }
      else{
	dir=file->GetDirectory(dirs_[idir].c_str());
      }
      dir->cd();
      TH1F* histo = (TH1F*)dir->Get(shape_[0].c_str());
      std::cout<<dirs_[idir]<<": "<<Integral(histo)<<std::endl;
    }
    return 0;
  };

}
