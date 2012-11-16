#include "UserCode/AGilbert/plugins/PFMETAllPairsProducerMVA.h"
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
#include "JetMETCorrections/Algorithms/interface/L1FastjetCorrector.h"
#include "TLorentzVector.h"

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

PFMETAllPairsProducerMVA::PFMETAllPairsProducerMVA(const edm::ParameterSet& cfg) 
  : mvaMEtAlgo_(cfg)
  ,mvaJetIdAlgo_(cfg)
{
  srcCorrJets_     = cfg.getParameter<edm::InputTag>("srcCorrJets");
  srcUncorrJets_   = cfg.getParameter<edm::InputTag>("srcUncorrJets");
  srcPFCandidates_ = cfg.getParameter<edm::InputTag>("srcPFCandidates");
  srcVertices_     = cfg.getParameter<edm::InputTag>("srcVertices");
  srcLeptons_      = cfg.getParameter<vInputTag>("srcLeptons");
  srcRho_          = cfg.getParameter<edm::InputTag>("srcRho");

  globalThreshold_ = cfg.getParameter<double>("globalThreshold");
  minCorrJetPt_    = cfg.getParameter<double>("minCorrJetPt");

  useType1_        = cfg.getParameter<bool>       ("useType1");
  correctorLabel_  = cfg.getParameter<std::string>("corrector");


  verbosity_ = ( cfg.exists("verbosity") ) ?
    cfg.getParameter<int>("verbosity") : 0;

  if ( verbosity_ ) {
    std::cout << "<PFMETAllPairsProducerMVA::PFMETAllPairsProducerMVA>:" << std::endl;
    std::cout << " srcCorrJets = " << srcCorrJets_.label() << std::endl;
    std::cout << " srcUncorrJets = " << srcUncorrJets_.label() << std::endl;
    std::cout << " srcPFCandidates = " << srcPFCandidates_.label() << std::endl;
    std::cout << " srcVertices = " << srcVertices_.label() << std::endl;
    std::cout << " srcLeptons = " << format_vInputTag(srcLeptons_) << std::endl;
    std::cout << " srcRho = " << srcVertices_.label() << std::endl;
  }

  produces<reco::PFMETCollection>();
  produces<std::vector<std::size_t> >("MVAMetId");

}

PFMETAllPairsProducerMVA::~PFMETAllPairsProducerMVA()
{
}

void PFMETAllPairsProducerMVA::produce(edm::Event& evt, const edm::EventSetup& es) 
{ 
  // get jets (corrected and uncorrected)
  edm::Handle<reco::PFJetCollection> corrJets;
  evt.getByLabel(srcCorrJets_, corrJets);
  
  const JetCorrector* corrector = 0;
  if(useType1_) corrector = JetCorrector::getJetCorrector (correctorLabel_, es);

  edm::Handle<reco::PFJetCollection> uncorrJets;
  evt.getByLabel(srcUncorrJets_, uncorrJets);

  // get PFCandidates
  edm::Handle<reco::PFCandidateCollection> pfCandidates;
  evt.getByLabel(srcPFCandidates_, pfCandidates);

  typedef edm::View<reco::Candidate> CandidateView;
  edm::Handle<CandidateView> pfCandidates_view;
  evt.getByLabel(srcPFCandidates_, pfCandidates_view);

  // get leptons
  // (excluded from sum over PFCandidates when computing hadronic recoil)
  std::vector<int> lId;
  std::vector< std::vector<mvaMEtUtilities::leptonInfo> > leptonInfo;
  std::auto_ptr<std::vector<std::size_t> > id_vec(new std::vector<std::size_t>());
  std::vector< std::vector<reco::Candidate const*> > cand_vec;
  std::vector< std::vector<reco::Candidate const*> > pairs_vec;

  for ( vInputTag::const_iterator srcLeptons_i = srcLeptons_.begin();
  srcLeptons_i != srcLeptons_.end(); ++srcLeptons_i ) {
    //std::cout << "Lepton Collection: " << unsigned(srcLeptons_i - srcLeptons_.begin()) << std::endl;
    edm::Handle<CandidateView> leptons;
    evt.getByLabel(*srcLeptons_i, leptons);
    //std::cout << "--Size: " << leptons->size() << std::endl;
    cand_vec.push_back(std::vector<reco::Candidate const*>()); 
    for ( CandidateView::const_iterator lepton1 = leptons->begin();
    lepton1 != leptons->end(); ++lepton1 ) {
      unsigned coll = unsigned(srcLeptons_i - srcLeptons_.begin());
      double pt = lepton1->pt();
      double eta = fabs(lepton1->eta());
      if (coll == 0 && !(pt > 9.5 && eta < 2.6)) continue;
      if (coll == 1 && !(pt > 9.5 && eta < 2.6)) continue;
      if (coll == 2 && !(pt > 18.0 && eta < 2.6)) continue;
      cand_vec.back().push_back(&(*lepton1));
    }
  }

  if (cand_vec.size() <= 3) {
    for (unsigned i = 0; i < (cand_vec.size() - 1); ++i) {
      for (unsigned j = i + 1; j < cand_vec.size(); ++j) {
        //std::cout << "-Using collections [" << i << "," << j << "]" << std::endl; 
        for (unsigned k = 0; k < cand_vec[i].size(); ++k) {
          for (unsigned l = 0; l < cand_vec[j].size(); ++l) {
            //std::cout << "--Making pair vec with elements: [" << k << "," << l << "]" << 
            //"  [" << cand_hasher(cand_vec[i][k]) << "," << cand_hasher(cand_vec[j][l]) << "]" << std::endl;
            pairs_vec.push_back(std::vector<reco::Candidate const*>());
            std::size_t id = 0;
            boost::hash_combine(id, cand_vec[i][k]);
            boost::hash_combine(id, cand_vec[j][l]);
            id_vec->push_back(id);
            pairs_vec.back().push_back(cand_vec[i][k]);
            pairs_vec.back().push_back(cand_vec[j][l]);
          }
        }
      }
    }
  } else {
    pairs_vec.push_back(std::vector<reco::Candidate const*>());
    for (unsigned i = 0; i < cand_vec.size(); ++i) {
      for (unsigned j = 0; j < cand_vec[i].size(); ++j) {
        pairs_vec.back().push_back(cand_vec[i][j]);
      }
    }
  }



 //  for ( vInputTag::const_iterator srcLeptons_i = srcLeptons_.begin();
	// srcLeptons_i != srcLeptons_.end(); ++srcLeptons_i ) {

  for (std::vector<std::vector<reco::Candidate const*> >::const_iterator pair_it = pairs_vec.begin();
      pair_it != pairs_vec.end(); ++pair_it ) {

    leptonInfo.push_back(std::vector<mvaMEtUtilities::leptonInfo>());
    lId.push_back(0);

   //  edm::Handle<CandidateView> leptons;
   //  evt.getByLabel(*srcLeptons_i, leptons);
   //  for ( CandidateView::const_iterator lepton1 = leptons->begin();
	  // lepton1 != leptons->end(); ++lepton1 ) {
    for (std::vector<reco::Candidate const*>::const_iterator cand_it = pair_it->begin();
         cand_it != pair_it->end(); ++cand_it) {
      bool pMatch = false;
 //      for ( vInputTag::const_iterator srcLeptons_j = srcLeptons_.begin();
	//     srcLeptons_j != srcLeptons_.end(); ++srcLeptons_j ) {
	// edm::Handle<CandidateView> leptons2;
	// evt.getByLabel(*srcLeptons_j, leptons2);
	// for ( CandidateView::const_iterator lepton2 = leptons2->begin();
	//       lepton2 != leptons2->end(); ++lepton2 ) {
    for (std::vector<reco::Candidate const*>::const_iterator cand_it2 = pair_it->begin();
         cand_it2 != pair_it->end(); ++cand_it2) {   
      if(cand_it == cand_it2) continue;
	  if(deltaR((*cand_it)->p4(),(*cand_it2)->p4()) < 0.5)                                                                    pMatch = true;
	  if(pMatch &&     !istau((*cand_it)) &&  istau((*cand_it2)))                                                     pMatch = false;
	  if(pMatch &&    ( (istau((*cand_it)) && istau((*cand_it2))) || (!istau((*cand_it)) && !istau((*cand_it2)))) 
	            &&     (*cand_it)->pt() > (*cand_it2)->pt())                                                                  pMatch = false;
	  if(pMatch && (*cand_it)->pt() == (*cand_it2)->pt()) {
	    pMatch = false;
	    for(unsigned int i0 = 0; i0 < leptonInfo.back().size(); i0++) {
	      if(fabs((*cand_it)->pt() - leptonInfo.back()[i0].p4_.pt()) < 0.1) pMatch = true;
	    }
	  }
	  if(pMatch) break;
	}
	if(pMatch) break;
      //}
      //if(pMatch) continue;
      //if(chargedFrac(&(*lepton1)) == 0) continue;
      mvaMEtUtilities::leptonInfo pLeptonInfo;
      pLeptonInfo.p4_          = (*cand_it)->p4();
      pLeptonInfo.chargedFrac_ = chargedFrac((*cand_it));
      leptonInfo.back().push_back(pLeptonInfo); 
    }
    (lId.back())++;
  }
  //if(lNMu == 2) std::cout << "=====> Di Muon Cand =======>"  << leptonInfo[0].p4_.pt() << " -- " << leptonInfo[1].p4_.pt() << std::endl;

  // get vertices
  edm::Handle<reco::VertexCollection> vertices;
  evt.getByLabel(srcVertices_, vertices); 
  // take vertex with highest sum(trackPt) as the vertex of the "hard scatter" interaction
  // (= first entry in vertex collection)
  const reco::Vertex* hardScatterVertex = ( vertices->size() >= 1 ) ?
    &(vertices->front()) : 0;

  // get average energy density in the event
  edm::Handle<double> rho;
  evt.getByLabel(srcRho_, rho);

  // reconstruct "standard" particle-flow missing Et
  CommonMETData pfMEt_data;
  metAlgo_.run(pfCandidates_view, &pfMEt_data, globalThreshold_);
  reco::PFMET pfMEt = pfMEtSpecificAlgo_.addInfo(pfCandidates_view, pfMEt_data);
  reco::Candidate::LorentzVector pfMEtP4_original = pfMEt.p4();
  
  // compute objects specific to MVA based MET reconstruction
  std::vector<mvaMEtUtilities::pfCandInfo> pfCandidateInfo = computePFCandidateInfo(*pfCandidates, hardScatterVertex);
  std::vector<reco::Vertex::Point>         vertexInfo      = computeVertexInfo(*vertices);

  std::auto_ptr<reco::PFMETCollection> pfMEtCollection(new reco::PFMETCollection());

  for (unsigned i = 0; i < leptonInfo.size(); ++i) {
  // compute MVA based MET and estimate of its uncertainty
  std::vector<mvaMEtUtilities::JetInfo>    jetInfo         = computeJetInfo(*uncorrJets, *corrJets, *vertices, hardScatterVertex, *rho,*corrector,evt,es,leptonInfo[i],pfCandidateInfo);
  mvaMEtAlgo_.setInput(leptonInfo[i], jetInfo, pfCandidateInfo, vertexInfo);
  mvaMEtAlgo_.evaluateMVA();

  //std::cout << "MVA MET: " << i << std::endl;
  //for (unsigned j = 0; j < leptonInfo[i].size(); ++j) {
  //  std::cout << leptonInfo[i][j].p4_.pt() << "  ";
  //}
  //std::cout << std::endl;
  //std::cout << "--: " << mvaMEtAlgo_.getMEt().pt() << std::endl;

  pfMEt.setP4(mvaMEtAlgo_.getMEt());
  pfMEt.setSignificanceMatrix(mvaMEtAlgo_.getMEtCov());

  if ( verbosity_ ) {
    std::cout << "<PFMETAllPairsProducerMVA::produce>:" << std::endl;
    std::cout << " PFMET: Pt = " << pfMEtP4_original.pt() << ", phi = " << pfMEtP4_original.phi() << " "
	      << "(Px = " << pfMEtP4_original.px() << ", Py = " << pfMEtP4_original.py() << ")" << std::endl;
    std::cout << " MVA MET: Pt = " << pfMEt.pt() << " phi = " << pfMEt.phi() << " " << evt.luminosityBlock() << " " << evt.id().event() << " (Px = " << pfMEt.px() << ", Py = " << pfMEt.py() << ")" << std::endl;
    std::cout << " Cov:" << std::endl;
    mvaMEtAlgo_.getMEtCov().Print();
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

if (leptonInfo.size() == 0) {
  std::vector<mvaMEtUtilities::leptonInfo> lInfo;
  std::vector<mvaMEtUtilities::JetInfo>    jetInfo         = computeJetInfo(*uncorrJets, *corrJets, *vertices, hardScatterVertex, *rho,*corrector,evt,es,lInfo,pfCandidateInfo);
  mvaMEtAlgo_.setInput(lInfo, jetInfo, pfCandidateInfo, vertexInfo);
  mvaMEtAlgo_.evaluateMVA();
  pfMEt.setP4(mvaMEtAlgo_.getMEt());
  pfMEt.setSignificanceMatrix(mvaMEtAlgo_.getMEtCov());
  pfMEtCollection->push_back(pfMEt);
  id_vec->push_back(0);
}

  evt.put(pfMEtCollection);
  evt.put(id_vec, "MVAMetId");

}

std::vector<mvaMEtUtilities::JetInfo> PFMETAllPairsProducerMVA::computeJetInfo(const reco::PFJetCollection& uncorrJets, 
                       const reco::PFJetCollection& corrJets, 
                       const reco::VertexCollection& vertices,
                       const reco::Vertex* hardScatterVertex,
                       double rho,const JetCorrector &iCorrector,edm::Event &iEvent,const edm::EventSetup &iSetup,
                       std::vector<mvaMEtUtilities::leptonInfo> &iLeptons,std::vector<mvaMEtUtilities::pfCandInfo> &iCands)
{
  const L1FastjetCorrector* lCorrector = dynamic_cast<const L1FastjetCorrector*>(&iCorrector);
  std::vector<mvaMEtUtilities::JetInfo> retVal;
  for ( reco::PFJetCollection::const_iterator uncorrJet = uncorrJets.begin();
  uncorrJet != uncorrJets.end(); ++uncorrJet ) {
    //int pIndex = uncorrJet-uncorrJets.begin();
    //edm::RefToBase<reco::Jet> jetRef(edm::Ref<PFJetCollection>(&uncorrJets,pIndex));
    for ( reco::PFJetCollection::const_iterator corrJet = corrJets.begin();
    corrJet != corrJets.end(); ++corrJet ) {
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
      mvaMEtUtilities::JetInfo jetInfo;
      
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
    mvaMEtUtilities::pfCandInfo pfCandidateInfo;
    pfCandidateInfo.p4_ = pType1Corr;
    pfCandidateInfo.dZ_ = -999;
    iCands.push_back(pfCandidateInfo);
  }
  //Scale
  lType1Corr /=corrJet->pt();
      }
      
      // check that jet Pt used to compute MVA based jet id. is above threshold
      if ( !(jetInfo.p4_.pt() > minCorrJetPt_) ) continue;
      jetInfo.mva_ = mvaJetIdAlgo_.computeIdVariables(&(*corrJet), jetEnCorrFactor, hardScatterVertex, vertices, true).mva();
      jetInfo.neutralEnFrac_ = (uncorrJet->neutralEmEnergy() + uncorrJet->neutralHadronEnergy())/uncorrJet->energy();
      if(fabs(corrJet->p4().eta()) > 2.5) jetInfo.neutralEnFrac_ = 1.; //===> This is a 53X fix only!
      if(useType1_) jetInfo.neutralEnFrac_ += lType1Corr;
      retVal.push_back(jetInfo);
      break;
    }
  }
  return retVal;
}

std::vector<mvaMEtUtilities::pfCandInfo> PFMETAllPairsProducerMVA::computePFCandidateInfo(const reco::PFCandidateCollection& pfCandidates,
										  const reco::Vertex* hardScatterVertex)
{
  std::vector<mvaMEtUtilities::pfCandInfo> retVal;
  for ( reco::PFCandidateCollection::const_iterator pfCandidate = pfCandidates.begin();
	pfCandidate != pfCandidates.end(); ++pfCandidate ) {
    double dZ = -999.; // PH: If no vertex is reconstructed in the event
                       //     or PFCandidate has no track, set dZ to -999
    if ( hardScatterVertex ) {
      if      ( pfCandidate->trackRef().isNonnull()    ) dZ = fabs(pfCandidate->trackRef()->dz(hardScatterVertex->position()));
      else if ( pfCandidate->gsfTrackRef().isNonnull() ) dZ = fabs(pfCandidate->gsfTrackRef()->dz(hardScatterVertex->position()));
    }
    mvaMEtUtilities::pfCandInfo pfCandidateInfo;
    pfCandidateInfo.p4_ = pfCandidate->p4();
    pfCandidateInfo.dZ_ = dZ;
    retVal.push_back(pfCandidateInfo);
  }
  return retVal;
}

std::vector<reco::Vertex::Point> PFMETAllPairsProducerMVA::computeVertexInfo(const reco::VertexCollection& vertices)
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
double PFMETAllPairsProducerMVA::chargedFrac(const reco::Candidate *iCand) { 
  if(iCand->isMuon())     {
    //const reco::Muon *lMuon = 0; 
    //lMuon = dynamic_cast<const reco::Muon*>(iCand);//} 
    //std::cout << " ---> Iso Mu  : " << lMuon->pfIsolationR03().sumChargedHadronPt << " - " << lMuon->pfIsolationR03().sumPUPt << std::endl;
    return 1;
  }
  if(iCand->isElectron())   {
    //const reco::GsfElectron *lElectron = 0; 
    //lElectron = dynamic_cast<const reco::GsfElectron*>(iCand);//} 
    //std::cout << " ---> Iso  : " << lElectron->pfIsolationVariables().neutralHadronIso << " -- " << lElectron->pfIsolationVariables().neutralHadronIso  << " -- " << lElectron->pfIsolationVariables().chargedHadronIso  << " -- " << lElectron->isolationVariables04().tkSumPt << " -- " << lElectron->pt() << " -- " << lElectron->eta() << std::endl;
    return 1.;
  }
  if(iCand->isPhoton()  )   return 0.;
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
    for (UInt_t i0 = 0; i0 < lPatPFTau->signalPFCands().size(); i0++) { 
      lPtTot += (lPatPFTau->signalPFCands())[i0]->pt(); 
      if((lPatPFTau->signalPFCands())[i0]->charge() == 0) continue;
      lPtCharged += (lPatPFTau->signalPFCands())[i0]->pt(); 
    }
  }
  if(lPtTot == 0) lPtTot = 1.;
  return lPtCharged/lPtTot;
}
//Return tau id by process of elimination
bool PFMETAllPairsProducerMVA::istau(const reco::Candidate *iCand) { 
  if(iCand->isMuon())     return false;
  if(iCand->isElectron()) return false;
  if(iCand->isPhoton())   return false;
  return true;
}
bool PFMETAllPairsProducerMVA::passPFLooseId(const PFJet *iJet) { 
  if(iJet->energy()== 0)                                  return false;
  if(iJet->neutralHadronEnergy()/iJet->energy() > 0.99)   return false;
  if(iJet->neutralEmEnergy()/iJet->energy()     > 0.99)   return false;
  if(iJet->nConstituents() <  2)                          return false;
  if(iJet->chargedHadronEnergy()/iJet->energy() <= 0 && fabs(iJet->eta()) < 2.4 ) return false;
  if(iJet->chargedEmEnergy()/iJet->energy() >  0.99  && fabs(iJet->eta()) < 2.4 ) return false;
  if(iJet->chargedMultiplicity()            < 1      && fabs(iJet->eta()) < 2.4 ) return false;
  return true;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PFMETAllPairsProducerMVA);



