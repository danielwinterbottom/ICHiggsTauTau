#ifndef ICHiggsTauTau_CombineTools_Observation_h
#define ICHiggsTauTau_CombineTools_Observation_h
#include <memory>
#include <string>
#include "TH1.h"
#include "CombineTools/interface/MakeUnique.h"

namespace ch {

class Observation {
 public:
  Observation();
  ~Observation();
  Observation(Observation const& other);
  Observation(Observation&& other);
  Observation& operator=(Observation other);

  void bin(std::string const& bin) { bin_ = bin; }
  std::string const& bin() const { return bin_; }

  void rate(double const& rate) { rate_ = rate; }
  double rate() const { return rate_; }

  void analysis(std::string const& analysis) { analysis_ = analysis; }
  std::string const& analysis() const { return analysis_; } 

  void era(std::string const& era) { era_ = era; }
  std::string const& era() const { return era_; }

  void channel(std::string const& channel) { channel_ = channel; }
  std::string const& channel() const { return channel_; }

  void bin_id(int const& bin_id) { bin_id_ = bin_id; }
  int bin_id() const { return bin_id_; }

  void mass(std::string const& mass) { mass_ = mass; }
  std::string const& mass() const { return mass_; }

  void shape(std::unique_ptr<TH1> shape) { shape_ = std::move(shape); }
  TH1 const* shape() const { return shape_.get(); }

  friend std::ostream& operator<< (std::ostream &out, Observation &val);
  static std::ostream& PrintHeader(std::ostream &out);

 private:
  std::string bin_;
  double rate_;
  std::string analysis_;
  std::string era_;
  std::string channel_;
  int bin_id_;
  std::string mass_;
  std::unique_ptr<TH1> shape_;

  friend void swap(Observation& first, Observation& second);
};
}

#endif
