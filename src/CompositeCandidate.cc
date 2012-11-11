#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include <set>
#include <map>
#include <string>
#include "Math/VectorUtil.h"

namespace ic {
  // Constructors
  CompositeCandidate::CompositeCandidate() {
  }

  CompositeCandidate::~CompositeCandidate() {
  }

  void CompositeCandidate::AddCandidate(std::string name, Candidate *cand) {
    cand_map_[name] = cand;
    cand_vec_.push_back(cand);
    Candidate::set_vector(Candidate::vector() + cand->vector());
    Candidate::set_charge(Candidate::charge() + cand->charge());
  }

  Candidate * CompositeCandidate::GetCandidate(std::string name) const {
    if (Verify(name)) {
      return cand_map_.find(name)->second;
    } else {
      return 0;
    }
  }

  Candidate * CompositeCandidate::At(unsigned index) const {
    return cand_vec_.at(index);
  }

  double CompositeCandidate::PtOf(std::string name) const {
    if (!Verify(name)) {
      return 0.0;
    } else {
      return cand_map_.find(name)->second->pt();
    }
  }


  double CompositeCandidate::DeltaR(std::string name1,
                                    std::string name2) const {
    if (!Verify(name1) || !Verify(name2)) {
      return 0.0;
    } else {
      return ROOT::Math::VectorUtil::DeltaR(
          (cand_map_.find(name1)->second->vector()),
          (cand_map_.find(name2)->second->vector()));
    }
  }

  double CompositeCandidate::DeltaPhi(std::string name1,
                                      std::string name2) const {
    if (!Verify(name1) || !Verify(name2)) {
      return 0.0;
    } else {
      return ROOT::Math::VectorUtil::DeltaPhi(
          (cand_map_.find(name1)->second->vector()),
          (cand_map_.find(name2)->second->vector()));
    }
  }

  void CompositeCandidate::Print() const {
    std::cout << "CompositeCandidate: " << vector() << std::endl;
    std::map<std::string, Candidate*>::const_iterator iter;
    for (iter = cand_map_.begin(); iter != cand_map_.end(); ++iter) {
      std::cout << iter->first << "\t" << iter->second->vector() << std::endl;
    }
  }

  bool CompositeCandidate::Verify(std::string const& name) const {
    static std::set<std::string> warned;
    if (!cand_map_.count(name)) {
      if (!warned.count(name)) {
        warned.insert(name);
        std::cerr <<
            "Warning in CompositeCandidate: Candidate with label \""
            << name << "\" not found" << std::endl;
      }
      return false;
    } else {
      return true;
    }
  }
}

