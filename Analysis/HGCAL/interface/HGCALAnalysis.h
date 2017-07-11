#ifndef HGCAL_HGCALAnalysis_h
#define HGCAL_HGCALAnalysis_h
#include <string>
#include <cstdint>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "Core/interface/TreeEvent.h"
#include "Core/interface/ModuleBase.h"
#include "Objects/interface/Candidate.hh"
#include "Objects/interface/CompositeCandidate.hh"
#include "Objects/interface/TriggerObject.hh"


namespace ic {

  class RecHit : public Candidate {

  };

  class SimCluster : public Candidate {

  };

  std::vector<RecHit *> const& BuildRecHitCollection(TreeEvent *event);
  std::vector<SimCluster *> const& BuildSimClusterCollection(TreeEvent *event);

  class HGCALTest : public ModuleBase {
   private:
    // CLASS_MEMBER(HGCALTest, fwlite::TFileService*, fs)

   public:
    HGCALTest(std::string const& name);
    virtual ~HGCALTest();

    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
  };

}


#endif
