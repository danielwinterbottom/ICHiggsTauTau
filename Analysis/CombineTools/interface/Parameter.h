#ifndef ICHiggsTauTau_CombineTools_Parameter_h
#define ICHiggsTauTau_CombineTools_Parameter_h
#include <memory>
#include <string>
#include "TH1.h"
#include "CombineTools/interface/MakeUnique.h"

namespace ch {

class Parameter {
 public:
  Parameter();
  ~Parameter();
  Parameter(Parameter const& other);
  Parameter(Parameter&& other);
  Parameter& operator=(Parameter other);

  void set_name(std::string const& name) { name_ = name; }
  std::string const& name() const { return name_; }

  void set_val(double const& val) { val_ = val; }
  double val() const { return val_; }

  void set_err_u(double const& err_u) { err_u_ = err_u; }
  double err_u() const { return err_u_; }

  void set_err_d(double const& err_d) { err_d_ = err_d; }
  double err_d() const { return err_d_; }

  friend std::ostream& operator<< (std::ostream &out, Parameter &val);
  static std::ostream& PrintHeader(std::ostream &out);

 private:
  std::string name_;
  double val_;
  double err_u_;
  double err_d_;
  friend void swap(Parameter& first, Parameter& second);
};
}

#endif
