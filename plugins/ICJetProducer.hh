#ifndef UserCode_ICHiggsTauTau_ICJetProducer_h
#define UserCode_ICHiggsTauTau_ICJetProducer_h

#include <memory>
#include "boost/functional/hash.hpp"
#include "boost/format.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/JPTJet.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "UserCode/ICHiggsTauTau/plugins/ICJetProducerConfig.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/CaloJet.hh"
#include "UserCode/ICHiggsTauTau/interface/JPTJet.hh"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"


/*
Checklist:
[ ] Should consider sorting ic::Jet vector at the end if we've done JEC
    --> but no guarantee in any producer of pT ordering
[ ] JPT Jet specific construction
[ ] Calculation of extra PFJet variables: beta, beta_max, linear_radial_moment, charged_multiplicity_no_pu
[ ] Calculation of JPTJet extra variables: beta, beta_max, track_pt_total
*/

template <class T, class U>
class ICJetProducer : public edm::EDProducer {
 public:
  explicit ICJetProducer(const edm::ParameterSet &);
  ~ICJetProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();
  void constructSpecific(edm::Handle<edm::View<U> > const& jets_handle,
                         edm::Event& event, const edm::EventSetup& setup);

  std::vector<T> *jets_;
  edm::InputTag input_;
  std::string branch_;
  boost::hash<U const*> jet_hasher_;
  boost::hash<reco::Vertex const*> vertex_hasher_;

  edm::InputTag input_jet_flavour_;
  bool include_jet_flavour_;

  std::vector<std::pair<std::string, std::string> > jecs_;
  bool apply_jec_factors_;
  bool include_jec_factors_;

  StringCutObjectSelector<U> cut_;
  std::string cut_string_;
  bool apply_post_jec_cut_;

  edm::InputTag input_sv_info_;
  bool include_sv_info_ids_;

  std::vector<std::pair<std::string, edm::InputTag> > input_btags_;


  std::vector<unsigned> passed_;

  SpecificConfig<U> cfg_;
};

// =============================
// Template class implementation
// =============================
template <class T, class U>
ICJetProducer<T, U>::ICJetProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      input_jet_flavour_(config.getParameter<edm::InputTag>("inputJetFlavour")),
      include_jet_flavour_(config.getParameter<bool>("includeJetFlavour")),
      apply_jec_factors_(config.getParameter<bool>("applyJECs")),
      include_jec_factors_(config.getParameter<bool>("includeJECs")),
      cut_(config.getParameter<std::string>("cutAfterJECs")),
      cut_string_(config.getParameter<std::string>("cutAfterJECs")),
      apply_post_jec_cut_(config.getParameter<bool>("applyCutAfterJECs")),
      input_sv_info_(config.getParameter<edm::InputTag>("inputSVInfo")),
      include_sv_info_ids_(config.getParameter<bool>("requestSVInfo")),
      cfg_(config.getParameterSet("specificConfig")) {
  jets_ = new std::vector<T>();

  // std::string line(79, '-');
  // std::cout << "ICJetProducer" << "\n";
  // std::cout << line << "\n";
  // std::cout << boost::format("%-20s [%c] %-5s Input: %-30s\n")
  //     % "Include jet flavour"
  //     % (include_jet_flavour_ ? 'x' : ' ') % " "
  //     % input_jet_flavour_.encode();
  // std::cout << boost::format("%-20s [%c]\n")
  //     % "Apply JECs"
  //     % (apply_jec_factors_ ? 'x' : ' ');
  // std::cout << boost::format("%-20s [%c]\n")
  //     % "Include JECs"
  //     % (include_jec_factors_ ? 'x' : ' ');
  // std::cout << boost::format("%-20s [%c] %-5s Cut: '%-30s'\n")
  //     % "applyCutAfterJECs"
  //     % (apply_post_jec_cut_ ? 'x' : ' ') % " "
  //     % cut_string_;

  if (apply_jec_factors_ || include_jec_factors_) {
    edm::ParameterSet pset = config.getParameter<edm::ParameterSet>("JECs");
    std::vector<std::string> vec = pset.getParameterNamesForType<std::string>();
    for (unsigned i = 0; i < vec.size(); ++i) {
      jecs_.push_back(
          std::make_pair(vec[i], pset.getParameter<std::string>(vec[i])));
      // std::cout << boost::format("%-20s %-15s %-7.7s\n") % vec[i] % " " %
      //                  CityHash64(vec[i]);
    }
  }

  edm::ParameterSet btag_pset =
      config.getParameter<edm::ParameterSet>("BTagDiscriminators");
  std::vector<std::string> btag_vec =
      btag_pset.getParameterNamesForType<edm::InputTag>();
  for (unsigned i = 0; i < btag_vec.size(); ++i) {
    input_btags_.push_back(std::make_pair(
        btag_vec[i], btag_pset.getParameter<edm::InputTag>(btag_vec[i])));
  }

  if (include_sv_info_ids_) {
    produces<reco::SecondaryVertexTagInfoRefVector>("requestedSVInfo");
  }

  cfg_.DoSetup(this);
}

template <class T, class U>
ICJetProducer<T, U>::~ICJetProducer() { delete jets_; }

// =============
// Main producer
// =============
template <class T, class U>
void ICJetProducer<T, U>::produce(edm::Event& event,
                                 const edm::EventSetup& setup) {
  edm::Handle<edm::View<U> > jets_handle;
  event.getByLabel(input_, jets_handle);

  edm::Handle<edm::ValueMap<int> > jet_flavour_handle;
  if (include_jet_flavour_)
    event.getByLabel(input_jet_flavour_, jet_flavour_handle);

  std::vector<JetCorrector const*> correctors(jecs_.size(), NULL);
  for (unsigned i = 0; i < correctors.size(); ++i) {
    correctors[i] = JetCorrector::getJetCorrector(jecs_[i].second, setup);
  }

  edm::Handle<reco::SecondaryVertexTagInfoCollection> sv_info_handle;
  if (include_sv_info_ids_) {
    event.getByLabel(input_sv_info_, sv_info_handle);
  }

  std::vector<edm::Handle<reco::JetFloatAssociation::Container> > btag_handles(
      input_btags_.size());
  for (unsigned i = 0; i < btag_handles.size(); ++i) {
    event.getByLabel(input_btags_[i].second, btag_handles[i]);
  }

  std::auto_ptr<reco::SecondaryVertexTagInfoRefVector> sv_info_requests(
      new reco::SecondaryVertexTagInfoRefVector());

  jets_->clear();
  passed_.clear();
  jets_->reserve(jets_handle->size());
  passed_.reserve(jets_handle->size());

  for (unsigned i = 0; i < jets_handle->size(); ++i) {
    U const& src = jets_handle->at(i);
    double full_jec_factor = 1.0;
    std::vector<float> factors(correctors.size(), 0.);
    bool keep_jet = true;
    // Are there jet energy corrections to apply?
    if (correctors.size() > 0) {
      // Yes - make a copy of the source Jet
      U jet_cpy = jets_handle->at(i);
      // Loop through each correction and apply
      for (unsigned j = 0; j < correctors.size(); ++j) {
        double factor = correctors[j]->correction(jet_cpy, event, setup);
        if (apply_jec_factors_) full_jec_factor *= factor;
        factors[j] = factor;
        jet_cpy.scaleEnergy(factor);
      }
      if (apply_post_jec_cut_ && !cut_(jet_cpy)) keep_jet = false;
    }

    if (keep_jet) {
      passed_.push_back(i);
    } else {
      continue;
    }

    jets_->push_back(T());
    T & dest = jets_->back();

    dest.set_id(jet_hasher_(&src));
    dest.set_pt(src.pt() * full_jec_factor);
    dest.set_eta(src.eta());
    dest.set_phi(src.phi());
    dest.set_energy(src.energy() * full_jec_factor);
    dest.set_charge(src.charge());
    dest.set_jet_area(src.jetArea());
    if (include_jet_flavour_) {
      dest.set_parton_flavour((*jet_flavour_handle)[jets_handle->refAt(i)]);
    }
    // Only write correction into the output jet if the user wants it
    if (include_jec_factors_) {
      for (unsigned j = 0; j < correctors.size(); ++j) {
        dest.SetJecFactor(jecs_[j].first, factors[j]);
      }
    }

    if (include_sv_info_ids_) {
      for (unsigned iS = 0; iS < sv_info_handle->size(); ++iS) {
        if (&src == (*sv_info_handle)[iS].jet().get()) {
          reco::SecondaryVertexTagInfoRef ref(sv_info_handle, iS);
          sv_info_requests->push_back(ref);
          std::vector<std::size_t> jet_sv_ids;
          for (unsigned iV = 0; iV < (*sv_info_handle)[iS].nVertices(); ++iV) {
            reco::Vertex const& sv = (*sv_info_handle)[iS].secondaryVertex(iV);
            jet_sv_ids.push_back(vertex_hasher_(&sv));
          }
          dest.set_secondary_vertices(jet_sv_ids);
        }
      }
    }

    // edm::RefToBase<reco::Jet> xyz(jets_handle->refAt(i));
    // std::cout << xyz.isNonnull()<< "\n";

    for (unsigned iB = 0; iB < btag_handles.size(); ++iB) {
      dest.SetBDiscriminator(input_btags_[iB].first,
                             (*btag_handles[iB])[edm::RefToBase<reco::Jet>(
                                 jets_handle->refAt(i))]);
    }
  }
  constructSpecific(jets_handle, event, setup);
  if (include_sv_info_ids_) {
    event.put(sv_info_requests, "requestedSVInfo");
  }
}

// ==================
// Specific producers
// ==================
template <class T, class U>
void ICJetProducer<T, U>::constructSpecific(
    edm::Handle<edm::View<U> > const& jets_handle, edm::Event& event,
    const edm::EventSetup& setup) {}

template <>
void ICJetProducer<ic::CaloJet, reco::CaloJet>::constructSpecific(
    edm::Handle<edm::View<reco::CaloJet> > const& jets_handle,
    edm::Event& event, const edm::EventSetup& setup) {
  edm::Handle<reco::JetIDValueMap> jet_id_handle;
  if (cfg_.do_jet_id) event.getByLabel(cfg_.input_jet_id, jet_id_handle);
  for (unsigned i = 0; i < passed_.size(); ++i) {
    reco::CaloJet const& src = jets_handle->at(passed_[i]);
    ic::CaloJet & dest = jets_->at(i);
    dest.set_max_em_tower_energy(src.maxEInEmTowers());
    dest.set_max_had_tower_energy(src.maxEInHadTowers());
    dest.set_energy_frac_had(src.energyFractionHadronic());
    dest.set_energy_frac_em(src.emEnergyFraction());
    dest.set_had_energy_HB(src.hadEnergyInHB());
    dest.set_had_energy_HO(src.hadEnergyInHO());
    dest.set_had_energy_HE(src.hadEnergyInHE());
    dest.set_had_energy_HF(src.hadEnergyInHF());
    dest.set_em_energy_EB(src.emEnergyInEB());
    dest.set_em_energy_EE(src.emEnergyInEE());
    dest.set_em_energy_HF(src.emEnergyInHF());
    dest.set_towers_area(src.towersArea());
    if (cfg_.do_n_carrying) {
      dest.set_n90(src.n90());
      dest.set_n60(src.n60());
    }
    //  Assume input jet is uncorrected
    dest.set_uncorrected_energy(src.energy());
    if (cfg_.do_jet_id) {
      reco::JetID const& jet_id =
          (*jet_id_handle)[jets_handle->refAt(passed_[i])];
      dest.set_fHPD(jet_id.fHPD);
      dest.set_fRBX(jet_id.fRBX);
      dest.set_id_loose((*(cfg_.loose_id))(src, jet_id));
      dest.set_id_tight((*(cfg_.tight_id))(src, jet_id));
    }
  }
}

template <>
void ICJetProducer<ic::JPTJet, reco::JPTJet>::constructSpecific(
    edm::Handle<edm::View<reco::JPTJet> > const& jets_handle,
    edm::Event& event, const edm::EventSetup& setup) {
  edm::Handle<reco::JetIDValueMap> jet_id_handle;
  if (cfg_.do_jet_id) event.getByLabel(cfg_.input_jet_id, jet_id_handle);

  edm::Handle<reco::TrackCollection> trk_handle;
  edm::Handle<reco::VertexCollection> vtx_handle;
  std::map<unsigned, unsigned> trk_vtx_map;

  std::auto_ptr<reco::TrackRefVector> track_requests(
      new reco::TrackRefVector());

  if (cfg_.do_trk_vars) {
    event.getByLabel(cfg_.input_trks, trk_handle);
    event.getByLabel(cfg_.input_vtxs, vtx_handle);
    if (trk_handle->size() > 0) {
      reco::Track const* ptr_to_first = &(trk_handle->at(0));
      for (unsigned i = 0; i < vtx_handle->size(); ++i) {
        std::vector<reco::TrackBaseRef>::const_iterator trk_it;
        for (trk_it = vtx_handle->at(i).tracks_begin();
             trk_it != vtx_handle->at(i).tracks_end(); ++trk_it) {
          reco::Track const* ptr_to_trk =
              dynamic_cast<reco::Track const*>(&(**trk_it));
          unsigned trk_idx = (unsigned(ptr_to_trk - ptr_to_first));
          trk_vtx_map[trk_idx] = i;
        }
      }
    }
  }

  for (unsigned i = 0; i < passed_.size(); ++i) {
    reco::JPTJet const& src = jets_handle->at(passed_[i]);

    reco::CaloJetRef calo(src.getCaloJetRef().castTo<reco::CaloJetRef>());
    ic::JPTJet & dest = jets_->at(i);
    dest.set_max_em_tower_energy(calo->maxEInEmTowers());
    dest.set_max_had_tower_energy(calo->maxEInHadTowers());
    dest.set_energy_frac_had(calo->energyFractionHadronic());
    dest.set_energy_frac_em(calo->emEnergyFraction());
    dest.set_had_energy_HB(calo->hadEnergyInHB());
    dest.set_had_energy_HO(calo->hadEnergyInHO());
    dest.set_had_energy_HE(calo->hadEnergyInHE());
    dest.set_had_energy_HF(calo->hadEnergyInHF());
    dest.set_em_energy_EB(calo->emEnergyInEB());
    dest.set_em_energy_EE(calo->emEnergyInEE());
    dest.set_em_energy_HF(calo->emEnergyInHF());
    dest.set_towers_area(calo->towersArea());
    if (cfg_.do_n_carrying) {
      dest.set_n90(calo->n90());
      dest.set_n60(calo->n60());
    }
    dest.set_muon_multiplicity(src.muonMultiplicity());
    dest.set_charged_multiplicity(src.chargedMultiplicity());
    dest.set_charged_em_energy(src.chargedEmEnergy());
    dest.set_neutral_em_energy(src.neutralEmEnergy());
    dest.set_charged_had_energy(src.chargedHadronEnergy());
    dest.set_neutral_had_energy(src.neutralHadronEnergy());
    //  Assume input jet is uncorrected
    dest.set_uncorrected_energy(src.energy());
    if (cfg_.do_jet_id) {
      reco::JetID const& jet_id = (*jet_id_handle)[calo];
      dest.set_id_loose((*(cfg_.loose_id))(*calo, jet_id));
      dest.set_id_tight((*(cfg_.tight_id))(*calo, jet_id));
    }

    if (cfg_.request_trks) {
      dest.set_pions_in_vtx_in_calo(cfg_.DoTrackCollection(
          src.getPionsInVertexInCalo(), track_requests.get()));
      dest.set_pions_in_vtx_out_calo(cfg_.DoTrackCollection(
          src.getPionsInVertexOutCalo(), track_requests.get()));
      dest.set_pions_out_vtx_in_calo(cfg_.DoTrackCollection(
          src.getPionsOutVertexInCalo(), track_requests.get()));
      dest.set_muons_in_vtx_in_calo(cfg_.DoTrackCollection(
          src.getMuonsInVertexInCalo(), track_requests.get()));
      dest.set_muons_in_vtx_out_calo(cfg_.DoTrackCollection(
          src.getMuonsInVertexOutCalo(), track_requests.get()));
      dest.set_muons_out_vtx_in_calo(cfg_.DoTrackCollection(
          src.getMuonsOutVertexInCalo(), track_requests.get()));
      dest.set_elecs_in_vtx_in_calo(cfg_.DoTrackCollection(
          src.getElecsInVertexInCalo(), track_requests.get()));
      dest.set_elecs_in_vtx_out_calo(cfg_.DoTrackCollection(
          src.getElecsInVertexOutCalo(), track_requests.get()));
      dest.set_elecs_out_vtx_in_calo(cfg_.DoTrackCollection(
          src.getElecsOutVertexInCalo(), track_requests.get()));
    }

    double beta = -1.0;
    double beta_max = -1.0;
    double trk_pt_total = 0.0;

    if (cfg_.do_trk_vars && trk_handle->size() > 0) {
      std::vector<reco::Track const*> all_tracks;
      cfg_.AddTrackCollection(src.getPionsInVertexInCalo(), &all_tracks);
      cfg_.AddTrackCollection(src.getPionsInVertexOutCalo(), &all_tracks);
      cfg_.AddTrackCollection(src.getPionsOutVertexInCalo(), &all_tracks);
      cfg_.AddTrackCollection(src.getMuonsInVertexInCalo(), &all_tracks);
      cfg_.AddTrackCollection(src.getMuonsInVertexOutCalo(), &all_tracks);
      cfg_.AddTrackCollection(src.getMuonsOutVertexInCalo(), &all_tracks);
      cfg_.AddTrackCollection(src.getElecsInVertexInCalo(), &all_tracks);
      cfg_.AddTrackCollection(src.getElecsInVertexOutCalo(), &all_tracks);
      cfg_.AddTrackCollection(src.getElecsOutVertexInCalo(), &all_tracks);
      std::vector<double> pt_at_vtx_vec(vtx_handle->size(), 0.0);
      reco::Track const* ptr_first = &(trk_handle->at(0));
      for (unsigned j = 0; j < all_tracks.size(); ++j) {
          unsigned idx = unsigned(all_tracks[j] - ptr_first);
          trk_pt_total += all_tracks[j]->pt();
          // Is track associated to a vertex?
          if (trk_vtx_map.count(idx) > 0) {
            pt_at_vtx_vec[trk_vtx_map.find(idx)->second] += all_tracks[j]->pt();
          } else {  // No, so is it within 0.2 cm wrt the closest vertex in z
            std::vector<double> dz_with_vtx(vtx_handle->size(), 0.0);
            for (unsigned k = 0; k < vtx_handle->size(); ++k) {
              dz_with_vtx[k] =
                  fabs(vtx_handle->at(k).z() - all_tracks[j]->vz());
            }
            std::vector<double>::const_iterator min =
                std::min_element(dz_with_vtx.begin(), dz_with_vtx.end());
            if (min != dz_with_vtx.end()) {
              if (*min < 0.2) {
                pt_at_vtx_vec[unsigned(min - dz_with_vtx.begin())] +=
                    all_tracks[j]->pt();
              }
            }
          }
      }
      if (vtx_handle->size() > 0 && trk_pt_total > 0.0) {
        for (unsigned j = 0; j < pt_at_vtx_vec.size(); ++j) {
          pt_at_vtx_vec[j] = pt_at_vtx_vec[j] / trk_pt_total;
        }
        beta = pt_at_vtx_vec[0];
        beta_max =
            *std::max_element(pt_at_vtx_vec.begin(), pt_at_vtx_vec.end());
      }
    }

    dest.set_beta(beta);
    dest.set_beta_max(beta_max);
    dest.set_track_pt_total(trk_pt_total);
  }
  if (cfg_.request_trks) {
    event.put(track_requests, "requestedTracks");
  }
}

template <>
void ICJetProducer<ic::PFJet, reco::PFJet>::constructSpecific(
    edm::Handle<edm::View<reco::PFJet> > const& jets_handle,
    edm::Event& event, const edm::EventSetup& setup) {
  edm::Handle<edm::ValueMap<float> > pu_id_handle;
  if (cfg_.do_pu_id) event.getByLabel(cfg_.input_pu_id, pu_id_handle);

  edm::Handle<reco::TrackCollection> trk_handle;
  edm::Handle<reco::VertexCollection> vtx_handle;
  std::map<unsigned, unsigned> trk_vtx_map;

  std::auto_ptr<reco::TrackRefVector> track_requests(
      new reco::TrackRefVector());

  if (cfg_.do_trk_vars) {
    event.getByLabel(cfg_.input_trks, trk_handle);
    event.getByLabel(cfg_.input_vtxs, vtx_handle);
    if (trk_handle->size() > 0) {
      reco::Track const* ptr_to_first = &(trk_handle->at(0));
      for (unsigned i = 0; i < vtx_handle->size(); ++i) {
        std::vector<reco::TrackBaseRef>::const_iterator trk_it;
        for (trk_it = vtx_handle->at(i).tracks_begin();
             trk_it != vtx_handle->at(i).tracks_end(); ++trk_it) {
          reco::Track const* ptr_to_trk =
              dynamic_cast<reco::Track const*>(&(**trk_it));
          unsigned trk_idx = (unsigned(ptr_to_trk - ptr_to_first));
          trk_vtx_map[trk_idx] = i;
        }
      }
    }
  }

  for (unsigned i = 0; i < passed_.size(); ++i) {
    reco::PFJet const& src = jets_handle->at(passed_[i]);
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
    //  Assume input jet is uncorrected
    dest.set_uncorrected_energy(src.energy());
    if (cfg_.do_pu_id) {
      dest.set_pu_id_mva_value((*pu_id_handle)[jets_handle->refAt(passed_[i])]);
    }

    if (cfg_.request_trks) {
      std::vector<std::size_t> track_ids;
      std::vector<reco::PFCandidatePtr> const& pfcands =
          src.getPFConstituents();
      for (unsigned iP = 0; iP < pfcands.size(); ++iP) {
        if (pfcands[iP]->trackRef().isNonnull()) {
          track_requests->push_back(pfcands[iP]->trackRef());
          track_ids.push_back(
                    cfg_.track_hasher(&(*(pfcands[iP]->trackRef()))));
        }
      }
      dest.set_constituent_tracks(track_ids);
    }

    if (cfg_.do_trk_vars && trk_handle->size() > 0) {
      int charged_multiplicity_nopu = 0;
      float linear_radial_moment_nopu = 0.0;
      double trk_pt_total = 0.0;
      std::vector<double> pt_at_vtx_vec(vtx_handle->size(), 0.0);
      reco::Track const* ptr_first = &(trk_handle->at(0));
      std::vector<reco::PFCandidatePtr> const& pfcands =
          src.getPFConstituents();
      for (unsigned iP = 0; iP < pfcands.size(); ++iP) {
        if (pfcands[iP]->trackRef().isNonnull()) {
          unsigned idx = unsigned(&(*(pfcands[iP]->trackRef())) - ptr_first);
          trk_pt_total += pfcands[iP]->trackRef()->pt();
          // Is track associated to a vertex?
          if (trk_vtx_map.count(idx) > 0) {
            // If this track is mapped to the first PV, increase number of nopu
            // charged multiplicity
            if (trk_vtx_map.find(idx)->second == 0) {
              ++charged_multiplicity_nopu;
              linear_radial_moment_nopu +=
                  pfcands[iP]->pt() * deltaR(src.rapidity(), src.phi(),
                                             pfcands[iP]->rapidity(),
                                             pfcands[iP]->phi());
            }
            pt_at_vtx_vec[trk_vtx_map.find(idx)->second] +=
                pfcands[iP]->trackRef()->pt();
          } else {  // No, so is it within 0.2 cm wrt the closest vertex in z
            std::vector<double> dz_with_vtx(vtx_handle->size(), 0.0);
            for (unsigned iV = 0; iV < vtx_handle->size(); ++iV) {
              dz_with_vtx[iV] =
                  fabs(vtx_handle->at(iV).z() - pfcands[iP]->trackRef()->vz());
            }
            std::vector<double>::const_iterator min =
                std::min_element(dz_with_vtx.begin(), dz_with_vtx.end());
            if (min != dz_with_vtx.end()) {
              if (*min < 0.2) {
                // As above, if this track is not associated to any vertex, but
                // is within 0.2 cm of first PV, then increase multiplicity
                // count
                if (unsigned(min-dz_with_vtx.begin()) == 0) {
                  ++charged_multiplicity_nopu;
                  linear_radial_moment_nopu +=
                      pfcands[iP]->pt() * deltaR(src.rapidity(), src.phi(),
                                                 pfcands[iP]->rapidity(),
                                                 pfcands[iP]->phi());
                }
                pt_at_vtx_vec[unsigned(min - dz_with_vtx.begin())] +=
                    pfcands[iP]->trackRef()->pt();
              }
            }
          }
          // if (store_ids_) {
            // jet_tracks->push_back(idx);
            // trks.push_back(track_hasher(&(*(pfcands[iP]->trackRef()))));
          // }
        } else {  // No track found, assume PF candidate is not PU
          linear_radial_moment_nopu +=
              pfcands[iP]->pt() * deltaR(src.rapidity(), src.phi(),
                                         pfcands[iP]->rapidity(),
                                         pfcands[iP]->phi());
        }
      }
      // IMPORTANT: taking the dest pt() here not the src()!
      linear_radial_moment_nopu = linear_radial_moment_nopu / dest.pt();
      dest.set_linear_radial_moment(linear_radial_moment_nopu);
      dest.set_charged_multiplicity_nopu(charged_multiplicity_nopu);
      // jet.set_constituent_tracks(trks);
      if (vtx_handle->size() > 0 && trk_pt_total > 0.0) {
        for (unsigned iV = 0; iV < pt_at_vtx_vec.size(); ++iV) {
          pt_at_vtx_vec[iV] = pt_at_vtx_vec[iV] / trk_pt_total;
        }
        dest.set_beta(pt_at_vtx_vec[0]);
        dest.set_beta_max(
            *std::max_element(pt_at_vtx_vec.begin(), pt_at_vtx_vec.end()));
      } else {
        dest.set_beta(-1.0);
        dest.set_beta_max(-1.0);
      }
    }
  }
  if (cfg_.request_trks) {
    event.put(track_requests, "requestedTracks");
  }
}

template <class T, class U>
void ICJetProducer<T, U>::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &jets_);
}

template <class T, class U>
void ICJetProducer<T, U>::endJob() {
}


#endif
