#include "UserCode/ICHiggsTauTau/plugins/ICElectronProducer.hh"
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
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
// #include "boost/format.hpp"

ICElectronProducer::IsoTags::IsoTags()
    : do_charged_all(false),
      do_charged(false),
      do_neutral(false),
      do_gamma(false),
      do_pu(false) {}

void ICElectronProducer::IsoTags::Set(const edm::ParameterSet &pset) {
  if (pset.exists("chargedAll")) {
    charged_all = pset.getParameter<edm::InputTag>("chargedAll");
    do_charged_all = true;
  }
  if (pset.exists("charged")) {
    charged = pset.getParameter<edm::InputTag>("charged");
    do_charged = true;
  }
  if (pset.exists("neutral")) {
    neutral = pset.getParameter<edm::InputTag>("neutral");
    do_neutral = true;
  }
  if (pset.exists("gamma")) {
    gamma = pset.getParameter<edm::InputTag>("gamma");
    do_gamma = true;
  }
  if (pset.exists("pu")) {
    pu = pset.getParameter<edm::InputTag>("pu");
    do_pu = true;
  }
}

ICElectronProducer::ICElectronProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      do_r9_(false),
      do_hcal_sum_(false),
      do_vertex_ip_(false),
      do_beamspot_ip_(false),
      do_conversion_matches_(false) {
  electrons_ = new std::vector<ic::Electron>();

  if (config.exists("pfIso03")) {
    pf_iso_03_.Set(config.getParameterSet("pfIso03"));
  }
  if (config.exists("pfIso04")) {
    pf_iso_04_.Set(config.getParameterSet("pfIso04"));
  }

  if (config.exists("includeR9")) {
    input_r9_ = config.getParameter<edm::InputTag>("includeR9");
    do_r9_ = true;
  }

  if (config.exists("includeHcalSum")) {
    input_hcal_sum_ = config.getParameter<edm::InputTag>("includeHcalSum");
    do_hcal_sum_ = true;
  }

  if (config.exists("includeConversionMatches")) {
    input_conversion_matches_ =
        config.getParameter<edm::InputTag>("includeConversionMatches");
    do_conversion_matches_ = true;
  }

  if (config.exists("includeVertexIP")) {
    input_vertices_ = config.getParameter<edm::InputTag>("includeVertexIP");
    do_vertex_ip_ = true;
  }

  if (config.exists("includeBeamspotIP")) {
    input_beamspot_ = config.getParameter<edm::InputTag>("includeBeamspotIP");
    do_beamspot_ip_ = true;
  }

  if (config.exists("includeFloats")) {
    edm::ParameterSet pset =
        config.getParameter<edm::ParameterSet>("includeFloats");
    std::vector<std::string> vec =
        pset.getParameterNamesForType<edm::InputTag>();
    for (unsigned i = 0; i < vec.size(); ++i) {
      input_vmaps_.push_back(
          std::make_pair(vec[i], pset.getParameter<edm::InputTag>(vec[i])));
    }
  }
}

ICElectronProducer::~ICElectronProducer() {
  delete electrons_;
}

void ICElectronProducer::produce(edm::Event& event,
                                 const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::GsfElectron> > elecs_handle;
  event.getByLabel(input_, elecs_handle);

  edm::Handle<edm::ValueMap<float> > r9_handle;
  if (do_r9_) event.getByLabel(input_r9_, r9_handle);

  edm::Handle<edm::ValueMap<float> > hcal_sum_handle;
  if (do_hcal_sum_) event.getByLabel(input_hcal_sum_, hcal_sum_handle);

  edm::Handle<edm::ValueMap<bool> > conversion_matches_handle;
  if (do_conversion_matches_)
    event.getByLabel(input_conversion_matches_, conversion_matches_handle);

  edm::Handle<reco::VertexCollection> vertices_handle;
  if (do_vertex_ip_) event.getByLabel(input_vertices_, vertices_handle);

  edm::Handle<reco::BeamSpot> beamspot_handle;
  if (do_beamspot_ip_) event.getByLabel(input_beamspot_, beamspot_handle);

  std::vector<edm::Handle<edm::ValueMap<float> > > float_handles(
      input_vmaps_.size());
  for (unsigned i = 0; i < float_handles.size(); ++i) {
    event.getByLabel(input_vmaps_[i].second, float_handles[i]);
  }

  edm::Handle<edm::ValueMap<double> > charged_all_03;
  edm::Handle<edm::ValueMap<double> > charged_03;
  edm::Handle<edm::ValueMap<double> > neutral_03;
  edm::Handle<edm::ValueMap<double> > gamma_03;
  edm::Handle<edm::ValueMap<double> > pu_03;
  if (pf_iso_03_.do_charged_all)
    event.getByLabel(pf_iso_03_.charged_all, charged_all_03);
  if (pf_iso_03_.do_charged)
    event.getByLabel(pf_iso_03_.charged, charged_03);
  if (pf_iso_03_.do_neutral)
    event.getByLabel(pf_iso_03_.neutral, neutral_03);
  if (pf_iso_03_.do_gamma)
    event.getByLabel(pf_iso_03_.gamma, gamma_03);
  if (pf_iso_03_.do_pu)
    event.getByLabel(pf_iso_03_.pu, pu_03);

  edm::Handle<edm::ValueMap<double> > charged_all_04;
  edm::Handle<edm::ValueMap<double> > charged_04;
  edm::Handle<edm::ValueMap<double> > neutral_04;
  edm::Handle<edm::ValueMap<double> > gamma_04;
  edm::Handle<edm::ValueMap<double> > pu_04;
  if (pf_iso_04_.do_charged_all)
    event.getByLabel(pf_iso_04_.charged_all, charged_all_04);
  if (pf_iso_04_.do_charged)
    event.getByLabel(pf_iso_04_.charged, charged_04);
  if (pf_iso_04_.do_neutral)
    event.getByLabel(pf_iso_04_.neutral, neutral_04);
  if (pf_iso_04_.do_gamma)
    event.getByLabel(pf_iso_04_.gamma, gamma_04);
  if (pf_iso_04_.do_pu)
    event.getByLabel(pf_iso_04_.pu, pu_04);

  // Prepare output collection
  electrons_->clear();
  electrons_->resize(elecs_handle->size(), ic::Electron());

  for (unsigned i = 0; i < elecs_handle->size(); ++i) {
    reco::GsfElectron const& src = elecs_handle->at(i);
    edm::RefToBase<reco::GsfElectron> ref = elecs_handle->refAt(i);
    ic::Electron & dest = electrons_->at(i);
    dest.set_id(gsf_electron_hasher_(&src));
    dest.set_pt(src.pt());
    dest.set_eta(src.eta());
    dest.set_phi(src.phi());
    dest.set_energy(src.energy());
    dest.set_charge(src.charge());
    dest.set_dr03_tk_sum_pt(src.dr03TkSumPt());
    dest.set_dr03_ecal_rechit_sum_et(src.dr03EcalRecHitSumEt());
    dest.set_dr03_hcal_tower_sum_et(src.dr03HcalTowerSumEt());

    dest.set_hadronic_over_em(src.hadronicOverEm());
    dest.set_sigma_IetaIeta(src.sigmaIetaIeta());
    dest.set_dphi_sc_tk_at_vtx(src.deltaPhiSuperClusterTrackAtVtx());
    dest.set_deta_sc_tk_at_vtx(src.deltaEtaSuperClusterTrackAtVtx());
    if (src.gsfTrack().isNonnull()) {
      dest.set_gsf_tk_nhits(
          src.gsfTrack()->trackerExpectedHitsInner().numberOfHits());
    }
    dest.set_conv_dist(src.convDist());
    dest.set_conv_dcot(src.convDcot());
    dest.set_f_brem(src.fbrem());
    if (src.superCluster().isNonnull()) {
      dest.set_sc_eta(src.superCluster()->eta());
      dest.set_sc_theta(src.superCluster()->position().theta());
      dest.set_sc_energy(src.superCluster()->energy());
      dest.set_sc_e_over_p(src.eSuperClusterOverP());
    }
    dest.set_ecal_energy(src.ecalEnergy());
    dest.set_vx(src.vx());
    dest.set_vy(src.vy());
    dest.set_vz(src.vz());

    if (do_r9_) dest.set_r9((*r9_handle)[ref]);
    if (do_hcal_sum_)
      dest.set_hcal_sum((*hcal_sum_handle)[ref]);

    if (do_conversion_matches_)
      dest.set_has_matched_conversion((*conversion_matches_handle)[ref]);

    for (unsigned v = 0; v < float_handles.size(); ++v) {
      dest.SetIdIso(input_vmaps_[v].first,
                    (*(float_handles[v]))[ref]);
    }

    if (pf_iso_03_.do_charged_all)
      dest.set_dr03_pfiso_charged_all((*charged_all_03)[ref]);
    if (pf_iso_03_.do_charged)
      dest.set_dr03_pfiso_charged((*charged_03)[ref]);
    if (pf_iso_03_.do_neutral)
      dest.set_dr03_pfiso_neutral((*neutral_03)[ref]);
    if (pf_iso_03_.do_gamma)
      dest.set_dr03_pfiso_gamma((*gamma_03)[ref]);
    if (pf_iso_03_.do_pu)
      dest.set_dr03_pfiso_pu((*pu_03)[ref]);

    if (pf_iso_04_.do_charged_all)
      dest.set_dr04_pfiso_charged_all((*charged_all_04)[ref]);
    if (pf_iso_04_.do_charged)
      dest.set_dr04_pfiso_charged((*charged_04)[ref]);
    if (pf_iso_04_.do_neutral)
      dest.set_dr04_pfiso_neutral((*neutral_04)[ref]);
    if (pf_iso_04_.do_gamma)
      dest.set_dr04_pfiso_gamma((*gamma_04)[ref]);
    if (pf_iso_04_.do_pu)
      dest.set_dr04_pfiso_pu((*pu_04)[ref]);

    if (do_vertex_ip_) {
      if (vertices_handle->size() > 0) {
        reco::Vertex const& vtx = vertices_handle->at(0);
        dest.set_dz_vertex(src.gsfTrack()->dz(vtx.position()));
        dest.set_dxy_vertex(src.gsfTrack()->dxy(vtx.position()));
      }
    }
    if (do_beamspot_ip_) {
      dest.set_dxy_beamspot(src.gsfTrack()->dxy(*beamspot_handle));
    }
  }
}

void ICElectronProducer::beginJob() {
  // std::cout << "-----------------------------------------------------------------" << std::endl;
  // std::cout << "Info in <ICElectronProducer>: BeginJob Summary for input: " << input_.label() << std::endl;
  // std::cout << "-----------------------------------------------------------------" << std::endl;
  ic::StaticTree::tree_->Branch(branch_.c_str(), &electrons_);
}

void ICElectronProducer::endJob() {
  // std::cout << "-----------------------------------------------------------------" << std::endl;
  // std::cout << "Info in <ICElectronProducer>: EndJob Summary for input: " << input_.label() << std::endl;
  // std::cout << "-----------------------------------------------------------------" << std::endl;
}

// define this as a plug-in
DEFINE_FWK_MODULE(ICElectronProducer);
