#ifndef ICHiggsTauTau_HiggsNuNu_TrigEffAnalysis_h
#define ICHiggsTauTau_HiggsNuNu_TrigEffAnalysis_h
#include <vector>
#include <map>
#include "TH1.h"
#include "TH3.h"
#include "TFile.h"
#include "TTree.h"
#include "Utilities/interface/SimpleParamParser.h"
#include "Utilities/interface/FnRootTools.h"
#include "HiggsNuNu/interface/HinvVar.h"
#include "HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"

namespace ic{

  class TrigEffAnalysis {
  protected:
    std::map<std::string,std::string> filepaths_;
    std::map<std::string, TFile *> tfiles_;
    CLASS_MEMBER(TrigEffAnalysis,int,verbosity)
    CLASS_MEMBER(TrigEffAnalysis,std::string,baseselection)
    CLASS_MEMBER(TrigEffAnalysis,std::vector<Rebinned1DVar*>,triggers)
    CLASS_MEMBER(TrigEffAnalysis,std::vector<Rebinned1DVar*>, vars)
  public:
    virtual TrigEffAnalysis & set_filepaths(std::map<std::string,std::string> const& filepaths){filepaths_=filepaths;return *this;}
    virtual std::map<std::string,std::string> & filepaths() {return filepaths_;}
    virtual std::vector<std::string> filenames();
    virtual std::string filenames(int);
    virtual unsigned nfiles();

    TrigEffAnalysis & triggersPushBack(Rebinned1DVar*);
    TrigEffAnalysis & varsPushBack(Rebinned1DVar*);
    TrigEffAnalysis(int);
    TrigEffAnalysis();
    TrigEffAnalysis & addfile(std::string,std::string);
    virtual int OpenFiles(std::string);
    TTree* GetTree(std::string);

  };

  class EffAndError1D {
  public:
    std::vector<double> effs;
    std::vector<std::pair<double,double> > errs;
  };

  class EffAndError3D {
  public:
    std::vector<std::vector<std::vector<double> > > effs;
    std::vector<std::vector<std::vector<std::pair<double,double> > > > errs;
  };

}
#endif
