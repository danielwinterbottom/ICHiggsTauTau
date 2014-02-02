#ifndef ICHiggsTauTau_CombineTools_Nuisance_h
#define ICHiggsTauTau_CombineTools_Nuisance_h
#include <memory>
#include <string>
#include "TH1.h"
#include "CombineTools/interface/MakeUnique.h"

namespace ch {

class Nuisance {
 public:
  Nuisance();
  ~Nuisance();
  Nuisance(Nuisance const& other);
  Nuisance(Nuisance&& other);
  Nuisance& operator=(Nuisance other);

  void bin(std::string const& bin) { bin_ = bin; }
  std::string const& bin() const { return bin_; }

  void process(std::string const& process) { process_ = process; }
  std::string const& process() const { return process_; }

  void process_id(int const& process_id) { process_id_ = process_id; }
  int process_id() const { return process_id_; }

  void name(std::string const& name) { name_ = name; }
  std::string const& name() const { return name_; }

  void type(std::string const& type) { type_ = type; }
  std::string const& type() const { return type_; }

  void value_u(double const& value_u) { value_u_ = value_u; }
  double value_u() const { return value_u_; }

  void value_d(double const& value_d) { value_d_ = value_d; }
  double value_d() const { return value_d_; }

  void asymm(bool const& asymm) { asymm_ = asymm; }
  bool asymm() const { return asymm_; }

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

  void shape_u(std::unique_ptr<TH1> shape_u) { shape_u_ = std::move(shape_u); }
  TH1 const* shape_u() const { return shape_u_.get(); }

  void shape_d(std::unique_ptr<TH1> shape_d) { shape_d_ = std::move(shape_d); }
  TH1 const* shape_d() const { return shape_d_.get(); }

  friend std::ostream& operator<< (std::ostream &out, Nuisance &val);
  static std::ostream& PrintHeader(std::ostream &out);

 private:
  std::string bin_;
  std::string process_;
  int process_id_;
  std::string name_;
  std::string type_;
  double value_u_;
  double value_d_;
  bool asymm_;
  std::string analysis_;
  std::string era_;
  std::string channel_;
  int bin_id_;
  std::string mass_;
  std::unique_ptr<TH1> shape_u_;
  std::unique_ptr<TH1> shape_d_;

  friend void swap(Nuisance& first, Nuisance& second);
};
}

#endif
