#include "UserCode/ICHiggsTauTau/interface/LumiMap.hh"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/lexical_cast.hpp"

namespace ic {
  //Constructors/Destructors
  LumiMap::LumiMap() {
    run_info_ = false;
    ls_info_ = false;
    run_lumi_ = false;
    ls_lumi_ = false;
  }

  LumiMap::~LumiMap() {
  }

  bool LumiMap::ParseJsonFile(std::string const& file) {
   using boost::property_tree::ptree;
   using boost::lexical_cast;
   ptree pt;
   boost::property_tree::read_json(file,pt);
   run_info_ = true;
   ls_info_ = true;
   ptree::const_iterator it;
   for (it = pt.begin(); it != pt.end(); ++it) {
    unsigned run = lexical_cast<unsigned>(it->first);
    run_data_[run] = DEFAULT_LUMI;
    ls_data_[run] = UDMap();
    ptree const& l1 = it->second;
    ptree::const_iterator l1_it;
    for (l1_it = l1.begin(); l1_it != l1.end(); ++l1_it) {
      ptree const& l2 = l1_it->second;
      ptree::const_iterator l2_it;
      unsigned range_min = 0;
      unsigned range_max = 0;
      unsigned counted = 0;
      for (l2_it = l2.begin(); l2_it != l2.end(); ++l2_it) {
        ++counted;
        if (counted == 1) range_min = lexical_cast<unsigned>(l2_it->second.data());
        if (counted == 2) range_max = lexical_cast<unsigned>(l2_it->second.data());
      }
      if (counted != 2 || range_max < range_min) {
        std::cout << "Something has gone wrong" << std::endl;
        return false;
      } else {
        for (unsigned i = range_min; i <= range_max; ++i) {
          ls_data_[run][i] = DEFAULT_LUMI;
        }
      }
    }
  }
  return true;
  }

  bool LumiMap::InMap(unsigned run) {
    if (!run_info_) {
      std::cerr << "Warning: No run information has been entered into this map" << std::endl;
      return false;
    } {
      if (run_data_.count(run)) {
        return true;
      } else {
        return false;
      }
    }
  }

  bool LumiMap::InMap(unsigned run, unsigned ls) {
    if (!ls_info_) {
      std::cerr << "Warning: No lumisection information has been entered into this map" << std::endl;
      return false;
    } {
      if (ls_data_.count(run)) {
        if (ls_data_[run].count(ls)) {
          return true;
        } else {
          return false;
        }
      } else {
        return false;
      }
    }
  }




}
    