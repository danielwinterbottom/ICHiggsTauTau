#ifndef UserCode_ICHiggsTauTau_PrintConfigTools_h
#define UserCode_ICHiggsTauTau_PrintConfigTools_h

#include <memory>
#include <vector>
#include <string>
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

void PrintHeaderWithBranch(edm::ParameterSet const& config, std::string branch);

void PrintHeaderWithProduces(edm::ParameterSet const& config,
                             edm::InputTag const& in, std::string branch);

void PrintOptional(unsigned depth, bool value, std::string text);

#endif
