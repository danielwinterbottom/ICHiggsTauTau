#ifndef ICHiggsTauTau_Met_hh
#define ICHiggsTauTau_Met_hh
#include <vector>
#include <map>
#include <utility>
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "Rtypes.h"

namespace ic {

/**
 * @brief Stores a missing transverse energy object and the corresponding
 * significance and corrections.
 */
class Met : public Candidate {
public:
  struct BasicMet{
    double px, py, sumet; 
    double pt() const { return sqrt(px*px+py*py); }  
    double phi() const { return std::atan2(py,px); }
  };


 private:
  typedef std::map<std::size_t, ic::Met::BasicMet> UBMmap;
  typedef std::map<std::size_t, std::string> TSmap;

 public:
  Met();
  virtual ~Met();
  virtual void Print() const;

  /// @name Properties
  /**@{*/
  /// Scalar sum of transverse energies for all input objects
  inline double sum_et() const { return sum_et_; }

  /// Signifiance of the missing transverse energy
  inline double et_sig() const { return et_sig_; }

  /// The \f$xx\f$ component of the significance matrix
  inline double xx_sig() const { return xx_sig_; }

  /// The \f$xy\f$ component of the significance matrix
  inline double xy_sig() const { return xy_sig_; }

  /// The \f$yx\f$ component of the significance matrix
  inline double yx_sig() const { return yx_sig_; }

/// The \f$yy\f$ component of the significance matrix
  inline double yy_sig() const { return yy_sig_; }
  /**@}*/

/// The vector of shifted mets
  inline std::map<std::size_t, ic::Met::BasicMet> const& shifted_mets() const { 
    return shiftedmets_; 
  }
  /**@}*/


  /// map of MET corrections.
  inline std::map<std::size_t, ic::Met::BasicMet> const& corrected_mets() const {
    return correctedmets_;
  }
  /**@}*/

  /// Returns a specific correction factor if `name` is defined, otherwise
  /// returns zero
  ic::Met::BasicMet GetCorrectedMet(std::string const& name) const;
  /**@}*/
 
 /// Returns a specific correction factor if `name` is defined, otherwise
  /// returns zero
  ic::Met::BasicMet GetShiftedMet(std::string const& name) const;
  /**@}*/

  /// @name Setters
  /**@{*/
  /// @copybrief sum_et()
  inline void set_sum_et(double const& sum_et) { sum_et_ = sum_et; }

  /// @copybrief et_sig()
  inline void set_et_sig(double const& et_sig) { et_sig_ = et_sig; }

  /// @copybrief xx_sig()
  inline void set_xx_sig(double const& xx_sig) { xx_sig_ = xx_sig; }

  /// @copybrief xy_sig()
  inline void set_xy_sig(double const& xy_sig) { xy_sig_ = xy_sig; }

  /// @copybrief yx_sig()
  inline void set_yx_sig(double const& yx_sig) { yx_sig_ = yx_sig; }

  /// @copybrief yy_sig()
  inline void set_yy_sig(double const& yy_sig) { yy_sig_ = yy_sig; }
  /**@}*/

  /// @copybrief shiftedmets()
  inline void set_shiftedmets(std::map<std::size_t, ic::Met::BasicMet> const& shiftedmets) { shiftedmets_ = shiftedmets; }
  /**@}*/

  /// @copybrief met_corrections()
  inline void set_correctedmets(std::map<std::size_t, ic::Met::BasicMet> const& cormets) {
    correctedmets_ = cormets;
  }
  /**@}*/

  /// Store a corrected met, overwriting any existing value with
  /// label `name`
  void SetCorrectedMet(std::string const& name, ic::Met::BasicMet const& value);
  /**@}*/
  /// Store a shifted met, overwriting any existing value with
  /// label `name`
  void SetShiftedMet(std::string const& name, ic::Met::BasicMet const& value);
  /**@}*/

 private:
  double sum_et_;
  double et_sig_;
  double xx_sig_;
  double xy_sig_;
  double yx_sig_;
  double yy_sig_;
  UBMmap shiftedmets_;
  UBMmap correctedmets_;

  std::string UnHashMetCor(std::size_t cor) const;
  std::string UnHashMetUnc(std::size_t unc) const;

 #ifndef SKIP_CINT_DICT
 public:
  ClassDef(Met, 5);
 #endif
};

  std::ostream& operator<<(std::ostream& os,const ic::Met::BasicMet& met);
  
  typedef std::vector<ic::Met> MetCollection;
}
/** \example plugins/ICMetProducer.cc */
#endif
