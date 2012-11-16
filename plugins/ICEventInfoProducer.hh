#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"


class ICEventInfoProducer : public edm::EDProducer {
   public:
      explicit ICEventInfoProducer(const edm::ParameterSet&);
      ~ICEventInfoProducer();

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
      //edm::InputTag input_label_;
      ic::EventInfo *info_;      
      std::string rho_jets_name_;
      std::string pfnopu_rho_jets_name_;
      std::string vertex_name_;

      //std::string embed_weight_;
};