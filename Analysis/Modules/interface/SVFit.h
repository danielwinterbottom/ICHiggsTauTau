#ifndef ICHiggsTauTau_Module_SVFit_h
#define ICHiggsTauTau_Module_SVFit_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "boost/filesystem.hpp"

#include <string>
#include <fstream>

namespace ic {

class SVFit : public ModuleBase {
 private:
  boost::filesystem::path total_path_;
  std::map<std::string, double> mass_map;
 
  CLASS_MEMBER(SVFit, unsigned, mode)
  CLASS_MEMBER(SVFit, int, op)
  CLASS_MEMBER(SVFit, std::string, outname)
  CLASS_MEMBER(SVFit, unsigned, split)
  CLASS_MEMBER(SVFit, std::string, fullpath)
  CLASS_MEMBER(SVFit, std::string, met_label)
  CLASS_MEMBER(SVFit, std::string, dilepton_label)

  unsigned file_counter;
  unsigned event_counter;

  std::ofstream outFile;
  std::ofstream outFileSubmit;


 public:
  SVFit(std::string const& name);
  virtual ~SVFit();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  void WriteRunScript();
};

}

#endif
