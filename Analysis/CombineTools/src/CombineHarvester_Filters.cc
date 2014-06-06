#include "CombineTools/interface/CombineHarvester.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <utility>
#include <set>
#include <fstream>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/range/algorithm_ext/erase.hpp"
#include "boost/range/algorithm/find.hpp"
#include "boost/format.hpp"
#include "TDirectory.h"
#include "TH1.h"
#include "Utilities/interface/FnRootTools.h"
#include "Utilities/interface/FnPredicates.h"
#include "CombineTools/interface/Observation.h"
#include "CombineTools/interface/Process.h"
#include "CombineTools/interface/Nuisance.h"
#include "CombineTools/interface/Parameter.h"
#include "CombineTools/interface/MakeUnique.h"
#include "CombineTools/interface/HelperFunctions.h"

// #include "TMath.h"
// #include "boost/format.hpp"
// #include "Utilities/interface/FnPredicates.h"
// #include "Math/QuantFuncMathCore.h"

namespace ch {

CombineHarvester & CombineHarvester::bin(bool cond,
    std::vector<std::string> const& vec) {
  FilterContaining(procs_, vec, std::mem_fn(&Process::bin), cond);
  FilterContaining(obs_, vec, std::mem_fn(&Observation::bin), cond);
  FilterContaining(nus_, vec, std::mem_fn(&Nuisance::bin), cond);
  return *this;
}

CombineHarvester & CombineHarvester::bin_id(bool cond,
    std::vector<int> const& vec) {
  FilterContaining(procs_, vec, std::mem_fn(&Process::bin_id), cond);
  FilterContaining(obs_, vec, std::mem_fn(&Observation::bin_id), cond);
  FilterContaining(nus_, vec, std::mem_fn(&Nuisance::bin_id), cond);
  return *this;
}

CombineHarvester & CombineHarvester::process(bool cond,
    std::vector<std::string> const& vec) {
  FilterContaining(procs_, vec, std::mem_fn(&Process::process), cond);
  FilterContaining(nus_, vec, std::mem_fn(&Nuisance::process), cond);
  return *this;
}

CombineHarvester & CombineHarvester::process_id(bool cond,
    std::vector<int> const& vec) {
  FilterContaining(procs_, vec, std::mem_fn(&Process::process_id), cond);
  FilterContaining(nus_, vec, std::mem_fn(&Nuisance::process_id), cond);
  return *this;
}

CombineHarvester & CombineHarvester::analysis(bool cond,
    std::vector<std::string> const& vec) {
  FilterContaining(procs_, vec, std::mem_fn(&Process::analysis), cond);
  FilterContaining(obs_, vec, std::mem_fn(&Observation::analysis), cond);
  FilterContaining(nus_, vec, std::mem_fn(&Nuisance::analysis), cond);
  return *this;
}

CombineHarvester & CombineHarvester::era(bool cond,
    std::vector<std::string> const& vec) {
  FilterContaining(procs_, vec, std::mem_fn(&Process::era), cond);
  FilterContaining(obs_, vec, std::mem_fn(&Observation::era), cond);
  FilterContaining(nus_, vec, std::mem_fn(&Nuisance::era), cond);
  return *this;
}

CombineHarvester & CombineHarvester::channel(bool cond,
    std::vector<std::string> const& vec) {
  FilterContaining(procs_, vec, std::mem_fn(&Process::channel), cond);
  FilterContaining(obs_, vec, std::mem_fn(&Observation::channel), cond);
  FilterContaining(nus_, vec, std::mem_fn(&Nuisance::channel), cond);
  return *this;
}

CombineHarvester & CombineHarvester::mass(bool cond,
    std::vector<std::string> const& vec) {
  FilterContaining(procs_, vec, std::mem_fn(&Process::mass), cond);
  FilterContaining(obs_, vec, std::mem_fn(&Observation::mass), cond);
  FilterContaining(nus_, vec, std::mem_fn(&Nuisance::mass), cond);
  return *this;
}

CombineHarvester & CombineHarvester::nus_name(bool cond,
    std::vector<std::string> const& vec) {
  FilterContaining(nus_, vec, std::mem_fn(&Nuisance::name), cond);
  return *this;
}

CombineHarvester & CombineHarvester::nus_type(bool cond,
    std::vector<std::string> const& vec) {
  FilterContaining(nus_, vec, std::mem_fn(&Nuisance::type), cond);
  return *this;
}

 CombineHarvester & CombineHarvester::signals() {
   ic::erase_if(nus_, [&] (std::shared_ptr<Nuisance> val) { return val->process_id() > 0; });
   ic::erase_if(procs_, [&] (std::shared_ptr<Process> val) { return val->process_id() > 0; });
   return *this;
 }

 CombineHarvester & CombineHarvester::backgrounds() {
   ic::erase_if(nus_, [&] (std::shared_ptr<Nuisance> val) { return val->process_id() <= 0; });
   ic::erase_if(procs_, [&] (std::shared_ptr<Process> val) { return val->process_id() <= 0; });
   return *this;
 }

 CombineHarvester & CombineHarvester::histograms() {
   ic::erase_if(obs_, [&] (std::shared_ptr<Observation> val) { return val->shape() == nullptr; });
   ic::erase_if(procs_, [&] (std::shared_ptr<Process> val) { return val->shape() == nullptr; });
   return *this;
 }

 CombineHarvester & CombineHarvester::pdfs() {
   ic::erase_if(procs_, [&] (std::shared_ptr<Process> val) { return val->pdf() == nullptr; });
   return *this;
 }

 CombineHarvester & CombineHarvester::data() {
   ic::erase_if(obs_, [&] (std::shared_ptr<Observation> val) { return val->data() == nullptr; });
   return *this;
 }

}
