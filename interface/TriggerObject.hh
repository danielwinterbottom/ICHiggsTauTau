#ifndef ICHiggsTauTau_TriggerObject_hh
#define ICHiggsTauTau_TriggerObject_hh
#include <vector>
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"

namespace ic {

/**
 * @brief Stores the four-momentum of a trigger object as well as a list of the
 * (hashed) filter labels the object was accepted by
 */
class TriggerObject : public Candidate {
 private:
 public:
  TriggerObject();
  virtual ~TriggerObject();
  virtual void Print() const;

  /// @name Properties
  /**@{*/
  /// The list of filter label hashes this object was accepted by, typically
  /// restricted to the modules in some particular HLT path
  inline std::vector<std::size_t> const& filters() const { return filters_; }
  /**@}*/

  /// @name Setters
  /**@{*/
  /// @copybrief filters()
  inline void set_filters(std::vector<std::size_t> const& filters) {
    filters_ = filters;
  }
  /**@}*/

 private:
  std::vector<std::size_t> filters_;
};

typedef std::vector<ic::TriggerObject> TriggerObjectCollection;
}
/** \example plugins/ICTriggerObjectProducer.cc */
#endif
