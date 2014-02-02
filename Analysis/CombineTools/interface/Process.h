#ifndef ICHiggsTauTau_CombineTools_Process_h
#define ICHiggsTauTau_CombineTools_Process_h
#include <memory>
#include <string>
#include "TH1.h"
#include "CombineTools/interface/MakeUnique.h"

namespace ch {

class Process {
 public:
  Process();
  ~Process();
  Process(Process const& other);
  Process(Process&& other);
  Process& operator=(Process other);

  void bin(std::string const& bin) { bin_ = bin; }
  std::string const& bin() { return bin_; }

  void rate(double const& rate) { rate_ = rate; }
  double rate() { return rate_; }

  void process(std::string const& process) { process_ = process; }
  std::string const& process() { return process_; }

  void process_id(int const& process_id) { process_id_ = process_id; }
  int process_id() { return process_id_; }

  void analysis(std::string const& analysis) { analysis_ = analysis; }
  std::string const& analysis() { return analysis_; }

  void era(std::string const& era) { era_ = era; }
  std::string const& era() { return era_; }

  void channel(std::string const& channel) { channel_ = channel; }
  std::string const& channel() { return channel_; }

  void bin_id(int const& bin_id) { bin_id_ = bin_id; }
  int bin_id() { return bin_id_; }

  void mass(std::string const& mass) { mass_ = mass; }
  std::string const& mass() { return mass_; }

  void shape(std::unique_ptr<TH1> shape) { shape_ = std::move(shape); }
  TH1 const* shape() { return shape_.get(); }
 private:
  std::string bin_;
  double rate_;
  std::string process_;
  int process_id_;
  std::string analysis_;
  std::string era_;
  std::string channel_;
  int bin_id_;
  std::string mass_;
  std::unique_ptr<TH1> shape_;

  friend void swap(Process& first, Process& second);
};
}

#endif
