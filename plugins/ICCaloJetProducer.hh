#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"

#include "UserCode/ICHiggsTauTau/interface/CaloJet.hh"


class ICCaloJetProducer : public edm::EDProducer {
   public:
      explicit ICCaloJetProducer(const edm::ParameterSet&);
      ~ICCaloJetProducer();

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
      std::vector<ic::CaloJet> *calojets_;
      edm::InputTag input_label_;
      std::string branch_name_;
      std::map<std::string, std::size_t> observed_btag_;
      std::map<std::string, std::size_t> observed_jec_;
      JetIDSelectionFunctor *loose_id_;
      JetIDSelectionFunctor *tight_id_;

};