#include "UserCode/ICHiggsTauTau/plugins/ICPhotonProducer.hh"
#include <string>
#include <vector>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"

ICPhotonProducer::IsoTags::IsoTags(edm::ParameterSet const& pset, edm::ConsumesCollector && collector)
  : charged_all(pset.getParameter<edm::InputTag>("chargedAll")),
    charged(pset.getParameter<edm::InputTag>("charged")),
    neutral(pset.getParameter<edm::InputTag>("neutral")),
    gamma(pset.getParameter<edm::InputTag>("gamma")),
    pu(pset.getParameter<edm::InputTag>("pu")) {
     collector.consumes<edm::ValueMap<double>>(charged_all);
     collector.consumes<edm::ValueMap<double>>(charged);
     collector.consumes<edm::ValueMap<double>>(neutral);
     collector.consumes<edm::ValueMap<double>>(gamma);
     collector.consumes<edm::ValueMap<double>>(pu);
    }


ICPhotonProducer::ICPhotonProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      input_electron_veto_(
          config.getParameter<edm::InputTag>("inputElectronVeto")),
      do_electron_veto_(config.getParameter<bool>("includeElectronVeto")),
      do_had_tow_over_em_(config.getParameter<bool>("includeHadTowOverEm")),
      pf_iso_03_(config.getParameterSet("pfIso03"),consumesCollector()),
      pf_iso_04_(config.getParameterSet("pfIso04"),consumesCollector()),
      do_pf_iso_03_(config.getParameter<bool>("includePFIso03")),
      do_pf_iso_04_(config.getParameter<bool>("includePFIso04")),
      do_iso_from_pat_(config.getParameter<bool>("includeIsoFromPat")) {
  // isolator_ = new PFIsolationEstimator();
  // isolator_->initializePhotonIsolation(true);
  // isolator_->setConeSize(0.3);
  if(do_iso_from_pat_){
    consumes<edm::View<pat::Photon>>(input_);
  } else {
    consumes<edm::View<reco::Photon>>(input_);
  }
  consumes<edm::ValueMap<bool>>(input_electron_veto_);
  photons_ = new std::vector<ic::Photon>();

  PrintHeaderWithProduces(config, input_, branch_);
  PrintOptional(1, do_electron_veto_, "includeElectronVeto");
  PrintOptional(1, do_had_tow_over_em_, "includeHadTowOverEm");
  PrintOptional(1, do_pf_iso_03_, "includePFIso03");
  PrintOptional(1, do_pf_iso_04_, "includePFIso04");
  PrintOptional(1, do_iso_from_pat_, "includeIsoFromPat");
}

ICPhotonProducer::~ICPhotonProducer() {
  delete photons_;
  // delete isolator_;
}

void ICPhotonProducer::produce(edm::Event& event,
                               const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::Photon> > photons_handle;
  event.getByLabel(input_, photons_handle);
  edm::Handle<edm::View<pat::Photon> > pat_photons_handle;
  if(do_iso_from_pat_)event.getByLabel(input_, pat_photons_handle);

  edm::Handle<edm::ValueMap<bool> > electron_veto_handle;
  if (do_electron_veto_)
    event.getByLabel(input_electron_veto_, electron_veto_handle);

  edm::Handle<edm::ValueMap<double> > charged_all_03;
  edm::Handle<edm::ValueMap<double> > charged_03;
  edm::Handle<edm::ValueMap<double> > neutral_03;
  edm::Handle<edm::ValueMap<double> > gamma_03;
  edm::Handle<edm::ValueMap<double> > pu_03;

  //!!print stuff
  //std::cout<<"n photons: "<<photons_handle->size()<<std::endl;
  //std::cout<<"pho"<<pf_iso_03_.charged_all<<std::endl;
  if (do_pf_iso_03_&&!do_iso_from_pat_) {
    event.getByLabel(pf_iso_03_.charged_all, charged_all_03);
    event.getByLabel(pf_iso_03_.charged, charged_03);
    event.getByLabel(pf_iso_03_.neutral, neutral_03);
    event.getByLabel(pf_iso_03_.gamma, gamma_03);
    event.getByLabel(pf_iso_03_.pu, pu_03);
  }

  edm::Handle<edm::ValueMap<double> > charged_all_04;
  edm::Handle<edm::ValueMap<double> > charged_04;
  edm::Handle<edm::ValueMap<double> > neutral_04;
  edm::Handle<edm::ValueMap<double> > gamma_04;
  edm::Handle<edm::ValueMap<double> > pu_04;
  if (do_pf_iso_04_&&!do_iso_from_pat_) {
    event.getByLabel(pf_iso_04_.charged_all, charged_all_04);
    event.getByLabel(pf_iso_04_.charged, charged_04);
    event.getByLabel(pf_iso_04_.neutral, neutral_04);
    event.getByLabel(pf_iso_04_.gamma, gamma_04);
    event.getByLabel(pf_iso_04_.pu, pu_04);
  }

  // Get other inputs
  // Handle<reco::PFCandidateCollection> pfCandidatesH;
  // event.getByLabel("particleFlow", pfCandidatesH);
  // edm::Handle<reco::VertexCollection> vertexCollection;
  // event.getByLabel("offlinePrimaryVertices", vertexCollection);

  photons_->clear();
  photons_->resize(photons_handle->size(), ic::Photon());

  // unsigned idx = 0;
  for (unsigned i = 0; i < photons_handle->size(); ++i) {
    reco::Photon const& src = photons_handle->at(i);
    edm::RefToBase<reco::Photon> ref = photons_handle->refAt(i);
    ic::Photon & dest = photons_->at(i);

    dest.set_id(photon_hasher_(&src));
    dest.set_pt(src.pt());
    dest.set_eta(src.eta());
    dest.set_phi(src.phi());
    dest.set_energy(src.energy());
    dest.set_charge(src.charge());
    dest.set_sigma_IetaIeta(src.sigmaIetaIeta());

    // isolator_->fGetIsolation(&(*iter), &(*pfCandidatesH),
    // reco::VertexRef(vertexCollection,0), vertexCollection);

    // pho.set_dr03_pfiso_charged(isolator_->getIsolationCharged());
    // pho.set_dr03_pfiso_gamma(isolator_->getIsolationPhoton());
    // pho.set_dr03_pfiso_neutral(isolator_->getIsolationNeutral());

    if (do_electron_veto_) {
      dest.set_pass_electron_veto((*electron_veto_handle)[ref]);
    }

    if (do_had_tow_over_em_) {
      dest.set_had_tower_over_em(src.hadTowOverEm());
    }

    if (do_pf_iso_03_&&!do_iso_from_pat_) {
      dest.set_dr03_pfiso_charged_all((*charged_all_03)[ref]);
      dest.set_dr03_pfiso_charged((*charged_03)[ref]);
      dest.set_dr03_pfiso_neutral((*neutral_03)[ref]);
      dest.set_dr03_pfiso_gamma((*gamma_03)[ref]);
      dest.set_dr03_pfiso_pu((*pu_03)[ref]);
    }

    if (do_pf_iso_04_&&!do_iso_from_pat_) {
      dest.set_dr04_pfiso_charged_all((*charged_all_04)[ref]);
      dest.set_dr04_pfiso_charged((*charged_04)[ref]);
      dest.set_dr04_pfiso_neutral((*neutral_04)[ref]);
      dest.set_dr04_pfiso_gamma((*gamma_04)[ref]);
      dest.set_dr04_pfiso_pu((*pu_04)[ref]);
    }

    if(do_pf_iso_03_&&do_iso_from_pat_&&do_pf_iso_04_){
      throw cms::Exception("FillingTwoIsosFromOnePatIso") << "You are trying to fill both the 03 and 04 photon isolation from the single pat isolation values!!";
    }

    if(do_pf_iso_03_&&do_iso_from_pat_) {
      pat::Photon const& patsrc = pat_photons_handle->at(i);
      dest.set_dr03_pfiso_charged(patsrc.chargedHadronIso());
      dest.set_dr03_pfiso_neutral(patsrc.neutralHadronIso());
      dest.set_dr03_pfiso_gamma(patsrc.photonIso());
#if CMSSW_MAJOR_VERSION >= 7 && CMSSW_MINOR_VERSION >= 0
#if CMSSW_MINOR_VERSION >= 6
      dest.set_dr03_pfiso_charged_all(patsrc.patParticleIso());
#else
      dest.set_dr03_pfiso_charged_all(patsrc.particleIso());
#endif
      dest.set_dr03_pfiso_pu(patsrc.puChargedHadronIso());
#endif
    }

    if(do_pf_iso_04_&&do_iso_from_pat_) {
      pat::Photon const& patsrc = pat_photons_handle->at(i);
      dest.set_dr04_pfiso_charged(patsrc.chargedHadronIso());
      dest.set_dr04_pfiso_neutral(patsrc.neutralHadronIso());
      dest.set_dr04_pfiso_gamma(patsrc.photonIso());
#if CMSSW_MAJOR_VERSION >= 7 && CMSSW_MINOR_VERSION >= 0
#if CMSSW_MINOR_VERSION >= 6
      dest.set_dr04_pfiso_charged_all(patsrc.patParticleIso());
#else
      dest.set_dr04_pfiso_charged_all(patsrc.particleIso());
#endif
      dest.set_dr04_pfiso_pu(patsrc.puChargedHadronIso());
#endif
    }

    

  // #ifndef CMSSW_4_2_8_patch7
  //     pho.set_had_tower_over_em(iter->hadTowOverEm());
  // #else
  //     pho.set_had_tower_over_em(0.0);
  // #endif
  }
}

void ICPhotonProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &photons_);
}

void ICPhotonProducer::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICPhotonProducer);
