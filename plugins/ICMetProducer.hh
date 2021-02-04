#ifndef UserCode_ICHiggsTauTau_ICMetProducer_h
#define UserCode_ICHiggsTauTau_ICMetProducer_h

#include <memory>
#include <vector>
#include <string>
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
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
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"
#include "FWCore/Utilities/interface/Exception.h"

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
  bool store_id_from_user_cand_;

  struct SigTags {
    edm::InputTag metsig;
    edm::InputTag metsigcov00;
    edm::InputTag metsigcov01;
    edm::InputTag metsigcov10;
    edm::InputTag metsigcov11;
    explicit SigTags(edm::ParameterSet const& pset,edm::ConsumesCollector && collector);
  };

  struct SigTagsMethod2 {
    edm::InputTag metsig;
    edm::InputTag metsigcov;
    explicit SigTagsMethod2(edm::ParameterSet const& pset, edm::ConsumesCollector && collector);
  };

  bool do_gen_met_;

  bool do_external_metsig_;
  bool do_external_metsig_method2_;
  SigTags metsig_;
  SigTagsMethod2 metsig_method2_;

  bool do_metcorrections_;
  std::vector<std::string> metcorrections_;
  bool do_metuncertainties_;
  std::vector<std::string> metuncertainties_;
};

template <class T>
ICMetProducer<T>::SigTags::SigTags(edm::ParameterSet const& pset, edm::ConsumesCollector && collector)
  : metsig(pset.getParameter<edm::InputTag>("metsig")),
    metsigcov00(pset.getParameter<edm::InputTag>("metsigcov00")),
    metsigcov01(pset.getParameter<edm::InputTag>("metsigcov01")),
    metsigcov10(pset.getParameter<edm::InputTag>("metsigcov10")),
    metsigcov11(pset.getParameter<edm::InputTag>("metsigcov11")) {
      collector.consumes<double>(metsig);
      collector.consumes<double>(metsigcov00);
      collector.consumes<double>(metsigcov01);
      collector.consumes<double>(metsigcov10);
      collector.consumes<double>(metsigcov11);
    }

template <class T>
ICMetProducer<T>::SigTagsMethod2::SigTagsMethod2(edm::ParameterSet const& pset, edm::ConsumesCollector && collector)
  : metsig(pset.getParameter<edm::InputTag>("metsig")),
    metsigcov(pset.getParameter<edm::InputTag>("metsigcov")) {
     collector.consumes<double>(metsig);
     collector.consumes<math::Error<2>::type>(metsigcov);
    }

template <class T>
ICMetProducer<T>::ICMetProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      do_custom_id_(config.getParameter<bool>("includeCustomID")),
      inputID_(config.getParameter<edm::InputTag>("inputCustomID")),
      do_gen_met_(config.getParameter<bool>("doGenMet")),
      do_external_metsig_(config.getParameter<bool>("includeExternalMetsig")),
      do_external_metsig_method2_(config.getParameter<bool>("includeExternalMetsigMethod2")),
      metsig_(config.getParameterSet("metsig"),consumesCollector()),
      metsig_method2_(config.getParameterSet("metsig_method2"),consumesCollector()){
      //do_metuncertainties_(config.getParameter<bool>("includeMetUncertainties")),
      //metuncertainties_(config.getParameter<std::vector<std::string> >("metuncertainties")) {
  consumes<edm::View<T>>(input_);
  consumes<std::vector<std::size_t>>(inputID_);
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
      store_id_from_user_cand_(config.getParameter<bool>("includeUserCandID")),
      do_gen_met_(config.getParameter<bool>("doGenMet")),
      do_external_metsig_(config.getParameter<bool>("includeExternalMetsig")),
      do_external_metsig_method2_(config.getParameter<bool>("includeExternalMetsigMethod2")),
      metsig_(config.getParameterSet("metsig"),consumesCollector()),
      metsig_method2_(config.getParameterSet("metsig_method2"),consumesCollector()),
      do_metcorrections_(config.getParameter<bool>("includeMetCorrections")),
      metcorrections_(config.getParameter<std::vector<std::string> >("metcorrections")),
      do_metuncertainties_(config.getParameter<bool>("includeMetUncertainties")),
      metuncertainties_(config.getParameter<std::vector<std::string> >("metuncertainties")) {
  consumes<edm::View<pat::MET>>(input_);
  consumes<std::vector<std::size_t>>(inputID_);

  met_ = new std::vector<ic::Met>();
  PrintHeaderWithProduces(config, input_, branch_);
  PrintOptional(1, do_custom_id_, "includeCustomID");
  PrintOptional(1, do_metuncertainties_, "includeMetUncertainties");
  PrintOptional(1, do_metcorrections_, "includeMetCorrections");
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
    if (!do_gen_met_){
      
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
    }//!dogenmet
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
      if(store_id_from_user_cand_){
        std::size_t id = 0;
        std::vector<std::string> candidate_names = src.userCandNames(); 
        for(unsigned j = 0; j < candidate_names.size(); ++j){
          reco::CandidatePtr cand_ref = src.userCand(candidate_names.at(j));
          boost::hash_combine(id, cand_ref.get());
        }
        dest.set_id(id);
      }


      if (do_gen_met_){
	dest.set_pt(src.genMET()->pt());
	dest.set_eta(src.genMET()->eta());
	dest.set_phi(src.genMET()->phi());
	dest.set_energy(src.genMET()->energy());
	dest.set_sum_et(src.genMET()->sumEt());
	return;
      }
      
      dest.set_pt(src.pt());
      dest.set_eta(src.eta());
      dest.set_phi(src.phi());
      dest.set_energy(src.energy());
      dest.set_sum_et(src.sumEt());
      
      
#if CMSSW_MAJOR_VERSION > 7 || (CMSSW_MAJOR_VERSION ==7 && CMSSW_MINOR_VERSION >=4)
      // Only write correction into the output met if the user wants it
#if CMSSW_MAJOR_VERSION > 7 || (CMSSW_MAJOR_VERSION == 7 && CMSSW_REVISION >= 12)
      if (do_metcorrections_) {
	if (metcorrections_.size() != pat::MET::METCorrectionLevel::METCorrectionLevelSize){
	  throw cms::Exception("MetCorrectionNotRecognised")<<__FILE__ << " line " << __LINE__ << ": size of expected met correction object is " << metcorrections_.size() << " but pat::MET::METCorrectionLevel enum contains " << pat::MET::METCorrectionLevel::METCorrectionLevelSize << " elements. Code needs updating.\n";
	}
	
	for (unsigned j = 0; j < static_cast<unsigned>(pat::MET::METCorrectionLevel::METCorrectionLevelSize) ; ++j) {
	  ic::Met::BasicMet tmp;
	  tmp.px = src.corPx(static_cast<pat::MET::METCorrectionLevel>(j));
	  tmp.py = src.corPy(static_cast<pat::MET::METCorrectionLevel>(j));
	  tmp.sumet = src.corSumEt(static_cast<pat::MET::METCorrectionLevel>(j));
	  dest.SetCorrectedMet(metcorrections_[j],tmp);
	}

      }
#endif
      if(do_metuncertainties_){
	if (metuncertainties_.size() != pat::MET::METUncertainty::METUncertaintySize){
	  throw cms::Exception("MetCorrectionNotRecognised")<<__FILE__ << " line " << __LINE__ << ": size of expected met uncertainties object is " << metuncertainties_.size() << " but pat::MET::METUncertainty enum contains " << pat::MET::METUncertainty::METUncertaintySize << " elements. Code needs updating.\n";
	}

	for(unsigned iunc=0;iunc<static_cast<unsigned>(pat::MET::METUncertainty::METUncertaintySize);iunc++){
	  ic::Met::BasicMet tmp;
	  tmp.px = src.shiftedPx(static_cast<pat::MET::METUncertainty>(iunc));
	  tmp.py = src.shiftedPy(static_cast<pat::MET::METUncertainty>(iunc));
	  tmp.sumet = src.shiftedSumEt(static_cast<pat::MET::METUncertainty>(iunc));
	  dest.SetShiftedMet(metuncertainties_[iunc],tmp);


	}

      }
#endif
    }
}



template <class T>
void ICMetProducer<T>::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &met_);
}

template <class T>
void ICMetProducer<T>::endJob() {}




#endif
