#ifndef UserCode_ICHiggsTauTau_ICRun2JetProducer_h
#define UserCode_ICHiggsTauTau_ICRun2JetProducer_h

#include <memory>
#include <typeinfo>
#include "boost/functional/hash.hpp"
#include "boost/format.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Modules/interface/CorrectedJetProducer.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/CaloJet.hh"
#include "UserCode/ICHiggsTauTau/interface/JPTJet.hh"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"


template <class T>
class ICRun2JetProducer : public edm::EDProducer {
 public:
  explicit ICRun2JetProducer(const edm::ParameterSet &);
  ~ICRun2JetProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();
  void constructSpecific(edm::Handle<edm::View<T> > const& jets_handle,
                         edm::Event& event, const edm::EventSetup& setup);

  std::vector<ic::PFJet> *jets_;
  edm::InputTag input_;
  std::string branch_;

  std::vector<unsigned> passed_;
  edm::InputTag input_jet_flavour_; 
  bool include_jet_flavour_;
  bool apply_jec_factors_;
  bool include_jec_factors_;
  std::string cut_;
  std::string cut_string_;
  bool apply_post_jec_cut_;
  edm::InputTag input_sv_info_;
  bool include_sv_info_ids_;
  bool do_pu_id_;
  edm::InputTag input_pu_id_;
  bool do_trk_vars_;
  edm::InputTag input_trks_;
  edm::InputTag input_vtxs_;
  bool request_trks_;
  bool is_slimmed_;
  std::string slimmed_puid_label_;
  std::vector<std::pair<std::string, edm::InputTag> > input_btags_;
  boost::hash<reco::Track const*> track_hasher_;

  std::vector<std::pair<std::string, std::string> > jecs;
  boost::hash<T const*> jet_hasher;
  boost::hash<reco::Vertex const*> vertex_hasher;

  };
  template <class T>
  ICRun2JetProducer<T>::ICRun2JetProducer(const edm::ParameterSet& config)
      : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      input_jet_flavour_(config.getParameter<edm::InputTag>("inputJetFlavour")),
      include_jet_flavour_(config.getParameter<bool>("includeJetFlavour")),
      apply_jec_factors_(config.getParameter<bool>("applyJECs")),
      include_jec_factors_(config.getParameter<bool>("includeJECs")),
      cut_string_(config.getParameter<std::string>("cutAfterJECs")),
      apply_post_jec_cut_(config.getParameter<bool>("applyCutAfterJECs")),
      input_sv_info_(config.getParameter<edm::InputTag>("inputSVInfo")),
      include_sv_info_ids_(config.getParameter<bool>("requestSVInfo")),
      do_pu_id_(config.getParameter<bool>("includePileupID")),
      input_pu_id_(config.getParameter<edm::InputTag>("inputPileupID")),
      do_trk_vars_(config.getParameter<bool>("includeTrackBasedVars")),
      input_trks_(config.getParameter<edm::InputTag>("inputTracks")),
      input_vtxs_(config.getParameter<edm::InputTag>("inputVertices")),
      request_trks_(config.getParameter<bool>("requestTracks")){
        consumes<edm::View<T>>(input_);
        consumes<edm::ValueMap<float>>(input_pu_id_);
        consumes<reco::TrackCollection>(input_trks_);
        consumes<reco::TrackCollection>(input_vtxs_);
        consumes<edm::ValueMap<std::vector<int>>>(input_jet_flavour_);
        consumes<reco::SecondaryVertexTagInfoCollection>(input_sv_info_);
/*        if (apply_jec_factors_ || include_jec_factors_) {
          edm::ParameterSet pset = config.getParameter<edm::ParameterSet>("JECs");
          std::vector<std::string> vec =
              pset.getParameterNamesForType<std::string>();
          for (unsigned i = 0; i < vec.size(); ++i) {
            jecs.push_back(
                std::make_pair(vec[i], pset.getParameter<std::string>(vec[i])));
            collector.consumes<reco::JetCorrector>(jecs[i].second);
         }
       }
      edm::ParameterSet btag_pset =
          config.getParameter<edm::ParameterSet>("BTagDiscriminators");
      std::vector<std::string> btag_vec =
          btag_pset.getParameterNamesForType<edm::InputTag>();
      for (unsigned i = 0; i < btag_vec.size(); ++i) {
        input_btags_.push_back(std::make_pair(
            btag_vec[i], btag_pset.getParameter<edm::InputTag>(btag_vec[i])));
        consumes<reco::JetFloatAssociation::Container>(input_btags_[i].second);
      }*/

        jets_ = new std::vector<ic::PFJet>();

    PrintHeaderWithProduces(config, input_, branch_);
    if (include_sv_info_ids_) {
      produces<reco::SecondaryVertexTagInfoRefVector>("requestedSVInfo");
    }
    if (request_trks_) {
      produces<reco::TrackRefVector>("requestedTracks");
    }
    PrintOptional(1, include_jet_flavour_, "includeJetFlavour");
    PrintOptional(1, apply_jec_factors_, "applyJECs");
    PrintOptional(1, include_jec_factors_, "includeJECs");
    PrintOptional(1, apply_post_jec_cut_, "applyCutAfterJECs");
    PrintOptional(1, include_sv_info_ids_, "requestSVInfo");
    PrintOptional(1, do_pu_id_, "includePileupID");
    PrintOptional(1, request_trks_, "requestTracks");
    PrintOptional(1, do_trk_vars_, "includeTrackBasedVars");

}

  template <>
  ICRun2JetProducer<pat::Jet>::ICRun2JetProducer(const edm::ParameterSet& config)
      : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      input_jet_flavour_(config.getParameter<edm::InputTag>("inputJetFlavour")),
      include_jet_flavour_(config.getParameter<bool>("includeJetFlavour")),
      include_jec_factors_(config.getParameter<bool>("includeJECs")),
      input_sv_info_(config.getParameter<edm::InputTag>("inputSVInfo")),
      include_sv_info_ids_(config.getParameter<bool>("requestSVInfo")),
      do_pu_id_(config.getParameter<bool>("includePileupID")),
      input_pu_id_(config.getParameter<edm::InputTag>("inputPileupID")),
      do_trk_vars_(config.getParameter<bool>("includeTrackBasedVars")),
      input_trks_(config.getParameter<edm::InputTag>("inputTracks")),
      input_vtxs_(config.getParameter<edm::InputTag>("inputVertices")),
      request_trks_(config.getParameter<bool>("requestTracks")),
      is_slimmed_(config.getParameter<bool>("isSlimmed")),
      slimmed_puid_label_(config.getParameter<std::string>("slimmedPileupIDLabel")){
        consumes<edm::View<pat::Jet>>(input_);
        consumes<edm::ValueMap<float>>(input_pu_id_);
        consumes<reco::TrackCollection>(input_trks_);
        consumes<reco::TrackCollection>(input_vtxs_);
        consumes<edm::ValueMap<std::vector<int>>>(input_jet_flavour_);
        consumes<reco::SecondaryVertexTagInfoCollection>(input_sv_info_);
        if (apply_jec_factors_ || include_jec_factors_) {
          edm::ParameterSet pset = config.getParameter<edm::ParameterSet>("JECs");
          std::vector<std::string> vec =
              pset.getParameterNamesForType<std::string>();
          for (unsigned i = 0; i < vec.size(); ++i) {
            jecs.push_back(
                std::make_pair(vec[i], pset.getParameter<std::string>(vec[i])));
              consumes<reco::JetCorrector>(jecs[i].second);
         }
       }
      edm::ParameterSet btag_pset =
          config.getParameter<edm::ParameterSet>("BTagDiscriminators");
      std::vector<std::string> btag_vec =
          btag_pset.getParameterNamesForType<edm::InputTag>();
      for (unsigned i = 0; i < btag_vec.size(); ++i) {
        input_btags_.push_back(std::make_pair(
            btag_vec[i], btag_pset.getParameter<edm::InputTag>(btag_vec[i])));
        consumes<reco::JetFloatAssociation::Container>(input_btags_[i].second);
      }

        jets_ = new std::vector<ic::PFJet>();

    PrintHeaderWithProduces(config, input_, branch_);
    if (include_sv_info_ids_) {
      produces<reco::SecondaryVertexTagInfoRefVector>("requestedSVInfo");
    }
    if (request_trks_) {
      produces<reco::TrackRefVector>("requestedTracks");
    }
    PrintOptional(1, include_jet_flavour_, "includeJetFlavour");
    PrintOptional(1, apply_jec_factors_, "applyJECs");
    PrintOptional(1, include_jec_factors_, "includeJECs");
    PrintOptional(1, apply_post_jec_cut_, "applyCutAfterJECs");
    PrintOptional(1, include_sv_info_ids_, "requestSVInfo");
    PrintOptional(1, do_pu_id_, "includePileupID");
    PrintOptional(1, request_trks_, "requestTracks");
    PrintOptional(1, do_trk_vars_, "includeTrackBasedVars");

}

template <class T>
ICRun2JetProducer<T>::~ICRun2JetProducer() {
  delete jets_;
}

template <class T>
void ICRun2JetProducer<T>::produce(edm::Event& event, const edm::EventSetup& setup) {
  edm::Handle<edm::View<T> > jets_handle;
  event.getByLabel(input_, jets_handle);
  jets_->clear();
  jets_->reserve(jets_handle->size());
  for (unsigned i = 0; i < jets_handle->size(); ++i) {
    T const& src = jets_handle->at(i);
    ic::PFJet & dest = jets_->at(i);
    dest.set_muon_multiplicity(src.muonMultiplicity());
    dest.set_charged_multiplicity(src.chargedMultiplicity());
    dest.set_charged_had_multiplicity(src.chargedHadronMultiplicity());
    dest.set_neutral_had_multiplicity(src.neutralHadronMultiplicity());
    dest.set_photon_multiplicity(src.photonMultiplicity());
    dest.set_electron_multiplicity(src.electronMultiplicity());
    dest.set_HF_had_multiplicity(src.HFHadronMultiplicity());
    dest.set_HF_em_multiplicity(src.HFEMMultiplicity());
    dest.set_neutral_multiplicity(src.neutralMultiplicity());
    dest.set_charged_em_energy(src.chargedEmEnergy());
    dest.set_neutral_em_energy(src.neutralEmEnergy());
    dest.set_charged_had_energy(src.chargedHadronEnergy());
    dest.set_neutral_had_energy(src.neutralHadronEnergy());
    dest.set_photon_energy(src.photonEnergy());
    dest.set_electron_energy(src.electronEnergy());
    dest.set_muon_energy(src.muonEnergy());
    dest.set_HF_had_energy(src.HFHadronEnergy());
    dest.set_HF_em_energy(src.HFEMEnergy());
    dest.set_charged_mu_energy(src.chargedMuEnergy());
  }
  constructSpecific(jets_handle,event, setup);

}


//Specific stuff:
template <class T>
void ICRun2JetProducer<T>::constructSpecific(
  edm::Handle<edm::View<T> > const& jets_handle, edm::Event& event,
  const edm::EventSetup& setup) {}

template <>
void ICRun2JetProducer<reco::PFJet>::constructSpecific(
  edm::Handle<edm::View<reco::PFJet> > const& jets_handle, edm::Event& event,
  const edm::EventSetup& setup) {
  }



template <>
void ICRun2JetProducer<pat::Jet>::constructSpecific(
  edm::Handle<edm::View<pat::Jet> > const& jets_handle, edm::Event& event,
  const edm::EventSetup& setup) {
  edm::Handle<edm::ValueMap<float> > pu_id_handle;
  if (do_pu_id_) event.getByLabel(input_pu_id_, pu_id_handle);
  edm::Handle<reco::TrackCollection> trk_handle;
  edm::Handle<reco::VertexCollection> vtx_handle;
  std::map<unsigned, unsigned> trk_vtx_map;

  std::auto_ptr<reco::TrackRefVector> track_requests(
      new reco::TrackRefVector());

  if (do_trk_vars_) {
    event.getByLabel(input_trks_, trk_handle);
    event.getByLabel(input_vtxs_, vtx_handle);
//    trk_vtx_map = BuildTrkVtxMap(trk_handle, vtx_handle);
  }
    edm::Handle<reco::SecondaryVertexTagInfoCollection> sv_info_handle;
    std::auto_ptr<reco::SecondaryVertexTagInfoRefVector> sv_info_requests(
        new reco::SecondaryVertexTagInfoRefVector());
  if(include_sv_info_ids_){
    event.getByLabel(input_sv_info_, sv_info_handle);
  }


  for (unsigned i = 0; i < jets_handle->size(); ++i) {
    pat::Jet const& src = jets_handle->at(i);
    ic::PFJet & dest = jets_->at(i);
    dest.set_uncorrected_energy(
        (src.jecSetsAvailable() ? src.jecFactor(0) : 1.) * src.energy());
    if (do_pu_id_) {
      if (is_slimmed_) {
        dest.set_pu_id_mva_value(src.userFloat(slimmed_puid_label_));
	//std::cout << " -- jet " << i << " pt " << dest.pt() << " pumva = " << dest.pu_id_mva_value() << std::endl;
      } else {
        dest.set_pu_id_mva_value(
            (*pu_id_handle)[jets_handle->refAt(passed_[i])->originalObjectRef()]);
      }
   }

/*    if (request_trks_) {
      FillPFTracksAndRequest(&dest, src.getPFConstituents(),
                             track_requests.get(), &(track_hasher_));
    }

    if (do_trk_vars_ && trk_handle->size() > 0) {
      FillPFTrackVars(&dest, src.getPFConstituents(), trk_handle, vtx_handle,
                      trk_vtx_map);
    }*/
    dest.set_id(jet_hasher(&src));
    dest.set_pt(src.pt());
    dest.set_eta(src.eta());
    dest.set_phi(src.phi());
    dest.set_energy(src.energy());
    dest.set_charge(src.charge());
    dest.set_jet_area(src.jetArea());
    if (include_jet_flavour_) {
      dest.set_parton_flavour(src.partonFlavour());
      dest.set_hadron_flavour(src.hadronFlavour());
    }
    // Only write correction into the output jet if the user wants it
    if (include_jec_factors_ && src.availableJECLevels().size() >= 1) {
      std::vector<std::string> jec_levels = src.availableJECLevels();
      for (unsigned j = 1; j < jec_levels.size(); ++j) {
        dest.SetJecFactor(
            jec_levels[j],
            src.jecFactor(jec_levels[j]) / src.jecFactor(jec_levels[j - 1]));
      }
     }

    if (include_sv_info_ids_) {
      for (unsigned iS = 0; iS < sv_info_handle->size(); ++iS) {
        if (&(*(src.originalObjectRef())) ==
            (*sv_info_handle)[iS].jet().get()) {
            reco::SecondaryVertexTagInfoRef ref(sv_info_handle, iS);
          sv_info_requests->push_back(ref);
          std::vector<std::size_t> jet_sv_ids;
          for (unsigned iV = 0; iV < (*sv_info_handle)[iS].nVertices();
               ++iV) {
            reco::Vertex const &sv =
                (*sv_info_handle)[iS].secondaryVertex(iV);
            jet_sv_ids.push_back(vertex_hasher(&sv));
          }
          dest.set_secondary_vertices(jet_sv_ids);
        }
      }
    }

    for (unsigned iB = 0; iB < src.getPairDiscri().size(); ++iB) {
      dest.SetBDiscriminator(src.getPairDiscri().at(iB).first,
                             src.getPairDiscri().at(iB).second);
    }
  }
  if (request_trks_) {
    event.put(track_requests, "requestedTracks");
  }
    if (include_sv_info_ids_) {
      event.put(sv_info_requests, "requestedSVInfo");
    }


}


template <class T>
void ICRun2JetProducer<T>::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &jets_);
}

template <class T>
void ICRun2JetProducer<T>::endJob() {
}


#endif
