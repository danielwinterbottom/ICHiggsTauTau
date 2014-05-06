#ifndef ICHiggsTauTau_HiggsNuNu_LightTreeFiles_h
#define ICHiggsTauTau_HiggsNuNu_LightTreeFiles_h
#include <vector>
#include <map>
#include "HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH3F.h"

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
    int Close();
    int AddFriend(TTree*);
    TH1F GetShape(std::string const&, std::string const&, std::string const&, std::string const&);
    TH3F GetShape3D(std::string const&, std::string const&, std::string const&, std::string const&);
  };

  class LTFiles{
    CLASS_MEMBER(LTFiles,std::string,infolder)
    CLASS_MEMBER(LTFiles,std::string,input_params)
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
    std::vector<std::string> GetSetNames(std::string);
    
    int OpenFile(std::string);
    int OpenSet(std::string);
    int OpenAll();

    int CloseFile(std::string);
    int CloseSet(std::string);
    int CloseAll();

    int AddFriend(std::string,TTree*);
    TH1F GetShape(std::string,std::string const&, std::string const&, std::string const&, std::string const&);
    TH1F GetSetShape(std::string,std::string const&, std::string const&, std::string const&, std::string const&,bool);
    TH1F GetSetsShape(std::vector<std::string>,std::string const&, std::string const&, std::string const&, std::string const&,bool);
    TH3F GetShape3D(std::string,std::string const&, std::string const&, std::string const&, std::string const&);
    TH3F GetSetShape3D(std::string,std::string const&, std::string const&, std::string const&, std::string const&,bool);
  };

}

#endif
