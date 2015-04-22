#include "../interface/Jet.hh"
#include <map>
#include <string>
#include <vector>
#include "../interface/city.h"
#include "boost/format.hpp"

namespace ic {

// Constructors/Destructors
Jet::Jet()
    : Candidate::Candidate(),
      uncorrected_energy_(0.),
      jet_area_(0.),
      pu_jet_id_(0.),
      parton_flavour_(0) {}

Jet::~Jet() {}

  void Jet::Print() const {
    Candidate::Print();
    std::cout << "--JEC Factors--" << std::endl;
    UFmap::const_iterator uf_it;
    for (uf_it = jec_factors_.begin(); uf_it != jec_factors_.end(); ++uf_it) {
      std::cout << boost::format("%-30s %-30s\n") %
                       UnHashJecFactor(uf_it->first) % uf_it->second;
    }
  }

  void Jet::SetJecFactor(std::string const& name, float const& factor) {
    jec_factors_[CityHash64(name)] = factor;
  }

  float Jet::GetJecFactor(std::string const& name) const {
    UFmap::const_iterator iter = jec_factors_.find(CityHash64(name));
    if (iter != jec_factors_.end()) {
      return iter->second;
    } else {
      std::cerr << "Warning in <Jet::GetJecFactor>: JEC Factor \"" << name
                << "\" not found" << std::endl;
      return 0.0;
    }
  }

  void Jet::SetBDiscriminator(std::string const& name, float const& value) {
    b_discriminators_[CityHash64(name)] = value;
  }

  float Jet::GetBDiscriminator(std::string const& name) const {
    UFmap::const_iterator iter = b_discriminators_.find(CityHash64(name));
    if (iter != b_discriminators_.end()) {
      return iter->second;
    } else {
      std::cerr << "Warning in <Jet::GetBDiscriminator>: Algorithm \"" << name
                << "\" not found" << std::endl;
      return 0.0;
    }
  }

  std::string Jet::UnHashJecFactor(std::size_t jec) const {
    static TSmap dict;
    static bool is_set = false;
    if (!is_set) {
      dict[CityHash64("Uncorrected")] = "Uncorrected";
      dict[CityHash64("L1Offset")] = "L1Offset";
      dict[CityHash64("L1FastJet")] = "L1FastJet";
      dict[CityHash64("L2Relative")] = "L2Relative";
      dict[CityHash64("L3Absolute")] = "L3Absolute";
      dict[CityHash64("L2L3Residual")] = "L2L3Residual";
    }
    is_set = true;
    TSmap::const_iterator it = dict.find(jec);
    if (it != dict.end()) {
      return it->second;
    } else {
      std::cerr << "Warning in <Jet>: Unable to unhash \"" << jec
                << "\", returning empty string" << std::endl;
    }
    return "";
  }

  std::string Jet::UnHashDiscr(std::size_t dis) const {
    static TSmap dict;
    static bool is_set = false;
    if (!is_set) {
      dict[CityHash64("combinedSecondaryVertexBJetTags")] =
          "combinedSecondaryVertexBJetTags";
      dict[CityHash64("combinedSecondaryVertexMVABJetTags")] =
          "combinedSecondaryVertexMVABJetTags";
      dict[CityHash64("jetBProbabilityBJetTags")] = "jetBProbabilityBJetTags";
      dict[CityHash64("jetProbabilityBJetTags")] = "jetProbabilityBJetTags";
      dict[CityHash64("simpleSecondaryVertexHighEffBJetTags")] =
          "simpleSecondaryVertexHighEffBJetTags";
      dict[CityHash64("simpleSecondaryVertexHighPurBJetTags")] =
          "simpleSecondaryVertexHighPurBJetTags";
      dict[CityHash64("softMuonBJetTags")] = "softMuonBJetTags";
      dict[CityHash64("softMuonByIP3dBJetTags")] = "softMuonByIP3dBJetTags";
      dict[CityHash64("softMuonByPtBJetTags")] = "softMuonByPtBJetTags";
      dict[CityHash64("trackCountingHighEffBJetTags")] =
          "trackCountingHighEffBJetTags";
      dict[CityHash64("trackCountingHighPurBJetTags")] =
          "trackCountingHighPurBJetTags";
    }
    is_set = true;
    TSmap::const_iterator it = dict.find(dis);
    if (it != dict.end()) {
      return it->second;
    } else {
      std::cerr << "Warning in <Jet>: Unable to unhash \"" << dis
                << "\", returning empty string" << std::endl;
    }
    return "";
  }
  }
