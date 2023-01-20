#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"


class ICGenTauProductProducer : public edm::one::EDProducer<edm::one::WatchRuns> {
   public:
      explicit ICGenTauProductProducer(const edm::ParameterSet&);
      ~ICGenTauProductProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run const& run, edm::EventSetup const& es);
      virtual void endRun(edm::Run const& run, edm::EventSetup const& es);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void FindDecayProducts(const reco::GenParticle* theParticle, std::vector<const reco::GenParticle *> &container);

      // ----------member data ---------------------------
      edm::InputTag input_label_;
      
};
