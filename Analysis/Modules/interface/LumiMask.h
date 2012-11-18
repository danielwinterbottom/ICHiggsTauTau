#ifndef ICHiggsTauTau_Module_LumiMask_h
#define ICHiggsTauTau_Module_LumiMask_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include <string>

namespace ic {


class LumiMask : public ModuleBase {


 private:
 	typedef std::map< unsigned, std::set<unsigned> > Json;
 	typedef std::map< unsigned, std::set<unsigned> >::const_iterator JsonIt;

 	Json input_json;
 	Json accept_json;
 	Json reject_json;
 	Json all_json;
  CLASS_MEMBER(LumiMask, std::string, input_file)
  CLASS_MEMBER(LumiMask, std::string, produce_output_jsons)

  void WriteJson(Json const& json, std::ofstream & output);


 public:
  LumiMask(std::string const& name);
  virtual ~LumiMask();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
