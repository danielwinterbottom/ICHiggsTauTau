#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "boost/regex.hpp"

class ICGenParticleProducer : public edm::EDProducer {
   public:
      explicit ICGenParticleProducer(const edm::ParameterSet&);
      ~ICGenParticleProducer();

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
      std::vector<ic::GenParticle> *cand_vec;
      std::vector<std::string> merge_labels_;
      bool store_mothers_;
      bool store_daughters_;
      bool add_status_1_;
      bool add_status_2_;
      bool add_status_3_;
      std::vector<std::string> status_1_str_;
      std::vector<std::string> status_2_str_;
      std::vector<std::string> status_3_str_;
      std::vector<boost::regex> status_1_regex_;
      std::vector<boost::regex> status_2_regex_;
      std::vector<boost::regex> status_3_regex_;
      double status_1_pt_;
      double status_2_pt_;
      double status_3_pt_;
      // std::string override_collection_;
      std::string branch_name_;
      edm::InputTag input_label_;


};