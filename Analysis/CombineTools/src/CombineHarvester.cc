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

CombineHarvester::CombineHarvester() { }

CombineHarvester::~CombineHarvester() { }

void swap(CombineHarvester& first, CombineHarvester& second) {
  using std::swap;
  swap(first.obs_, second.obs_);
  swap(first.procs_, second.procs_);
  swap(first.nus_, second.nus_);
  swap(first.params_, second.params_);
}

CombineHarvester::CombineHarvester(CombineHarvester const& other)
    : obs_(other.obs_.size(), nullptr),
      procs_(other.procs_.size(), nullptr),
      nus_(other.nus_.size(), nullptr) {
  for (std::size_t i = 0; i < obs_.size(); ++i) {
    if (other.obs_[i]) {
      obs_[i] = std::make_shared<Observation>(*(other.obs_[i]));
    }
  }
  for (std::size_t i = 0; i < procs_.size(); ++i) {
    if (other.procs_[i]) {
      procs_[i] = std::make_shared<Process>(*(other.procs_[i]));
    }
  }
  for (std::size_t i = 0; i < nus_.size(); ++i) {
    if (other.nus_[i]) {
      nus_[i] = std::make_shared<Nuisance>(*(other.nus_[i]));
    }
  }
  for (auto const& it : other.params_) {
    if (it.second) {
      params_.insert({it.first, std::make_shared<Parameter>(*(it.second))});
    } else {
      params_.insert({it.first, nullptr});
    }
  }
}

CombineHarvester::CombineHarvester(CombineHarvester&& other) {
  swap(*this, other);
}

CombineHarvester& CombineHarvester::operator=(CombineHarvester other) {
  swap(*this, other);
  return (*this);
}

CombineHarvester CombineHarvester::shallow_copy() {
  CombineHarvester cpy;
  cpy.obs_    = obs_;
  cpy.procs_  = procs_;
  cpy.nus_    = nus_;
  cpy.params_ = params_;
  return cpy;
}


CombineHarvester::StrPairVec CombineHarvester::GenerateShapeMapAttempts(
    std::string process, std::string category) {
  CombineHarvester::StrPairVec result;
  result.push_back(std::make_pair(process   , category));
  result.push_back(std::make_pair("*"       , category));
  result.push_back(std::make_pair(process   , "*"));
  result.push_back(std::make_pair("*"       , "*"));
  return result;
}

TH1 * CombineHarvester::GetHistFromFile(
    std::vector<HistMapping> const& mappings,
    std::string const& bin,
    std::string const& process,
    std::string const& mass,
    std::string const& nuisance,
    unsigned type) {
  StrPairVec attempts = this->GenerateShapeMapAttempts(process, bin);
  for (unsigned a = 0; a < attempts.size(); ++a) {
    for (unsigned m = 0; m < mappings.size(); ++m) {
      if ((attempts[a].first == mappings[m].process) &&
        (attempts[a].second == mappings[m].category)) {
        std::string p = (type == 0 ?
            mappings[m].pattern : mappings[m].syst_pattern);
        boost::replace_all(p, "$CHANNEL", bin);
        boost::replace_all(p, "$PROCESS", process);
        boost::replace_all(p, "$MASS", mass);
        if (type == 1) boost::replace_all(p, "$SYSTEMATIC", nuisance+"Down");
        if (type == 2) boost::replace_all(p, "$SYSTEMATIC", nuisance+"Up");
        mappings[m].file->cd();
        TH1 *h = dynamic_cast<TH1*>(gDirectory->Get(p.c_str()));
        h->SetDirectory(0);
        return h;
      }
    }
  }
  return nullptr;
}

void CombineHarvester::WriteHistToFile(
    TH1 const* hist,
    TFile * file,
    std::vector<HistMapping> const& mappings,
    std::string const& bin,
    std::string const& process,
    std::string const& mass,
    std::string const& nuisance,
    unsigned type) {
  StrPairVec attempts = this->GenerateShapeMapAttempts(process, bin);
  for (unsigned a = 0; a < attempts.size(); ++a) {
    for (unsigned m = 0; m < mappings.size(); ++m) {
      if ((attempts[a].first == mappings[m].process) &&
        (attempts[a].second == mappings[m].category)) {
        std::string p = (type == 0 ?
            mappings[m].pattern : mappings[m].syst_pattern);
        boost::replace_all(p, "$CHANNEL", bin);
        boost::replace_all(p, "$PROCESS", process);
        boost::replace_all(p, "$MASS", mass);
        if (type == 1) boost::replace_all(p, "$SYSTEMATIC", nuisance+"Down");
        if (type == 2) boost::replace_all(p, "$SYSTEMATIC", nuisance+"Up");
        WriteToTFile(hist, file, p);
        return;
      }
    }
  }
}

int CombineHarvester::ParseDatacard(std::string const& filename,
    std::string const& analysis,
    std::string const& era,
    std::string const& channel,
    int bin_id,
    std::string const& mass) {
  // Load the entire datacard into memory as a vector of strings
  std::vector<std::string> lines = ic::ParseFileLines(filename);
  // Loop through lines, trimming whitespace at the beginning or end
  // then splitting each line into a vector of words (using any amount
  // of whitespace as the separator).  We skip any line of zero length
  // or which starts with a "#" or "-" character.
  std::vector<std::vector<std::string>> words;
  for (unsigned i = 0; i < lines.size(); ++i) {
    boost::trim(lines[i]);
    if (lines[i].size() == 0) continue;
    if (lines[i].at(0) == '#' || lines[i].at(0) == '-') continue;
    words.push_back(std::vector<std::string>());
    boost::split(words.back(), lines[i], boost::is_any_of("\t "),
        boost::token_compress_on);
  }

  std::vector<HistMapping> hist_mapping;
  bool start_nuisance_scan = false;
  unsigned r = 0;

  // Loop through the vector of word vectors
  for (unsigned i = 0; i < words.size(); ++i) {
    // Ignore line if it only has one word
    if (words[i].size() <= 1) continue;

    // If the line begins "shapes" then we've
    // found process --> TH1F mapping information
    if (words[i][0] == "shapes" && words[i].size() >= 5) {
      hist_mapping.push_back(HistMapping());
      hist_mapping.back().process = words[i][1];
      hist_mapping.back().category = words[i][2];
      // The root file path given in the datacard is relative to the datacard
      // path, so we join the path to the datacard with the path to the file
      std::string dc_path;
      std::size_t slash = filename.find_last_of('/');
      if (slash != filename.npos) {
        dc_path = filename.substr(0, slash) + "/" + words[i][3];
      } else {
        dc_path = words[i][3];
      }
      hist_mapping.back().file = ic::make_unique<TFile>(dc_path.c_str());
      hist_mapping.back().pattern = words[i][4];
      if (words[i].size() > 5) hist_mapping.back().syst_pattern = words[i][5];
    }

    // Want to check this line and the previous one, so need i >= 1.
    // If the first word on this line is "observation" and "bin" on
    // the previous line then we've found the entries for data, and
    // can add Observation objects
    if (i >= 1) {
      if (  words[i][0]     == "observation" &&
            words[i-1][0]   == "bin" &&
            words[i].size() == words[i-1].size()) {
        for (unsigned p = 1; p < words[i].size(); ++p) {
          auto obs = std::make_shared<Observation>();
          obs->set_bin(words[i-1][p]);
          obs->set_rate(boost::lexical_cast<double>(words[i][p]));
          obs->set_analysis(analysis);
          obs->set_era(era);
          obs->set_channel(channel);
          obs->set_bin_id(bin_id);
          obs->set_mass(mass);
          TH1 *h = GetHistFromFile(hist_mapping,
            obs->bin(), "data_obs", obs->mass(), "", 0);
          if (h->Integral() > 0.0) h->Scale(1.0/h->Integral());
          if (h) obs->set_shape(std::unique_ptr<TH1>(h));
          obs_.push_back(obs);
        }
      }
    }

    // Similarly look for the lines indicating the different signal
    // and background processes
    // Once these are found save in line index for the rate line as r
    // to we can refer back to these later, then assume that every
    // line that follows is a nuisance parameter
    if (i >= 3) {
      if (  words[i][0]   == "rate" &&
            words[i-1][0] == "process" &&
            words[i-2][0] == "process" &&
            words[i-3][0] == "bin" &&
            words[i].size() == words[i-1].size() &&
            words[i].size() == words[i-2].size() &&
            words[i].size() == words[i-3].size()) {
        for (unsigned p = 1; p < words[i].size(); ++p) {
          auto proc = std::make_shared<Process>();
          proc->set_bin(words[i-3][p]);
          proc->set_rate(boost::lexical_cast<double>(words[i][p]));
          proc->set_process(words[i-1][p]);
          proc->set_process_id(boost::lexical_cast<int>(words[i-2][p]));
          proc->set_analysis(analysis);
          proc->set_era(era);
          proc->set_channel(channel);
          proc->set_bin_id(bin_id);
          proc->set_mass(mass);
          TH1 *h = GetHistFromFile(hist_mapping,
            proc->bin(), proc->process(), proc->mass(), "", 0);
          if (h->Integral() > 0.0) h->Scale(1.0/h->Integral());
          if (h) proc->set_shape(std::unique_ptr<TH1>(h));
          procs_.push_back(proc);
        }
        r = i;
        start_nuisance_scan = true;
      }
    }

    if (start_nuisance_scan && words[i].size()-1 == words[r].size()) {
      for (unsigned p = 2; p < words[i].size(); ++p) {
        if (words[i][p] == "-") continue;
        // if (words[i][0].at(0) == '#') continue;
        auto nus = std::make_shared<Nuisance>();
        nus->set_bin(words[r-3][p-1]);
        nus->set_process(words[r-1][p-1]);
        nus->set_process_id(boost::lexical_cast<int>(words[r-2][p-1]));
        nus->set_name(words[i][0]);
        nus->set_type(words[i][1]);
        nus->set_analysis(analysis);
        nus->set_era(era);
        nus->set_channel(channel);
        nus->set_bin_id(bin_id);
        nus->set_mass(mass);
        std::size_t slash_pos = words[i][p].find("/");
        if (slash_pos != words[i][p].npos) {
          // Assume asymmetric of form kDown/kUp
          nus->set_value_d(
            boost::lexical_cast<double>(words[i][p].substr(0, slash_pos)));
          nus->set_value_u(
            boost::lexical_cast<double>(words[i][p].substr(slash_pos+1)));
          nus->set_asymm(true);
        } else {
          nus->set_value_u(boost::lexical_cast<double>(words[i][p]));
          nus->set_asymm(false);
        }
        if (nus->type() == "shape") {
          TH1 *h = GetHistFromFile(hist_mapping,
            nus->bin(), nus->process(), nus->mass(), "", 0);
          TH1 *h_d = GetHistFromFile(hist_mapping,
            nus->bin(), nus->process(), nus->mass(), nus->name(), 1);
          TH1 *h_u = GetHistFromFile(hist_mapping,
            nus->bin(), nus->process(), nus->mass(), nus->name(), 2);
          // Is the rate of the central process > 0?
          if (h->Integral() > 0.0) {
            // If the up and down templates also > 0 can proceed normally
            if (h_u->Integral() > 0.0) {
              nus->set_value_u(h_u->Integral()/h->Integral());
              h_u->Scale(1.0/h_u->Integral());
            } else { // otherwise we force the yield part to 1.0, i.e. no effect
              nus->set_value_u(1.0);
            }
            if (h_d->Integral() > 0.0) {
              nus->set_value_d(h_d->Integral()/h->Integral());            
              h_d->Scale(1.0/h_d->Integral());
            } else { // otherwise we force the yield part to 1.0, i.e. no effect
              nus->set_value_d(1.0);
            }
          } else {
            nus->set_value_u(1.0);
            if (h_u->Integral() > 0.0) {
              h_u->Scale(1.0/h_u->Integral());
            }
            nus->set_value_d(1.0);
            if (h_d->Integral() > 0.0) {
              h_d->Scale(1.0/h_d->Integral());
            }
          }
          if (h_u) nus->set_shape_u(std::unique_ptr<TH1>(h_u));
          if (h_d) nus->set_shape_d(std::unique_ptr<TH1>(h_d));
          nus->set_asymm(true);
          delete h;
        }
        if (!params_.count(nus->name())) {
          auto param = std::make_shared<Parameter>(Parameter());
          param->set_name(nus->name());
          params_.insert({nus->name(), param});
        }
        nus_.push_back(nus);
      }
    }
  }
  return 0;
}

void CombineHarvester::WriteDatacard(std::string const& name, std::string const& root_file) {
  std::ofstream txt_file;
  txt_file.open(name);

  std::string dashes(80,'-');

  auto bin_set = this->GenerateSetFromObs<std::string>(std::mem_fn(&ch::Observation::bin));
  auto proc_set = this->GenerateSetFromProcs<std::string>(std::mem_fn(&ch::Process::process));
  auto nus_set = this->GenerateSetFromNus<std::string>(std::mem_fn(&ch::Nuisance::name));
  txt_file << "imax    " << bin_set.size() << " number of bins\n";
  txt_file << "jmax    " << proc_set.size()-1 << " number of processes minus 1\n";
  txt_file << "kmax    " << nus_set.size() << " number of nuisance parameters\n";
  txt_file << dashes << "\n";

  // Generate a sensible default for the mapping of process and
  // systematic templates into a root file
  std::vector<HistMapping> mappings;
  mappings.push_back({
    "*", "*", nullptr, "$CHANNEL/$PROCESS", "$CHANNEL/$PROCESS_$SYSTEMATIC"
  });
  CombineHarvester ch_signals = this->shallow_copy().signals();
  auto sig_proc_set = ch_signals.GenerateSetFromProcs<std::string>(
    std::mem_fn(&ch::Process::process));
  for (auto sig_proc : sig_proc_set) {
    mappings.push_back({
      sig_proc, "*", nullptr, "$CHANNEL/$PROCESS$MASS", "$CHANNEL/$PROCESS$MASS_$SYSTEMATIC"
    });
  }

  for (auto const& mapping : mappings) {
    txt_file << boost::format("shapes %s %s %s %s %s\n")
      % mapping.process
      % mapping.category
      % root_file
      % mapping.pattern
      % mapping.syst_pattern;
  }
  txt_file << dashes << "\n";

  TFile file(root_file.c_str(), "RECREATE");

  // Writing observations
  txt_file << "bin          ";
  for (auto const& obs : obs_) {
    txt_file << boost::format("%-15s ") % obs->bin();
    std::unique_ptr<TH1> h((TH1*)(obs->shape()->Clone()));
    h->Scale(obs->rate());
    WriteHistToFile(h.get(), &file, mappings, obs->bin(), "data_obs", obs->mass(), "", 0);
  }
  txt_file << "\n";
  txt_file << "observation  ";
  for (auto const& obs : obs_) {
    txt_file << boost::format("%-15.1f ") % obs->rate();
  }
  txt_file << "\n";
  txt_file << dashes << "\n";

  auto proc_nus_map = this->GenerateProcNusMap();
  unsigned nus_str_len = 14;
  for (auto const& nus : nus_set) {
    if (nus.length() > nus_str_len) nus_str_len = nus.length();
  }

  txt_file << boost::format("%-"+boost::lexical_cast<std::string>(nus_str_len+9)+"s") % "bin";
  for (auto const& proc : procs_) {
    std::unique_ptr<TH1> h((TH1*)(proc->shape()->Clone()));
    h->Scale(proc->rate());
    WriteHistToFile(h.get(), &file, mappings, proc->bin(), proc->process(), proc->mass(), "", 0);
    txt_file << boost::format("%-15s ") % proc->bin();
  }
  txt_file << "\n";

  txt_file << boost::format("%-"+boost::lexical_cast<std::string>(nus_str_len+9)+"s") % "process";

  for (auto const& proc : procs_) {
    txt_file << boost::format("%-15s ") % proc->process();
  }
  txt_file << "\n";

  txt_file << boost::format("%-"+boost::lexical_cast<std::string>(nus_str_len+9)+"s") % "process";

  for (auto const& proc : procs_) {
    txt_file << boost::format("%-15s ") % proc->process_id();
  }
  txt_file << "\n";


  txt_file << boost::format("%-"+boost::lexical_cast<std::string>(nus_str_len+9)+"s") % "rate";
  for (auto const& proc : procs_) {
    txt_file << boost::format("%-15.4f ") % proc->rate();
  }
  txt_file << "\n";
  txt_file << dashes << "\n";


  for (auto const& nus : nus_set) {
    std::vector<std::string> line(procs_.size() + 2);
    line[0] = nus;
    bool seen_lnN = false;
    bool seen_shape = false;
    for (unsigned p = 0; p < procs_.size(); ++p) {
      line[p+2] = "-";
      for (unsigned n = 0; n < proc_nus_map[p].size(); ++n) {
        ch::Nuisance const* nus_ptr = proc_nus_map[p][n];
        if (nus_ptr->name() == nus) {
          if (nus_ptr->type() == "lnN") {
            seen_lnN = true;
            line[p+2] = nus_ptr->asymm() ?
              (boost::format("%g/%g") % nus_ptr->value_d() % nus_ptr->value_u()).str() :
              (boost::format("%g") % nus_ptr->value_u()).str();
            break;
          }
          if (nus_ptr->type() == "shape") {
            std::unique_ptr<TH1> h_d((TH1*)(nus_ptr->shape_d()->Clone()));
            h_d->Scale(procs_[p]->rate()*nus_ptr->value_d());
            WriteHistToFile(h_d.get(), &file, mappings, nus_ptr->bin(), nus_ptr->process(), nus_ptr->mass(), nus_ptr->name(), 1);
            std::unique_ptr<TH1> h_u((TH1*)(nus_ptr->shape_u()->Clone()));
            h_u->Scale(procs_[p]->rate()*nus_ptr->value_u());
            WriteHistToFile(h_u.get(), &file, mappings, nus_ptr->bin(), nus_ptr->process(), nus_ptr->mass(), nus_ptr->name(), 2);
            seen_shape = true;
            line[p+2] = "1.0";
            break;
          }
        }
      }
    }
    if (seen_lnN && !seen_shape) line[1] = "lnN";
    if (!seen_lnN && seen_shape) line[1] = "shape";
    if (seen_lnN && seen_shape) line[1] = "shape?";
    txt_file << boost::format(
      "%-"+boost::lexical_cast<std::string>(nus_str_len)+"s %-7s ")
      % line[0] % line[1];
    for (unsigned p = 0; p < procs_.size(); ++p) {
      txt_file << boost::format("%-15s ") % line[p+2];
    }
    txt_file << "\n";
  }




  txt_file.close();
  file.Close();
}

CombineHarvester & CombineHarvester::PrintAll() {
  std::cout << Observation::PrintHeader;
  for (unsigned i = 0; i < obs_.size(); ++i) 
      std::cout << *(obs_[i]) << std::endl;
  std::cout << Process::PrintHeader;
  for (unsigned i = 0; i < procs_.size(); ++i) 
      std::cout << *(procs_[i]) << std::endl;
  std::cout << Nuisance::PrintHeader;
  for (unsigned i = 0; i < nus_.size(); ++i) 
      std::cout << *(nus_[i]) << std::endl;
  std::cout << Parameter::PrintHeader;
  for (auto const& it : params_) std::cout << *(it.second) << std::endl;
  return *this;
}

CombineHarvester::ProcNusMap CombineHarvester::GenerateProcNusMap() {
  ProcNusMap lookup(procs_.size());
  for (unsigned i = 0; i < nus_.size(); ++i) {
    for (unsigned j = 0; j < procs_.size(); ++j) {
      if (MatchingProcess(*(nus_[i]), *(procs_[j]))) {
        lookup[j].push_back(nus_[i].get());
      }
    }
  }
  return lookup;
}

double CombineHarvester::GetUncertainty() {
  auto lookup = GenerateProcNusMap();
  double err_sq = 0.0;
  for (auto param_it : params_) {
    double backup = param_it.second->val();
    param_it.second->set_val(param_it.second->err_d());
    double rate_d = this->GetRateInternal(lookup, param_it.first);
    param_it.second->set_val(param_it.second->err_u());
    double rate_u = this->GetRateInternal(lookup, param_it.first);
    double err = std::fabs(rate_u-rate_d) / 2.0;
    err_sq += err * err;
    param_it.second->set_val(backup);
  }
  return std::sqrt(err_sq);
}

double CombineHarvester::GetRate() {
  auto lookup = GenerateProcNusMap();
  return GetRateInternal(lookup);
}


double CombineHarvester::GetRateInternal(ProcNusMap const& lookup,
    std::string const& single_nus) {
  // possible optimisation by restricting summed p_rates to
  // just those where a given nuisance appies
  // but would have to walk down the nus_it first to see if
  // the nuisance in question appears
  // Can we do the same for shapes? should be fine
  double rate = 0.0;
  // TH1F tot_shape
  for (unsigned i = 0; i < procs_.size(); ++i) {
    double p_rate = procs_[i]->rate();
    // TH1F proc_shape = ....
    // If we are evaluating the effect of a single parameter
    // check the list of associated nuisances and skip if
    // this "single_nus" is not in the list
    if (single_nus != "") {
      if (!ch::any_of(lookup[i], [&](Nuisance const* nus) {
        return nus->name() == single_nus;
      })) continue;
    }
    for (auto nus_it : lookup[i]) {
      double x = params_[nus_it->name()]->val();
      if (nus_it->asymm()) {
        if (x >= 0) {
          p_rate *= std::pow(nus_it->value_u(), x);
        } else {
          p_rate *= std::pow(nus_it->value_d(), -1.0*x);
        }
      } else {
        p_rate *= std::pow(nus_it->value_u(), x);
      }
      /* if (shape_nuisance) 
      *  TH1F diff = Morph(x, nom, down, up)
      *  proc_shape.Add(diff)
      */
    }
    // proc_shape.Scale(p_rate), tot_shape.Add(proc_shape)
    rate += p_rate;
  }
  return rate;
}

TH1F CombineHarvester::GetShape() {
  auto lookup = GenerateProcNusMap();
  TH1F shape = *((TH1F*)(procs_[0]->shape()));
  shape.Reset();
  for (unsigned i = 0; i < procs_.size(); ++i) {
    double p_rate = procs_[i]->rate();
    TH1F proc_shape = *((TH1F*)(procs_[i]->shape()));
    // If we are evaluating the effect of a single parameter
    // check the list of associated nuisances and skip if
    // this "single_nus" is not in the list
    // if (single_nus != "") {
    //   if (!ch::any_of(lookup[i], [&](Nuisance const* nus) {
    //     return nus->name() == single_nus;
    //   })) continue;
    // }
    for (auto nus_it : lookup[i]) {
      double x = params_[nus_it->name()]->val();
      if (nus_it->asymm()) {
        if (x >= 0) {
          p_rate *= std::pow(nus_it->value_u(), x);
        } else {
          p_rate *= std::pow(nus_it->value_d(), -1.0*x);
        }
        if (nus_it->type() == "shape") {
          TH1F diff = ShapeDiff(x, procs_[i]->shape(), 
            nus_it->shape_d(), nus_it->shape_u());
          proc_shape.Add(&diff);
        }
      } else {
        p_rate *= std::pow(nus_it->value_u(), x);
      }
    }
    proc_shape.Scale(p_rate);
    shape.Add(&proc_shape);
  }
  return shape;
}

TH1F CombineHarvester::GetObservedShape() {
  TH1F shape = *((TH1F*)(obs_[0]->shape()));
  shape.Reset();
  for (unsigned i = 0; i < obs_.size(); ++i) {
    TH1F proc_shape = *((TH1F*)(obs_[i]->shape()));
    double p_rate = obs_[i]->rate();
    proc_shape.Scale(p_rate);
    shape.Add(&proc_shape);
  }
  return shape;
}

TH1F CombineHarvester::ShapeDiff(double x, 
    TH1 const* nom, 
    TH1 const* low, 
    TH1 const* high) {
  TH1F diff = *(TH1F*)(high);
  diff.Add(low, -1.0);
  TH1F sum = *(TH1F*)(high);
  sum.Add(low, 1.0);
  sum.Add(nom, -2.0);
  sum.Scale(smoothStepFunc(x));
  sum.Add(&diff);
  sum.Scale(0.5*x);
  return sum;
}

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

// void CombineHarvester::Validate() {
//   std::set<std::string> bins;
//   for (auto const p : procs_) {
//     bins.insert(p->bin());
//   }
//   for (auto b : bins) {
//     std::cout << "Checking bin: " << b << std::endl;
//     TH1F bkg =  this->shallow_copy().bin(true, {b}).backgrounds().GetShape();
//     TH1F sig =  this->shallow_copy().bin(true, {b}).signals().GetShape();
//     TH1F dat = this->shallow_copy().bin(true, {b}).GetObservedShape();
//     for (unsigned i = 1; i <= dat.GetNbinsX(); ++i) {
//       double y_dat = dat.GetBinContent(i);
//       double y_sig = sig.GetBinContent(i);
//       double y_bkg = bkg.GetBinContent(i);
//       if (y_sig > y_bkg) std::cout << "Histogram bin " << i << " has sig " << y_sig <<
//         ", bkg " << y_bkg << " and data " << y_dat << std::endl;
//     }
//   }
// }
//  /*

//  TGraphAsymmErrors BuildPoissonErrors(TH1F const& hist) {
//    TGraphAsymmErrors result(&hist);
//    double alpha = 1 - 0.6827;
//    for (int k = 0; k <result.GetN(); ++k) {
//      double x;
//      double y;
//      result.GetPoint(k, x, y);
//      // double err_y_up = 0.5 + std::sqrt(y+0.25);
//      // double err_y_dn = -0.5 + std::sqrt(y+0.25);
//      double L =  (y==0) ? 0  : (ROOT::Math::gamma_quantile(alpha/2,y,1.));
//      double U =  ROOT::Math::gamma_quantile_c(alpha/2,y+1,1) ;
//      result.SetPointEYhigh(k, U - y);
//      result.SetPointEYlow(k, y - L);
//    }
//    return result;
//  }


//  void Pull::Print() const {
//    std::cout << boost::format("%-60s %-4.2f +/- %-4.2f   %+-4.2f +/- %-4.2f   %+-4.2f +/- %-4.2f   %+-4.2f \n")
//      % name % prefit %prefit_err % bonly % bonly_err % splusb % splusb_err % rho;
//  }

//  bool BvsSBComparator(Pull const& pull1, Pull const& pull2) {
//    return ( fabs(pull1.bonly - pull1.splusb) > fabs(pull2.bonly - pull2.splusb) );
//  }

//  void PullsFromFile(std::string const& filename, std::vector<ic::Pull> & pullvec, bool verbose) {
//    std::vector<std::string> lines = ParseFileLines(filename);
//    for (unsigned i = 0; i < lines.size(); ++i) {
//      boost::erase_all(lines[i],"*");
//      boost::erase_all(lines[i],"!");
//      std::vector<std::string> blocks;
//      boost::split(blocks, lines[i], boost::is_any_of(" "), boost::token_compress_on);
//      if (blocks.size() > 0) {
//        if (blocks[0] == "name") continue;
//      }
//      if (blocks.size() == 15) {
//        pullvec.push_back(ic::Pull());
//        ic::Pull & new_pull = pullvec.back();
//        new_pull.name = blocks[0];
//        new_pull.prefit = boost::lexical_cast<double>(blocks[1]);
//        new_pull.prefit_err = boost::lexical_cast<double>(blocks[3]);
//        new_pull.bonly = boost::lexical_cast<double>(blocks[4]);
//        new_pull.bonly_err = boost::lexical_cast<double>(blocks[6]);
//        new_pull.splusb = boost::lexical_cast<double>(blocks[9]);
//        new_pull.splusb_err = boost::lexical_cast<double>(blocks[11]);
//        new_pull.rho = boost::lexical_cast<double>(blocks[14]);
//        if (verbose) new_pull.Print();
//      }
//      for (unsigned j = 0; j < blocks.size(); ++j) {
//      }
//    }
//  }


//  CategoryKey Nuisance::GetKey() const { return CategoryKey(mass, era, channel, category); }
//  CategoryKey Observation::GetKey() const { return CategoryKey(mass, era, channel, category); }
//  CategoryKey Process::GetKey() const { return CategoryKey(mass, era, channel, category); }

//  CombineHarvester::CombineHarvester() {
//    ignore_nuisance_correlations_ = false;
//  }

//  CombineHarvester CombineHarvester::process(std::vector<std::string> const& process) const {
//    CombineHarvester result = *this;
//    ic::erase_if(result.nuisances_, [&] (Nuisance const& val) { return std::find(process.begin(), process.end(), val.process) == process.end(); });
//    ic::erase_if(result.processes_, [&] (Process const& proc) { return std::find(process.begin(), process.end(), proc.process) == process.end(); });
//    return result;
//  }

//  CombineHarvester CombineHarvester::era(std::vector<std::string> const& process) const {
//    CombineHarvester result = *this;
//    ic::erase_if(result.nuisances_, [&] (Nuisance const& val) { return std::find(process.begin(), process.end(), val.era) == process.end(); });
//    ic::erase_if(result.processes_, [&] (Process const& proc) { return std::find(process.begin(), process.end(), proc.era) == process.end(); });
//    ic::erase_if(result.obs_, [&] (Observation const& proc) { return std::find(process.begin(), process.end(), proc.era) == process.end(); });
//    return result;
//  }

//  CombineHarvester CombineHarvester::category_id(std::vector<int> const& id) const {
//    CombineHarvester result = *this;
//    ic::erase_if(result.nuisances_, [&] (Nuisance const& val) { return std::find(id.begin(), id.end(), val.category_id) == id.end(); });
//    ic::erase_if(result.processes_, [&] (Process const& proc) { return std::find(id.begin(), id.end(), proc.category_id) == id.end(); });
//    ic::erase_if(result.obs_, [&] (Observation const& proc) { return std::find(id.begin(), id.end(), proc.category_id) == id.end(); });
//    return result;
//  }


//  CombineHarvester CombineHarvester::nuisance(std::vector<std::string> const& nuisance) const {
//    CombineHarvester result = *this;
//    ic::erase_if(result.nuisances_, [&] (Nuisance const& val) { return std::find(nuisance.begin(), nuisance.end(), val.nuisance) == nuisance.end(); });
//    return result;
//  }

//  CombineHarvester CombineHarvester::nuisance_pred(std::function<bool(Nuisance const&)> fn) const {
//    CombineHarvester result = *this;
//    ic::erase_if(result.nuisances_, std::not1(fn));
//    return result;
//  }


//  CombineHarvester CombineHarvester::no_shapes() const {
//    CombineHarvester result = *this;
//    ic::erase_if(result.nuisances_, [&] (Nuisance const& val) { return val.type == "shape"; });
//    return result;
//  }


//  CombineHarvester CombineHarvester::key_match(CategoryKey const & keyval) const {
//    CombineHarvester result = *this;
//    ic::erase_if(result.nuisances_, [&] (Nuisance const& val) { return !(val.GetKey() == keyval); });
//    ic::erase_if(result.processes_, [&] (Process const& proc) { return !(proc.GetKey() == keyval); });
//    ic::erase_if(result.obs_, [&] (Observation const& proc) { return !(proc.GetKey() == keyval); });
//    return result;
//  }

//  /*
//  New logic here should be:
//    loop through processes:
//      find set of nuisance paramters that apply to this process
//      loop through nuisance parameters:
//        work out product of scaling factors for this background
//  */
//  double CombineHarvester::GetRate() {
//    double total = 0.0;
//    for (unsigned i = 0; i < processes_.size(); ++i) {
//      total += processes_[i].rate;
//    }
//    return total;
//  }

//    double CombineHarvester::GetObservedRate() {
//    double total = 0.0;
//    for (unsigned i = 0; i < obs_.size(); ++i) {
//      total += obs_[i].rate;
//    }
//    return total;
//  }

//  std::set<std::string> CombineHarvester::GetNuisanceSet() {
//    std::set<std::string> result;
//    for (unsigned i = 0; i < nuisances_.size(); ++i) {
//      result.insert(nuisances_[i].nuisance);
//    }
//    return result;
//  }



//  /* New procedure
//    Get set of all nuisance parameters and loop:
//      find current value and error
//      calculate dx for the effect of the error on each background:
//        dx += process->GetRate() * get symm error

//        e.g. nuisance value 1.10
//             param value 0.5 +/- 0.5

//             process rate is nominal * 1.10^0.5
//             process rate up is nominal * 1.10^1.0
//             process rate down is nominal * 1.10^0.0


//             nuisance value 1.10 up, 0.80 down
//             param value 0.5 +/- 0.8
//             process rate is nominal * 1.10^0.5
//             process rate up is nominal * 1.10^1.3
//             process rate down is 0.80^(0.3) <-- sign flip param


//              potentially slow, calculating same process rate over and over

//              the new GetShape should not automatically add errors, for toys we won't
//              need this.

//              ideally want a fast way to map from:

//                          (1 to many)
//              nuisance name --> nuisances(1)

//      (1 to many)       (1 to 1)
// nuis. name --> nuisance(s) --> parameter value
//                | (1 to 1)
//                v
//              process --> nuisance(s)
//                       (1 to many)

//                       can't use pointers if these stay in vectors,
//                       current looping is slow (a filter operation requires
//                       a complete traversal)

//                       map<nuis. name, parameter values>
//                       map<nuis. name, nuisances>
//                       map<key, process>
//                       map<key, nuisnaces>

//                       e.g. eff_e maps to W eff_e 1.05, QCD eff_e 1.10
//                           or W maps to W eff_e 1.05 W eff_m 1.10






//  */
//  double CombineHarvester::GetUncertainty() {
//    // Process::PrintHeader(std::cout);
//    std::set<std::string> nuisances = this->GetNuisanceSet();
//    std::vector<double> uncert_vec;
//    for (auto & nu : nuisances) {
//      // std::cout << "--- Nuisance: " << nu << std::endl;
//      // std::cout << "--- Correlated between: " << std::endl;
//      double dx = 0;
//      for (unsigned i = 0; i < nuisances_.size(); ++i) {
//        if (nuisances_[i].nuisance != nu) continue;
//        for (unsigned j = 0; j < processes_.size(); ++j) {
//          if (processes_[j].era       != nuisances_[i].era      ||
//              processes_[j].channel   != nuisances_[i].channel  ||
//              processes_[j].category  != nuisances_[i].category   ||
//              processes_[j].process   != nuisances_[i].process) continue;
//          // std::cout << processes_[j] << std::endl;
//          if (ignore_nuisance_correlations_) {
//            if (nuisances_[i].type == "lnN") {
//              double dxx = ( (nuisances_[i].value-1.0) * processes_[j].rate );
//              dx = sqrt(dx*dx +  dxx*dxx); 
//            }
//          } else {
//            if (nuisances_[i].type == "lnN") dx += ( (nuisances_[i].value-1.0) * processes_[j].rate ); 
//          } 
//          if (nuisances_[i].type == "shape") {
//            // Do we have the shapes?
//            if (!nuisances_[i].shape_up || !nuisances_[i].shape_down) {
//              std::cerr << "Warning in <CombineHarvester::GetUncertainty>: Shape uncertainty histograms not loaded for nuisance " << nuisances_[i].nuisance << std::endl;
//              continue;
//            }
//            if (nuisances_[i].shape->Integral() == 0.) {
//              std::cerr << "Warning in <CombineHarvester::GetUncertainty>: Shape uncertainty defined for empty histogram" << std::endl;
//              continue;
//            }
//            // The yield uncertainty due to a shape variation could be
//            // asymmetric - we take the mean variation here
//            double y = nuisances_[i].shape->Integral();
//            double y_up = nuisances_[i].shape_up->Integral()/nuisances_[i].value;
//            double y_down = nuisances_[i].shape_down->Integral()/nuisances_[i].value;
//            // std::cout << "Var+ = " << y_up-y << std::endl;
//            // std::cout << "Var- = " << y_down-y << std::endl;
//            double var = (fabs(y_up-y)+fabs(y-y_down))/2.0;
//            // std::cout << "MeanAbsVar = " << var << std::endl;
//            if ((y_up-y >= 0.))   dx += (var/y)*processes_[j].rate; 
//            if ((y_up-y < 0.))  dx -= (var/y)*processes_[j].rate; 
//          }
//        }
//      }
//      uncert_vec.push_back(dx);
//    }
//    double result = 0.0;
//    for (unsigned i = 0; i < uncert_vec.size(); ++i) {
//      result += (uncert_vec[i] * uncert_vec[i]);
//    }
//    result = sqrt(result);
//    return result;
//  }

//  TH1F  CombineHarvester::GetShape() {
//    TH1F shape = *(processes_[0].shape);
//    // Don't count shape norm. uncertainty in the total
//    double tot_uncert = this->no_shapes().GetUncertainty() / this->GetRate();
//    for (unsigned i = 1; i < processes_.size(); ++i) shape.Add(processes_[i].shape);

//    std::set<std::string> nuisances = this->GetNuisanceSet();
//    std::map<int, std::vector<double>> uncert_map;
//    for (auto& nu : nuisances) {
//      std::map<int, double> dx;
//      for (unsigned i = 0; i < nuisances_.size(); ++i) {
//        if (nuisances_[i].nuisance != nu) continue;
//        for (unsigned j = 0; j < processes_.size(); ++j) {
//          if (processes_[j].era       != nuisances_[i].era      ||
//              processes_[j].channel   != nuisances_[i].channel  ||
//              processes_[j].category  != nuisances_[i].category   ||
//              processes_[j].process   != nuisances_[i].process) continue;
//          if (nuisances_[i].type == "shape") {
//            // Do we have the shapes?
//            if (!nuisances_[i].shape_up || !nuisances_[i].shape_down) {
//              std::cerr << "Warning in <CombineHarvester::GetShape>: Shape uncertainty histograms not loaded for nuisance " << nuisances_[i].nuisance << std::endl;
//              continue;
//            }
//            if (nuisances_[i].shape->Integral() == 0.) {
//              std::cerr << "Warning in <CombineHarvester::GetShape>: Shape uncertainty defined for empty histogram" << std::endl;
//              continue;
//            }
//            for (int k = 1; k <= nuisances_[i].shape->GetNbinsX(); ++k) {
//              double y = nuisances_[i].shape->GetBinContent(k);
//              double y_up = nuisances_[i].shape_up->GetBinContent(k)/nuisances_[i].value;
//              double y_down = nuisances_[i].shape_down->GetBinContent(k)/nuisances_[i].value;
//              double var = (fabs(y_up-y)+fabs(y-y_down))/2.0;
//              if ((y_up-y >= 0.))   dx[k] += (var)*processes_[j].shape->Integral()/nuisances_[i].shape->Integral(); 
//              if ((y_up-y < 0.))  dx[k] -= (var)*processes_[j].shape->Integral()/nuisances_[i].shape->Integral(); 
//            }
//          }
//        }
//      }
//      for (auto it = dx.begin(); it != dx.end(); ++it) uncert_map[it->first].push_back(it->second);
//    }
//    std::map<int, double> result;

//    for (int i = 1; i <= shape.GetNbinsX(); ++i) {
//      uncert_map[i].push_back(shape.GetBinContent(i) * tot_uncert);
//    }

//    for (auto it = uncert_map.begin(); it != uncert_map.end(); ++it) {
//      double binresult = 0.0;
//      for (unsigned p = 0; p < it->second.size(); ++p) binresult += (it->second.at(p) * it->second.at(p));
//      result[it->first] = sqrt(binresult);
//    }
//    for (int i = 1; i <= shape.GetNbinsX(); ++i) {
//      shape.SetBinError(i, result[i]);
//    }

//    return shape;
//  }

//  TH1F  CombineHarvester::GetObservedShape() {
//    TH1F shape = *(obs_[0].shape);
//    for (unsigned i = 1; i < obs_.size(); ++i) shape.Add(obs_[i].shape);
//    return shape;
//  }

//  TGraphAsymmErrors CombineHarvester::GetObservedShapeErrors() {
//    TGraphAsymmErrors shape = *(obs_[0].errors);
//    for (int k = 0; k < shape.GetN(); ++k) {
//      double x;
//      double y;
//      shape.GetPoint(k, x, y);
//    }
//    for (unsigned i = 1; i < obs_.size(); ++i) {
//      TGraphAsymmErrors const* add = obs_[i].errors;
//      for (int k = 0; k < add->GetN(); ++k) {
//        double x1, x2;
//        double y1, y2;
//        shape.GetPoint(k, x1, y1);
//        add->GetPoint(k, x2, y2);
//        shape.SetPoint(k, x1, y1+y2);
//        shape.SetPointEYlow(k, std::sqrt(std::pow(shape.GetErrorYlow(k),2.) + std::pow(add->GetErrorYlow(k),2.)));
//        shape.SetPointEYhigh(k, std::sqrt(std::pow(shape.GetErrorYhigh(k),2.) + std::pow(add->GetErrorYhigh(k),2.)));
//      }
//    }

//    return shape;
//  }



//  int CombineHarvester::ParsePulls(std::string const& filename) {
//    PullsFromFile(filename, pulls_, false);
//    return 0;
//  }

//  void CombineHarvester::ApplyPulls(bool use_b_only) {
//    std::map<std::string, Pull> pmap;
//    for (unsigned i = 0; i < pulls_.size(); ++i) pmap[pulls_[i].name] = pulls_[i];
    
//    for (unsigned i = 0; i < nuisances_.size(); ++i) {
//      auto it = pmap.find(nuisances_[i].nuisance);
//      if (it == pmap.end()) continue;

//      if (nuisances_[i].type == "lnN") {
//        double yield_corr = (nuisances_[i].value - 1.0) * ((use_b_only) ? it->second.bonly : it->second.splusb);
//        // std::cout << nuisances_[i] << ":: Pull(" << it->second.splusb << "," << it->second.splusb_err << ")" << std::endl;
//        for (unsigned j = 0; j < processes_.size(); ++j) {
//          if (processes_[j].era       != nuisances_[i].era      ||
//              processes_[j].channel   != nuisances_[i].channel  ||
//              processes_[j].category  != nuisances_[i].category   ||
//              processes_[j].process   != nuisances_[i].process) continue;
//          processes_[j].rate += yield_corr*processes_[j].rate;
//          // Also scale the histogram if it exists
//          if (processes_[j].shape) processes_[j].shape->Scale(1. + yield_corr);
//        }
//        nuisances_[i].value = ((nuisances_[i].value - 1.0) * it->second.bonly_err) + 1.0;       
//      }

//      if (nuisances_[i].type == "shape") {
//        for (unsigned j = 0; j < processes_.size(); ++j) {
//          if (processes_[j].era       != nuisances_[i].era      ||
//              processes_[j].channel   != nuisances_[i].channel  ||
//              processes_[j].category  != nuisances_[i].category   ||
//              processes_[j].process   != nuisances_[i].process  ||
//              !nuisances_[i].shape || !nuisances_[i].shape_down || !nuisances_[i].shape_up) continue;
//          if (nuisances_[i].shape->Integral() == 0.) {
//            std::cerr << "Warning in <CombineHarvester::ApplyPulls>: Shape uncertainty defined for empty histogram" << std::endl;
//            continue;
//          }         
//          TH1F *central_new = (TH1F*)nuisances_[i].shape->Clone();
//          TH1F *up_new = (TH1F*)nuisances_[i].shape->Clone();
//          TH1F *down_new = (TH1F*)nuisances_[i].shape->Clone();

//          // for (unsigned k = 1; k <= central_new->GetNbinsX(); ++k) {
//          //  double shift = 0.;
//          //  if (it->second.splusb >= 0) {
//          //    shift = nuisances_[i].shape_up->GetBinContent(k) - nuisances_[i].shape->GetBinContent(k);
//          //  } else {
//          //    shift = nuisances_[i].shape->GetBinContent(k) - nuisances_[i].shape_down->GetBinContent(k);
//          //  }
//          //  central_new->SetBinContent(k, nuisances_[i].shape->GetBinContent(k) + it->second.splusb * shift);
//          //  up_new->SetBinContent(k, nuisances_[i].shape->GetBinContent(k) + (it->second.splusb + 1. * it->second.splusb_err) * shift);
//          //  down_new->SetBinContent(k, nuisances_[i].shape->GetBinContent(k) + (it->second.splusb - 1. * it->second.splusb_err) * shift);
//          // }

//          VerticalMorph(central_new, nuisances_[i].shape_up, nuisances_[i].shape_down, (use_b_only ? it->second.bonly : it->second.splusb)/nuisances_[i].value);
//          VerticalMorph(up_new, nuisances_[i].shape_up, nuisances_[i].shape_down,   (use_b_only ? (it->second.bonly + it->second.bonly_err) : (it->second.splusb + it->second.splusb_err))/nuisances_[i].value);
//          VerticalMorph(down_new, nuisances_[i].shape_up, nuisances_[i].shape_down, (use_b_only ? (it->second.bonly - it->second.bonly_err) : (it->second.splusb - it->second.splusb_err))/nuisances_[i].value);
          
//          // std::cout << "Applying pull for nuisance: " << nuisances_[i] << std::endl;
//          // std::cout << "To Process: " << processes_[j] << std::endl;
//          // std::cout << "New/Old rate ratio from morphing is " << central_new->Integral()/nuisances_[i].shape->Integral() << std::endl;
//          TH1F *transformation = (TH1F*)central_new->Clone();
//          transformation->Add(nuisances_[i].shape, -1); // Do shifted shape - original shape

//          // !!!! Official code doesn't seem to do this.
//          transformation->Scale(processes_[j].rate / nuisances_[i].shape->Integral()); // Scale transformation up to current rate
          
//          processes_[j].shape->Add(transformation);
//          // Scan and fix negative bins
//          for (int k = 1; k <= processes_[j].shape->GetNbinsX(); ++k) {
//            if (processes_[j].shape->GetBinContent(k) < 0.) processes_[j].shape->SetBinContent(k, 0.);
//          }
//          // Fix the normalisation of the new shape to the expected shift form the template morphing 
//          // But again, this isn't what the offical post-fit code does.  Visually, it just adjusts the bin contents
//          // and leaves it at that.  So change is in rate is absolute not fractional.
//          processes_[j].shape->Scale( processes_[j].rate * (central_new->Integral()/nuisances_[i].shape->Integral()) / processes_[j].shape->Integral() );
//          // processes_[j].shape->Scale( (processes_[j].rate + transformation->Integral()) / processes_[j].shape->Integral() );
          

//          // std::cout << "Current process rate is " << processes_[j].rate << std::endl;
//          processes_[j].rate = processes_[j].shape->Integral();
//          // std::cout << "New process rate is " << processes_[j].rate << std::endl;
//          if (transformation) delete transformation;
//          nuisances_[i].shape = central_new;
//          nuisances_[i].shape_up = up_new;
//          nuisances_[i].shape_down = down_new;
//        }
//      }

//    }
//  }

//  void CombineHarvester::WeightSoverB() {
//    // First need to identify the unique set of (mass, era, channel, category) - these will be
//    // reweighted individually. 
//    std::vector<CategoryKey> keys;
//    for (unsigned i = 0; i  < processes_.size(); ++i) {
//      CategoryKey a(processes_[i].GetKey());
//      if (std::find(keys.begin(), keys.end(), a) == keys.end()) keys.push_back(a);
//    }
//    for (unsigned i = 0; i < keys.size(); ++i) {
//      std::cout << "Calculating S/B weight for: " << keys[i].channel << " " << keys[i].era << " " << keys[i].category << std::endl;

//      TH1F sig_shape = this->key_match(keys[i]).signals().GetShape();
//      TH1F bkg_shape = this->key_match(keys[i]).backgrounds().GetShape();

//      // Find the range from the lowest edge containing 15.9% of the signal
//      double xmin = sig_shape.GetXaxis()->GetXmin();
//      double xmax = sig_shape.GetXaxis()->GetXmax();
//      unsigned steps = 350;
//      double step_size = (xmax-xmin)/double(steps);
//      double sig_tot = sig_shape.Integral();
//      double lower_limit = 0;
//      double upper_limit = 0;
//      for (unsigned j = 0; j < steps; ++j) {
//        double integral = IntegrateFloatRange(&sig_shape, xmin, xmin+(step_size*double(j)));
//        if (integral/sig_tot > 0.159) {
//          lower_limit = xmin+(step_size*double(j));
//          break;
//        }
//      }
//      for (unsigned j = 0; j < steps; ++j) {
//        double integral = IntegrateFloatRange(&sig_shape, xmax - (step_size*double(j)), xmax);
//        if (integral/sig_tot > 0.159) {
//          upper_limit = xmax - (step_size*double(j));
//          break;
//        }
//      }
//      std::cout << "Found 68\% limits at " << lower_limit << "," << upper_limit << std::endl;
//      double signal_yield = IntegrateFloatRange(&sig_shape, lower_limit, upper_limit);
//      double backgr_yield = IntegrateFloatRange(&bkg_shape, lower_limit, upper_limit);      
//      double weight = signal_yield / backgr_yield;
//      std::cout << "S/B: " << weight << std::endl;
//      for (unsigned j = 0; j < obs_.size(); ++j) {
//        if (obs_[j].GetKey() == keys[i]) {
//          obs_[j].rate *= weight;
//          if (obs_[j].shape) obs_[j].shape->Scale(weight);
//          if (obs_[j].errors) {
//            for (int k = 0; k < obs_[j].errors->GetN(); ++k) {
//              double x;
//              double y;
//              obs_[j].errors->GetPoint(k, x, y);
//              obs_[j].errors->SetPoint(k, x, y*weight);
//              double err_y_up = weight * obs_[j].errors->GetErrorYhigh(k);
//              double err_y_dn = weight * obs_[j].errors->GetErrorYlow(k);
//              obs_[j].errors->SetPointEYhigh(k, err_y_up);
//              obs_[j].errors->SetPointEYlow(k, err_y_dn);

//            }
//          }
//        }
//      }
//      for (unsigned j = 0; j < processes_.size(); ++j) {
//        if (processes_[j].GetKey() == keys[i]) {
//          processes_[j].rate *= weight;
//          if (processes_[j].shape) processes_[j].shape->Scale(weight);
//        }
//      }
//    }
//  }

//  void CombineHarvester::VariableRebin(std::vector<double> bins) {
//    for (unsigned i = 0; i < processes_.size(); ++i) {
//      if (processes_[i].shape) {
//        processes_[i].shape = (TH1F*)processes_[i].shape->Rebin(bins.size()-1,"",&(bins[0]));
//      }
//    }
//    for (unsigned i = 0; i < obs_.size(); ++i) {
//      if (obs_[i].shape) {
//        obs_[i].shape = (TH1F*)obs_[i].shape->Rebin(bins.size()-1,"",&(bins[0]));
//        // If we rebin then recreate the errors from scratch
//        if (obs_[i].errors) delete obs_[i].errors;
//        obs_[i].errors = new TGraphAsymmErrors(BuildPoissonErrors(*(obs_[i].shape)));
//      }
//    }
//    for (unsigned i = 0; i < nuisances_.size(); ++i) {
//      if (nuisances_[i].shape) nuisances_[i].shape = (TH1F*)nuisances_[i].shape->Rebin(bins.size()-1,"",&(bins[0]));
//      if (nuisances_[i].shape_down) nuisances_[i].shape_down = (TH1F*)nuisances_[i].shape_down->Rebin(bins.size()-1,"",&(bins[0]));
//      if (nuisances_[i].shape_up) nuisances_[i].shape_up = (TH1F*)nuisances_[i].shape_up->Rebin(bins.size()-1,"",&(bins[0]));
//    }
//  }


//  int CombineHarvester::ParseDatacard(std::string const& filename) {
//    std::vector<std::string> as_vec;
//    boost::split(as_vec, filename, boost::is_any_of("/"));
//    if (as_vec.size() > 0) {
//      std::vector<std::string> tmp;
//      boost::split(tmp, as_vec.back(), boost::is_any_of(".-_"));
//      if (tmp.size() == 6) {
//        std::cout << "Info in <CombineHarvester::ParseDatacard>: For file " << as_vec.back() 
//          << " interpret channel, id, era and mass as \"" << tmp[1] << "\", \"" << tmp[2] << "\", \"" << tmp[3]
//          << "\" and \"" << tmp[4] << "\" respectively" << std::endl;
//        return this->ParseDatacard(filename, tmp[1], boost::lexical_cast<int>(tmp[2]), tmp[3], tmp[4]);
//      } else {
//        std::cout << "Warning in <CombineHarvester::ParseDatacard>: Unable to interpret channel and era from file name" << std::endl;
//        return 1;
//      }
//    } else {
//      std::cout << "Warning in <CombineHarvester::ParseDatacard>: Unable to interpret channel and era from file name" << std::endl;
//      return 1;
//    }
//  }

//  int CombineHarvester::ParseDatacard(std::string const& filename, std::string mass) {
//    std::vector<std::string> as_vec;
//    boost::split(as_vec, filename, boost::is_any_of("/"));
//    if (as_vec.size() > 0) {
//      std::vector<std::string> tmp;
//      boost::split(tmp, as_vec.back(), boost::is_any_of(".-_"));
//      if (tmp.size() == 5) {
//        std::cout << "Info in <CombineHarvester::ParseDatacard>: For file " << as_vec.back() 
//          << " interpret channel, id, era and mass as \"" << tmp[1] << "\", \"" << tmp[2] << "\", \"" << tmp[3]
//          << "\" and \"" << mass << "\" respectively" << std::endl;
//        return this->ParseDatacard(filename, tmp[1], boost::lexical_cast<int>(tmp[2]), tmp[3], mass);
//      } else {
//        std::cout << "Warning in <CombineHarvester::ParseDatacard>: Unable to interpret channel and era from file name" << std::endl;
//        return 1;
//      }
//    } else {
//      std::cout << "Warning in <CombineHarvester::ParseDatacard>: Unable to interpret channel and era from file name" << std::endl;
//      return 1;
//    }
//  }

// //htt_mt.input_7TeV.root
//  int CombineHarvester::ParseROOTFile(std::string const& filename) {
//    // Assume filename is of the form htt_$CHANNEL.inputs-$ANALYSIS-$ERA.root
//    std::vector<std::string> as_vec;
//    boost::split(as_vec, filename, boost::is_any_of("/"));
//    if (as_vec.size() > 0) {
//      std::vector<std::string> tmp;
//      boost::split(tmp, as_vec.back(), boost::is_any_of(".-_"));
//      if (tmp.size() == 5) {
//        std::cout << "Info in <CombineHarvester::ParseROOTFile>: For file " << as_vec.back() 
//          << " interpret channel and era as \"" << tmp[1] << "\" and \"" << tmp[3] << "\" respectively" << std::endl;
//        return this->ParseROOTFile(filename, tmp[1], tmp[3]);
//      } else {
//        std::cout << "Warning in <CombineHarvester::ParseROOTFile>: Unable to interpret channel and era from file name" << std::endl;
//        return 1;
//      }
//    } else {
//      std::cout << "Warning in <CombineHarvester::ParseROOTFile>: Unable to interpret channel and era from file name" << std::endl;
//      return 1;
//    }
//  }

//  int CombineHarvester::ParseROOTFile(const std::string & filename, std::string const& channel, std::string era) {
//    // Try and find shapes for each known process
//    TFile *f = new TFile(filename.c_str());
//    if (!f) return 1;
//    f->cd();
//    for (unsigned i = 0; i < processes_.size(); ++i) {
//      if (processes_[i].channel == channel && processes_[i].era == era) {
//        std::string cat = processes_[i].category;
//        if (!gDirectory->cd(("/"+cat).c_str())) {
//          std::cerr << "Warning, category " << cat << " not found in ROOT File" << std::endl;
//          continue;
//        } 
//        std::string name = processes_[i].process;
//        if (processes_[i].process_id <= 0) name += processes_[i].mass;
//        TH1F* hist = (TH1F*)gDirectory->Get(name.c_str());
//        if (!hist) {
//          std::cerr << "Warning, histogram " << name << " not found in ROOT File" << std::endl;
//          continue;
//        } else {
//          hist = (TH1F*)hist->Clone();
//        }
//        processes_[i].shape = hist;
//        processes_[i].rate = hist->Integral();
//      }
//    }

//    for (unsigned i = 0; i < obs_.size(); ++i) {
//      if (obs_[i].channel == channel && obs_[i].era == era) {
//        std::string cat = obs_[i].category;
//        if (!gDirectory->cd(("/"+cat).c_str())) {
//          std::cerr << "Warning, category " << cat << " not found in ROOT File" << std::endl;
//          continue;
//        } 
//        std::string name = obs_[i].process;
//        TH1F* hist = (TH1F*)gDirectory->Get(name.c_str());
//        if (!hist) {
//          std::cerr << "Warning, histogram " << name << " not found in ROOT File" << std::endl;
//          continue;
//        } else {
//          hist = (TH1F*)hist->Clone();
//        }
//        obs_[i].shape = hist;
//        // Create poisson errors
//        obs_[i].errors = new TGraphAsymmErrors(BuildPoissonErrors(*hist));
//      }
//    }

//    for (unsigned i = 0; i < nuisances_.size(); ++i) {
//      if (nuisances_[i].type != "shape") continue;
//      if (nuisances_[i].channel != channel || nuisances_[i].era != era) continue;

//      std::string cat = nuisances_[i].category;
//      if (!gDirectory->cd(("/"+cat).c_str())) {
//        std::cerr << "Warning, category " << cat << " not found in ROOT File" << std::endl;
//        continue;
//      } 
//      std::string name = nuisances_[i].process;
//      if (nuisances_[i].process_id <= 0) name += nuisances_[i].mass;
//      std::string up_name = name + "_" + nuisances_[i].nuisance + "Up";
//      std::string down_name = name + "_" + nuisances_[i].nuisance + "Down";

//      TH1F* hist = (TH1F*)gDirectory->Get(name.c_str());
//      if (!hist) {
//        std::cerr << "Warning, histogram " << name << " not found in ROOT File" << std::endl;
//        continue;
//      } else {
//        nuisances_[i].shape = (TH1F*)hist->Clone(); 
//      }

//      TH1F* up_hist = (TH1F*)gDirectory->Get(up_name.c_str());
//      if (!up_hist) {
//        std::cerr << "Warning, histogram " << up_name << " not found in ROOT File" << std::endl;
//        continue;
//      } else {
//        nuisances_[i].shape_up = (TH1F*)up_hist->Clone(); 
//      }

//      TH1F* down_hist = (TH1F*)gDirectory->Get(down_name.c_str());
//      if (!down_hist) {
//        std::cerr << "Warning, histogram " << down_name << " not found in ROOT File" << std::endl;
//        continue;
//      } else {
//        nuisances_[i].shape_down = (TH1F*)down_hist->Clone(); 
//      }

//    }
//    return 0;

//    // Try and find shapes for each known shape nuisance
//  }

// algorithm for shapes
/*
  5 shapes * * ../common/htt_em.input_7TeV.root $CHANNEL/$PROCESS $CHANNEL/$PROCESS_$SYSTEMATIC
  6 shapes ggH * ../common/htt_em.input_7TeV.root $CHANNEL/$PROCESS$MASS $CHANNEL/$PROCESS$MASS_$SYSTEMATIC
  7 shapes qqH * ../common/htt_em.input_7TeV.root $CHANNEL/$PROCESS$MASS $CHANNEL/$PROCESS$MASS_$SYSTEMATIC
  8 shapes VH * ../common/htt_em.input_7TeV.root $CHANNEL/$PROCESS$MASS $CHANNEL/$PROCESS$MASS_$SYSTEMATIC

  process in list?
   yes? process,category in list?
     yes? go to file and load, subbing into expression

*/


  

//  bool CombineHarvester::HasProcess(std::string const& process) const {
//    for (unsigned i = 0; i < processes_.size(); ++i) {
//      if (processes_[i].process == process) return true;
//    }
//    return false;
//  }

//  void CombineHarvester::ScaleProcessByEra(std::string const& process, std::string const& era, double scale) {
//    for (unsigned i = 0; i < processes_.size(); ++i) {
//      if (processes_[i].process == process && processes_[i].era == era) {
//        processes_[i].rate *= scale;
//        if (processes_[i].shape) processes_[i].shape->Scale(scale);
//      } 
//    }
//  }

//  std::pair<double, int> CombineHarvester::GetPullsChi2(bool splusb) const {
//    double tot = 0.0;
//    for (unsigned i = 0; i < pulls_.size(); ++i) {
//      tot += splusb ? (pulls_[i].splusb*pulls_[i].splusb) : (pulls_[i].bonly*pulls_[i].bonly);
//    }
//    return std::make_pair(tot, pulls_.size());
//  }






}
