#ifndef UserCode_ICHiggsTauTau_ICPFJetProducerFromPatNew_h
#define UserCode_ICHiggsTauTau_ICPFJetProducerFromPatNew_h

#include <memory>
#include <typeinfo>
#include "boost/functional/hash.hpp"
#include "boost/format.hpp"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "UserCode/ICHiggsTauTau/plugins/ICJetSrcHelper.hh"
#include "UserCode/ICHiggsTauTau/plugins/ICJetDestHelper.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"

/**
 * @brief See documentation [here]
 *
 */
class ICPFJetProducerFromPatNew : public edm::stream::EDProducer<> {
 public:
  explicit ICPFJetProducerFromPatNew(const edm::ParameterSet &);
  ~ICPFJetProducerFromPatNew();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();
  void constructSpecificWithSmear(edm::Handle<edm::View<pat::Jet> > const& jets_handle,
                         edm::Handle<edm::View<pat::Jet> > const& smearjets_handle,
                         edm::Handle<edm::View<pat::Jet> > const& smearupjets_handle,
                         edm::Handle<edm::View<pat::Jet> > const& smeardownjets_handle,
                         edm::Event& event, const edm::EventSetup& setup);

  std::vector<ic::PFJet> *jets_;
  std::vector<ic::PFJet> *smearjets_;
  std::vector<ic::PFJet> *smearupjets_;
  std::vector<ic::PFJet> *smeardownjets_;
  edm::InputTag input_;
  edm::InputTag inputSmear_;
  edm::InputTag inputSmearUp_;
  edm::InputTag inputSmearDown_;
  std::string branch_;
  bool doSmear_;

  std::vector<unsigned> passed_;

  JetSrcHelper<pat::Jet> src_;
  JetDestHelper<ic::PFJet> dest_;
};


#endif
