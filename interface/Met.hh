#ifndef ICHiggsTauTau_Met_hh
#define ICHiggsTauTau_Met_hh
#include <vector>
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"

namespace ic {

class Met : public Candidate {
 private:
 public:
  Met();
  virtual ~Met();

  inline double sum_et() const { return sum_et_; }
  inline void set_sum_et(double const& sum_et) { sum_et_ = sum_et; }

  inline double et_sig() const { return et_sig_; }
  inline void set_et_sig(double const& et_sig) { et_sig_ = et_sig; }

  inline double xx_sig() const { return xx_sig_; }
  inline void set_xx_sig(double const& xx_sig) { xx_sig_ = xx_sig; }

  inline double xy_sig() const { return xy_sig_; }
  inline void set_xy_sig(double const& xy_sig) { xy_sig_ = xy_sig; }

  inline double yx_sig() const { return yx_sig_; }
  inline void set_yx_sig(double const& yx_sig) { yx_sig_ = yx_sig; }

  inline double yy_sig() const { return yy_sig_; }
  inline void set_yy_sig(double const& yy_sig) { yy_sig_ = yy_sig; }

  virtual void Print() const;

 private:
  double sum_et_;
  double et_sig_;
  double xx_sig_;
  double xy_sig_;
  double yx_sig_;
  double yy_sig_;
};

typedef std::vector<ic::Met> MetCollection;
}
#endif
