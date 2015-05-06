#include "UserCode/ICHiggsTauTau/plugins/MVAMETPairProducer.hh"

#include "FWCore/Utilities/interface/Exception.h"

#include "RecoMET/METAlgorithms/interface/METAlgo.h" 
#include "RecoMET/METAlgorithms/interface/PFSpecificAlgo.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/METReco/interface/CommonMETData.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/SpecificPFMETData.h"
#include "JetMETCorrections/Algorithms/interface/L1FastjetCorrector.h"
#include "TLorentzVector.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/Point3D.h"

#include <boost/functional/hash.hpp>

#include <TMatrixD.h>

#include <algorithm>

using namespace reco;

namespace
{
  template <typename T>
  std::string format_vT(const std::vector<T>& vT)
  {
    std::ostringstream os;
  
    os << "{ ";

    unsigned numEntries = vT.size();
    for ( unsigned iEntry = 0; iEntry < numEntries; ++iEntry ) {
      os << vT[iEntry];
      if ( iEntry < (numEntries - 1) ) os << ", ";
    }

    os << " }";
  
    return os.str();
  }

  std::string format_vInputTag(const std::vector<edm::InputTag>& vit)
  {
    std::vector<std::string> vit_string;
    for ( std::vector<edm::InputTag>::const_iterator vit_i = vit.begin();
	  vit_i != vit.end(); ++vit_i ) {
      vit_string.push_back(vit_i->label());
    }
    return format_vT(vit_string);
  }

  void printJets(std::ostream& stream, const reco::PFJetCollection& jets)
  {
    unsigned numJets = jets.size();
    for ( unsigned iJet = 0; iJet < numJets; ++iJet ) {
      const reco::Candidate::LorentzVector& jetP4 = jets.at(iJet).p4();
      stream << " #" << iJet << ": Pt = " << jetP4.pt() << "," 
	     << " eta = " << jetP4.eta() << ", phi = " << jetP4.phi() << std::endl;
    }
  }
}

MVAMETPairProducer::MVAMETPairProducer(const edm::ParameterSet& cfg) 
   : mvaMEtAlgo_(cfg), mvaMEtAlgo_isInitialized_(false), mvaJetIdAlgo_(cfg)
{
  srcCorrJets_     = cfg.getParameter<edm::InputTag>("srcCorrJets");
  srcUncorrJets_   = cfg.getParameter<edm::InputTag>("srcUncorrJets");
  srcPFCandidates_ = cfg.getParameter<edm::InputTag>("srcPFCandidates");
  srcVertices_     = cfg.getParameter<edm::InputTag>("srcVertices");

  // ICHiggsTauTau
  // srcLeptons_      = cfg.getParameter<vInputTag>("srcLeptons");
  srcLeg1_     = cfg.getParameter<edm::InputTag>("srcLeg1");
  srcLeg2_     = cfg.getParameter<edm::InputTag>("srcLeg2");
  leg1Pt_      = cfg.getParameter<double>("leg1Pt");
  leg2Pt_      = cfg.getParameter<double>("leg2Pt");
  leg1Eta_     = cfg.getParameter<double>("leg1Eta");
  leg2Eta_     = cfg.getParameter<double>("leg2Eta");
  minDeltaR_     = cfg.getParameter<double>("minDeltaR");
  // std::cout << "Info in <MVAMETPairProducer>: " << std::endl;
  // std::cout << "Leg 1: " << srcLeg1_.label() << ", pT > " << leg1Pt_ << " |Eta| < " << leg1Eta_ << std::endl;
  // std::cout << "Leg 2: " << srcLeg2_.label() << ", pT > " << leg2Pt_ << " |Eta| < " << leg2Eta_ << std::endl;
  // std::cout << "Require DeltaR > " << minDeltaR_ << std::endl;
  // ICHiggsTauTau

  srcRho_          = cfg.getParameter<edm::InputTag>("srcRho");

  globalThreshold_ = cfg.getParameter<double>("globalThreshold");

  minCorrJetPt_    = cfg.getParameter<double>     ("minCorrJetPt");
  useType1_        = cfg.getParameter<bool>       ("useType1");
  correctorLabel_  = cfg.getParameter<std::string>("corrector");
  isOld42_         = cfg.getParameter<bool>       ("useOld42");
  
  verbosity_ = ( cfg.exists("verbosity") ) ?
    cfg.getParameter<int>("verbosity") : 0;

  if ( verbosity_ ) {
    std::cout << "<MVAMETPairProducer::MVAMETPairProducer>:" << std::endl;
    std::cout << " srcCorrJets = " << srcCorrJets_.label() << std::endl;
    std::cout << " srcUncorrJets = " << srcUncorrJets_.label() << std::endl;
    std::cout << " srcPFCandidates = " << srcPFCandidates_.label() << std::endl;
    std::cout << " srcVertices = " << srcVertices_.label() << std::endl;
    // std::cout << " srcLeptons = " << format_vInputTag(srcLeptons_) << std::endl;
    std::cout << " srcRho = " << srcVertices_.label() << std::endl;
  }

  produces<reco::PFMETCollection>();
  produces<std::vector<std::size_t> >("MVAMetId");
}

MVAMETPairProducer::~MVAMETPairProducer(){}

void MVAMETPairProducer::produce(edm::Event& evt, const edm::EventSetup& es) 
{ 
  // get jets (corrected and uncorrected)
  edm::Handle<reco::PFJetCollection> corrJets;
  evt.getByLabel(srcCorrJets_, corrJets);

  edm::Handle<reco::PFJetCollection> uncorrJets;
  evt.getByLabel(srcUncorrJets_, uncorrJets);

  const JetCorrector* corrector = 0;
  if(useType1_) corrector = JetCorrector::getJetCorrector (correctorLabel_, es);

  // get PFCandidates
  typedef edm::View<reco::Candidate> CandidateView;
  edm::Handle<reco::CandidateView> pfCandidates_view;
  evt.getByLabel(srcPFCandidates_, pfCandidates_view);

//  typedef edm::View<reco::Candidate> CandidateView;
 // edm::Handle<CandidateView> pfCandidates_view;
//  evt.getByLabel(srcPFCandidates_, pfCandidates_view);

  // get vertices
  edm::Handle<reco::VertexCollection> vertices;
  evt.getByLabel(srcVertices_, vertices); 
  // take vertex with highest sum(trackPt) as the vertex of the "hard scatter" interaction
  // (= first entry in vertex collection)
  const reco::Vertex* hardScatterVertex = ( vertices->size() >= 1 ) ?
    &(vertices->front()) : 0;

  // get leptons
  // (excluded from sum over PFCandidates when computing hadronic recoil)
  std::vector<int> lId;
  std::vector< std::vector<reco::PUSubMETCandInfo> > leptonInfo;
  std::auto_ptr<std::vector<std::size_t> > id_vec(new std::vector<std::size_t>());

  // ICHiggsTauTau
  std::vector<reco::Candidate const*> leg1_filtered;
  std::vector<reco::Candidate const*> leg2_filtered;
  edm::Handle<CandidateView> leg1_view;
  edm::Handle<CandidateView> leg2_view;
  evt.getByLabel(srcLeg1_, leg1_view);
  evt.getByLabel(srcLeg2_, leg2_view);
  for ( CandidateView::const_iterator it = leg1_view->begin(); it != leg1_view->end(); ++it ) {
    if (it->pt() > leg1Pt_ && fabs(it->eta()) < leg1Eta_) leg1_filtered.push_back(&(*it));
  }
  for ( CandidateView::const_iterator it = leg2_view->begin(); it != leg2_view->end(); ++it ) {
    if (it->pt() > leg2Pt_ && fabs(it->eta()) < leg2Eta_) leg2_filtered.push_back(&(*it));
  }

  for (unsigned i = 0; i < leg1_filtered.size(); ++i) {
    for (unsigned j = 0; j < leg2_filtered.size(); ++j) {
      if (leg1_filtered[i] == leg2_filtered[j]) continue;
      if (deltaR(leg1_filtered[i]->p4(),leg2_filtered[j]->p4()) < minDeltaR_) continue; 
      leptonInfo.push_back(std::vector<reco::PUSubMETCandInfo>());
      lId.push_back(0);
      reco::PUSubMETCandInfo pLeptonInfo1;
      reco::PUSubMETCandInfo pLeptonInfo2;
      pLeptonInfo1.p4_          = leg1_filtered[i]->p4();
      pLeptonInfo2.p4_          = leg2_filtered[j]->p4();
//      pLeptonInfo1.chargedFrac_ = chargedFrac(leg1_filtered[i],*pfCandidates,hardScatterVertex);
 //     pLeptonInfo2.chargedFrac_ = chargedFrac(leg2_filtered[j],*pfCandidates,hardScatterVertex);
      leptonInfo.back().push_back(pLeptonInfo1); 
      leptonInfo.back().push_back(pLeptonInfo2); 
      (lId.back())++;
      (lId.back())++;
      std::size_t id = 0;
      boost::hash_combine(id, leg1_filtered[i]);
      boost::hash_combine(id, leg2_filtered[j]);
      id_vec->push_back(id);
    }
  }

  // get average energy density in the event
  // edm::Handle<double> rho;
  // evt.getByLabel(srcRho_, rho);
    if ( !mvaMEtAlgo_isInitialized_ ) {
    mvaMEtAlgo_.initialize(es);
    mvaMEtAlgo_isInitialized_ = true;
  }


  // reconstruct "standard" particle-flow missing Et
  CommonMETData pfMEt_data = metAlgo_.run( (*pfCandidates_view), globalThreshold_);
  SpecificPFMETData specificPfMET = pfMEtSpecificAlgo_.run( (*pfCandidates_view) );
  const reco::Candidate::LorentzVector p4( pfMEt_data.mex, pfMEt_data.mey, 0.0, pfMEt_data.met);
  const reco::Candidate::Point vtx(0.0, 0.0, 0.0 );
  reco::PFMET pfMEt(specificPfMET,pfMEt_data.sumet, p4, vtx);
  reco::Candidate::LorentzVector pfMEtP4_original = pfMEt.p4();

  /*CommonMETData pfMEt_data;
  pfMEt_data =  metAlgo_.run(*pfCandidates_view, globalThreshold_);
  PFSpecificAlgo v;
  SpecificPFMETData specific = v.run(*pfCandidates_view.product());
  const reco::Candidate::LorentzVector lp4(pfMEt_data.mex,pfMEt_data.mey,0.0,pfMEt_data.met);
  const reco::Candidate::Point lvtx(0.0,0.0,0.0);
*/
 
  
  // compute objects specific to MVA based MET reconstruction
  std::vector<reco::PUSubMETCandInfo> pfCandidateInfo = computePFCandidateInfo(*pfCandidates_view, hardScatterVertex);
  std::vector<reco::Vertex::Point>         vertexInfo      = computeVertexInfo(*vertices);

  std::auto_ptr<reco::PFMETCollection> pfMEtCollection(new reco::PFMETCollection());

  for (unsigned i = 0; i < leptonInfo.size(); ++i) {
  // compute MVA based MET and estimate of its uncertainty
  //std::vector<mvaMEtUtilities::JetInfo>    jetInfo         = computeJetInfo(*uncorrJets, *corrJets, *vertices, hardScatterVertex, *corrector,evt,es,leptonInfo[i],pfCandidateInfo);
  std::vector<reco::PUSubMETCandInfo>    jetInfo         = computeJetInfo(*uncorrJets, *corrJets, *vertices, hardScatterVertex, *corrector,evt,es,leptonInfo[i],pfCandidateInfo);

  mvaMEtAlgo_.setInput(leptonInfo[i], jetInfo, pfCandidateInfo, vertexInfo);
  mvaMEtAlgo_.setHasPhotons(false);
  mvaMEtAlgo_.evaluateMVA();

  if (verbosity_) {
    std::cout << "MVA MET: " << i << std::endl;
    for (unsigned j = 0; j < leptonInfo[i].size(); ++j) {
     std::cout << leptonInfo[i][j].p4_.pt() << "  ";
    }
    std::cout << std::endl;
  }

  pfMEt.setP4(mvaMEtAlgo_.getMEt());
  pfMEt.setSignificanceMatrix(mvaMEtAlgo_.getMEtCov());

  if ( verbosity_ ) {
    std::cout << "<MVAMETPairProducer::produce>:" << std::endl;
    std::cout << " PFMET: Pt = " << pfMEtP4_original.pt() << ", phi = " << pfMEtP4_original.phi() << " "
	      << "(Px = " << pfMEtP4_original.px() << ", Py = " << pfMEtP4_original.py() << ")" << std::endl;
    std::cout << " MVA MET: Pt = " << pfMEt.pt() << " phi = " << pfMEt.phi() << " " << evt.luminosityBlock() << " " << evt.id().event() << " (Px = " << pfMEt.px() << ", Py = " << pfMEt.py() << ")" << std::endl;
    std::cout << " Cov:" << std::endl;
    //mvaMEtAlgo_.getMEtCov().Print();
    mvaMEtAlgo_.print(std::cout);
    //std::cout << "corrJets:" << std::endl;
    //printJets(std::cout, *corrJets);
    //std::cout << "uncorrJets:" << std::endl;
    //printJets(std::cout, *uncorrJets);
    std::cout << std::endl;
  }
  
  // add PFMET object to the event
  pfMEtCollection->push_back(pfMEt);
  }

// if (leptonInfo.size() == 0) {
//   std::vector<mvaMEtUtilities::leptonInfo> lInfo;
//   std::vector<mvaMEtUtilities::JetInfo>    jetInfo         = computeJetInfo(*uncorrJets, *corrJets, *vertices, hardScatterVertex, *rho,*corrector,evt,es,lInfo,pfCandidateInfo);
//   mvaMEtAlgo_.setInput(lInfo, jetInfo, pfCandidateInfo, vertexInfo);
//   mvaMEtAlgo_.evaluateMVA();
//   pfMEt.setP4(mvaMEtAlgo_.getMEt());
//   pfMEt.setSignificanceMatrix(mvaMEtAlgo_.getMEtCov());
//   pfMEtCollection->push_back(pfMEt);
//   id_vec->push_back(0);
// }

  evt.put(pfMEtCollection);
  evt.put(id_vec, "MVAMetId");

}

std::vector<reco::PUSubMETCandInfo> MVAMETPairProducer::computeJetInfo(const reco::PFJetCollection& uncorrJets, 
                       const reco::PFJetCollection& corrJets, 
                       const reco::VertexCollection& vertices,
                       const reco::Vertex* hardScatterVertex,
                       const JetCorrector &iCorrector,edm::Event &iEvent,const edm::EventSetup &iSetup,
                       std::vector<reco::PUSubMETCandInfo> &iLeptons,std::vector<reco::PUSubMETCandInfo> &iCands)
{
  const L1FastjetCorrector* lCorrector = dynamic_cast<const L1FastjetCorrector*>(&iCorrector);
  std::vector<reco::PUSubMETCandInfo> retVal;
  for ( reco::PFJetCollection::const_iterator uncorrJet = uncorrJets.begin();
    uncorrJet != uncorrJets.end(); ++uncorrJet ) {
    //int pIndex = uncorrJet-uncorrJets.begin();
    //edm::RefToBase<reco::Jet> jetRef(edm::Ref<PFJetCollection>(&uncorrJets,pIndex));
    for ( reco::PFJetCollection::const_iterator corrJet = corrJets.begin();
    corrJet != corrJets.end(); ++corrJet ) {
//    for( size_t cjIdx=0;cjIdx<corrJets->size();cjIdx++) {
 //     reco::PFJetRef corrJet( corrJets, cjIdx );

      // match corrected and uncorrected jets
      if ( uncorrJet->jetArea() != corrJet->jetArea() ) continue;
      if ( deltaR(corrJet->p4(),uncorrJet->p4()) > 0.01 ) continue;

      // check that jet passes loose PFJet id.
      //bool passesLooseJetId = (*looseJetIdAlgo_)(*corrJet);
      //if ( !passesLooseJetId ) continue; 
      if(!passPFLooseId(&(*uncorrJet))) continue;
      // compute jet energy correction factor
      // (= ratio of corrected/uncorrected jet Pt)
      double jetEnCorrFactor = corrJet->pt()/uncorrJet->pt();
      reco::PUSubMETCandInfo jetInfo;
      
      // PH: apply jet energy corrections for all Jets ignoring recommendations
      jetInfo.p4_ = corrJet->p4();
      double lType1Corr = 0;
      if(useType1_) { //Compute the type 1 correction ===> This code is crap 
      double pCorr = lCorrector->correction(*uncorrJet,iEvent,iSetup);
      lType1Corr = (corrJet->pt()-pCorr*uncorrJet->pt());
      TLorentzVector pVec; pVec.SetPtEtaPhiM(lType1Corr,0,corrJet->phi(),0); 
      reco::Candidate::LorentzVector pType1Corr; pType1Corr.SetCoordinates(pVec.Px(),pVec.Py(),pVec.Pz(),pVec.E());
      //Filter to leptons
      bool pOnLepton = false;
      for(unsigned int i0 = 0; i0 < iLeptons.size(); i0++) if(deltaR(iLeptons[i0].p4_,corrJet->p4()) < 0.5) pOnLepton = true;
      //Add it to PF Collection
      if(corrJet->pt() > 10 && !pOnLepton) {
        reco::PUSubMETCandInfo pfCandidateInfo;
        pfCandidateInfo.p4_ = pType1Corr;
        pfCandidateInfo.dZ_ = -999;
        iCands.push_back(pfCandidateInfo);
      }
      //Scale
     lType1Corr = (pCorr*uncorrJet->pt()-uncorrJet->pt());
     lType1Corr /=corrJet->pt();
        }
      
      // check that jet Pt used to compute MVA based jet id. is above threshold
      if ( !(jetInfo.p4_.pt() > minCorrJetPt_) ) continue;
      jetInfo.mva_ = mvaJetIdAlgo_.computeIdVariables(&(*corrJet), jetEnCorrFactor, hardScatterVertex, vertices, true).mva();
//      jetInfo.mva_ = jetIds[ corrJet ];
      jetInfo.chargedEnFrac_ = (uncorrJet->chargedEmEnergy() + uncorrJet->chargedHadronEnergy() + uncorrJet->chargedMuEnergy() )/uncorrJet->energy();
      if(useType1_) jetInfo.chargedEnFrac_ += lType1Corr*(1-jetInfo.chargedEnFrac_);
      retVal.push_back(jetInfo);
      break;
    }
  }
  return retVal;
}

std::vector<reco::PUSubMETCandInfo> MVAMETPairProducer::computePFCandidateInfo(const reco::CandidateView& pfCandidates,
										  const reco::Vertex* hardScatterVertex)
{
  std::vector<reco::PUSubMETCandInfo> retVal;
  for ( reco::CandidateView::const_iterator pfCandidate = pfCandidates.begin();
	pfCandidate != pfCandidates.end(); ++pfCandidate ) {
    double dZ = -999.; // PH: If no vertex is reconstructed in the event
                       //     or PFCandidate has no track, set dZ to -999
     if ( hardScatterVertex ) {
      const reco::PFCandidate* pfc = dynamic_cast<const reco::PFCandidate* >( &(*pfCandidate) );
      if( pfc != nullptr ) { //PF candidate for RECO and PAT levels
	if      ( pfc->trackRef().isNonnull()    ) dZ = std::abs(pfc->trackRef()->dz(hardScatterVertex->position()));
	else if ( pfc->gsfTrackRef().isNonnull() ) dZ = std::abs(pfc->gsfTrackRef()->dz(hardScatterVertex->position()));
      }
      else { //if not, then packedCandidate for miniAOD level
	const pat::PackedCandidate* pfc = dynamic_cast<const pat::PackedCandidate* >( &(*pfCandidate) );
	dZ = std::abs( pfc->dz( hardScatterVertex->position() ) );
	//exact dz=zero corresponds to the -999 case for pfcandidate
	if(dZ==0) {dZ=-999;}
      }
    }
    reco::PUSubMETCandInfo pfCandidateInfo;
    pfCandidateInfo.p4_ = pfCandidate->p4();
    pfCandidateInfo.dZ_ = dZ;
    retVal.push_back(pfCandidateInfo);
  }
  return retVal;
}

std::vector<reco::Vertex::Point> MVAMETPairProducer::computeVertexInfo(const reco::VertexCollection& vertices)
{
  std::vector<reco::Vertex::Point> retVal;
  for ( reco::VertexCollection::const_iterator vertex = vertices.begin();
	vertex != vertices.end(); ++vertex ) {
    if(fabs(vertex->z())           > 24.) continue;
    if(vertex->ndof()              <  4.) continue;
    if(vertex->position().Rho()    >  2.) continue;
    retVal.push_back(vertex->position());
  }
  return retVal;
}
double MVAMETPairProducer::chargedFrac(const reco::Candidate *iCand,
                  const reco::PFCandidateCollection& pfCandidates,const reco::Vertex* hardScatterVertex) { 
  if(iCand->isMuon())     {
    return 1;
  }
  if(iCand->isElectron())   {
    return 1.;
  }
  if(iCand->isPhoton()  )   {return chargedFracInCone(iCand, pfCandidates,hardScatterVertex);}

  reco::PFCandidate const* pfTest = 0;
  pfTest = dynamic_cast<reco::PFCandidate const*>(iCand);
  if (pfTest != 0) {
    if (pfTest->muonRef().isNonnull()) return 1;
  }

  double lPtTot = 0; double lPtCharged = 0;
  const reco::PFTau *lPFTau = 0; 
  lPFTau = dynamic_cast<const reco::PFTau*>(iCand);//} 
  if(lPFTau != 0) { 
    for (UInt_t i0 = 0; i0 < lPFTau->signalPFCands().size(); i0++) { 
      lPtTot += (lPFTau->signalPFCands())[i0]->pt(); 
      if((lPFTau->signalPFCands())[i0]->charge() == 0) continue;
      lPtCharged += (lPFTau->signalPFCands())[i0]->pt(); 
    }
  } 
  else { 
    const pat::Tau *lPatPFTau = 0; 
    lPatPFTau = dynamic_cast<const pat::Tau*>(iCand);//} 
    if(lPatPFTau != 0) { 
      for (UInt_t i0 = 0; i0 < lPatPFTau->signalCands().size(); i0++) {
        lPtTot += (lPatPFTau->signalCands())[i0]->pt(); 
        if((lPatPFTau->signalCands())[i0]->charge() == 0) continue;
        lPtCharged += (lPatPFTau->signalCands())[i0]->pt(); 
      }
    }
  }
  if(lPtTot == 0) lPtTot = 1.;
  return lPtCharged/lPtTot;
}
//Return tau id by process of elimination
bool MVAMETPairProducer::istau(const reco::Candidate *iCand) { 
  if(iCand->isMuon())     return false;
  if(iCand->isElectron()) return false;
  if(iCand->isPhoton())   return false;
  return true;
}
bool MVAMETPairProducer::passPFLooseId(const PFJet *iJet) { 
  if(iJet->energy()== 0)                                  return false;
  if(iJet->neutralHadronEnergy()/iJet->energy() > 0.99)   return false;
  if(iJet->neutralEmEnergy()/iJet->energy()     > 0.99)   return false;
  if(iJet->nConstituents() <  2)                          return false;
  if(iJet->chargedHadronEnergy()/iJet->energy() <= 0 && fabs(iJet->eta()) < 2.4 ) return false;
  if(iJet->chargedEmEnergy()/iJet->energy() >  0.99  && fabs(iJet->eta()) < 2.4 ) return false;
  if(iJet->chargedMultiplicity()            < 1      && fabs(iJet->eta()) < 2.4 ) return false;
  return true;
}

double MVAMETPairProducer::chargedFracInCone(const reco::Candidate *iCand,
             const reco::PFCandidateCollection& pfCandidates,
             const reco::Vertex* hardScatterVertex,double iDRMax)
{

  reco::Candidate::LorentzVector lVis(0,0,0,0);
  for ( reco::PFCandidateCollection::const_iterator pfCandidate = pfCandidates.begin();
  pfCandidate != pfCandidates.end(); ++pfCandidate ) {
    if(deltaR(iCand->p4(),pfCandidate->p4()) > iDRMax)  continue;
    double dZ = -999.; // PH: If no vertex is reconstructed in the event
                       //     or PFCandidate has no track, set dZ to -999
    if ( hardScatterVertex ) {
      if      ( pfCandidate->trackRef().isNonnull()    ) dZ = fabs(pfCandidate->trackRef()->dz(hardScatterVertex->position()));
      else if ( pfCandidate->gsfTrackRef().isNonnull() ) dZ = fabs(pfCandidate->gsfTrackRef()->dz(hardScatterVertex->position()));
    }
    if(fabs(dZ) > 0.1) continue; 
    lVis += pfCandidate->p4();
  }
  return lVis.pt()/iCand->pt();
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(MVAMETPairProducer);



