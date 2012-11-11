#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "boost/format.hpp"



namespace ic {

  //Constructors/Destructors
  Tau::Tau() {
    ;
  }

  Tau::~Tau() {
  }

  void Tau::Print() const {
    Candidate::Print();
  }


  void Tau::SetTauID(std::string const& name, float const& value) {
    tau_ids_[CityHash64(name)] = value;
  }
  
  float Tau::GetTauID(std::string const& name) const {
    UFmap::const_iterator iter = tau_ids_.find(CityHash64(name));
    if (iter != tau_ids_.end()) {
      return iter->second;
    } else {
      std::cerr << "Warning in <Tau::GetTauID>: Algorithm \"" 
          << name << "\" not found" << std::endl;
      return 0.0;
    }
  }


/*
  std::string Tau::UnHashDiscr(std::size_t dis) const {
    static TSmap dict;
    static bool is_set = false;
    if (!is_set) {
      dict[CityHash64("combinedSecondaryVertexBTauTags")]                     = "combinedSecondaryVertexBTauTags";
      dict[CityHash64("combinedSecondaryVertexMVABTauTags")]                  = "combinedSecondaryVertexMVABTauTags";
      dict[CityHash64("jetBProbabilityBTauTags")]               
        = "jetBProbabilityBTauTags";
      dict[CityHash64("jetProbabilityBTauTags")]                
        = "jetProbabilityBTauTags";
      dict[CityHash64("simpleSecondaryVertexHighEffBTauTags")]  
        = "simpleSecondaryVertexHighEffBTauTags";
      dict[CityHash64("simpleSecondaryVertexHighPurBTauTags")]  
        = "simpleSecondaryVertexHighPurBTauTags";
      dict[CityHash64("softMuonBTauTags")]                      
        = "softMuonBTauTags";
      dict[CityHash64("softMuonByIP3dBTauTags")]                
        = "softMuonByIP3dBTauTags";
      dict[CityHash64("softMuonByPtBTauTags")]                  
        = "softMuonByPtBTauTags";
      dict[CityHash64("trackCountingHighEffBTauTags")]          
        = "trackCountingHighEffBTauTags";
      dict[CityHash64("trackCountingHighPurBTauTags")]          
        = "trackCountingHighPurBTauTags";
    }
    is_set = true;
    TSmap::const_iterator it = dict.find(dis);
    if (it != dict.end()) {
      return it->second;
    } else {
      std::cerr << "Warning in <Tau>: Unable to unhash \"" <<
          dis << "\", returning empty string" << std::endl;
    }
    return "";
  }
  */
}
