#include "UserCode/ICHiggsTauTau/plugins/TauDebug.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

bool TauDebug::passElectronSelection(const reco::GsfElectron& electron) {
  bool passEleSelection = false;
  // Electron ET
  float electronPt = electron.pt();
  std::cout << " -- electronPt:   " << electronPt << "\n";
  if (electronPt > 10.) {
    double isoDr03 = electron.dr03TkSumPt() + electron.dr03EcalRecHitSumEt() +
                     electron.dr03HcalTowerSumEt();
    double eleEta = fabs(electron.eta());
    std::cout << " -- electronEta:  " << eleEta << "\n";
    std::cout << " -- isoDr03:      " << isoDr03 << "\n";
    if (eleEta <= 1.485 && isoDr03 < 10.) {
      if (electron.mva_Isolated() > -0.1875) passEleSelection = true;
      std::cout << " -- mva_Isolated: " << electron.mva_Isolated() << "\t"
                << passEleSelection << "\n";
    } else if (eleEta > 1.485 && isoDr03 < 10.0) {
      if (electron.mva_Isolated() > -0.1075) passEleSelection = true;
      std::cout << " -- mva_Isolated: " << electron.mva_Isolated() << "\t"
                << passEleSelection << "\n";
    }
  }

  if (electron.mva_e_pi() > -0.1) {
    passEleSelection = true;
  }
  std::cout << " -- mva_e_pi:     " << electron.mva_e_pi() << "\t"
            << passEleSelection << "\n";
  return passEleSelection;
}

bool TauDebug::isElectron(const reco::GsfElectron& electron) {
  unsigned int nmisshits = electron.gsfTrack()->hitPattern().numberOfLostHits(
      reco::HitPattern::MISSING_INNER_HITS);
  std::cout << " -- nmisshits: " << nmisshits << "\n";
  if (nmisshits > 1) return false;
  return true;
}



TauDebug::TauDebug(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")) {
}

TauDebug::~TauDebug() {}

void TauDebug::produce(edm::Event& event, const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::PFCandidate> > cands_handle;
  event.getByLabel(input_, cands_handle);

  edm::Handle<reco::GsfElectronCollection> elecs_handle;
  event.getByLabel("gedGsfElectrons", elecs_handle);

  for (unsigned i = 0; i < cands_handle->size(); ++i) {
    reco::PFCandidate const& src = cands_handle->at(i);

    if (std::abs(src.pdgId()) == 22 &&
        (src.trackRef().isNonnull() || src.gsfTrackRef().isNonnull())) {
      std::cout << "***********************************************\n";
      std::cout << "*** Have photon PFCandidate with a track [" << n_cands_
                << "]\n";
      std::cout << "***********************************************\n";
      ++n_cands_;
      bool with_conv = false;
      bool with_convOneLeg = false;
      std::cout << "PFCandidate::p4(): " << src.p4() << "\n";
      std::cout << "------------------------------\n";
      reco::PhotonRef phref = src.photonRef();
      reco::ConversionRef cvref = src.conversionRef();
      reco::GsfElectronRef gsfref = src.gsfElectronRef();
      std::cout << "Has trackRef():       " << src.trackRef().isNonnull()
                << "\n";
      std::cout << "Has gsfTrackRef()     " << src.gsfTrackRef().isNonnull()
                << "\n";
      std::cout << "Has photonRef():      " << phref.isNonnull() << "\n";
      std::cout << "Has gsfElectronRef(): " << gsfref.isNonnull() << "\n";
      std::cout << "Has conversionRef():  " << cvref.isNonnull() << "\n";
      if (src.trackRef().isNonnull()) ++n_trackRefs_;
      if (src.gsfTrackRef().isNonnull()) ++n_gsfTrackRefs_;
      if (gsfref.isNonnull()) ++n_gsfElectronRefs_;
      if (cvref.isNonnull()) ++n_conversionRefs_;
      // std::cout << src << "\n";
      if (phref.isNull()) {
        // std::cout << "PhotonRef is null\n";
      } else {
        ++n_photonRefs_;
        std::cout << "------------------------------\n";
        std::cout << "PhotonRef is valid\n";
        std::cout << "PFCandidate::mva_nothing_gamma(): "
                  << src.mva_nothing_gamma() << "\n";
        std::cout << "Photon::p4():      " << phref->p4() << "\n";
        // event.getProvenance(phref.id()).write(std::cout);
        std::cout << "Photon::conversions().size():       "
                  << phref->conversions().size() << "\n";
        if (phref->conversions().size() > 0) {
          event.getProvenance(phref->conversions().id()).write(std::cout);
          ++n_photons_with_convs_;
          with_conv = true;
        }
        std::cout << "Photon::conversionsOneLeg().size(): "
                  << phref->conversionsOneLeg().size() << "\n";
        if (phref->conversionsOneLeg().size() > 0) {
          event.getProvenance(phref->conversionsOneLeg().id()).write(std::cout);
          ++n_photons_with_convsOneLeg_;
          with_convOneLeg = true;
        }
      }
      if (gsfref.isNull()) {
        // std::cout << "GsfElectronRef is null\n";
      } else {
        // event.getProvenance(gsfref.id()).write(std::cout);
      }
      if (src.gsfTrackRef().isNonnull()) {
        reco::GsfElectronRef myRef;
        GsfElectronEqual myEqual(src.gsfTrackRef());
        const reco::GsfElectronCollection::const_iterator itcheck =
            std::find_if(elecs_handle->begin(), elecs_handle->end(), myEqual);
        if (itcheck != elecs_handle->end()) {
          ++n_match_elec_;
          myRef = reco::GsfElectronRef(elecs_handle,
                                       itcheck - elecs_handle->begin());
          std::cout << "------------------------------\n";
          std::cout << "Found a GsfElectron with the same GsfTrack:\n";
          std::cout << "GsfElectron::p4(): " << myRef->p4() << "\n";
          bool passGood = passElectronSelection(*myRef);
          bool passPrimary = isElectron(*myRef);
          std::cout << "isGoodElectron:    " << passGood << "\n";
          std::cout << "isPrimaryElectron: " << passPrimary << "\n";
          if (!passGood && passPrimary) ++n_elec_fail_good_;
          if (passGood && !passPrimary) ++n_elec_fail_primary_;
          if (!passGood && !passPrimary) ++n_elec_fail_both_;
        } else {
          std::cout << "------------------------------\n";
          std::cout << "No GsfElectron sharing linked GsfTrack\n";
          if (with_conv) ++n_with_conv_no_elec_;
          if (with_convOneLeg) ++n_with_convOneLeg_no_elec_;
        }
      }
      if (cvref.isNull()) {
        // std::cout << "ConversionRef is null\n";
      } else {
        // event.getProvenance(cvref.id()).write(std::cout);
      }
    }
  }
}

void TauDebug::beginJob() {
}

void TauDebug::endJob() {
  std::cout << "n_cands: " << n_cands_ << "\n";
  std::cout << "n_trackRefs: " << n_trackRefs_ << "\n";
  std::cout << "n_gsfTrackRefs: " << n_gsfTrackRefs_ << "\n";
  std::cout << "n_gsfElectronRefs: " << n_gsfElectronRefs_ << "\n";
  std::cout << "n_conversionRefs: " << n_conversionRefs_ << "\n";
  std::cout << "n_photonRefs: " << n_photonRefs_ << "\n";
  std::cout << "n_photons_with_convs: " << n_photons_with_convs_ << "\n";
  std::cout << "n_photons_with_convsOneLeg: " << n_photons_with_convsOneLeg_ << "\n";
  std::cout << "n_match_elec: " << n_match_elec_ << "\n";
  std::cout << "n_elec_fail_good: " << n_elec_fail_good_ << "\n";
  std::cout << "n_elec_fail_primary: " << n_elec_fail_primary_ << "\n";
  std::cout << "n_elec_fail_both: " << n_elec_fail_both_ << "\n";
  std::cout << "n_with_conv_no_elec: " << n_with_conv_no_elec_ << "\n";
  std::cout << "n_with_convOneLeg_no_elec: " << n_with_convOneLeg_no_elec_ << "\n";
}

DEFINE_FWK_MODULE(TauDebug);