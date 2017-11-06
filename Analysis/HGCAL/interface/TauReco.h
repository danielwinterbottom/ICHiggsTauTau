#ifndef HGCAL_TauReco_h
#define HGCAL_TauReco_h
#include <string>
#include <cstdint>
#include "TGraph2D.h"
#include "TH2F.h"
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

  CLASS_MEMBER(TauReco, fwlite::TFileService*, fs)
  CLASS_MEMBER(TauReco, double, s1_rechit_threshold)
  CLASS_MEMBER(TauReco, int, s1_pu_strategy)
  CLASS_MEMBER(TauReco, double, s2_jet_distance)
  CLASS_MEMBER(TauReco, double, s2_min_jet_pt)
  CLASS_MEMBER(TauReco, int, s3_min_surrounding_hits)
  CLASS_MEMBER(TauReco, int, s3_min_lower_energy_hits)
  CLASS_MEMBER(TauReco, bool, s3_use_hcal_dxy)
  CLASS_MEMBER(TauReco, double, s3_hcal_dxy)
  CLASS_MEMBER(TauReco, bool, s3_use_single_hit)
  CLASS_MEMBER(TauReco, double, s3_single_hit_min)
  CLASS_MEMBER(TauReco, double, s4_hit_merge_dr)
  CLASS_MEMBER(TauReco, unsigned, s4_min_hits_for_prong)
  CLASS_MEMBER(TauReco, int, s5_merge_strategy)
  CLASS_MEMBER(TauReco, double, s5_exp_merge_scale)
  CLASS_MEMBER(TauReco, std::string, postfix)

  std::vector<TH1F> pu_bins;
  std::vector<TH2F *> pu_profiles;
  std::vector<TH2F *> jet_energy_cont_profiles;

 public:
  struct Settings {
    double s1_rechit_threshold;
    int s1_pu_strategy;
    double s2_jet_distance;
    double s2_min_jet_pt;
    int s3_min_surrounding_hits;
    int s3_min_lower_energy_hits;
    bool s3_use_hcal_dxy;
    double s3_hcal_dxy;
    bool s3_use_single_hit;
    double s3_single_hit_min;
    double s4_hit_merge_dr;
    unsigned s4_min_hits_for_prong;
    int s5_merge_strategy;
    double s5_exp_merge_scale;

    void Print() const;
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
