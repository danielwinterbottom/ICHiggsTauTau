#ifndef UserCode_ICHiggsTauTau_ICLeptonIsolation_h
#define UserCode_ICHiggsTauTau_ICLeptonIsolation_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

template <class T>
class ICLeptonIsolation : public edm::EDProducer {
 public:
  explicit ICLeptonIsolation(const edm::ParameterSet &);
  ~ICLeptonIsolation();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();
  void constructSpecific(edm::Handle<edm::View<T>> const& lepton_handle,
                         edm::Event& event, const edm::EventSetup& setup);

  edm::InputTag input_;
  double deltaR_;
  std::string input_isolation_type_;
  
};

template <class T>
ICLeptonIsolation<T>::ICLeptonIsolation(
    const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      deltaR_(config.getParameter<double>("deltaR")),
      input_isolation_type_(config.getParameter<std::string>("iso_type")) {
  produces<edm::ValueMap<double> >();
}


template <class T>
ICLeptonIsolation<T>::~ICLeptonIsolation() {}

template <class T>
void ICLeptonIsolation<T>::produce(edm::Event& event,
                                 const edm::EventSetup& setup) {

  edm::Handle <edm::View<T>> lepton_handle;
  event.getByLabel(input_, lepton_handle);
 constructSpecific(lepton_handle, event, setup);

}
template <class T>
void ICLeptonIsolation<T>::constructSpecific(
    edm::Handle<edm::View<T> > const& lepton_handle, edm::Event& event,
    const edm::EventSetup& setup) {}

template <>
void ICLeptonIsolation<pat::Electron>::constructSpecific(
    edm::Handle<edm::View<pat::Electron> > const& lepton_handle, edm::Event& event,
    const edm::EventSetup& setup) {
  std::auto_ptr<edm::ValueMap<double> > product(new edm::ValueMap<double>());
  std::vector<double> values(lepton_handle->size(), 0);
  if(deltaR_ == 0.4){
  for (unsigned i = 0; i < lepton_handle->size(); ++i) {
    pat::Electron const& src = lepton_handle->at(i);
    if(input_isolation_type_=="charged_iso"){
      values[i] = src.chargedHadronIso();
    } else if(input_isolation_type_ == "photon_iso"){
      values[i] = src.photonIso();
    } else if(input_isolation_type_ == "neutral_iso"){
      values[i] = src.neutralHadronIso();
    } else if(input_isolation_type_ == "pu_iso"){
      values[i] = src.puChargedHadronIso();
//    } else if(input_isolation_type_ == "allcharged_iso"){
 //     values[i] = src.chargedIso();
    } else values[i] = 0;

  }
} else if(deltaR_==0.3){

  for (unsigned i = 0; i < lepton_handle->size(); ++i) {
    values[i]=0;
 #if CMSSW_MAJOR_VERSION >= 7
    pat::Electron const& src = lepton_handle->at(i);
    if(input_isolation_type_=="charged_iso"){
      values[i] = src.pfIsolationVariables().sumChargedHadronPt;
    } else if(input_isolation_type_ == "photon_iso"){
      values[i] = src.pfIsolationVariables().sumPhotonEt;
    } else if(input_isolation_type_ == "neutral_iso"){
      values[i] = src.pfIsolationVariables().sumNeutralHadronEt;
    } else if(input_isolation_type_ == "pu_iso"){
      values[i] = src.pfIsolationVariables().sumPUPt;
//    } else if(input_isolation_type_ == "allcharged_iso"){
 //     values[i] = src.chargedIso();
    } else if(input_isolation_type_ == "ecal_pf_cluster_iso"){
      values[i] = src.ecalPFClusterIso();
    } else if(input_isolation_type_ == "hcal_pf_cluster_iso"){
      values[i] = src.hcalPFClusterIso();
    }  else values[i] = 0;
#endif
  }
}

  edm::ValueMap<double>::Filler filler(*product);
  filler.insert(lepton_handle, values.begin(), values.end());
  filler.fill();

  event.put(product);

}

template <>
void ICLeptonIsolation<reco::Muon>::constructSpecific(
    edm::Handle<edm::View<reco::Muon> > const& lepton_handle, edm::Event& event,
    const edm::EventSetup& setup) {
  std::auto_ptr<edm::ValueMap<double> > product(new edm::ValueMap<double>());
  std::vector<double> values(lepton_handle->size(), 0);
if(deltaR_==0.4){
  for (unsigned i = 0; i < lepton_handle->size(); ++i) {
    reco::Muon const& src = lepton_handle->at(i);
    if(input_isolation_type_=="charged_iso"){
      values[i] = src.pfIsolationR04().sumChargedHadronPt;
    } else if(input_isolation_type_ == "photon_iso"){
      values[i] = src.pfIsolationR04().sumPhotonEt;
    } else if(input_isolation_type_ == "neutral_iso"){
      values[i] = src.pfIsolationR04().sumNeutralHadronEt;
    } else if(input_isolation_type_ == "pu_iso"){
      values[i] = src.pfIsolationR04().sumPUPt;
//    } else if(input_isolation_type_ == "allcharged_iso"){
 //     values[i] = src.chargedIso();
    } else values[i] = 0;

  }
} else if(deltaR_==0.3){
  for (unsigned i = 0; i < lepton_handle->size(); ++i) {
    reco::Muon const& src = lepton_handle->at(i);
    if(input_isolation_type_=="charged_iso"){
      values[i] = src.pfIsolationR03().sumChargedHadronPt;
    } else if(input_isolation_type_ == "photon_iso"){
      values[i] = src.pfIsolationR03().sumPhotonEt;
    } else if(input_isolation_type_ == "neutral_iso"){
      values[i] = src.pfIsolationR03().sumNeutralHadronEt;
    } else if(input_isolation_type_ == "pu_iso"){
      values[i] = src.pfIsolationR03().sumPUPt;
//    } else if(input_isolation_type_ == "allcharged_iso"){
 //     values[i] = src.chargedIso();
    } else values[i] = 0;

  }
}


  edm::ValueMap<double>::Filler filler(*product);
  filler.insert(lepton_handle, values.begin(), values.end());
  filler.fill();

  event.put(product);

}

#if CMSSW_MAJOR_VERSION >= 7
template <>
void ICLeptonIsolation<reco::GsfElectron>::constructSpecific(
    edm::Handle<edm::View<reco::GsfElectron> > const& lepton_handle, edm::Event& event,
    const edm::EventSetup& setup) {
  std::auto_ptr<edm::ValueMap<double> > product(new edm::ValueMap<double>());
  std::vector<double> values(lepton_handle->size(), 0);
 if(deltaR_==0.3){
  for (unsigned i = 0; i < lepton_handle->size(); ++i) {
    reco::GsfElectron const& src = lepton_handle->at(i);
    if(input_isolation_type_=="charged_iso"){
      values[i] = src.pfIsolationVariables().sumChargedHadronPt;
    } else if(input_isolation_type_ == "photon_iso"){
      values[i] = src.pfIsolationVariables().sumPhotonEt;
    } else if(input_isolation_type_ == "neutral_iso"){
      values[i] = src.pfIsolationVariables().sumNeutralHadronEt;
    } else if(input_isolation_type_ == "pu_iso"){
      values[i] = src.pfIsolationVariables().sumPUPt;
//    } else if(input_isolation_type_ == "allcharged_iso"){
 //     values[i] = src.chargedIso();
    } else values[i] = 0;

  }
}

  edm::ValueMap<double>::Filler filler(*product);
  filler.insert(lepton_handle, values.begin(), values.end());
  filler.fill();

  event.put(product);

}
#endif

template <class T>
void ICLeptonIsolation<T>::beginJob() {}

template <class T>
void ICLeptonIsolation<T>::endJob() {}



#endif
