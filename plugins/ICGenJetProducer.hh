#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"


class ICGenJetProducer : public edm::EDProducer {
   public:
      explicit ICGenJetProducer(const edm::ParameterSet&);
      ~ICGenJetProducer();

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
      edm::InputTag input_label_;
      std::string branch_name_;
      bool add_all_;
      double add_all_pt_cut_;
      double add_all_eta_cut_;
      std::vector<ic::GenJet> *cand_vec;
      std::vector<std::string> merge_labels_;
      bool store_gen_particles_;
      
};