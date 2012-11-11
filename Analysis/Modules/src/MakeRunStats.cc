#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MakeRunStats.h"
#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include <fstream>

namespace ic {

  MakeRunStats::MakeRunStats(std::string const& name, std::string const& output_name) : ModuleBase(name) {
    output_name_ = output_name;
  }

  MakeRunStats::~MakeRunStats() {
    ;
  }

  int MakeRunStats::PreAnalysis() {
    return 0;
  }

  int MakeRunStats::Execute(TreeEvent *event) {
    std::vector<Vertex *> const& vec = event->GetPtrVec<Vertex>("vertices");
    EventInfo const *info = event->GetPtr<EventInfo>("eventInfo");
    run_yield_map_[info->run()].IncEvents();
    run_yield_map_[info->run()].IncVertices(vec.size());
    return 0;
  }
  int MakeRunStats::PostAnalysis() {
    std::cout << this->ModuleName() << std::endl;
    std::cout << "Run: 171484: " << run_yield_map_[171484].nEvents << std::endl;
    std::cout << "Run: 171812: " << run_yield_map_[171812].nEvents << std::endl;


    std::ofstream output;
    output.open(output_name_.c_str());
    std::cout << "Producing output file: " << output_name_ << std::endl;
    output << "unsigned arr_size = " << run_yield_map_.size() << ";" << std::endl;
    int wc = 0;
    output << "double runNumber[] = {" << std::endl;
    std::map<unsigned,RunStats>::iterator it2 = run_yield_map_.begin();
    ++it2;
    for (std::map<unsigned,RunStats>::iterator it = run_yield_map_.begin(); it!=run_yield_map_.end(); ++it){
      if (it2 == run_yield_map_.end()){
        output << (*it).first << std::endl;
      } else {
        output << (*it).first << ", ";
      }
      if (wc == 9) {
        output << std::endl;
        wc = -1;
      }
      ++wc;
      ++it2;
    }
    output << "};" << std::endl;
    wc = 0;

    output << "double evtCount[] = {" << std::endl;
    std::map<unsigned,RunStats>::iterator it3 = run_yield_map_.begin();
    ++it3;
    for (std::map<unsigned,RunStats>::iterator it = run_yield_map_.begin(); it!=run_yield_map_.end(); ++it){
      if (it3 == run_yield_map_.end()){
        output << (*it).second.nEvents << std::endl;
      } else {
        output << (*it).second.nEvents << ", ";
      }
      if (wc == 9) {
        output << std::endl;
        wc = -1;
      }
      ++wc;
      ++it3;
    }
    output << "};" << std::endl;
    wc = 0;

    output << "double vtxCount[] = {" << std::endl;
    std::map<unsigned,RunStats>::iterator it4 = run_yield_map_.begin();
    ++it4;
    for (std::map<unsigned,RunStats>::iterator it = run_yield_map_.begin(); it!=run_yield_map_.end(); ++it){
      if (it4 == run_yield_map_.end()){
        output << (*it).second.nVertices << std::endl;
      } else {
        output << (*it).second.nVertices << ", ";
      }
      if (wc == 9) {
        output << std::endl;
        wc = -1;
      }
      ++wc;
      ++it4;
    }
    output << "};" << std::endl;
    output.close();
    return 0;
  }

  void MakeRunStats::PrintInfo() {
    ;
  }
}
