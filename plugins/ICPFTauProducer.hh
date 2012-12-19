#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "UserCode/ICHiggsTauTau/interface/Tau.hh"


class ICTauProducer : public edm::EDProducer {
   public:
      explicit ICTauProducer(const edm::ParameterSet&);
      ~ICTauProducer();

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
      std::vector<ic::Tau> *taus_;
      edm::InputTag input_;
      edm::InputTag vertex_input_;
      edm::InputTag track_input_;
      std::vector< std::pair<std::string, edm::InputTag> > tau_ids_;
      std::string branch_name_;
      bool store_ids_;
      std::map<std::string, std::size_t> observed_id_;
      double min_pt_;
      double max_eta_;


};