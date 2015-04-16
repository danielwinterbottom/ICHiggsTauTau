#ifndef ICHiggsTauTau_Module_LumiMask_h
#define ICHiggsTauTau_Module_LumiMask_h

#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include <string>

namespace ic {

class LumiMask : public ModuleBase {
 private:
  typedef std::map<unsigned, std::set<unsigned> > JsonMap;
  typedef std::map<unsigned, std::set<unsigned> >::const_iterator JsonIt;

  JsonMap input_json;
  JsonMap accept_json;
  JsonMap reject_json;
  JsonMap all_json;
  CLASS_MEMBER(LumiMask, std::string, input_file)
  CLASS_MEMBER(LumiMask, std::string, produce_output_jsons)

 private:
  void WriteJson(JsonMap const& json, std::ofstream& output);

 public:
  LumiMask(std::string const& name);
  virtual ~LumiMask();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent* event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};
}

#endif
