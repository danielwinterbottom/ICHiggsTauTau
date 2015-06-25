#include "UserCode/ICHiggsTauTau/plugins/ICLeptonIsolation.h"
#include <vector>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/ValueMap.h"

typedef ICLeptonIsolation<pat::Electron> ICElectronIsolation;
typedef ICLeptonIsolation<reco::Muon> ICMuonIsolation;
typedef ICLeptonIsolation<reco::GsfElectron> ICRecoElectronIsolation;

DEFINE_FWK_MODULE(ICElectronIsolation);
DEFINE_FWK_MODULE(ICMuonIsolation);
DEFINE_FWK_MODULE(ICRecoElectronIsolation);
