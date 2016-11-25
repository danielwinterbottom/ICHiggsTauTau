#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"
#include <string>
#include <vector>
#include <iostream>
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "boost/format.hpp"

void PrintHeaderWithBranch(edm::ParameterSet const& config, std::string branch) {
  std::string label = config.getParameter<std::string>("@module_label");
  std::string type = config.getParameter<std::string>("@module_type");
  std::string header = (boost::format("%-39s%39s") % type % label).str();
  // std::string flow = (boost::format("%s") % branch).str();
  std::cout << std::string(78, '-') << "\n";
  std::cout << header << "\n";
  std::cout << boost::format("%-15s : %-60s\n") % "Branch" % branch;
}

void PrintHeaderWithProduces(edm::ParameterSet const& config,
                             edm::InputTag const& in, std::string branch) {
  std::string label = config.getParameter<std::string>("@module_label");
  std::string type = config.getParameter<std::string>("@module_type");
  std::string header = (boost::format("%-39s%39s") % type % label).str();
  // std::string flow = (boost::format("%s --> %s") % in.encode() % branch).str();
  std::cout << std::string(78, '-') << "\n";
  std::cout << header << "\n";
  // std::cout << "Produces: " << flow << "\n";
  std::cout << boost::format("%-15s : %-60s\n") % "Input" % in.encode();
  std::cout << boost::format("%-15s : %-60s\n") % "Branch" % branch;

}

void PrintOptional(unsigned depth, bool value, std::string text) {
  std::cout << boost::format("%s- [%c] %s\n")
      % std::string(depth, ' ')
      % (value ? 'x' : ' ')
      % text;
}

