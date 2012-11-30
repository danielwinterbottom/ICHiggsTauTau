#include "UserCode/ICHiggsTauTau/plugins/ICL1ExtraProducer.hh"
#include <boost/functional/hash.hpp>
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticle.h"

template<class T>
class ICL1ExtraProducer : public edm::EDProducer {
   public:
      explicit ICL1ExtraProducer(const edm::ParameterSet&);
      ~ICL1ExtraProducer();

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
      std::vector<ic::Candidate> *cand_vec;
      std::string branch_name_;
      edm::InputTag input_label_;

};




template<class T>
ICL1ExtraProducer<T>::ICL1ExtraProducer(const edm::ParameterSet& iConfig) {
  branch_name_ = iConfig.getUntrackedParameter<std::string>("branchName");
  input_label_ = iConfig.getParameter<edm::InputTag>("inputLabel");
  cand_vec = new std::vector<ic::Candidate>();

}

template<class T>
ICL1ExtraProducer<T>::~ICL1ExtraProducer() {
 // do anything here that needs to be done at desctruction time
 // (e.g. close files, deallocate resources etc.)
 delete cand_vec;
}



// ------------ method called to produce the data  ------------
template<class T>
void ICL1ExtraProducer<T>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  

  boost::hash<T const*> hasher;


  edm::Handle<std::vector<T> > candCollection;
  iEvent.getByLabel(input_label_,candCollection);
  cand_vec->resize(0);
  cand_vec->reserve(candCollection->size());

  for (typename std::vector<T>::const_iterator iter = candCollection->begin(); iter != candCollection->end(); ++iter) {
    cand_vec->push_back(ic::Candidate());
    ic::Candidate & cand = cand_vec->back();
    //Set candidate data
    cand.set_id(hasher(&(*iter)));
    cand.set_pt(iter->pt());
    cand.set_eta(iter->eta());
    cand.set_phi(iter->phi());
    cand.set_energy(iter->energy());
    cand.set_charge(iter->charge());
  }
}

// ------------ method called once each job just before starting event loop  ------------
template<class T>
void ICL1ExtraProducer<T>::beginJob() {
 ic::StaticTree::tree_->Branch(branch_name_.c_str() ,&cand_vec);
}

// ------------ method called once each job just after ending the event loop  ------------
template<class T>
void ICL1ExtraProducer<T>::endJob() {
}

// ------------ method called when starting to processes a run  ------------
template<class T>
void ICL1ExtraProducer<T>::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
template<class T>
void ICL1ExtraProducer<T>::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
template<class T>
void ICL1ExtraProducer<T>::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
template<class T>
void ICL1ExtraProducer<T>::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
template<class T>
void ICL1ExtraProducer<T>::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
typedef ICL1ExtraProducer<l1extra::L1MuonParticle> ICL1ExtraMuonProducer;
typedef ICL1ExtraProducer<l1extra::L1EtMissParticle> ICL1ExtraEtMissProducer;
DEFINE_FWK_MODULE(ICL1ExtraMuonProducer);
DEFINE_FWK_MODULE(ICL1ExtraEtMissProducer);
