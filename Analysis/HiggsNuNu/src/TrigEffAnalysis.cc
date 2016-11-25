#include "HiggsNuNu/interface/TrigEffAnalysis.h"

namespace ic{

  TrigEffAnalysis::TrigEffAnalysis(int verbosity=1){
    verbosity_=verbosity;
  };

  TrigEffAnalysis::TrigEffAnalysis(){
    verbosity_=1;
  };

  TrigEffAnalysis & TrigEffAnalysis::addfile(std::string name,std::string filepath){
    filepaths_[name]=filepath;
    return *this;
  };
 
  std::vector<std::string> TrigEffAnalysis::filenames(){
    std::vector<std::string> names;
    for(auto &file:tfiles_){
      names.push_back(file.first);
    }
    return names;
  }

  std::string TrigEffAnalysis::filenames(int iFile){
    std::vector<std::string> names;
    for(auto &file:tfiles_){
      names.push_back(file.first);
    }
    return names[iFile];
  }
  
  unsigned TrigEffAnalysis::nfiles(){
    return tfiles_.size();
  }

  TrigEffAnalysis & TrigEffAnalysis::triggersPushBack(Rebinned1DVar* newtrig){
    this->triggers_.push_back(newtrig);
    return *this;
  };

  TrigEffAnalysis & TrigEffAnalysis::varsPushBack(Rebinned1DVar* newvar){
    this->vars_.push_back(newvar);
    return *this;
  };

  int TrigEffAnalysis::OpenFiles(std::string infolder){
    if(verbosity_>=1) std::cout<<"Opening TFiles..."<<std::endl;
    for (auto &file:filepaths_){
      std::string filename = (file.second+".root");
      TFile * tmp = new TFile((infolder+"/"+filename).c_str());
      if (!tmp) {
	std::cerr << "Warning, file " << filename << " could not be opened." << std::endl;
	return 1;
      }
      else {
	tfiles_[(file.first)] = tmp;
      }
    }
    return 0;
  }

  TTree* TrigEffAnalysis::GetTree(std::string filename){
    //Get tree                                                                                                                             
    if(verbosity_>=1)std::cout<<filename<<std::endl;
    TTree *tree;
    tree = (TTree *)tfiles_[filename]->Get("TrigeffInputTree");
    tree->SetEstimate(1000);
    return tree;
  }
  
}
