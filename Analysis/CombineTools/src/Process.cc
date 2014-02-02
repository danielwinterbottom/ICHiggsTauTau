#include "CombineTools/interface/Process.h"
#include <iostream>

namespace ch {

Process::Process()
    : bin_(""),
      rate_(0.0),
      process_(""),
      process_id_(0),
      analysis_(""),
      era_(""),
      channel_(""),
      bin_id_(0),
      mass_(""),
      shape_() {
  }

Process::~Process() { }

void swap(Process& first, Process& second) {
  using std::swap;
  swap(first.bin_, second.bin_);
  swap(first.rate_, second.rate_);
  swap(first.process_, second.process_);
  swap(first.process_id_, second.process_id_);
  swap(first.analysis_, second.analysis_);
  swap(first.era_, second.era_);
  swap(first.channel_, second.channel_);
  swap(first.bin_id_, second.bin_id_);
  swap(first.mass_, second.mass_);
  swap(first.shape_, second.shape_);
}

Process::Process(Process const& other)
    : bin_(other.bin_),
      rate_(other.rate_),
      process_(other.process_),
      process_id_(other.process_id_),
      analysis_(other.analysis_),
      era_(other.era_),
      channel_(other.channel_),
      bin_id_(other.bin_id_),
      mass_(other.mass_) {
  TH1 *h = dynamic_cast<TH1*>(other.shape_->Clone());
  h->SetDirectory(0);
  shape_ = std::unique_ptr<TH1>(h);
}

Process::Process(Process&& other)
    : bin_(""),
      rate_(0.0),
      process_(""),
      process_id_(0),
      analysis_(""),
      era_(""),
      channel_(""),
      bin_id_(0),
      mass_(""),
      shape_() {
  swap(*this, other);
}

Process& Process::operator=(Process other) {
  swap(*this, other);
  return (*this);
}
}
