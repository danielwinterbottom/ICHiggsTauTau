#include "CombineTools/interface/HighLevelFunctions.h"
#include <vector>
#include <string>
#include <set>
#include "CombineTools/interface/CombineHarvester.h"
#include "CombineTools/interface/HelperFunctions.h"

namespace ch {
void SetStandardBinNames(CombineHarvester* cb) {
  cb->ForEachObs(ch::SetStandardBinName<ch::Observation>);
  cb->ForEachProc(ch::SetStandardBinName<ch::Process>);
  cb->ForEachNus(ch::SetStandardBinName<ch::Nuisance>);
}

std::vector<std::string> MassesFromRange(std::string const& input,
                                         std::string const& fmt) {
  std::set<double> mass_set;
  std::vector<std::string> tokens;
  boost::split(tokens, input, boost::is_any_of(","));
  for (auto const& t : tokens) {
    std::vector<std::string> sub_tokens;
    boost::split(sub_tokens, t, boost::is_any_of("-:"));
    if (sub_tokens.size() == 1) {
      double mass_val = boost::lexical_cast<double>(sub_tokens[0]);
      mass_set.insert(mass_val);
    } else if (sub_tokens.size() == 3) {
      double lo = boost::lexical_cast<double>(sub_tokens[0]);
      double hi = boost::lexical_cast<double>(sub_tokens[1]);
      double step = boost::lexical_cast<double>(sub_tokens[2]);
      if (hi <= lo)
        throw std::runtime_error(
            "[MassesFromRange] High mass is smaller than low mass!");
      double start = lo;
      while (start < hi + 0.001) {
        mass_set.insert(start);
        start += step;
      }
    }
  }
  std::vector<std::string> result;
  for (auto const& m : mass_set) {
    result.push_back((boost::format(fmt) % m).str());
  }
  return result;
}
}

