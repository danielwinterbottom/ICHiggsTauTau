#include "CombineTools/interface/CombineHarvester.h"
#include <vector>
#include <string>
#include <utility>
#include "Utilities/interface/FnRootTools.h"
#include "Utilities/interface/FnPredicates.h"
#include "CombineTools/interface/Observation.h"
#include "CombineTools/interface/Process.h"
#include "CombineTools/interface/Nuisance.h"
#include "CombineTools/interface/HelperFunctions.h"

namespace ch {

CombineHarvester& CombineHarvester::bin(
    std::vector<std::string> const& vec, bool cond) {
  FilterContaining(procs_, vec, std::mem_fn(&Process::bin), cond);
  FilterContaining(obs_, vec, std::mem_fn(&Observation::bin), cond);
  FilterContaining(nus_, vec, std::mem_fn(&Nuisance::bin), cond);
  return *this;
}

CombineHarvester& CombineHarvester::bin_id(
    std::vector<int> const& vec, bool cond) {
  FilterContaining(procs_, vec, std::mem_fn(&Process::bin_id), cond);
  FilterContaining(obs_, vec, std::mem_fn(&Observation::bin_id), cond);
  FilterContaining(nus_, vec, std::mem_fn(&Nuisance::bin_id), cond);
  return *this;
}

CombineHarvester& CombineHarvester::process(
    std::vector<std::string> const& vec, bool cond) {
  FilterContaining(procs_, vec, std::mem_fn(&Process::process), cond);
  FilterContaining(nus_, vec, std::mem_fn(&Nuisance::process), cond);
  return *this;
}

CombineHarvester& CombineHarvester::analysis(
    std::vector<std::string> const& vec, bool cond) {
  FilterContaining(procs_, vec, std::mem_fn(&Process::analysis), cond);
  FilterContaining(obs_, vec, std::mem_fn(&Observation::analysis), cond);
  FilterContaining(nus_, vec, std::mem_fn(&Nuisance::analysis), cond);
  return *this;
}

CombineHarvester& CombineHarvester::era(
    std::vector<std::string> const& vec, bool cond) {
  FilterContaining(procs_, vec, std::mem_fn(&Process::era), cond);
  FilterContaining(obs_, vec, std::mem_fn(&Observation::era), cond);
  FilterContaining(nus_, vec, std::mem_fn(&Nuisance::era), cond);
  return *this;
}

CombineHarvester& CombineHarvester::channel(
    std::vector<std::string> const& vec, bool cond) {
  FilterContaining(procs_, vec, std::mem_fn(&Process::channel), cond);
  FilterContaining(obs_, vec, std::mem_fn(&Observation::channel), cond);
  FilterContaining(nus_, vec, std::mem_fn(&Nuisance::channel), cond);
  return *this;
}

CombineHarvester& CombineHarvester::mass(
    std::vector<std::string> const& vec, bool cond) {
  FilterContaining(procs_, vec, std::mem_fn(&Process::mass), cond);
  FilterContaining(obs_, vec, std::mem_fn(&Observation::mass), cond);
  FilterContaining(nus_, vec, std::mem_fn(&Nuisance::mass), cond);
  return *this;
}

CombineHarvester& CombineHarvester::nus_name(
    std::vector<std::string> const& vec, bool cond) {
  FilterContaining(nus_, vec, std::mem_fn(&Nuisance::name), cond);
  return *this;
}

CombineHarvester& CombineHarvester::nus_type(
    std::vector<std::string> const& vec, bool cond) {
  FilterContaining(nus_, vec, std::mem_fn(&Nuisance::type), cond);
  return *this;
}

CombineHarvester & CombineHarvester::signals() {
  ic::erase_if(nus_, [&] (std::shared_ptr<Nuisance> val) {
    return !val->signal();
  });
  ic::erase_if(procs_, [&] (std::shared_ptr<Process> val) {
    return !val->signal();
  });
  return *this;
}

CombineHarvester & CombineHarvester::backgrounds() {
  ic::erase_if(nus_, [&] (std::shared_ptr<Nuisance> val) {
    return val->signal();
  });
  ic::erase_if(procs_, [&] (std::shared_ptr<Process> val) {
    return val->signal();
  });
  return *this;
}

CombineHarvester & CombineHarvester::histograms() {
  ic::erase_if(obs_, [&] (std::shared_ptr<Observation> val) {
    return val->shape() == nullptr;
  });
  ic::erase_if(procs_, [&] (std::shared_ptr<Process> val) {
    return val->shape() == nullptr;
  });
  return *this;
}

CombineHarvester & CombineHarvester::pdfs() {
  ic::erase_if(procs_, [&] (std::shared_ptr<Process> val) {
    return val->pdf() == nullptr;
  });
  return *this;
}

CombineHarvester & CombineHarvester::data() {
  ic::erase_if(obs_, [&] (std::shared_ptr<Observation> val) {
    return val->data() == nullptr;
  });
  return *this;
}
}
