#ifndef ICHiggsTauTau_HiggsNuNu_LightTreeFiles_h
#define ICHiggsTauTau_HiggsNuNu_LightTreeFiles_h
#include <vector>
#include <map>
#include "HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include "TTree.h"
#include "TFile.h"

namespace ic{

  class File{
    CLASS_MEMBER(File,std::string,name)
    CLASS_MEMBER(File,std::string,set)
    CLASS_MEMBER(File,std::string,path)
    protected:
    TFile * tfile_;
    TTree * tree_;
    public:
    File();
    File(std::string,std::string);
    File(std::string,std::string,std::string);
    int Open(std::string);
  };

  class Files{
    CLASS_MEMBER(Files,std::string,infolder)
    protected:								
    std::map<std::string,File> files_;					
    std::map<std::string,std::vector<std::string> > setlists_;
    public:
    Files();
    Files(std::string,std::string);
    Files(std::string,std::string,std::string);
    Files(std::vector<std::string>,std::vector<std::string>,std::vector<std::string>);
    Files(File);
    Files(std::vector<File>);

    void AddFile(std::string,std::string,std::string);
    void AddFile(std::string,std::string);
    void AddFiles(std::vector<std::string>,std::vector<std::string>,std::vector<std::string>);
    void AddFiles(std::vector<std::string>,std::vector<std::string>);
    void AddFile(File);
    void AddFiles(std::vector<File>);

    File GetFile(std::string);
    std::string GetPath(std::string);
    std::vector<File> GetFileSet(std::string);    
    std::vector<std::string> GetSetPaths(std::string);
    
    int OpenFile(std::string);
    int OpenSet(std::string);
    int OpenAll();
  };

}

#endif
