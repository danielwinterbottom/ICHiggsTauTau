#ifndef HGCAL_HGCALAnalysis_h
#define HGCAL_HGCALAnalysis_h
#include <string>
#include <cstdint>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "Math/Point3D.h"
#include "Math/Point3Dfwd.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "Objects/interface/Candidate.hh"
#include "Objects/interface/GenParticle.hh"
#include "Objects/interface/CompositeCandidate.hh"
#include "Objects/interface/TriggerObject.hh"
#include "TGraph2D.h"
#include "fastjet/ClusterSequence.hh"

namespace ic {

namespace hgcal {
  const int lastLayerFH = 40;
  const double noise_MIP = 0.2;
  const double fC_per_ele = 1.6020506e-4;
  const std::vector<double> nonAgedNoises = {2100.0, 2100.0, 1600.0};
  const std::vector<double> fCPerMIP = {1.25, 2.57, 3.88};
  const std::vector<double> thicknessCorrection = {1.132,1.092,1.084};
  const std::vector<double> dEdX_weights = {
      0.0,    8.603,   8.0675,  8.0675, 8.0675, 8.0675,  8.0675, 8.0675, 8.0675,
      8.0675, 8.9515,  10.135,  10.135, 10.135, 10.135,  10.135, 10.135, 10.135,
      10.135, 10.135,  11.682,  13.654, 13.654, 13.654,  13.654, 13.654, 13.654,
      13.654, 38.2005, 55.0265, 49.871, 49.871, 49.871,  49.871, 49.871, 49.871,
      49.871, 49.871,  49.871,  49.871, 62.005, 83.1675, 92.196, 92.196, 92.196,
      92.196, 92.196,  92.196,  92.196, 92.196, 92.196,  92.196, 46.098};
  const int firstLayer = 1;
  const int lastLayer = 52;
}

class RecHit : public Candidate {
 private:
  typedef ROOT::Math::XYZPoint Point;

 private:

  int layer_;
  float thickness_;
  Point position_;

 public:
  inline int layer() const { return layer_; }
  inline void set_layer(int const &layer) { layer_ = layer; }

  inline float thickness() const { return thickness_; }
  inline void set_thickness(float const &thickness) { thickness_ = thickness; }

  inline Point const& position() const { return position_; }
  inline void set_x(double const& x) { position_.SetX(x); }
  inline void set_y(double const& y) { position_.SetY(y); }
  inline void set_z(double const& z) { position_.SetZ(z); }

};

class SimCluster : public Candidate {};

std::tuple<double, bool> RecHitAboveThreshold(RecHit const &rHit, double ecut,
                                             bool dependSensor = false);

double sigmaNoiseMeV(int layer, int thicknessIndex);
double sigmaNoiseMIP(int layer, int thicknessIndex);
double MeVperMIP(int layer, int thicknessIndex);

std::vector<RecHit *> const &BuildRecHitCollection(TreeEvent *event);
std::vector<SimCluster *> const &BuildSimClusterCollection(TreeEvent *event);
std::vector<ic::GenParticle *> const &BuildGenParticleCollection(TreeEvent *event);

class HGCALTest : public ModuleBase {
 private:
  CLASS_MEMBER(HGCALTest, fwlite::TFileService*, fs)
  TTree * t_jets_;
  float jet_pt_;
  float jet_eta_;
  float jet_phi_;
  float jet_e_;
  bool gen_matched_;
  float genjet_pt_;
  float genjet_eta_;
  float genjet_phi_;
  float genjet_e_;
  bool gen_nonu_matched_;
  float genjet_nonu_pt_;
  float genjet_nonu_eta_;
  float genjet_nonu_phi_;
  float genjet_nonu_e_;

 public:
  HGCALTest(std::string const &name);
  virtual ~HGCALTest();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
};

std::vector<TGraph2D> PlotRecHitsInLayers(std::vector<RecHit *> const &rechits,
                                          TString name_prefix);

template <class T>
std::vector<ic::CompositeCandidate> ClusterJets(
    std::vector<T *> const &input, fastjet::JetDefinition const &def);
}  // namespace ic

template <class T>
std::vector<ic::CompositeCandidate> ic::ClusterJets(
    std::vector<T *> const &input, fastjet::JetDefinition const &def) {
  std::vector<fastjet::PseudoJet> converted_input(input.size());
  for (unsigned i = 0; i < input.size(); ++i) {
    converted_input[i] = fastjet::PseudoJet(
        input[i]->vector().px(), input[i]->vector().py(),
        input[i]->vector().pz(), input[i]->vector().energy());
    converted_input[i].set_user_index(i);
  }
  fastjet::ClusterSequence cs(converted_input, def);
  std::vector<fastjet::PseudoJet> jets = fastjet::sorted_by_pt(cs.inclusive_jets());
  std::vector<ic::CompositeCandidate> final_jets(jets.size());
  for (unsigned i = 0; i < jets.size(); ++i) {
    auto components = jets[i].constituents();
    for (unsigned j = 0; j < components.size(); ++j) {
      final_jets[i].AddCandidate(TString::Format("cand%i", j).Data(), input[components[j].user_index()]);
    }
  }
  return final_jets;
}


#endif
