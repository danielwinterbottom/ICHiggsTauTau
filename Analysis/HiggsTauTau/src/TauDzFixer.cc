#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/TauDzFixer.h"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"



namespace ic {

  TauDzFixer::TauDzFixer(std::string const& name) : ModuleBase(name) {
    tau_label_ = "taus";
    vertex_label_ = "vertices";
  }

  TauDzFixer::~TauDzFixer() {
    ;
  }

  int TauDzFixer::PreAnalysis() {
    return 0;
  }

  int TauDzFixer::Execute(TreeEvent *event) {
    std::vector<Tau *> & taus = event->GetPtrVec<Tau>(tau_label_);
    std::vector<Vertex *> const& vertices = event->GetPtrVec<Vertex>(vertex_label_);

    if (vertices.size() > 0) {
      for (auto tau : taus) tau->set_lead_dz_vertex(tau->vz() - vertices[0]->vz());
    }

    return 0;
  }



  int TauDzFixer::PostAnalysis() {
    return 0;
  }

  void TauDzFixer::PrintInfo() {
    ;
  }

}
