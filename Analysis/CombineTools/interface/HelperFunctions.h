#ifndef ICHiggsTauTau_CombineTools_HelperFunctions_h
#define ICHiggsTauTau_CombineTools_HelperFunctions_h
#include <algorithm>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include "boost/range/algorithm_ext/erase.hpp"

// #include <memory>
// #include <string>
// #include "TH1.h"
// #include "CombineTools/interface/Process.h"

namespace ch {

template<typename Range, typename Predicate> 
bool any_of(const Range &r, Predicate p) {
  return std::any_of(boost::begin(r), boost::end(r), p);
} 

template<typename Range, typename T> 
bool contains(const Range &r, T p) {
  return std::find(boost::begin(r), boost::end(r), p) != boost::end(r);
} 

template <typename Input, typename Filter, typename Converter>
void FilterContaining(Input & in, Filter const& filter, Converter fn, bool cond) {
  boost::remove_erase_if(in, [&](typename Input::value_type const& p) {
    return cond != ch::contains(filter, fn(p));
  });
}

template<class T, class U>
bool MatchingProcess(T const& first, U const& second) {
  if (first.bin()         == second.bin()         &&
      first.process()     == second.process()     && 
      first.process_id()  == second.process_id()  && 
      first.analysis()    == second.analysis()    &&
      first.era()         == second.era()         &&
      first.channel()     == second.channel()     &&
      first.bin_id()      == second.bin_id()      &&
      first.mass()        == second.mass()) {
    return true;
  } else {
    return false;
  }
}
}

#endif
