#ifndef ICHiggsTauTau_CompositeCandidate_hh
#define ICHiggsTauTau_CompositeCandidate_hh
#include <map>
#include <string>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"

namespace ic {

  class CompositeCandidate : public Candidate {
    public:
      //Constructors/Destructors
      CompositeCandidate();
      virtual ~CompositeCandidate();

      //Methods
      void AddCandidate(std::string name, Candidate *cand);
      Candidate* GetCandidate(std::string name) const;
      Candidate* At(unsigned index) const;
      double PtOf(std::string name) const;
      double ScalarPtSum() const;
      double DeltaR(std::string name1, std::string name2) const;
      double DeltaPhi(std::string name1, std::string name2) const;
      virtual void Print() const;
      std::vector<Candidate *> const& AsVector() const {
        return cand_vec_;
      }

    private:
      std::map<std::string, Candidate *> cand_map_;
      std::vector<Candidate *> cand_vec_;

      //Private method
      bool Verify(std::string const& name) const;
  };
  
}
#endif
