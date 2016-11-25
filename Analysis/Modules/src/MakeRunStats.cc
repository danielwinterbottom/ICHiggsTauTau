#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MakeRunStats.h"
#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include <fstream>

namespace ic {

  MakeRunStats::MakeRunStats(std::string const& name) : ModuleBase(name) {
    output_name_ = "default.out";
  }

  MakeRunStats::~MakeRunStats() {
    ;
  }

  int MakeRunStats::PreAnalysis() {
    return 0;
  }

  int MakeRunStats::Execute(TreeEvent *event) {
    EventInfo const *info = event->GetPtr<EventInfo>("eventInfo");
    yield_map_[info->run()] += 1;
    vtx_map_[info->run()] += info->good_vertices();
    return 0;
  }
  int MakeRunStats::PostAnalysis() {
    std::ofstream output;
    output.open(output_name_.c_str());
    std::cout << "Producing output file: " << output_name_ << std::endl;
    
    std::map<unsigned, unsigned>::const_iterator it;
    for (it = yield_map_.begin(); it != yield_map_.end(); ++it) {
      output << it->first << ":" << it->second << ":" << vtx_map_[it->first] << std::endl;
    }

    output.close();
    return 0;
  }

  void MakeRunStats::PrintInfo() {
    ;
  }
}
