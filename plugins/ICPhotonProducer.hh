#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "UserCode/ICHiggsTauTau/interface/Photon.hh"
#include "EgammaAnalysis/ElectronTools/interface/PFIsolationEstimator.h"


class ICPhotonProducer : public edm::EDProducer {
   public:
      explicit ICPhotonProducer(const edm::ParameterSet&);
      ~ICPhotonProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);


      // ----------member data ---------------------------
      std::vector<ic::Photon> *photons_;
      edm::InputTag input_;
      std::string branch_name_;
      std::string pfiso_postfix_;
      edm::InputTag vertex_input_;
      double min_pt_;
      double max_eta_;
      PFIsolationEstimator *isolator_;

};