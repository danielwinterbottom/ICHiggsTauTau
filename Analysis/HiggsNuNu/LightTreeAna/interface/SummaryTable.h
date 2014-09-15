#ifndef ICHiggsTauTau_HiggsNuNu_SummaryTable_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_SummaryTable_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>

namespace ic {

  class SummaryTable : public LTModule{
    CLASS_MEMBER(SummaryTable,std::vector<std::string>,shape)
    CLASS_MEMBER(SummaryTable,std::vector<std::string>,dirs)
    CLASS_MEMBER(SummaryTable,std::string,dirname)
  public:
    virtual SummaryTable & set_shape(std::string const& shape) {
      std::vector<std::string> shapes;
      shapes.push_back(shape);
      shape_ = shapes;
      return *this;
    }
    virtual SummaryTable & set_dirs(std::string const& dirs) {
      std::vector<std::string> dirss;
      dirss.push_back(dirs);
      dirs_ = dirss;
      return *this;
    }
    SummaryTable(std::string);
    virtual ~SummaryTable();
    virtual int Init(TFile*);
    virtual int Run(LTFiles*);
  };

}
#endif
