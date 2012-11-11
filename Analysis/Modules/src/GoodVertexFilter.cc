#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/GoodVertexFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"
#include <algorithm>

namespace ic {

  GoodVertexFilter::GoodVertexFilter(std::string const& name) : ModuleBase(name) {
    ;
  }

  GoodVertexFilter::~GoodVertexFilter() {
    ;
  }

  int GoodVertexFilter::PreAnalysis() {
    return 0;
  }

  int GoodVertexFilter::Execute(TreeEvent *event) {
    VertexPtrVec const& vertexPtrVec = event->GetPtrVec<Vertex>("vertices");
    unsigned good_vertices = std::count_if(vertexPtrVec.begin(), vertexPtrVec.end(),
      ic::GoodVertex);
    if (good_vertices > 0) {
      return 0;
    } else {
      return 1;
    }
  }
  
  int GoodVertexFilter::PostAnalysis() {
    return 0;
  }

  void GoodVertexFilter::PrintInfo() {
    ;
  }
}
