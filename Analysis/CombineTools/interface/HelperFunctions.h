#ifndef ICHiggsTauTau_CombineTools_HelperFunctions_h
#define ICHiggsTauTau_CombineTools_HelperFunctions_h
#include <algorithm>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include "boost/range/algorithm_ext/erase.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/regex.hpp"
#include "boost/bind.hpp"
#include "TFile.h"
#include "RooFitResult.h"

// #include <memory>
// #include <string>
// #include "TH1.h"
#include "CombineTools/interface/Parameter.h"
#include "CombineTools/interface/Nuisance.h"
#include "CombineTools/interface/Process.h"

namespace ch {

struct SOverBInfo {
  double s;
  double b;
  double x_lo;
  double x_hi;
  SOverBInfo() { ; }
  SOverBInfo(TH1F const* sig, TH1F const* bkg, unsigned steps, double frac);
};

double IntegrateFloatRange(TH1F const* hist, double xmin, double xmax);

std::vector<ch::Parameter> ExtractFitParameters(RooFitResult const& res);
std::vector<ch::Parameter> ExtractSampledFitParameters(RooFitResult const& res);

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
    if (!gDirectory->Get(as_vec.back().c_str())) {
      gDirectory->WriteTObject(ptr, as_vec.back().c_str());
    }
    gDirectory->cd("/");
  }
}

// Extracts objects from the form:
// "path/to/a/file.root:path/to/object"
template <class T>
T OpenFromTFile(std::string const& fullpath) {
  std::size_t pos = fullpath.find(':');
  std::string filepath = "";
  std::string objectpath = "";
  if (pos == std::string::npos || pos == 0 || pos == (fullpath.size()-1)) {
    std::cerr <<
      "Error in ch::OpenFromTFile: input path "
      "must of the format file.root:object" << std::endl;
    throw;
  } else {
    filepath = fullpath.substr(0, pos);
    objectpath = fullpath.substr(pos+1);
  }
  TFile file(filepath.c_str());
  if (!file.IsOpen()) {
    std::cerr << "Error in ch::OpenFromTFile: "
    "file \"" << filepath << "\" cannot be opened" << std::endl;
    throw;
  }
  file.cd();
  T* obj_ptr = dynamic_cast<T*>(gDirectory->Get(objectpath.c_str()));
  if (!obj_ptr) {
    std::cerr << "Error in ch::OpenFromTFile: "
    "Object " << objectpath << " in file " << filepath <<
    " missing or not of specified type" << std::endl;
    throw;
  }
  return *(dynamic_cast<T*>(gDirectory->Get(objectpath.c_str())));
}

template<class T>
void SetStandardBinName(T *input) {
  std::string newname;
  newname = input->analysis() + "_"
    + input->channel() + "_"
    + boost::lexical_cast<std::string>(input->bin_id()) + "_"
    + input->era();
  input->set_bin(newname);
}

template<class T>
void SetFromBinName(T *input, std::string parse_rules) {
  boost::replace_all(parse_rules, "{", "(?<");
  boost::replace_all(parse_rules, "}", ">\\w+)");
  boost::regex rgx(parse_rules);
  boost::smatch matches;
  boost::regex_search(input->bin(), matches, rgx);
  if (matches.str("ANALYSIS").length())
    input->set_analysis(matches.str("ANALYSIS"));
  if (matches.str("ERA").length())
    input->set_era(matches.str("ERA"));
  if (matches.str("CHANNEL").length())
    input->set_channel(matches.str("CHANNEL"));
  if (matches.str("BINID").length())
    input->set_bin_id( boost::lexical_cast<int>(matches.str("BINID")));
  if (matches.str("MASS").length())
    input->set_mass(matches.str("MASS"));
}

}

#endif
