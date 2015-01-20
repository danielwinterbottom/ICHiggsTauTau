#include "../interface/EventInfo.hh"

namespace ic {
EventInfo::EventInfo()
    : is_data_(false),
      event_(0),
      run_(0),
      lumi_block_(0),
      bunch_crossing_(0),
      jet_rho_(0.),
      lepton_rho_(0.),
      good_vertices_(0) {}

EventInfo::~EventInfo() {}

void EventInfo::Print() const {
  std::cout << "Run: " << run_ << "  Lumi: " << lumi_block_
            << "  Event: " << event_ << std::endl;
  std::cout << "Jet Rho: " << jet_rho_ << std::endl;
  std::cout << "Lepton Rho: " << lepton_rho_ << std::endl;
  std::cout << "Good vertices: " << good_vertices_ << std::endl;
  for (SDMap::const_iterator it = weights_.begin(); it != weights_.end();
       ++it) {
    std::cout << it->first << "\t\t" << it->second << "\t\t";
    SBMap::const_iterator st_it = weight_status_.find(it->first);
    if (st_it != weight_status_.end()) {
      std::cout << st_it->second << std::endl;
    } else {
      std::cout << true << std::endl;
    }
  }
}
}
