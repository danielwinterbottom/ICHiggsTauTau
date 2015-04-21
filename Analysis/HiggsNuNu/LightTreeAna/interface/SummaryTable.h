#ifndef ICHiggsTauTau_HiggsNuNu_SummaryTable_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_SummaryTable_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeModule.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/LightTreeFiles.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/LightTreeAna/interface/HistPlotter.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>

namespace ic {

  class SummaryTable : public LTModule{
    CLASS_MEMBER(SummaryTable,std::vector<LTShapeElement>,shape)
    CLASS_MEMBER(SummaryTable,std::vector<std::string>,dirs)
    CLASS_MEMBER(SummaryTable,std::string,dirname)
  public:
    virtual SummaryTable & set_shape(std::string const& shape) {
      std::vector<LTShapeElement> shapes;
      LTShapeElement thisshape;
      thisshape.set_name(shape);
      shapes.push_back(thisshape);
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
