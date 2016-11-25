#ifndef HiggsTauTau_HTTGenEventPlots_h
#define HiggsTauTau_HTTGenEventPlots_h

#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/PFCandidate.hh"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/interface/Track.hh"
#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "TH1F.h"
#include "TH2F.h"

#include <string>

namespace ic {

class HTTGenEventPlots : public ModuleBase {
 private:
  TFileDirectory * dir_;

  TH1F *h_gen_h_pt;
  TH1F *h_gen_h_eta;
  TH1F *h_gen_h_phi;
  TH1F *h_gen_h_mass;

  TH1F *h_gen_th_pt;
  TH1F *h_gen_th_eta;
  TH1F *h_gen_th_mode;

  CLASS_MEMBER(HTTGenEventPlots, fwlite::TFileService*, fs)

 public:
  HTTGenEventPlots(std::string const& name);
  virtual ~HTTGenEventPlots();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
};

}

#endif
