#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "UserCode/ICHiggsTauTau/plugins/ICElectronIDMVAProducerMiniAOD.hh"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"
//Based on https://github.com/HuguesBrun/ExampleElectronMVAid/blob/addMVAfromMiniAOD/plugins/ExampleElectronMVAid.cc

ICElectronIDMVAProducerMiniAOD::ICElectronIDMVAProducerMiniAOD(const edm::ParameterSet& config) {
    verbose_ = config.getUntrackedParameter<bool>("verbose", false);
//	electronToken_ = consumes<pat::ElectronCollection>(config.getParameter<edm::InputTag>("electronTag"));
        electronToken_ = config.getParameter<edm::InputTag>("electronTag");
	method_ = config.getParameter<string>("method");
	std::vector<string> fpMvaWeightFiles = config.getParameter<std::vector<std::string> >("mvaWeightFile");
	trig_ = config.getParameter<bool>("trig");
        consumes<pat::ElectronCollection>(electronToken_);
    
    produces<edm::ValueMap<float> >("");
    
    mvaID_ = new EGammaMvaEleEstimatorCSA14();
    
    EGammaMvaEleEstimatorCSA14::MVAType type_;
    
    if(trig_){type_ = EGammaMvaEleEstimatorCSA14::kTrig;}
    
    if(!trig_){
        if (fpMvaWeightFiles.size() > 4) type_ = EGammaMvaEleEstimatorCSA14::kNonTrigPhys14;
        else type_ = EGammaMvaEleEstimatorCSA14::kNonTrig;
    }
    bool manualCat_ = true;
    
	string path_mvaWeightFileEleID;
	for(unsigned ifile=0 ; ifile < fpMvaWeightFiles.size() ; ++ifile) {
        path_mvaWeightFileEleID = edm::FileInPath ( fpMvaWeightFiles[ifile].c_str() ).fullPath();
        mvaWeightFiles_.push_back(path_mvaWeightFileEleID);
	}
    
    mvaID_->initialize(method_, type_, manualCat_, mvaWeightFiles_);
    
}


ICElectronIDMVAProducerMiniAOD::~ICElectronIDMVAProducerMiniAOD()
{
    
    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)
    
}


//
// member functions
//

// ------------ method called on each new Event  ------------
void ICElectronIDMVAProducerMiniAOD::produce(edm::Event& event, const edm::EventSetup& setup) {
//	using namespace edm;
    std::unique_ptr<edm::ValueMap<float> > out(new edm::ValueMap<float>() );
    
    
    edm::Handle<pat::ElectronCollection> egCollection;
    event.getByLabel(electronToken_,egCollection);
    const pat::ElectronCollection egCandidates = (*egCollection.product());
    
    
    std::vector<float> values;
    values.reserve(egCollection->size());
    for ( pat::ElectronCollection::const_iterator egIter = egCandidates.begin(); egIter != egCandidates.end(); ++egIter) {
        
        double mvaVal = -999999;
//        mvaVal = mvaID_->mvaValue( *egIter, *pv,thebuilder,lazyTools, verbose_);
        mvaVal = mvaID_->mvaValue(*egIter,verbose_);
        
        values.push_back( mvaVal );
	}
    
    edm::ValueMap<float>::Filler filler(*out);
    filler.insert(egCollection, values.begin(), values.end() );
    filler.fill();
    
    event.put(std::move(out));
    
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICElectronIDMVAProducerMiniAOD);
