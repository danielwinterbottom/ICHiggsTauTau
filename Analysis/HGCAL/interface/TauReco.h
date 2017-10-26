#ifndef HGCAL_TauReco_h
#define HGCAL_TauReco_h
#include <string>
#include <cstdint>
#include "TGraph2D.h"
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "Objects/interface/Candidate.hh"
#include "Objects/interface/GenParticle.hh"
#include "Objects/interface/GenJet.hh"
#include "Objects/interface/CompositeCandidate.hh"
#include "Objects/interface/TriggerObject.hh"
#include "Utilities/interface/FnPairs.h"
#include "HGCAL/interface/HGCALTools.h"
#include "fastjet/ClusterSequence.hh"

namespace ic {

// s1: rechit selection
// s2: jet clustering and selection
// s3: hit pattern selection
// s4: hit merging
// s5: energy assignment to merged hits
// s6: particle ID / mass assignment

class TauReco : public ModuleBase {
 private:
  typedef std::vector<RecHit *> RecHitPtrVec;
  typedef std::vector<RecHitPtrVec> RecHitPtrVecVec;
  typedef std::vector<ic::CompositeCandidate> CompositeVec;
  typedef std::vector<CompositeVec> CompositeVecVec;

 public:
  struct Settings {
    double s1_rechit_threshold = 5.;
    double s2_jet_distance = 0.2;
    double s2_min_jet_pt = 15.;
    int s3_min_surrounding_hits = 5;
    int s3_min_lower_energy_hits = 5;
    bool s3_use_hcal_dxy = true;
    double s3_hcal_dxy = 10.;
    double s4_hit_merge_dr = 0.01;
    unsigned s4_min_hits_for_prong = 3;
    int s5_merge_strategy = 0;  // 0 = exp(-R/lambda) weighting, 1 = closest in DR
    double s5_exp_merge_scale = 0.01;
  };

  struct TauInfo {
    ic::CompositeCandidate jet;
    RecHitPtrVecVec hits_in_layers;
    std::vector<RecHitPtrVecVec> selected_hit_patterns_in_layers;
    RecHitPtrVec all_selected_central_hits;
    RecHitPtrVecVec merged_central_hits;
    std::vector<ProngCandidate> prongs;

    Settings used_settings;

    void Print() const;
  };

 private:
  Settings settings;
  void RunStep3(TauInfo &cand);
  void RunStep4(TauInfo &cand);
  void RunStep5(TauInfo &cand);

 public:
  TauReco(std::string const &name);
  virtual ~TauReco();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
};

}
#endif
