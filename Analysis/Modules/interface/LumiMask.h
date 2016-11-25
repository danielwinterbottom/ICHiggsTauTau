#ifndef ICHiggsTauTau_Module_LumiMask_h
#define ICHiggsTauTau_Module_LumiMask_h

#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "Utilities/interface/JsonTools.h"
#include <string>

namespace ic {

/**
 * Filters events using a standard CMS luminosity json file
 *
 */
class LumiMask : public ModuleBase {
 private:
  typedef std::map<unsigned, std::set<unsigned>> JsonMap;
  typedef std::map<unsigned, std::set<unsigned>>::const_iterator JsonIt;

  JsonMap input_json_;
  JsonMap accept_json_;
  JsonMap reject_json_;
  JsonMap all_json_;
  CLASS_MEMBER(LumiMask, std::string, input_file)
  CLASS_MEMBER(LumiMask, std::string, produce_output_jsons)

 private:
  void FillJsonMapFromJson(JsonMap & jsmap, Json::Value const& js);
  Json::Value JsonFromJsonMap(JsonMap const& jsmap);
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
