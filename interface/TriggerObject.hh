#ifndef ICHiggsTauTau_TriggerObject_hh
#define ICHiggsTauTau_TriggerObject_hh
#include <vector>
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"

namespace ic {

class TriggerObject : public Candidate {
 private:
 public:
  TriggerObject();
  virtual ~TriggerObject();

  inline std::vector<std::size_t> const& filters() const { return filters_; }
  inline void set_filters(std::vector<std::size_t> const& filters) {
    filters_ = filters;
  }

  virtual void Print() const;

 private:
  std::vector<std::size_t> filters_;
};

typedef std::vector<ic::TriggerObject> TriggerObjectCollection;
}
#endif
