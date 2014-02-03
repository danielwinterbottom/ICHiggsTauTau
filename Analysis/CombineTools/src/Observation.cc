#include "CombineTools/interface/Observation.h"
#include <iostream>
#include "boost/format.hpp"

namespace ch {

Observation::Observation()
    : bin_(""),
      rate_(0.0),
      analysis_(""),
      era_(""),
      channel_(""),
      bin_id_(0),
      mass_(""),
      shape_() {
  }

Observation::~Observation() { }

void swap(Observation& first, Observation& second) {
  using std::swap;
  swap(first.bin_, second.bin_);
  swap(first.rate_, second.rate_);
  swap(first.analysis_, second.analysis_);
  swap(first.era_, second.era_);
  swap(first.channel_, second.channel_);
  swap(first.bin_id_, second.bin_id_);
  swap(first.mass_, second.mass_);
  swap(first.shape_, second.shape_);
}

Observation::Observation(Observation const& other)
    : bin_(other.bin_),
      rate_(other.rate_),
      analysis_(other.analysis_),
      era_(other.era_),
      channel_(other.channel_),
      bin_id_(other.bin_id_),
      mass_(other.mass_) {
  TH1 *h = dynamic_cast<TH1*>(other.shape_->Clone());
  h->SetDirectory(0);
  shape_ = std::unique_ptr<TH1>(h);
}

Observation::Observation(Observation&& other)
    : bin_(""),
      rate_(0.0),
      analysis_(""),
      era_(""),
      channel_(""),
      bin_id_(0),
      mass_(""),
      shape_() {
  swap(*this, other);
}

Observation& Observation::operator=(Observation other) {
  swap(*this, other);
  return (*this);
}

std::ostream& Observation::PrintHeader(std::ostream &out) {
  std::string line =
    (boost::format("%-6s %-9s %-6s %-8s %-28s %-3i %-26s %-10.5g %-8i")
    % "mass" % "analysis" % "era" % "channel" % "bin" % "id" % "process" %
    "rate" % "shape").str();
  std::string div(line.length(), '-');
  out << div << std::endl;
  out << line << std::endl;
  out << div << std::endl;
  return out;
}

std::ostream& operator<< (std::ostream &out, Observation &val) {
  out << boost::format("%-6s %-9s %-6s %-8s %-28s %-3i %-26s %-10.5g %-8i")
  % val.mass()
  % val.analysis()
  % val.era()
  % val.channel()
  % val.bin()
  % val.bin_id()
  % "data_obs"
  % val.rate()
  % bool(val.shape());
  return out;
}
}
