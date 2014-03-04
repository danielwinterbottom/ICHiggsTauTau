#ifndef ICHiggsTauTau_CombineTools_HelperFunctions_h
#define ICHiggsTauTau_CombineTools_HelperFunctions_h
#include <algorithm>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include "boost/range/algorithm_ext/erase.hpp"
#include "boost/algorithm/string.hpp"
#include "TFile.h"

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

template<class T>
void WriteToTFile(T const* ptr, TFile *file, std::string const& path) {
  file->cd();
  std::vector<std::string> as_vec;
  boost::split(as_vec, path, boost::is_any_of("/"));
  if (as_vec.size() >= 1) {
    for (unsigned i = 0; i < as_vec.size()-1; ++i) {
      if (!gDirectory->GetDirectory(as_vec[i].c_str())) {
        gDirectory->mkdir(as_vec[i].c_str());
      }
      gDirectory->cd(as_vec[i].c_str());
    }
    gDirectory->WriteTObject(ptr, as_vec.back().c_str());
    gDirectory->cd("/");    
  }
}
}

#endif
