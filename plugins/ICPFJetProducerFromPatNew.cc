#include "UserCode/ICHiggsTauTau/plugins/ICPFJetProducerFromPatNew.hh"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Jet.hh"


ICPFJetProducerFromPatNew::ICPFJetProducerFromPatNew(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      inputSmear_(config.getParameter<edm::InputTag>("inputSmear")),
      inputSmearUp_(config.getParameter<edm::InputTag>("inputSmearUp")),
      inputSmearDown_(config.getParameter<edm::InputTag>("inputSmearDown")),
      branch_(config.getParameter<std::string>("branch")),
      doSmear_(config.getParameter<bool>("doSmear")),
      src_(config.getParameterSet("srcConfig"),consumesCollector()),
      dest_(config.getParameterSet("destConfig"),consumesCollector()) {
  consumes<edm::View<pat::Jet>>(input_);
  consumes<edm::View<pat::Jet>>(inputSmear_);
  consumes<edm::View<pat::Jet>>(inputSmearUp_);
  consumes<edm::View<pat::Jet>>(inputSmearDown_);
  jets_ = new std::vector<ic::PFJet>();
  smearjets_ = new std::vector<ic::PFJet>();
  smearupjets_ = new std::vector<ic::PFJet>();
  smeardownjets_ = new std::vector<ic::PFJet>();

  PrintHeaderWithProduces(config, input_, branch_);

  src_.DoSetup(this);
  dest_.DoSetup(this);
}

ICPFJetProducerFromPatNew::~ICPFJetProducerFromPatNew() { delete jets_; }

void ICPFJetProducerFromPatNew::constructSpecificWithSmear(
    edm::Handle<edm::View<pat::Jet> > const& jets_handle,
    edm::Handle<edm::View<pat::Jet> > const& smearjets_handle,
    edm::Handle<edm::View<pat::Jet> > const& smearupjets_handle,
    edm::Handle<edm::View<pat::Jet> > const& smeardownjets_handle,
    edm::Event& event, const edm::EventSetup& setup) {

  edm::Handle<edm::ValueMap<float> > pu_id_handle;
  if (dest_.do_pu_id) event.getByLabel(dest_.input_pu_id, pu_id_handle);

  /* edm::Handle<reco::TrackCollection> trk_handle; */
  /* edm::Handle<reco::VertexCollection> vtx_handle; */
  /* std::map<unsigned, unsigned> trk_vtx_map; */

  /* std::unique_ptr<reco::TrackRefVector> track_requests( */
  /*     new reco::TrackRefVector()); */

  /* if (dest_.do_trk_vars) { */
  /*   event.getByLabel(dest_.input_trks, trk_handle); */
  /*   event.getByLabel(dest_.input_vtxs, vtx_handle); */
  /*   trk_vtx_map = BuildTrkVtxMap(trk_handle, vtx_handle); */
  /* } */

  for (unsigned i = 0; i < passed_.size(); ++i) {
    pat::Jet const& src = jets_handle->at(passed_[i]);
    ic::PFJet & dest = jets_->at(i);
    FillCommonPFJet(&dest, src);
    if (doSmear_) {
      for (unsigned j = 0; j < smearjets_handle->size(); ++j) {
        if (src.eta() == smearjets_handle->at(j).eta()) {
          pat::Jet const& smeared = smearjets_handle->at(j);
          dest.set_jer_shift(smeared.pt() / src.pt());
          for (unsigned k = 0; k < smearupjets_handle->size(); ++k) {
            if (src.eta() == smearupjets_handle->at(k).eta()) {
              pat::Jet const& smearedup = smearupjets_handle->at(k);
              dest.set_jerup_shift(smearedup.pt() / src.pt());
              for (unsigned l = 0; l < smeardownjets_handle->size(); ++l) {
                if (src.eta() == smeardownjets_handle->at(l).eta()) {
                  pat::Jet const& smeareddown = smeardownjets_handle->at(l);
                  dest.set_jerdown_shift(smeareddown.pt() / src.pt());
                }
              }
            }
          }
        }
        else {
          dest.set_jer_shift(1.);
          dest.set_jerup_shift(1.);
          dest.set_jerdown_shift(1.);
        }
      }
    }
    else {
      dest.set_jer_shift(1.);
      dest.set_jerup_shift(1.);
      dest.set_jerdown_shift(1.);
    }
    dest.set_uncorrected_energy(
        (src.jecSetsAvailable() ? src.jecFactor(0) : 1.) * src.energy());
    if (dest_.do_pu_id) {
      if (src_.is_slimmed) {
        dest.set_pu_id_mva_value(src.userFloat(src_.slimmed_puid_label));
	//std::cout << " -- jet " << i << " pt " << dest.pt() << " pumva = " << dest.pu_id_mva_value() << std::endl;
      } else {
        dest.set_pu_id_mva_value(
            (*pu_id_handle)[jets_handle->refAt(passed_[i])->originalObjectRef()]);
      }
   }

    /* if (dest_.request_trks) { */
    /*   FillPFTracksAndRequest(&dest, src.getPFConstituents(), */
    /*                          track_requests.get(), &(dest_.track_hasher)); */
    /* } */

    /* if (dest_.do_trk_vars && trk_handle->size() > 0) { */
    /*   FillPFTrackVars(&dest, src.getPFConstituents(), trk_handle, vtx_handle, */
    /*                   trk_vtx_map); */
    /* } */
  } 
  /* if (dest_.request_trks) { */
    /* event.put(std::move(track_requests), "requestedTracks"); */
  /* } */
}

void ICPFJetProducerFromPatNew::produce(edm::Event& event,
                                 const edm::EventSetup& setup) {
  
  edm::Handle<edm::View<pat::Jet> > jets_handle;
  event.getByLabel(input_, jets_handle);

  edm::Handle<edm::View <pat::Jet>> smearjets_handle;
  event.getByLabel(inputSmear_, smearjets_handle);

  edm::Handle<edm::View <pat::Jet>> smearupjets_handle;
  event.getByLabel(inputSmearUp_, smearupjets_handle);

  edm::Handle<edm::View <pat::Jet>> smeardownjets_handle;
  event.getByLabel(inputSmearDown_, smeardownjets_handle);

  jets_->clear();
  passed_.clear();
  jets_->reserve(jets_handle->size());
  passed_.reserve(jets_handle->size());

  src_.Produce(jets_handle, jets_, &passed_, event, setup);

  constructSpecificWithSmear(jets_handle, smearjets_handle, 
          smearupjets_handle, smeardownjets_handle, event, setup);
}

void ICPFJetProducerFromPatNew::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &jets_);
}

void ICPFJetProducerFromPatNew::endJob() {
}


DEFINE_FWK_MODULE(ICPFJetProducerFromPatNew);
