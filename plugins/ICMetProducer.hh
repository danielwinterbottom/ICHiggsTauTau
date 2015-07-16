#ifndef UserCode_ICHiggsTauTau_ICMetProducer_h
#define UserCode_ICHiggsTauTau_ICMetProducer_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"


/**
 * @brief See documentation [here](\ref objs-met)
 */
template <class T>
class ICMetProducer : public edm::EDProducer {
 public:
  explicit ICMetProducer(const edm::ParameterSet&);
  ~ICMetProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob();
  void constructSpecific(edm::Handle<edm::View<T> > const& mets_handle,
                         edm::Event& event, const edm::EventSetup& setup);
  std::vector<ic::Met>* met_;
  edm::InputTag input_;
  std::string branch_;
  boost::hash<T const*> met_hasher_;
  bool do_custom_id_;
  edm::InputTag inputID_;

  struct SigTags {
    edm::InputTag metsig;
    edm::InputTag metsigcov00;
    edm::InputTag metsigcov01;
    edm::InputTag metsigcov10;
    edm::InputTag metsigcov11;
    explicit SigTags(edm::ParameterSet const& pset);
  };

  struct SigTagsMethod2 {
    edm::InputTag metsig;
    edm::InputTag metsigcov;
    explicit SigTagsMethod2(edm::ParameterSet const& pset);
  };

  bool do_external_metsig_;
  bool do_external_metsig_method2_;
  SigTags metsig_;
  SigTagsMethod2 metsig_method2_;
  bool get_raw_from_pat_;
};

template <class T>
ICMetProducer<T>::SigTags::SigTags(edm::ParameterSet const& pset)
  : metsig(pset.getParameter<edm::InputTag>("metsig")),
    metsigcov00(pset.getParameter<edm::InputTag>("metsigcov00")),
    metsigcov01(pset.getParameter<edm::InputTag>("metsigcov01")),
    metsigcov10(pset.getParameter<edm::InputTag>("metsigcov10")),
    metsigcov11(pset.getParameter<edm::InputTag>("metsigcov11")) {}

template <class T>
ICMetProducer<T>::SigTagsMethod2::SigTagsMethod2(edm::ParameterSet const& pset)
  : metsig(pset.getParameter<edm::InputTag>("metsig")),
    metsigcov(pset.getParameter<edm::InputTag>("metsigcov")) {}

template <class T>
ICMetProducer<T>::ICMetProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      do_custom_id_(config.getParameter<bool>("includeCustomID")),
      inputID_(config.getParameter<edm::InputTag>("inputCustomID")),
      do_external_metsig_(config.getParameter<bool>("includeExternalMetsig")),
      do_external_metsig_method2_(config.getParameter<bool>("includeExternalMetsigMethod2")),
      metsig_(config.getParameterSet("metsig")),
      metsig_method2_(config.getParameterSet("metsig_method2")) {
  met_ = new std::vector<ic::Met>();
  PrintHeaderWithProduces(config, input_, branch_);
  PrintOptional(1, do_custom_id_, "includeCustomID");
}

template <>
ICMetProducer<pat::MET>::ICMetProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      do_custom_id_(config.getParameter<bool>("includeCustomID")),
      inputID_(config.getParameter<edm::InputTag>("inputCustomID")),
      do_external_metsig_(config.getParameter<bool>("includeExternalMetsig")),
      do_external_metsig_method2_(config.getParameter<bool>("includeExternalMetsigMethod2")),
      metsig_(config.getParameterSet("metsig")),
      metsig_method2_(config.getParameterSet("metsig_method2")),
      get_raw_from_pat_(config.getParameter<bool>("getUncorrectedMet")){
  met_ = new std::vector<ic::Met>();
  PrintHeaderWithProduces(config, input_, branch_);
  PrintOptional(1, do_custom_id_, "includeCustomID");
}


template <class T>
ICMetProducer<T>::~ICMetProducer() {
  delete met_;
}

template <class T>
void ICMetProducer<T>::produce(edm::Event& event, const edm::EventSetup& setup) {
  edm::Handle<edm::View<T> > mets_handle;
  event.getByLabel(input_, mets_handle);
  edm::Handle<std::vector<std::size_t> > id_handle;
  if (do_custom_id_) {
    event.getByLabel(inputID_, id_handle);
  }
  // event.getByLabel(merge_labels_[i], "MVAMetId", id_handle);

  met_->clear();
  met_->resize(mets_handle->size(), ic::Met());

  for (unsigned i = 0; i < mets_handle->size(); ++i) {
    T const& src = mets_handle->at(i);
    ic::Met& dest = met_->at(i);
    if (do_custom_id_) {
      dest.set_id(id_handle->at(i));
    } else {
      dest.set_id(met_hasher_(&src));
    }
/*    dest.set_pt(src.pt());
    dest.set_eta(src.eta());
    dest.set_phi(src.phi());
    dest.set_energy(src.energy());
    dest.set_sum_et(src.sumEt());
*/
    if(!do_external_metsig_&&!do_external_metsig_method2_){
      dest.set_et_sig(src.mEtSig());
      dest.set_xx_sig(src.getSignificanceMatrix()(0, 0));
      dest.set_xy_sig(src.getSignificanceMatrix()(0, 1));
      dest.set_yx_sig(src.getSignificanceMatrix()(1, 0));
      dest.set_yy_sig(src.getSignificanceMatrix()(1, 1));
    }
    else{
      if(!do_external_metsig_method2_){
        edm::Handle<double> metsig;
        edm::Handle<double> metsigcov00;
        edm::Handle<double> metsigcov01;
        edm::Handle<double> metsigcov10;
        edm::Handle<double> metsigcov11;
        event.getByLabel(metsig_.metsig, metsig);
        event.getByLabel(metsig_.metsigcov00, metsigcov00);
        event.getByLabel(metsig_.metsigcov01, metsigcov01);
        event.getByLabel(metsig_.metsigcov10, metsigcov10);
        event.getByLabel(metsig_.metsigcov11, metsigcov11);
        dest.set_et_sig(*metsig);
        dest.set_xx_sig(*metsigcov00);
        dest.set_xy_sig(*metsigcov01);
        dest.set_yx_sig(*metsigcov10);
        dest.set_yy_sig(*metsigcov11);
       } else{
	edm::Handle<double> metsig;
	edm::Handle<math::Error<2>::type> metsigcov;
	event.getByLabel(metsig_method2_.metsig, metsig);
	event.getByLabel(metsig_method2_.metsigcov, metsigcov);
	dest.set_et_sig(*metsig);
	dest.set_xx_sig((*metsigcov)(0,0));
	dest.set_xy_sig((*metsigcov)(0,1));
	dest.set_yx_sig((*metsigcov)(1,0));
	dest.set_yy_sig((*metsigcov)(1,1));
      }
    }
  }
  constructSpecific(mets_handle, event, setup);
}

//Specific stuff:
template <class T>
void ICMetProducer<T>::constructSpecific(
  edm::Handle<edm::View<T> > const& mets_handle, edm::Event& event,
  const edm::EventSetup& setup) {}

template <>
void ICMetProducer<reco::MET>::constructSpecific(
  edm::Handle<edm::View<reco::MET> > const& mets_handle, edm::Event& event,
  const edm::EventSetup& setup) {
    for (unsigned i = 0; i < mets_handle->size(); ++i) {
      reco::MET const& src = mets_handle->at(i);
      ic::Met& dest = met_->at(i);
      dest.set_pt(src.pt());
      dest.set_eta(src.eta());
      dest.set_phi(src.phi());
      dest.set_energy(src.energy());
      dest.set_sum_et(src.sumEt());
     }
   }

template <>
void ICMetProducer<pat::MET>::constructSpecific(
  edm::Handle<edm::View<pat::MET> > const& mets_handle, edm::Event& event,
  const edm::EventSetup& setup) {
    for (unsigned i = 0; i < mets_handle->size(); ++i) {
      pat::MET const& src = mets_handle->at(i);
      ic::Met& dest = met_->at(i);
      if(!get_raw_from_pat_){
        dest.set_pt(src.pt());
        dest.set_eta(src.eta());
        dest.set_phi(src.phi());
        dest.set_energy(src.energy());
        dest.set_sum_et(src.sumEt());
      } else {
#if CMSSW_MAJOR_VERSION >=7 && CMSSW_MINOR_VERSION >=4
        dest.set_pt(src.uncorrectedPt());
        dest.set_eta(src.eta());
        dest.set_phi(src.uncorrectedPhi());
        dest.set_energy(src.energy());
        dest.set_sum_et(src.uncorrectedSumEt());
#endif
      }
    }
  }



template <class T>
void ICMetProducer<T>::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &met_);
}

template <class T>
void ICMetProducer<T>::endJob() {}




#endif
