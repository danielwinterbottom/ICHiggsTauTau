#ifndef UserCode_ICHiggsTauTau_ICElectronIDMVAProducerMiniAOD_h
#define UserCode_ICHiggsTauTau_ICElectronIDMVAProducerMiniAOD_h

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "UserCode/ICHiggsTauTau/interface/EGammaMvaEleEstimatorCSA14.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

//using namespace std;
//using namespace reco;
class ICElectronIDMVAProducerMiniAOD : public edm::EDProducer {
public:
    explicit ICElectronIDMVAProducerMiniAOD(const edm::ParameterSet&);
    ~ICElectronIDMVAProducerMiniAOD();
    
private:
    virtual void produce(edm::Event&, const edm::EventSetup&);
    // ----------member data ---------------------------
    bool verbose_;
    edm::EDGetTokenT<pat::ElectronCollection> electronToken_;
    
    //used tokens
    //double _Rho;
    std::string method_;
    std::vector<string> mvaWeightFiles_;
    bool trig_;
    
    EGammaMvaEleEstimatorCSA14* mvaID_;
    
};
#endif
