#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"

#include "UserCode/ICHiggsTauTau/interface/JPTJet.hh"
#include "DataFormats/PatCandidates/interface/Jet.h"



class ICJPTJetProducer : public edm::EDProducer {
   public:
      explicit ICJPTJetProducer(const edm::ParameterSet&);
      ~ICJPTJetProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      std::vector<std::size_t> makeTrackIDVec(reco::TrackRefVector const& ref_vec, 
                                              std::vector<unsigned> & idx_vec,
                                              reco::Track const* ptr_first);
      std::vector<reco::Track const*> TrackPtrVec(reco::TrackRefVector const& ref_vec);

      // ----------member data ---------------------------
      std::vector<ic::JPTJet> *jptjets_;
      edm::InputTag input_label_;
      std::string branch_name_;
      bool store_ids_;

      std::map<std::string, std::size_t> observed_btag_;
      std::map<std::string, std::size_t> observed_jec_;
      JetIDSelectionFunctor *loose_id_;
      JetIDSelectionFunctor *tight_id_;

};