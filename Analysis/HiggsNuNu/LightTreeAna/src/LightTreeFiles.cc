#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <iostream>
#include <vector>

namespace ic{
  File::File(){
  };

  File::File(std::string name, std::string path){
    name_=name;
    path_=path;
    set_="";
  };
  
  File::File(std::string name, std::string set, std::string path){
    name_=name;
    set_=set;
    path_=path;
  };

  int File::Open(std::string infolder){
    //std::cout<<"Opening TFile..."<<std::endl;
    std::string filepath = (this->path());
    TFile * tmp = new TFile((infolder+"/"+this->path()).c_str());
    if (!tmp) {
      std::cerr << "Warning, file " << this->name() << " could not be opened." << std::endl;
      return 1;
    }
    else {
      tfile_ = tmp;
      tree_=(TTree *)tfile_->Get("LightTree");
      tree_->SetEstimate(1000);
      return 0;
    }
  }

  Files::Files(){
  };

  Files::Files(std::string name, std::string set, std::string path){
    files_[name]=File(name,set,path);
    setlists_[set].push_back(name);
  };

  Files::Files(std::string name, std::string path){
    files_[name]=File(name,path);
  };
  
  Files::Files(std::vector<std::string> names,std::vector<std::string> sets, std::vector<std::string> paths){
    if(names.size()!=sets.size() || sets.size()!=paths.size()) std::cout<<"Error different numbers of names, sets and paths making empty Files object"<<std::endl;
    else{
      for(unsigned iname=0;iname<names.size();iname++){
	files_[names[iname]]=File(names[iname],sets[iname],paths[iname]);
	setlists_[sets[iname]].push_back(names[iname]);
      }
    }
  };

  Files::Files(File file){
    files_[file.name()]=file;
    if(file.set()!=""){
      setlists_[file.set()].push_back(file.name());
    }
  };
  
  Files::Files(std::vector<File> files){
    for(unsigned ifile=0;ifile<files.size();ifile++){
      files_[files[ifile].name()]=files[ifile];
      if(files[ifile].set()!=""){
	setlists_[files[ifile].set()].push_back(files[ifile].name());
      }
    }
  };

  void Files::AddFile(std::string name, std::string set, std::string path){
    setlists_[set].push_back(name);
    files_[name]=File(name,set,path);
  };

  void Files::AddFile(std::string name, std::string path){
    files_[name]=File(name,"",path);
  };

  void Files::AddFiles(std::vector<std::string> names, std::vector<std::string> sets, std::vector<std::string> paths){
    if(names.size()!=sets.size() || sets.size()!=paths.size()) std::cout<<"Error different numbers of names, sets and paths not adding!"<<std::endl;
    else{
      for(unsigned iname=0;iname<names.size();iname++){
        files_[names[iname]]=File(names[iname],sets[iname],paths[iname]);
        setlists_[sets[iname]].push_back(names[iname]);
      }
    }
  };

  void Files::AddFiles(std::vector<std::string> names, std::vector<std::string> paths){
    if(names.size()!=paths.size()) std::cout<<"Error different numbers of names, and paths not adding!"<<std::endl;
    else{
      for(unsigned iname=0;iname<names.size();iname++){
        files_[names[iname]]=File(names[iname],"",paths[iname]);
      }
    }
  };

  void Files::AddFile(File file){
    files_[file.name()]=file;
    if(file.set()!=""){
      setlists_[file.set()].push_back(file.name());
    }
  }

  void Files::AddFiles(std::vector<File> files){
    for(unsigned ifile=0;ifile<files.size();ifile++){
      files_[files[ifile].name()]=files[ifile];
      if(files[ifile].set()!=""){
	setlists_[files[ifile].set()].push_back(files[ifile].name());
      }
    }
  }

  File Files::GetFile(std::string filename){
    if (files_.count(filename)>0){
      File file=files_[filename];
      return file;
    }
    else{
      std::cout<<"No file called "<<filename<<" returning empty File object expect errors"<<std::endl;
      File file;
      return file;
    }
  };

  std::vector<File> Files::GetFileSet(std::string setname){
    std::vector<File> filesinset;
    if(setlists_.count(setname)>0){
      for(auto iter=setlists_[setname].begin(); iter!=setlists_[setname].end();++iter){
	filesinset.push_back(files_[*iter]);
      }
    }
    else{
      std::cout<<"No set called "<<setname<<" returning empty File vector object expect errors"<<std::endl;
    }
    return filesinset;
  };

  std::string Files::GetPath(std::string filename){
    std::string path;
    if(files_.count(filename)>0){
      path=files_[filename].path();
    }
    else{
      std::cout<<"No file called "<<filename<<" returning empty string expect errors"<<std::endl;
    }
    return path;
  };

  std::vector<std::string> Files::GetSetPaths(std::string setname){
    std::vector<std::string> pathsinset;
    if(setlists_.count(setname)>0){
      for(auto iter=setlists_[setname].begin(); iter!=setlists_[setname].end();++iter){
	pathsinset.push_back(files_[*iter].path());
      }
    }
    else{
      std::cout<<"No set called "<<setname<<" returning empty string vector expect errors"<<std::endl;
    }
    return pathsinset;
  };

  int Files::OpenFile(std::string filename){
    if(files_.count(filename)>0){
      files_[filename].Open(infolder_);
      return 0;
    }
    else{
      std::cout<<"No file called "<<filename<<" error! "<<std::endl;
      return 1;
    }
  };

  int Files::OpenSet(std::string setname){
    if(setlists_.count(setname)>0){
      for(auto iter=setlists_[setname].begin(); iter!=setlists_[setname].end();++iter){
	files_[*iter].Open(infolder_);      
      }
      return 0;
    }
    else{
      std::cout<<"No set called "<<setname<<" error!"<<std::endl;
      return 1;
    }
    
  };

  int Files::OpenAll(){
    for(auto iter=files_.begin();iter!=files_.end();iter++){
      iter->second.Open(infolder_);
    }
    return 0;
  };
  
}




