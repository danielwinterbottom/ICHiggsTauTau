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
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

ICElectronProducer::IsoTags::IsoTags(edm::ParameterSet const& pset,edm::ConsumesCollector && collector)
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

ICElectronProducer::ClusterIsoTags::ClusterIsoTags(edm::ParameterSet const& pset, edm::ConsumesCollector && collector)
    : ecal(pset.getParameter<edm::InputTag>("ecal")),
      hcal(pset.getParameter<edm::InputTag>("hcal")) {
        collector.consumes<edm::ValueMap<double>>(ecal);
        collector.consumes<edm::ValueMap<double>>(hcal);
      }

ICElectronProducer::ICElectronProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      input_r9_(config.getParameter<edm::InputTag>("inputR9")),
      do_r9_(config.getParameter<bool>("includeR9")),
      input_hcal_sum_(config.getParameter<edm::InputTag>("inputHcalSum")),
      do_hcal_sum_(config.getParameter<bool>("includeHcalSum")),
      input_vertices_(config.getParameter<edm::InputTag>("inputVertices")),
      do_vertex_ip_(config.getParameter<bool>("includeVertexIP")),
      input_beamspot_(config.getParameter<edm::InputTag>("inputBeamspot")),
      do_beamspot_ip_(config.getParameter<bool>("includeBeamspotIP")),
      input_conversion_matches_(
          config.getParameter<edm::InputTag>("inputConversionMatches")),
      do_conversion_matches_(
          config.getParameter<bool>("includeConversionMatches")),
      cluster_iso_(config.getParameterSet("clusterIso"),consumesCollector()),
      pf_iso_03_(config.getParameterSet("pfIso03"),consumesCollector()),
      pf_iso_04_(config.getParameterSet("pfIso04"),consumesCollector()),
      do_cluster_iso_(config.getParameter<bool>("includeClusterIso")),
      do_pf_iso_03_(config.getParameter<bool>("includePFIso03")),
      do_pf_iso_04_(config.getParameter<bool>("includePFIso04")) {
        consumes<edm::View<reco::GsfElectron>>(input_);
        consumes<edm::ValueMap<float>>(input_r9_);
        consumes<edm::ValueMap<float>>(input_hcal_sum_);
        consumes<edm::View<reco::Vertex>>(input_vertices_);
        consumes<reco::BeamSpot>(input_beamspot_);
        consumes<edm::ValueMap<bool>>(input_conversion_matches_);

     electrons_ = new std::vector<ic::Electron>();

  edm::ParameterSet pset_floats =
      config.getParameter<edm::ParameterSet>("includeFloats");
  std::vector<std::string> vec =
      pset_floats.getParameterNamesForType<edm::InputTag>();
  for (unsigned i = 0; i < vec.size(); ++i) {
    input_vmaps_.push_back(std::make_pair(
        vec[i], pset_floats.getParameter<edm::InputTag>(vec[i])));
       consumes<edm::ValueMap<float>>(input_vmaps_[i].second);
  }

  //PrintHeaderWithProduces(config, input_, branch_);
  PrintOptional(1, do_r9_, "includeR9");
  PrintOptional(1, do_hcal_sum_, "includeHcalSum");
  PrintOptional(1, do_vertex_ip_, "includeVertexIP");
  PrintOptional(1, do_beamspot_ip_, "includeBeamspotIP");
  PrintOptional(1, do_conversion_matches_, "includeConversionMatches");
  PrintOptional(1, do_pf_iso_03_, "includePFIso03");
  PrintOptional(1, do_pf_iso_04_, "includePFIso04");
}

ICElectronProducer::~ICElectronProducer() { delete electrons_; }

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

  edm::Handle<edm::View<reco::Vertex> > vertices_handle;
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
  edm::Handle<edm::ValueMap<double> > ecal_pf_cluster_iso;
  edm::Handle<edm::ValueMap<double> > hcal_pf_cluster_iso;

  if (do_cluster_iso_){
    event.getByLabel(cluster_iso_.ecal, ecal_pf_cluster_iso);
    event.getByLabel(cluster_iso_.hcal, hcal_pf_cluster_iso);
  }
  if (do_pf_iso_03_) {
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
  if (do_pf_iso_04_) {
    event.getByLabel(pf_iso_04_.charged_all, charged_all_04);
    event.getByLabel(pf_iso_04_.charged, charged_04);
    event.getByLabel(pf_iso_04_.neutral, neutral_04);
    event.getByLabel(pf_iso_04_.gamma, gamma_04);
    event.getByLabel(pf_iso_04_.pu, pu_04);
  }

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
#if CMSSW_MAJOR_VERSION >7 || (CMSSW_MAJOR_VERSION == 7 && CMSSW_MINOR_VERSION >= 1)
    dest.set_full5x5_sigma_IetaIeta(src.full5x5_sigmaIetaIeta());
#endif
    dest.set_dphi_sc_tk_at_vtx(src.deltaPhiSuperClusterTrackAtVtx());
    dest.set_deta_sc_tk_at_vtx(src.deltaEtaSuperClusterTrackAtVtx());
    if (src.gsfTrack().isNonnull()) {
#if CMSSW_MAJOR_VERSION > 7 || (CMSSW_MAJOR_VERSION == 7 && CMSSW_MINOR_VERSION >= 2)
      dest.set_gsf_tk_nhits(
          src.gsfTrack()->hitPattern().numberOfHits(
              reco::HitPattern::MISSING_INNER_HITS));
#else
      dest.set_gsf_tk_nhits(
          src.gsfTrack()->trackerExpectedHitsInner().numberOfHits());
#endif
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

    if (do_r9_) {
      dest.set_r9((*r9_handle)[ref]);
    }
    if (do_hcal_sum_) {
      dest.set_hcal_sum((*hcal_sum_handle)[ref]);
    }
    if (do_conversion_matches_) {
      dest.set_has_matched_conversion((*conversion_matches_handle)[ref]);
    }

    for (unsigned v = 0; v < float_handles.size(); ++v) {
      dest.SetIdIso(input_vmaps_[v].first,
                    (*(float_handles[v]))[ref]);
    }

    if (do_cluster_iso_) {
      dest.set_ecal_pf_cluster_iso((*ecal_pf_cluster_iso)[ref]);
      dest.set_hcal_pf_cluster_iso((*hcal_pf_cluster_iso)[ref]);
    }

    if (do_pf_iso_03_) {
      dest.set_dr03_pfiso_charged_all((*charged_all_03)[ref]);
      dest.set_dr03_pfiso_charged((*charged_03)[ref]);
      dest.set_dr03_pfiso_neutral((*neutral_03)[ref]);
      dest.set_dr03_pfiso_gamma((*gamma_03)[ref]);
      dest.set_dr03_pfiso_pu((*pu_03)[ref]);
    }

    if (do_pf_iso_04_) {
      dest.set_dr04_pfiso_charged_all((*charged_all_04)[ref]);
      dest.set_dr04_pfiso_charged((*charged_04)[ref]);
      dest.set_dr04_pfiso_neutral((*neutral_04)[ref]);
      dest.set_dr04_pfiso_gamma((*gamma_04)[ref]);
      dest.set_dr04_pfiso_pu((*pu_04)[ref]);
    }

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
  ic::StaticTree::tree_->Branch(branch_.c_str(), &electrons_);
}

void ICElectronProducer::endJob() {
}

// define this as a plug-in
DEFINE_FWK_MODULE(ICElectronProducer);
