#ifndef UserCode_ICHiggsTauTau_ICJetSrcHelper_h
#define UserCode_ICHiggsTauTau_ICJetSrcHelper_h

#include <memory>
#include "boost/functional/hash.hpp"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#if CMSSW_MAJOR_VERSION > 7 || (CMSSW_MAJOR_VERSION == 7 && CMSSW_MINOR_VERSION >= 6)
#include "JetMETCorrections/Modules/interface/CorrectedJetProducer.h"
#else
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#endif
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "UserCode/ICHiggsTauTau/interface/JPTJet.hh"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/CaloJet.hh"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

/**
 * @brief A struct to help with the configuration of the ICJetProducer, in
 *particular the aspects that depend on the source jet class.
 * @details This templated version of the struct will be used when the source
 *collection is comprised of any reco jet type. A specialised version of this
 *struct is defined for pat::Jet sources, and can be found [here](@ref
 *JetSrcHelper<pat::Jet>). The reco jet sources require additional configuration
 *for features like jet energy corrections and b-tagging scale factors, which
 *cannot be extracted automatically as in the pat::Jet case. A
 *JetSrcHelper<U>::Produce method is also defined, which handles extracting such
 *properties for both kinds of jet inputs. Note, the type of the ic jet
 *collection being produced isn't specified here, but is left as a template
 *parameter of the JetSrcHelper<U>::Produce method. Therefore this method is
 *only concerned with filling the jet properties common to all ic classes,
 *namely those defined in the ic::Jet base class.
 *
 * @tparam U The type of the input jet collection, e.g. reco::PFJet,
 *reco::CaloJet.
 */
template <class U>
struct JetSrcHelper {
  explicit JetSrcHelper(const edm::ParameterSet &config, edm::ConsumesCollector && collector)
      : input_jet_flavour(
            config.getParameter<edm::InputTag>("inputJetFlavour")),
        include_jet_flavour(config.getParameter<bool>("includeJetFlavour")),
        apply_jec_factors(config.getParameter<bool>("applyJECs")),
        include_jec_factors(config.getParameter<bool>("includeJECs")),
        cut(config.getParameter<std::string>("cutAfterJECs")),
        cut_string(config.getParameter<std::string>("cutAfterJECs")),
        apply_post_jec_cut(config.getParameter<bool>("applyCutAfterJECs")),
        input_sv_info((config.getParameter<edm::InputTag>("inputSVInfo"))),
        include_sv_info_ids(config.getParameter<bool>("requestSVInfo")) {
        collector.consumes<edm::ValueMap<std::vector<int>>>(input_jet_flavour);
        collector.consumes<reco::SecondaryVertexTagInfoCollection>(input_sv_info);
    if (apply_jec_factors || include_jec_factors) {
      edm::ParameterSet pset = config.getParameter<edm::ParameterSet>("JECs");
      std::vector<std::string> vec =
          pset.getParameterNamesForType<std::string>();
      for (unsigned i = 0; i < vec.size(); ++i) {
        jecs.push_back(
            std::make_pair(vec[i], pset.getParameter<std::string>(vec[i])));
#if CMSSW_MAJOR_VERSION > 7 || (CMSSW_MAJOR_VERSION ==7 && CMSSW_MINOR_VERSION >= 6 )
        collector.consumes<reco::JetCorrector>(jecs[i].second);
#else
        collector.consumes<double>(jecs[i].second);
#endif
      }
    }
    edm::ParameterSet btag_pset =
        config.getParameter<edm::ParameterSet>("BTagDiscriminators");
    std::vector<std::string> btag_vec =
        btag_pset.getParameterNamesForType<edm::InputTag>();
    for (unsigned i = 0; i < btag_vec.size(); ++i) {
      input_btags.push_back(std::make_pair(
          btag_vec[i], btag_pset.getParameter<edm::InputTag>(btag_vec[i])));
      collector.consumes<reco::JetFloatAssociation::Container>(input_btags[i].second);
    }
  }

  void DoSetup(edm::EDProducer * prod) {
    if (include_sv_info_ids) {
      prod->produces<reco::SecondaryVertexTagInfoRefVector>("requestedSVInfo");
    }
    PrintOptional(1, include_jet_flavour, "includeJetFlavour");
    PrintOptional(1, apply_jec_factors, "applyJECs");
    PrintOptional(1, include_jec_factors, "includeJECs");
    PrintOptional(1, apply_post_jec_cut, "applyCutAfterJECs");
    PrintOptional(1, include_sv_info_ids, "requestSVInfo");
  }
  ~JetSrcHelper() {}

  template <class T>
  void Produce(edm::Handle<edm::View<U> > const &jets_handle,
               std::vector<T> *jets, std::vector<unsigned> *passed,
               edm::Event &event, const edm::EventSetup &setup) {
    edm::Handle<edm::ValueMap<std::vector<int>> > jet_flavour_handle;
    if (include_jet_flavour)
      event.getByLabel(input_jet_flavour, jet_flavour_handle);
  #if CMSSW_MAJOR_VERSION > 7 || (CMSSW_MAJOR_VERSION == 7 && CMSSW_MINOR_VERSION >= 6 )
    std::vector<reco::JetCorrector const *> correctors(jecs.size(), NULL);
  #else
    std::vector<JetCorrector const*> correctors(jecs.size(), NULL);
  #endif
    for (unsigned i = 0; i < correctors.size(); ++i) {
  #if CMSSW_MAJOR_VERSION > 7 || (CMSSW_MAJOR_VERSION == 7 && CMSSW_MINOR_VERSION >= 6)
      edm::Handle<reco::JetCorrector> corrector_handle;
      event.getByLabel(jecs[i].second,corrector_handle);
      correctors[i] = corrector_handle.product();
  #else
      correctors[i] = JetCorrector::getJetCorrector(jecs[i].second, setup);
  #endif
    }

    edm::Handle<reco::SecondaryVertexTagInfoCollection> sv_info_handle;
    if (include_sv_info_ids) {
      event.getByLabel(input_sv_info, sv_info_handle);
    }

    std::vector<edm::Handle<reco::JetFloatAssociation::Container> >
        btag_handles(input_btags.size());
    for (unsigned i = 0; i < btag_handles.size(); ++i) {
      event.getByLabel(input_btags[i].second, btag_handles[i]);
    }

    std::unique_ptr<reco::SecondaryVertexTagInfoRefVector> sv_info_requests(
        new reco::SecondaryVertexTagInfoRefVector());

    for (unsigned i = 0; i < jets_handle->size(); ++i) {
      U const &src = jets_handle->at(i);
      double full_jec_factor = 1.0;
      std::vector<float> factors(correctors.size(), 0.);
      bool keep_jet = true;
      // Are there jet energy corrections to apply?
      if (correctors.size() > 0) {
        // Yes - make a copy of the source Jet
        U jet_cpy = jets_handle->at(i);
        // Loop through each correction and apply
        for (unsigned j = 0; j < correctors.size(); ++j) {
          #ifndef CMSSW_4_2_8_patch7
            #if CMSSW_MAJOR_VERSION > 7 || (CMSSW_MAJOR_VERSION ==7 && CMSSW_MINOR_VERSION >= 6)
          double factor = correctors[j]->correction(jet_cpy);
            #else
          double factor = correctors[j]->correction(jet_cpy, event, setup);
            #endif
          #else 
          double factor = correctors[j]->correction(jet_cpy,
              edm::RefToBase<reco::Jet>(jets_handle->refAt(i)), event, setup);
          #endif
          if (apply_jec_factors) full_jec_factor *= factor;
          factors[j] = factor;
          jet_cpy.scaleEnergy(factor);
        }
        if (apply_post_jec_cut && !cut(jet_cpy)) keep_jet = false;
      }

      if (keep_jet) {
        passed->push_back(i);
      } else {
        continue;
      }

      jets->push_back(T());
      T &dest = jets->back();

      dest.set_id(jet_hasher(&src));
      dest.set_pt(src.pt() * full_jec_factor);
      dest.set_eta(src.eta());
      dest.set_phi(src.phi());
      dest.set_energy(src.energy() * full_jec_factor);
      dest.set_charge(src.charge());
      dest.set_jet_area(src.jetArea());
      if (include_jet_flavour) {
        dest.set_parton_flavour((*jet_flavour_handle)[jets_handle->refAt(i)].at(0));
        dest.set_hadron_flavour((*jet_flavour_handle)[jets_handle->refAt(i)].at(1));
      }
      // Only write correction into the output jet if the user wants it
      if (include_jec_factors) {
        for (unsigned j = 0; j < correctors.size(); ++j) {
          dest.SetJecFactor(jecs[j].first, factors[j]);
        }
      }

      if (include_sv_info_ids) {
        for (unsigned iS = 0; iS < sv_info_handle->size(); ++iS) {
          if (&src == (*sv_info_handle)[iS].jet().get()) {
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

      for (unsigned iB = 0; iB < btag_handles.size(); ++iB) {
        dest.SetBDiscriminator(input_btags[iB].first,
                               (*btag_handles[iB])[edm::RefToBase<reco::Jet>(
                                   jets_handle->refAt(i))]);
      }
    }
    if (include_sv_info_ids) {
      event.put(std::move(sv_info_requests), "requestedSVInfo");
    }
  }

  boost::hash<U const*> jet_hasher;
  boost::hash<reco::Vertex const*> vertex_hasher;

  edm::InputTag input_jet_flavour;
  bool include_jet_flavour;

  std::vector<std::pair<std::string, std::string> > jecs;
  bool apply_jec_factors;
  bool include_jec_factors;

  StringCutObjectSelector<U> cut;
  std::string cut_string;
  bool apply_post_jec_cut;

  edm::InputTag input_sv_info;
  bool include_sv_info_ids;

  std::vector<std::pair<std::string, edm::InputTag> > input_btags;
};

/**
 * @brief A specialisation of the JetSrcHelper<U> struct for pat::Jet input
 * @details As properties such as the jet flavour and the b-tagging results can
 * be extracted directly from a pat::Jet this specialisation requires fewer
 * input parameters than the generic version for reco jets. Of note, the Produce
 * methods must handle jet energy corrections differently: the pat::Jet input is
 * assumed to be corrected already, whereas for reco input the jets are
 * uncorrected and the user must specificy which corrections (if any) to apply.
 */
template <>
struct JetSrcHelper<pat::Jet> {
  explicit JetSrcHelper(const edm::ParameterSet &config, edm::ConsumesCollector && collector)
      : include_jet_flavour(config.getParameter<bool>("includeJetFlavour")),
        include_jec_factors(config.getParameter<bool>("includeJECs")),
        input_sv_info((config.getParameter<edm::InputTag>("inputSVInfo"))),
        include_sv_info_ids(config.getParameter<bool>("requestSVInfo")),
        is_slimmed(config.getParameter<bool>("isSlimmed")),
        slimmed_puid_label(config.getParameter<std::string>("slimmedPileupIDLabel")) {
         collector.consumes<reco::SecondaryVertexTagInfoCollection>(input_sv_info);
       }
  void DoSetup(edm::EDProducer * prod) {
    if (include_sv_info_ids) {
      prod->produces<reco::SecondaryVertexTagInfoRefVector>("requestedSVInfo");
    }
    PrintOptional(1, include_jet_flavour, "includeJetFlavour");
    PrintOptional(1, include_jec_factors, "includeJECs");
    PrintOptional(1, include_sv_info_ids, "requestSVInfo");
  }
  ~JetSrcHelper() {}

  template <class T>
  void Produce(edm::Handle<edm::View<pat::Jet> > const &jets_handle,
               std::vector<T> *jets, std::vector<unsigned> *passed,
               edm::Event &event, const edm::EventSetup &setup) {
    edm::Handle<reco::SecondaryVertexTagInfoCollection> sv_info_handle;
    if (include_sv_info_ids) {
      event.getByLabel(input_sv_info, sv_info_handle);
    }

    std::unique_ptr<reco::SecondaryVertexTagInfoRefVector> sv_info_requests(
        new reco::SecondaryVertexTagInfoRefVector());

    for (unsigned i = 0; i < jets_handle->size(); ++i) {
      pat::Jet const &src = jets_handle->at(i);
      passed->push_back(i);
      jets->push_back(T());
      T &dest = jets->back();

      dest.set_id(jet_hasher(&src));
      dest.set_pt(src.pt());
      dest.set_eta(src.eta());
      dest.set_phi(src.phi());
      dest.set_energy(src.energy());
      dest.set_charge(src.charge());
      dest.set_jet_area(src.jetArea());
      if (include_jet_flavour) {
        dest.set_parton_flavour(src.partonFlavour());
        dest.set_hadron_flavour(src.hadronFlavour());
      }
      // Only write correction into the output jet if the user wants it
      if (include_jec_factors && src.availableJECLevels().size() >= 1) {
        std::vector<std::string> jec_levels = src.availableJECLevels();
        for (unsigned j = 1; j < jec_levels.size(); ++j) {
          dest.SetJecFactor(
              jec_levels[j],
              src.jecFactor(jec_levels[j]) / src.jecFactor(jec_levels[j - 1]));
        }
      }

      if (include_sv_info_ids) {
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
    if (include_sv_info_ids) {
      event.put(std::move(sv_info_requests), "requestedSVInfo");
    }
  }

  boost::hash<pat::Jet const*> jet_hasher;
  boost::hash<reco::Vertex const*> vertex_hasher;

  bool include_jet_flavour;
  bool include_jec_factors;
  edm::InputTag input_sv_info;
  bool include_sv_info_ids;
  bool is_slimmed;
  std::string slimmed_puid_label;
};

#endif
