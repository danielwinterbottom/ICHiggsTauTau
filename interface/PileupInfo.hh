#ifndef ICHiggsTauTau_PileupInfo_hh
#define ICHiggsTauTau_PileupInfo_hh
#include <string>
#include <vector>
#include <iostream>

namespace ic {

/**
 * @brief Stores information on the in-time or out-of-time simulated pileup
 * interactions
 */
class PileupInfo {
 public:
  PileupInfo();
  virtual ~PileupInfo();
  virtual void Print() const;

  /// @name Properties
  /**@{*/
  /// Poisson-sampled number of interactions
  inline int num_interactions() const { return num_interactions_; }

  /// Bunch crossing where zero implies in-time and non-zero out-of-time
  inline int bunch_crossing() const { return bunch_crossing_; }

  /// True number of interactions used as Poisson mean for sampling
  inline float true_num_interactions() const { return true_num_interactions_; }
  /**@}*/

  /// @name Event weights
  /**@{*/
  /// @copybrief num_interactions()
  inline void set_num_interactions(int const& num_interactions) {
    num_interactions_ = num_interactions;
  }

  /// @copybrief bunch_crossing()
  inline void set_bunch_crossing(int const& bunch_crossing) {
    bunch_crossing_ = bunch_crossing;
  }

  /// @copybrief true_num_interactions()
  inline void set_true_num_interactions(float const& true_num_interactions) {
    true_num_interactions_ = true_num_interactions;
  }
  /**@}*/

 private:
  int num_interactions_;
  int bunch_crossing_;
  float true_num_interactions_;
};

typedef std::vector<ic::PileupInfo> PileupInfoCollection;
}
/** \example plugins/ICPileupInfoProducer.cc */
#endif
