#include "CombineTools/interface/CombineHarvester.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <utility>
#include <set>
#include <fstream>
// #include "boost/lexical_cast.hpp"
// #include "boost/algorithm/string.hpp"
// #include "boost/range/algorithm_ext/erase.hpp"
// #include "boost/range/algorithm/find.hpp"
#include "boost/format.hpp"
#include "TDirectory.h"
#include "TH1.h"
#include "Utilities/interface/FnRootTools.h"
// #include "Utilities/interface/FnPredicates.h"
#include "CombineTools/interface/Observation.h"
#include "CombineTools/interface/Process.h"
#include "CombineTools/interface/Nuisance.h"
#include "CombineTools/interface/Parameter.h"
// #include "CombineTools/interface/MakeUnique.h"
// #include "CombineTools/interface/HelperFunctions.h"


namespace ch {
void CombineHarvester::AddObservations(
    std::vector<std::string> mass, std::vector<std::string> analysis,
    std::vector<std::string> era, std::vector<std::string> channel,
    std::vector<std::pair<int, std::string>> bin) {
  std::vector<unsigned> lengths = {
      unsigned(mass.size()),    unsigned(analysis.size()), unsigned(era.size()),
      unsigned(channel.size()), unsigned(bin.size())};
  auto comb = ic::GenerateCombinations(lengths);
  for (auto const& c : comb) {
    auto obs = std::make_shared<Observation>();
    obs->set_mass(mass[c[0]]);
    obs->set_analysis(analysis[c[1]]);
    obs->set_era(era[c[2]]);
    obs->set_channel(channel[c[3]]);
    obs->set_bin_id(bin[c[4]].first);
    obs->set_bin(bin[c[4]].second);
    obs_.push_back(obs);
  }
}

void CombineHarvester::AddProcesses(
    std::vector<std::string> mass, std::vector<std::string> analysis,
    std::vector<std::string> era, std::vector<std::string> channel,
    std::vector<std::string> procs,
    std::vector<std::pair<int, std::string>> bin, bool signal) {
  std::vector<unsigned> lengths = {
      unsigned(mass.size()),    unsigned(analysis.size()), unsigned(era.size()),
      unsigned(channel.size()), unsigned(bin.size())};
  auto comb = ic::GenerateCombinations(lengths);
  for (auto const& c : comb) {
    std::set<int> existing_ids = this->shallow_copy()
        .mass(true, {mass[c[0]]})
        .analysis(true, {analysis[c[1]]})
        .era(true, {era[c[2]]})
        .channel(true, {channel[c[3]]})
        .bin_id(true, {bin[c[4]].first})
        .bin(true, {bin[c[4]].second})
        .GenerateSetFromProcs<int>(std::mem_fn(&Process::process_id));
    std::vector<int> ids;
    int n = signal ? 0 : 1;
    while (ids.size() != procs.size()) {
      if (!existing_ids.count(n)) ids.push_back(n);
      if (signal) {
        --n;
      } else {
        ++n;
      }
    }
    for (unsigned i = 0; i < procs.size(); ++i) {
      auto proc = std::make_shared<Process>();
      proc->set_mass(mass[c[0]]);
      proc->set_analysis(analysis[c[1]]);
      proc->set_era(era[c[2]]);
      proc->set_channel(channel[c[3]]);
      proc->set_bin_id(bin[c[4]].first);
      proc->set_bin(bin[c[4]].second);
      proc->set_process(procs[i]);
      proc->set_process_id(ids[i]);
      procs_.push_back(proc);
    }
  }
}

void CombineHarvester::ExtractShapes(std::string const& file,
                                     std::string const& rule) {
  TFile f(file.c_str());
  for (unsigned  i = 0; i < obs_.size(); ++i) {
    if (!obs_[i]->shape()) {
      std::string p = rule;
      boost::replace_all(p, "$CHANNEL", obs_[i]->bin());
      boost::replace_all(p, "$PROCESS", "data_obs");
      boost::replace_all(p, "$MASS", obs_[i]->mass());
      TH1 *h = dynamic_cast<TH1*>(gDirectory->Get(p.c_str()));
      if (h) {
        h->SetDirectory(0);
        obs_[i]->set_rate(h->Integral());
        if (h->Integral() > 0.0) h->Scale(1.0/h->Integral());
        obs_[i]->set_shape(std::unique_ptr<TH1>(h));
      }
    }
  }
  for (unsigned  i = 0; i < procs_.size(); ++i) {
    if (!procs_[i]->shape()) {
      std::string p = rule;
      boost::replace_all(p, "$CHANNEL", procs_[i]->bin());
      boost::replace_all(p, "$PROCESS", procs_[i]->process());
      boost::replace_all(p, "$MASS", procs_[i]->mass());
      TH1 *h = dynamic_cast<TH1*>(gDirectory->Get(p.c_str()));
      if (h) {
        h->SetDirectory(0);
        procs_[i]->set_rate(h->Integral());
        if (h->Integral() > 0.0) h->Scale(1.0/h->Integral());
        procs_[i]->set_shape(std::unique_ptr<TH1>(h));
      }
    }
  }
}
}
