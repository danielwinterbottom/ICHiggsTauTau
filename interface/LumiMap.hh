#ifndef ICHiggsTauTau_LumiMap_hh
#define ICHiggsTauTau_LumiMap_hh
#include <string>
#include <map>
#include <iostream>

namespace ic {

  class LumiMap {

  private:
    typedef std::map<unsigned, double> UDMap;

  public:
    LumiMap();
    virtual ~LumiMap();
    // If we parse a JSON file set run_info and ls_info to true;
    bool ParseJsonFile(std::string const& file);
    // If we parse a Run-based CSV file set run_info, ls_info and
    // run_lumi to true;
    //bool ParseRunCsvFile(std::string const& file);
    // If we parse a LS-based CSV file set run_info, ls_info,
    // ls_lumi and run_lumi to true;
    //bool ParseLsCsvFile(std::string const& file);
    bool InMap(unsigned run);
    bool InMap(unsigned run, unsigned ls);


private:
  //Whether or not we have a list of runs
  bool run_info_;
  // Whether or not we have lists of LSs per run
  bool ls_info_;
  // Whether or not we have lumi per LS
  bool ls_lumi_;
  // Whether or not we have lumi per run
  bool run_lumi_;
  std::map<unsigned, UDMap> ls_data_;
  UDMap run_data_;

  static const double DEFAULT_LUMI;

};

const double LumiMap::DEFAULT_LUMI = -1.0;

}
#endif
