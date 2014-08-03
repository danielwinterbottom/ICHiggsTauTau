#ifndef ICHiggsTauTau_CompositeCandidate_hh
#define ICHiggsTauTau_CompositeCandidate_hh
#include <map>
#include <string>
#include <numeric>
#include <functional>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "Math/VectorUtil.h"

namespace ic {

/**
 * @brief A candidate built by adding the four-momenta of other candidates.
 * Pointers to each consituent candidate are stored, and may be retrieved either
 * by label or by index.
 *
 * Due to the non-persistent nature of the pointers stored in this class it is
 *not possible to write a CompositeCandidate into an ntuple. Instead it should
 *be used as an "on-the-fly" tool to combine multiple candidates during an
 *offline analysis sequence.
 *
 * @warning The internal four-momentum is updated as soon as a Candidate is
 *added. If the four-momentum of this Candidate changes later, it will not be
 *reflected in the four-momentum of the CompositeCandidate.
 */
class CompositeCandidate : public Candidate {
 public:
  CompositeCandidate();
  virtual ~CompositeCandidate();
  virtual void Print() const;

  typedef std::vector<Candidate*>::const_iterator const_iterator;

  /**
   * @brief Add a new Candidate
   * @details Requires that a Candidate with label "name" has not already been
   *defined
   * @param name Label for the new Candidate
   * @param cand Pointer to the new Candidate
   */
  void AddCandidate(std::string name, Candidate* cand);

  /// Return a Candidate* using a vector index. Will throw if index is out of
  /// range
  inline Candidate* at(std::size_t const& index) const {
    return cand_vec_.at(index);
  }

  /// Return a Candidate* using a label key. Will throw if a Candidate with
  /// label does not exist
  inline Candidate* at(std::string const& label) const {
    return cand_map_.at(label);
  }

  /// Return a const_iterator to the start of the internal vector<Candidate*>
  inline const_iterator begin() const { return cand_vec_.begin(); }

  /// Return a const_iterator to the end of the internal vector<Candidate*>
  inline const_iterator end() const { return cand_vec_.end(); }

  /// Return a copy of the internal vector<Candidate*>
  inline std::vector<Candidate*> const& AsVector() const { return cand_vec_; }

  /**
   * @deprecated Use at(std::string const&) instead
   */
  Candidate* GetCandidate(std::string name) const;

  /**
   * @deprecated Use at(std::size_t const&) instead
   */
  Candidate* At(unsigned index) const;

  /**
   * @deprecated Use a free function or at("name")->pt() instead
   */
  double PtOf(std::string name) const;

  /**
   * @deprecated Avoid methods that aren't simple getters/setters in the future.
   * Can be replaced by a free function doing something like:
   * @code{.cpp}
   *    double pt_sum = std::accumulate(begin(), end(), 0.,
   *          [](double x, Candidate* c) { return x + c->pt(); });
   * @endcode
   */
  double ScalarPtSum() const;

  /**
   * @deprecated Avoid methods that aren't simple getters/setters in the future.
   * Use a free function instead.
   */
  double DeltaR(std::string name1, std::string name2) const;

  /**
   * @deprecated Avoid methods that aren't simple getters/setters in the future.
   * Use a free function instead.
   */
  double DeltaPhi(std::string name1, std::string name2) const;

 private:
  std::map<std::string, Candidate*> cand_map_;
  std::vector<Candidate*> cand_vec_;

  bool Verify(std::string const& name) const;
};
}
#endif
