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
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"

ICPhotonProducer::IsoTags::IsoTags(edm::ParameterSet const& pset)
    : charged(pset.getParameter<edm::InputTag>("charged")),
      neutral(pset.getParameter<edm::InputTag>("neutral")),
      gamma(pset.getParameter<edm::InputTag>("gamma")) {}

ICPhotonProducer::ICPhotonProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      input_electron_veto_(
          config.getParameter<edm::InputTag>("inputElectronVeto")),
      do_electron_veto_(config.getParameter<bool>("includeElectronVeto")),
      input_had_tow_over_em_(
          config.getParameter<edm::InputTag>("inputHadTowOverEm")),
      do_had_tow_over_em_(config.getParameter<bool>("includeHadTowOverEm")),
      pf_iso_(config.getParameterSet("pfIso")),
      do_pf_iso_(config.getParameter<bool>("includePFIso")) {
  // isolator_ = new PFIsolationEstimator();
  // isolator_->initializePhotonIsolation(true);
  // isolator_->setConeSize(0.3);
  photons_ = new std::vector<ic::Photon>();

  PrintHeaderWithProduces(config, input_, branch_);
  PrintOptional(1, do_electron_veto_, "includeElectronVeto");
  PrintOptional(1, do_had_tow_over_em_, "includeHadTowOverEm");
  PrintOptional(1, do_pf_iso_, "includePFIso");
}

ICPhotonProducer::~ICPhotonProducer() {
  delete photons_;
  // delete isolator_;
}

void ICPhotonProducer::produce(edm::Event& event,
                               const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::Photon> > photons_handle;
  event.getByLabel(input_, photons_handle);

  edm::Handle<edm::ValueMap<bool> > electron_veto_handle;
  if (do_electron_veto_)
    event.getByLabel(input_electron_veto_, electron_veto_handle);

  edm::Handle<edm::ValueMap<float> > had_tow_over_em_handle;
  if (do_had_tow_over_em_)
    event.getByLabel(input_had_tow_over_em_, had_tow_over_em_handle);

  edm::Handle<edm::ValueMap<double> > charged_03;
  edm::Handle<edm::ValueMap<double> > neutral_03;
  edm::Handle<edm::ValueMap<double> > gamma_03;
  if (do_pf_iso_) {
    event.getByLabel(pf_iso_.charged, charged_03);
    event.getByLabel(pf_iso_.neutral, neutral_03);
    event.getByLabel(pf_iso_.gamma, gamma_03);
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
      dest.set_had_tower_over_em((*had_tow_over_em_handle)[ref]);
    }

    if (do_pf_iso_) {
      dest.set_dr03_pfiso_charged((*charged_03)[ref]);
      dest.set_dr03_pfiso_neutral((*neutral_03)[ref]);
      dest.set_dr03_pfiso_gamma((*gamma_03)[ref]);
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
