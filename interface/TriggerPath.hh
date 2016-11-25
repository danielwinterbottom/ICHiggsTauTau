#ifndef ICHiggsTauTau_TriggerPath_hh
#define ICHiggsTauTau_TriggerPath_hh
#include <string>
#include <vector>
#include "Rtypes.h"

namespace ic {

/**
 * @brief Information on a trigger path, including accept, prescale and version
 * information.
 */
class TriggerPath {
 public:
  TriggerPath();
  virtual ~TriggerPath();
  virtual void Print() const;

  /// @name Properties
  /**@{*/
  /// The trigger path name
  inline std::string const& name() const { return name_; }

  /// True if the trigger fired, false otherwise
  inline bool accept() const { return accept_; }

  /// The trigger prescale
  inline unsigned prescale() const { return prescale_; }

  /// Unique identifier, usually a hash of the trigger name
  inline std::size_t id() const { return id_; }

  /// The trigger version number
  inline unsigned version() const { return version_; }
  /**@}*/

  /// @name Setters
  /**@{*/
  /// @copybrief name()
  inline void set_name(std::string const& name) { name_ = name; }

  /// @copybrief accept()
  inline void set_accept(bool const& accept) { accept_ = accept; }

  /// @copybrief prescale()
  inline void set_prescale(unsigned const& prescale) { prescale_ = prescale; }

  /// @copybrief id()
  inline void set_id(std::size_t const& id) { id_ = id; }

  /// @copybrief version()
  inline void set_version(unsigned const& version) { version_ = version; }
  /**@}*/

 private:
  std::string name_;
  bool accept_;
  unsigned prescale_;
  std::size_t id_;
  unsigned version_;

 #ifndef SKIP_CINT_DICT
 public:
  ClassDef(TriggerPath, 2);
 #endif
};

typedef std::vector<ic::TriggerPath> TriggerPathCollection;
}
/** \example plugins/ICTriggerPathProducer.cc */
#endif
