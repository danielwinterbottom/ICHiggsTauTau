#ifndef ICHiggsTauTau_HiggsNuNu_LightTreeFiles_h
#define ICHiggsTauTau_HiggsNuNu_LightTreeFiles_h
#include <vector>
#include <map>
#include "HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include "TTree.h"
#include "TFile.h"

namespace ic{

  class LTFile{
    CLASS_MEMBER(LTFile,std::string,name)
    CLASS_MEMBER(LTFile,std::string,set)
    CLASS_MEMBER(LTFile,std::string,path)
    protected:
    TFile * tfile_;
    TTree * tree_;
    public:
    LTFile();
    LTFile(std::string,std::string);
    LTFile(std::string,std::string,std::string);
    int Open(std::string);
    int AddFriend(TTree*);
  };

  class LTFiles{
    CLASS_MEMBER(LTFiles,std::string,infolder)
    protected:								
    std::map<std::string,LTFile> files_;					
    std::map<std::string,std::vector<std::string> > setlists_;
    public:
    LTFiles();
    LTFiles(std::string,std::string);
    LTFiles(std::string,std::string,std::string);
    LTFiles(std::vector<std::string>,std::vector<std::string>,std::vector<std::string>);
    LTFiles(LTFile);
    LTFiles(std::vector<LTFile>);

    void AddFile(std::string,std::string,std::string);
    void AddFile(std::string,std::string);
    void AddFiles(std::vector<std::string>,std::vector<std::string>,std::vector<std::string>);
    void AddFiles(std::vector<std::string>,std::vector<std::string>);
    void AddFile(LTFile);
    void AddFiles(std::vector<LTFile>);

    LTFile GetFile(std::string);
    std::string GetPath(std::string);
    std::vector<LTFile> GetFileSet(std::string);    
    std::vector<std::string> GetSetPaths(std::string);
    
    int OpenFile(std::string);
    int OpenSet(std::string);
    int OpenAll();

    int AddFriend(std::string,TTree*);

  };

}

#endif
