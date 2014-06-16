#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include "boost/lexical_cast.hpp"
#include <iostream>
#include <vector>

namespace ic{
  LTFile::LTFile(){
  };

  LTFile::LTFile(std::string name, std::string path){
    name_=name;
    path_=path;
    set_="";
  };
  
  LTFile::LTFile(std::string name, std::string set, std::string path){
    name_=name;
    set_=set;
    path_=path;
  };

  int LTFile::Open(std::string infolder){
    //std::cout<<"Opening TFile..."<<std::endl;
    std::string filepath = (this->path());
    TFile * tmp = TFile::Open((infolder+"/"+this->path()).c_str());
    if (!tmp) {
      std::cerr << "Warning, file " << this->name() << " could not be opened." << std::endl;
      return 1;
    }
    tfile_ = tmp;
    tree_=(TTree *)tfile_->Get("LightTree");
    tree_->SetEstimate(1000);
    if(friendTrees.size()!=0){
      for(unsigned iFriend=0;iFriend<friendTrees.size();iFriend++){
	tree_->AddFriend(friendTrees[iFriend].first.c_str(),friendTrees[iFriend].second.c_str());
      }
    }
    return 0;
  }

  int LTFile::Close(){
    tfile_->Close();
    return 0;
  }

  int LTFile::AddFriend(TTree* treeptr){
    TFile* file = treeptr->GetCurrentFile();
    tree_->AddFriend(treeptr->GetName(),file->GetName());
    friendTrees.push_back(std::make_pair(treeptr->GetName(),file->GetName()));
    return 0;
  };

  int LTFile::AddFriend(std::string treeName, std::string fileName){
    tree_->AddFriend(treeName.c_str(),fileName.c_str());
    friendTrees.push_back(std::make_pair(treeName,fileName));
    return 0;
  };


  TH1F LTFile::GetShape(std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight){
    return ic::GetShape(variable,selection,category,weight,tree_);
  };

  TH3F LTFile::GetShape3D(std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight){
    return ic::GetShape3D(variable,selection,category,weight,tree_);
  };

  TTree* LTFile::GetTree(){
    return tree_;
  }




  LTFiles::LTFiles(){
  };

  LTFiles::LTFiles(std::string name, std::string set, std::string path){
    files_[name]=LTFile(name,set,path);
    setlists_[set].push_back(name);
  };

  LTFiles::LTFiles(std::string name, std::string path){
    files_[name]=LTFile(name,path);
  };
  
  LTFiles::LTFiles(std::vector<std::string> names,std::vector<std::string> sets, std::vector<std::string> paths){
    if(names.size()!=sets.size() || sets.size()!=paths.size()) std::cout<<"Error different numbers of names, sets and paths making empty Files object"<<std::endl;
    else{
      for(unsigned iname=0;iname<names.size();iname++){
	files_[names[iname]]=LTFile(names[iname],sets[iname],paths[iname]);
	setlists_[sets[iname]].push_back(names[iname]);
      }
    }
  };

  LTFiles::LTFiles(LTFile file){
    files_[file.name()]=file;
    if(file.set()!=""){
      setlists_[file.set()].push_back(file.name());
    }
  };
  
  LTFiles::LTFiles(std::vector<LTFile> files){
    for(unsigned ifile=0;ifile<files.size();ifile++){
      files_[files[ifile].name()]=files[ifile];
      if(files[ifile].set()!=""){
	setlists_[files[ifile].set()].push_back(files[ifile].name());
      }
    }
  };

  void LTFiles::AddFile(std::string name, std::string set, std::string path){
    setlists_[set].push_back(name);
    files_[name]=LTFile(name,set,path);
  };

  void LTFiles::AddFile(std::string name, std::string path){
    files_[name]=LTFile(name,"",path);
  };

  void LTFiles::AddFiles(std::vector<std::string> names, std::vector<std::string> sets, std::vector<std::string> paths){
    if(names.size()!=sets.size() || sets.size()!=paths.size()) std::cout<<"Error different numbers of names, sets and paths not adding!"<<std::endl;
    else{
      for(unsigned iname=0;iname<names.size();iname++){
        files_[names[iname]]=LTFile(names[iname],sets[iname],paths[iname]);
        setlists_[sets[iname]].push_back(names[iname]);
      }
    }
  };

  void LTFiles::AddFiles(std::vector<std::string> names, std::vector<std::string> paths){
    if(names.size()!=paths.size()) std::cout<<"Error different numbers of names, and paths not adding!"<<std::endl;
    else{
      for(unsigned iname=0;iname<names.size();iname++){
        files_[names[iname]]=LTFile(names[iname],"",paths[iname]);
      }
    }
  };

  void LTFiles::AddFile(LTFile file){
    files_[file.name()]=file;
    if(file.set()!=""){
      setlists_[file.set()].push_back(file.name());
    }
  }

  void LTFiles::AddFiles(std::vector<LTFile> files){
    for(unsigned ifile=0;ifile<files.size();ifile++){
      files_[files[ifile].name()]=files[ifile];
      if(files[ifile].set()!=""){
	setlists_[files[ifile].set()].push_back(files[ifile].name());
      }
    }
  }

  LTFile LTFiles::GetFile(std::string filename){
    if (files_.count(filename)>0){
      LTFile file=files_[filename];
      return file;
    }
    else{
      std::cout<<"No file called "<<filename<<" returning empty File object expect errors"<<std::endl;
      LTFile file;
      return file;
    }
  };

  std::vector<LTFile> LTFiles::GetFileSet(std::string setname){
    std::vector<LTFile> filesinset;
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

  std::string LTFiles::GetPath(std::string filename){
    std::string path;
    if(files_.count(filename)>0){
      path=files_[filename].path();
    }
    else{
      std::cout<<"No file called "<<filename<<" returning empty string expect errors"<<std::endl;
    }
    return path;
  };

  std::vector<std::string> LTFiles::GetSetPaths(std::string setname){
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

  std::vector<std::string> LTFiles::GetSetNames(std::string setname){
    std::vector<std::string> namesinset;
    if(setlists_.count(setname)>0){
      for(auto iter=setlists_[setname].begin(); iter!=setlists_[setname].end();++iter){
	namesinset.push_back(files_[*iter].name());
      }
    }
    else{
      std::cout<<"No set called "<<setname<<" returning empty string vector expect errors"<<std::endl;
    }
    return namesinset;
  };

  int LTFiles::OpenFile(std::string filename){
    if(files_.count(filename)>0){
      if(files_[filename].Open(infolder_)==1){
	std::cout<<"other stuff";
	return 1;
      }
      return 0;
    }
    else{
      std::cout<<"No file called "<<filename<<" error! "<<std::endl;
      return 1;
    }
  };

  int LTFiles::OpenSet(std::string setname){
    if(setlists_.count(setname)>0){
      for(auto iter=setlists_[setname].begin(); iter!=setlists_[setname].end();++iter){
	if((files_[*iter].Open(infolder_))==1){
	  std::cout<<"stuff";
	  return 1;
	}
      }
      return 0;
    }
    else{
      std::cout<<"No set called "<<setname<<" error!"<<std::endl;
      return 1;
    }
    
  };

  int LTFiles::OpenAll(){
    for(auto iter=files_.begin();iter!=files_.end();iter++){
      if((iter->second.Open(infolder_))==1)return 1;
    }
    return 0;
  };



  int LTFiles::CloseFile(std::string filename){
    if(files_.count(filename)){
      files_[filename].Close();
      return 0;
    }
    else{
      std::cout<<"No file called "<<filename<<" error! "<<std::endl;
      return 1;
    }
  }

  int LTFiles::CloseSet(std::string setname){
    if(setlists_.count(setname)>0){
      for(auto iter=setlists_[setname].begin(); iter!=setlists_[setname].end();++iter){
	files_[*iter].Close();
      }
      return 0;
    }
    else{
      std::cout<<"No set called "<<setname<<" error!"<<std::endl;
      return 1;
    }
  }

  int LTFiles::CloseAll(){
    for(auto iter=files_.begin();iter!=files_.end();iter++){
      iter->second.Close();
    }
    return 0;
  };

  int LTFiles::AddFriend(std::string filename, std::string treename, std::string treefilename){
    files_[filename].AddFriend(treename,treefilename);
    return 0;
  };

  TH1F LTFiles::GetShape(std::string filename, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight){
    if(OpenFile(filename)==1){
      std::cout<<"Problem opening file "<<filename << " returning empty TH1"<<std::endl;
      TH1F temp;
      return temp;
    }
    return files_[filename].GetShape(variable,selection,category,weight);
    CloseFile(filename);
  };


  TH1F LTFiles::GetSetShape(std::string setname, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, bool do_lumixs_weights_=true){
    TH1F setshape;
    if(setlists_.count(setname)>0){
      if(OpenSet(setname)==1){
	std::cout<<"Problem opening set "<<setname<<" returning empty TH1"<<std::endl;
	TH1F temp;
	return temp;
      }
      bool first=true;
      for(auto iter=setlists_[setname].begin(); iter!=setlists_[setname].end();++iter){
	//ADAPT LUMIXS BIT
	std::string sample_path_=files_[*iter].path();
	double lumixsweight=1;
	if(do_lumixs_weights_){
	  //Get Sample name file
	  std::string suffix=".root";
	  std::size_t found = sample_path_.find(suffix);
	  if(found==std::string::npos){
	    lumixsweight=1;
	    std::cout<<"Non-standard sample name format not doing lumixs weight"<<std::endl;
	  }
	  else{
	    sample_path_.erase(found,5);
	    //std::cout << "Sample Path: "<<sample_path_<<std::endl;

	    //Get lumi xs and events from params file
	    SimpleParamParser parser;
	    //std::cout << "** Parsing parameter file... **" << input_params_ << std::endl;
	    parser.ParseFile(input_params_);
	    //std::cout<<"parsed"<<std::endl;
	    double xs=parser.GetParam<double>("XS_"+sample_path_);
	    //std::cout<<"got xs"<<std::endl;
	    double events=parser.GetParam<double>("EVT_"+sample_path_);
	    //std::cout<<"got events"<<std::endl;
	    double lumi=parser.GetParam<double>("LUMI_DATA");
	    //std::cout<<"got lumi"<<std::endl;

	    //std::cout<<"XS is: "<<xs<<"pb"<<std::endl;
	    //std::cout<<"EVT is: "<<events<<std::endl;
	    //std::cout<<"LUMI is: "<<lumi<<"pb^-1"<<std::endl;
	    if(xs==-1) lumixsweight=1;
	    else lumixsweight=xs*lumi/events;
	    //std::cout<<"LUMIXSWEIGHT is: "<<lumixsweight<<std::endl;
	  }
	}
	std::cout<<"Getting shape for "<<files_[*iter].name()<<std::endl;//!!
	TH1F temp=files_[*iter].GetShape(variable,selection,category,weight+"*"+boost::lexical_cast<std::string>(lumixsweight));
	temp.Sumw2();
	if(first){
	  setshape=temp;
	  first=false;
	}
	else setshape.Add(&temp);
      }
      CloseSet(setname);
    }
    else{
      std::cout<<"No set called "<<setname<<" returning empty TH1 object expect errors"<<std::endl;
    }
    return setshape;
  };

  TH1F LTFiles::GetSetsShape(std::vector<std::string> setnames, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, bool do_lumixs_weights_=true){
    TH1F setsshape;
    bool firstset=true;
    for(unsigned iset=0;iset<setnames.size();iset++){
      TH1F setshape;
	if(setlists_.count(setnames[iset])>0){
	  if(OpenSet(setnames[iset])==1){
	    std::cout<<"Problem opening set "<<setnames[iset]<<" returning empty TH1"<<std::endl;
	    TH1F temp;
	    return temp;
	  }
	  bool firstshape=true;
	  for(auto iter=setlists_[setnames[iset]].begin(); iter!=setlists_[setnames[iset]].end();++iter){
	    //ADAPT LUMIXS BIT
	    std::string sample_path_=files_[*iter].path();
	    double lumixsweight=1;
	    if(do_lumixs_weights_){
	      //Get Sample name file
	      std::string suffix=".root";
	      std::size_t found = sample_path_.find(suffix);
	      if(found==std::string::npos){
		lumixsweight=1;
		std::cout<<"Non-standard sample name format not doing lumixs weight"<<std::endl;
	      }
	      else{
		sample_path_.erase(found,5);
		//std::cout << "Sample Path: "<<sample_path_<<std::endl;
		
		//Get lumi xs and events from params file
		SimpleParamParser parser;
		//std::cout << "** Parsing parameter file... **" << input_params_ << std::endl;
		parser.ParseFile(input_params_);
		//std::cout<<"parsed"<<std::endl;
		double xs=parser.GetParam<double>("XS_"+sample_path_);
		//std::cout<<"got xs"<<std::endl;
		double events=parser.GetParam<double>("EVT_"+sample_path_);
		//std::cout<<"got events"<<std::endl;
		double lumi=parser.GetParam<double>("LUMI_DATA");
		//std::cout<<"got lumi"<<std::endl;
		
// 		std::cout<<"XS is: "<<xs<<"pb"<<std::endl;
// 		std::cout<<"EVT is: "<<events<<std::endl;
// 		std::cout<<"LUMI is: "<<lumi<<"pb^-1"<<std::endl;
		if(xs==-1) lumixsweight=1;
		else lumixsweight=xs*lumi/events;
		//std::cout<<"LUMIXSWEIGHT is: "<<lumixsweight<<std::endl;
	      }
	    }
	    TH1F temp=files_[*iter].GetShape(variable,selection,category,weight+"*"+boost::lexical_cast<std::string>(lumixsweight));
	    temp.Sumw2();
	    if(firstshape){
	      setshape=temp;
	      firstshape=false;
	    }
	    else setshape.Add(&temp);
	  }
	  CloseSet(setnames[iset]);
	}
	else{
	  std::cout<<"No set called "<<setnames[iset]<<" returning empty TH1 object expect errors"<<std::endl;
	}
	if(firstset){
	  setsshape=setshape;
	  firstset=false;
	}
	else setsshape.Add(&setshape);
    }     
    return setsshape;
  };

  TH3F LTFiles::GetShape3D(std::string filename, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight){
    return files_[filename].GetShape3D(variable,selection,category,weight);
  };

  TH3F LTFiles::GetSetShape3D(std::string setname, std::string const& variable, std::string const& selection, std::string const& category, std::string const& weight, bool do_lumixs_weights_=true){
    TH3F setshape;
    if(setlists_.count(setname)>0){
      bool first=true;
      for(auto iter=setlists_[setname].begin(); iter!=setlists_[setname].end();++iter){
	std::string sample_path_=files_[*iter].path();
	double lumixsweight=1;
	if(do_lumixs_weights_){
	  //Get Sample name file
	  std::string suffix=".root";
	  std::size_t found = sample_path_.find(suffix);
	  if(found==std::string::npos){
	    lumixsweight=1;
	    std::cout<<"Non-standard sample name format not doing lumixs weight"<<std::endl;
	  }
	  else{
	    sample_path_.erase(found,5);
	    //std::cout << "Sample Name: "<<sample_path_<<std::endl;

	    //Get lumi xs and events from params file
	    SimpleParamParser parser;
	    //std::cout << "** Parsing parameter file... **" << input_params_ << std::endl;
	    parser.ParseFile(input_params_);
	    //std::cout<<"parsed"<<std::endl;
	    double xs=parser.GetParam<double>("XS_"+sample_path_);
	    //std::cout<<"got xs"<<std::endl;
	    double events=parser.GetParam<double>("EVT_"+sample_path_);
	    //std::cout<<"got events"<<std::endl;
	    double lumi=parser.GetParam<double>("LUMI_DATA");
	    //std::cout<<"got lumi"<<std::endl;

	    //std::cout<<"XS is: "<<xs<<"pb"<<std::endl;
	    // std::cout<<"EVT is: "<<events<<std::endl;
	    // std::cout<<"LUMI is: "<<lumi<<"pb^-1"<<std::endl;

	    if(xs==-1) lumixsweight=1;
	    else lumixsweight=xs*lumi/events;
	    //std::cout<<"LUMIXSWEIGHT is: "<<lumixsweight<<std::endl;
	  }
	}

	TH3F temp=files_[*iter].GetShape3D(variable,selection,category,weight+"*"+boost::lexical_cast<std::string>(lumixsweight));
	if(first){
	  setshape=temp;
	  first=false;
	}
	else setshape.Add(&temp);
      }
    }
    else{
      std::cout<<"No set called "<<setname<<" returning empty TH3 object expect errors"<<std::endl;
    }
    return setshape;
  };


  
}




