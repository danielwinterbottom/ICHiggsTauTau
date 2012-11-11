#ifndef ICHiggsTauTau_PileupInfo_hh
#define ICHiggsTauTau_PileupInfo_hh
#include <string>
#include <vector>
#include <iostream>

namespace ic {

  class PileupInfo {

  public:
    PileupInfo();
    virtual ~PileupInfo();

    inline int num_interactions() const { return num_interactions_; }
    inline void set_num_interactions(int const& num_interactions) { num_interactions_ = num_interactions; }

    inline int bunch_crossing() const { return bunch_crossing_; }
    inline void set_bunch_crossing(int const& bunch_crossing) { bunch_crossing_ = bunch_crossing; }

    // inline std::vector<float> const& z_positions() const { return z_positions_; }
    // inline void set_z_positions(std::vector<float> const& z_positions) { z_positions_ = z_positions; }

    inline float true_num_interactions() const { return true_num_interactions_; }
    inline void set_true_num_interactions(float const& true_num_interactions) { true_num_interactions_ = true_num_interactions; }


virtual void Print() const;


private:
      int num_interactions_;
      int bunch_crossing_;
      //std::vector<float> z_positions_;
      float true_num_interactions_;
};

  typedef std::vector<ic::PileupInfo> PileupInfoCollection;

}
#endif
