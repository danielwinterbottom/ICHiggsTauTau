#include "UserCode/ICHiggsTauTau/plugins/ICPFJetProducer.hh"
#include <boost/functional/hash.hpp>
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/Jet.hh"
#include "DataFormats/Common/interface/ValueMap.h"



#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"

#include "UserCode/ICHiggsTauTau/interface/PATConstruction.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"

//#include "RecoJets/JetProducers/interface/PileupJetIdentifier.h"

#include "boost/format.hpp"


ICPFJetProducer::ICPFJetProducer(const edm::ParameterSet& iConfig) {
  produces<std::vector<unsigned> >("selectGenParticles");
  produces<std::vector<unsigned> >("selectTracks");
  input_label_ = iConfig.getParameter<edm::InputTag>("inputLabel");
  branch_name_ = iConfig.getUntrackedParameter<std::string>("branchName");
  store_ids_ = iConfig.getParameter<bool>("StoreTrackIds");
  pfjets_ = new std::vector<ic::PFJet>();
}


ICPFJetProducer::~ICPFJetProducer() {
 delete pfjets_;
}

// ------------ method called to produce the data  ------------
void ICPFJetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  boost::hash<reco::GenParticle const*> hasher;


  edm::Handle<std::vector<pat::Jet> > jetCollection;
  iEvent.getByLabel(input_label_,jetCollection);
  edm::Handle<edm::View<pat::Jet> > jetCollectionView;
  iEvent.getByLabel(input_label_,jetCollectionView);

  std::vector<pat::Jet>::const_iterator iter;
  //Set the Jet vector size to zero, then allocate enough space
  //for at least as many elements in the PAT::Jet vector
  pfjets_->resize(0);
  pfjets_->reserve(jetCollection->size());

  edm::Handle<reco::GenParticleCollection> partCollection;
  iEvent.getByLabel("genParticles",partCollection);
  edm::Handle<reco::TrackCollection> trackCollection;
  iEvent.getByLabel("generalTracks",trackCollection);
  edm::Handle<reco::VertexCollection> vertexCollection;
  iEvent.getByLabel("offlinePrimaryVertices",vertexCollection);
  edm::Handle<edm::ValueMap<float> > puJetIdMVA;
  iEvent.getByLabel("puJetMva","fullDiscriminant", puJetIdMVA);
  edm::Handle<edm::ValueMap<int> > puJetIdFlag;
  iEvent.getByLabel("puJetMva","fullId", puJetIdFlag);

  // Build map to associate tracks->vertices
  std::vector<reco::TrackBaseRef>::const_iterator trk_it;
  std::map<unsigned, unsigned> trk_vtx_map;
  reco::Track const* ptr_to_first = 0;
  if (trackCollection->size() > 0) {
    ptr_to_first = &(trackCollection->at(0));
    for (unsigned i = 0; i < vertexCollection->size(); ++i) {
      for (trk_it = vertexCollection->at(i).tracks_begin(); trk_it != vertexCollection->at(i).tracks_end(); ++trk_it) {
        reco::Track const* ptr_to_trk = dynamic_cast<reco::Track const*>(&(**trk_it));
        unsigned trk_idx = (unsigned(ptr_to_trk - ptr_to_first));
        trk_vtx_map[trk_idx] =  i;
      }
    }
  }

  std::auto_ptr<std::vector<unsigned> > jet_particles(new std::vector<unsigned>());
  std::auto_ptr<std::vector<unsigned> > jet_tracks(new std::vector<unsigned>());

  unsigned iJ = 0;
  for (iter = jetCollection->begin(); iter != jetCollection->end(); ++iter, ++iJ) {
    pfjets_->push_back(ic::PFJet());
    ic::PFJet & jet = pfjets_->back();
    ic::ConstructPFJet(&jet, &(*iter));
    //Set candidate data


    if (iter->genParton()) {
      unsigned idx = unsigned((reco::GenParticle*)iter->genParton() - &(partCollection->at(0)));
      jet_particles->push_back(idx);
      std::vector<std::size_t> gen_index;
      gen_index.push_back(hasher(iter->genParton()));
      jet.set_gen_particles(gen_index); 
    } 

    //iter->jetArea();

    double beta = -1.0;
    double beta_max = -1.0;
    double trk_pt_total = 0.0;

    int charged_multiplicity = 0;
    int charged_multiplicity_nopu = 0;
    float linear_radial_moment_nopu = 0.0;

    std::vector<double> pt_at_vtx_vec(vertexCollection->size(), 0.0);

    if (trackCollection->size() > 0) {
      reco::Track const* ptr_first = &(trackCollection->at(0));
      static boost::hash<reco::Track const*> track_hasher;
      std::vector<std::size_t> trks;
      std::vector<reco::PFCandidatePtr> const & pfcands = iter->getPFConstituents();
      for (unsigned i = 0; i < pfcands.size(); ++i) {
        if (pfcands[i]->trackRef().isNonnull()) {
          ++charged_multiplicity;
          unsigned idx = unsigned(&(*(pfcands[i]->trackRef())) - ptr_first);
          trk_pt_total += pfcands[i]->trackRef()->pt();
          // Is track associated to a vertex?
          if (trk_vtx_map.count(idx) > 0) {
            // If this track is mapped to the first PV, increase number of nopu charged multiplicity
            if (trk_vtx_map.find(idx)->second == 0) {
              ++charged_multiplicity_nopu;
              linear_radial_moment_nopu += pfcands[i]->pt() * deltaR(iter->rapidity(), iter->phi(), pfcands[i]->rapidity(), pfcands[i]->phi());
            }
            pt_at_vtx_vec[trk_vtx_map.find(idx)->second] += pfcands[i]->trackRef()->pt();
          } else { // No, so is it within 0.2 cm wrt the closest vertex in z
            std::vector<double> dz_with_vtx(vertexCollection->size(), 0.0);
            for (unsigned j = 0; j < vertexCollection->size(); ++j) {
              dz_with_vtx[j] = fabs(vertexCollection->at(j).z() - pfcands[i]->trackRef()->vz());
            }
            std::vector<double>::const_iterator min = std::min_element(dz_with_vtx.begin(),dz_with_vtx.end());
            if (min != dz_with_vtx.end()) {
              if (*min < 0.2) {
                // As above, if this track is not associated to any vertex, but is within 0.2 cm of first
                // PV, then increase multiplicity count
                if (unsigned(min-dz_with_vtx.begin()) == 0) {
                  ++charged_multiplicity_nopu;
                  linear_radial_moment_nopu += pfcands[i]->pt() * deltaR(iter->rapidity(), iter->phi(), pfcands[i]->rapidity(), pfcands[i]->phi());
                }
                pt_at_vtx_vec[unsigned(min-dz_with_vtx.begin())] += pfcands[i]->trackRef()->pt();
              }
            } 
          }
          if (store_ids_) {
            jet_tracks->push_back(idx);
            trks.push_back(track_hasher(&(*(pfcands[i]->trackRef()))));
          }
        } else { // No track found, assume PF candidate is not PU;
          linear_radial_moment_nopu += pfcands[i]->pt() * deltaR(iter->rapidity(), iter->phi(), pfcands[i]->rapidity(), pfcands[i]->phi());
        }
      }
      jet.set_constituent_tracks(trks);
      if (vertexCollection->size() > 0 && trk_pt_total > 0.0) {
        for (unsigned i = 0; i < pt_at_vtx_vec.size(); ++i) {
          pt_at_vtx_vec[i] = pt_at_vtx_vec[i] / trk_pt_total;
        }
        beta = pt_at_vtx_vec[0];
        beta_max = *std::max_element(pt_at_vtx_vec.begin(), pt_at_vtx_vec.end());
      }
    }

    jet.set_beta(beta);
    jet.set_beta_max(beta_max);
    //int idflag = (*puJetIdFlag)[jetCollectionView->refAt(iJ)->originalObjectRef()];
    jet.set_pu_id_mva_value((*puJetIdMVA)[jetCollectionView->refAt(iJ)->originalObjectRef()]);
    // jet.set_pu_id_mva_loose(PileupJetIdentifier::passJetId(idflag, PileupJetIdentifier::kLoose));
    // jet.set_pu_id_mva_medium(PileupJetIdentifier::passJetId(idflag, PileupJetIdentifier::kMedium));
    // jet.set_pu_id_mva_tight(PileupJetIdentifier::passJetId(idflag, PileupJetIdentifier::kTight));


    //Set observed jet energy correction factors
    std::vector<std::string> jec_levels = iter->availableJECLevels();
    for (unsigned i = 0; i < jec_levels.size(); ++i) {
      observed_jec_[jec_levels[i]] = CityHash64(jec_levels[i]);
    }

    //Set observed b-tagging discriminators
    std::vector<std::pair<std::string, float> > const& b_discr = iter->getPairDiscri();
    for (unsigned i = 0; i < b_discr.size(); ++i) {
      observed_btag_[b_discr.at(i).first] = CityHash64(b_discr.at(i).first);
    }

    linear_radial_moment_nopu = linear_radial_moment_nopu / iter->pt();
    jet.set_linear_radial_moment(linear_radial_moment_nopu);
    jet.set_charged_multiplicity_nopu(charged_multiplicity_nopu);
  }
  iEvent.put(jet_particles, "selectGenParticles");
  iEvent.put(jet_tracks, "selectTracks");
}

// ------------ method called once each job just before starting event loop  ------------
void ICPFJetProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_name_.c_str(), &pfjets_);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICPFJetProducer::endJob() {
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Info in <ICPFJetProducer>: EndJob Summary" << std::endl;
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Jet Energy Correction Hash Summary:" << std::endl;
  std::map<std::string, std::size_t>::const_iterator iter;
  for (iter = observed_jec_.begin(); iter != observed_jec_.end(); ++iter) {
    std::cout << boost::format("%-30s %-30s\n") % iter->first % iter->second;
  }
  std::cout << "b-Tag Discriminator Hash Summary:" << std::endl;
  for (iter = observed_btag_.begin(); iter != observed_btag_.end(); ++iter) {
    std::cout << boost::format("%-45s %-20s\n") % iter->first % iter->second;
  }
}

// ------------ method called when starting to processes a run  ------------
void ICPFJetProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICPFJetProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICPFJetProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICPFJetProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICPFJetProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICPFJetProducer);
