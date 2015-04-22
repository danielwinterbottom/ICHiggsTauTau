#ifndef ICHiggsTauTau_Utilities_FnPredicates_h
#define ICHiggsTauTau_Utilities_FnPredicates_h
#include <algorithm>
#include <vector>
#include <string>
#include <utility>
#include <cmath>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include "Math/VectorUtil.h"
#include "TVector3.h"

#include "UserCode/ICHiggsTauTau/interface/Objects.hh"
#include "UserCode/ICHiggsTauTau/interface/SuperCluster.hh"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"

namespace ic {

  template <class T, class U> void erase_if(T & t, U pred) {
    t.erase(std::remove_if(t.begin(), t.end(), pred), t.end());
  }

  //Dummy
  template <class T> bool DummyFunction(T const* cand ){
    (void)cand;
    return true;
  }


  //----------------------------------------------------------
  // Candidate
  //----------------------------------------------------------
  bool MinPtMaxEta(Candidate const* cand, double const& minPt, double const& maxEta);

  //----------------------------------------------------------
  // Vertex
  //----------------------------------------------------------
  bool GoodVertex(Vertex const* vertex);

  //----------------------------------------------------------
  // Electron
  //----------------------------------------------------------
  bool InEcalGap(Electron const* electron);
  bool MaxEtaSC(Electron const* electron, double const& maxEta);
  bool passAntiEMVA(Tau const * tau, int WP);
  bool TauEoverP(Tau const* tau, double cut);
  bool Electron2011WP85Iso(Electron const* elec, double const& rho);
  bool Electron2011WP85IsoNoRhoSub(Electron const* elec);
  bool Electron2011WP70ID(Electron const* elec);
  bool Electron2011WP85ID(Electron const* elec);
  bool Electron2011WP95ID(Electron const* elec);
  bool VetoElectronID(Electron const* elec);
  bool ElectronSimpleWP85Iso(Electron const* elec);
  bool ElectronSimpleWP85ID(Electron const* elec);
  bool ElectronHTTIdIso(Electron const* elec, unsigned const& mode);
  bool ElectronHTTVHID(Electron const* elec);
  bool ElectronHTTId(Electron const* elec, bool loose_wp);
  bool ElectronHTTTrigNoIPId(Electron const* elec, bool loose_wp);

  bool HttEMuFakeElectron(Electron const* elec);
  bool HttEMuFakeMuon(Muon const* muon);
  bool HttEMuFakeMuon2011(Muon const* muon);

  bool ElectronHTT2011IdIso(Electron const* elec);

  bool ElectronZbbID(Electron const * elec);
  bool MVAMETElectron(Electron const * elec);

  bool GenParticleInMassBand(GenParticle const* part, int pdgid, double m_low, double m_high);




  bool MinPtMaxEtaSC(SuperCluster const* cand, double const& minPt, double const& maxEta);

  bool InEcalGapSC(SuperCluster const* electron);



  bool CaloJetID(CaloJet const* jet);

  // Standard particle-flow jet id
  bool PFJetID(PFJet const* jet);
  // Particle-flow jet id without the HF energy in the neutral energy cut
  bool PFJetIDNoHFCut(PFJet const* jet);
  bool PUJetID(PFJet const* jet, bool is_2012);
  // Pileup jet id cuts.  0 = 2011, 1 = 2012, 2 = 2012 (december training)
  bool PileupJetID(PFJet const* jet, unsigned training);

  bool IsReBTagged(Jet const* jet, std::map<std::size_t, bool> const& tag_map);

  bool CSVMediumWP(Jet const* cand, double const& minDiscri);

  double PZeta(CompositeCandidate const* cand, Candidate const* met, double const& alpha);
  double PZetaVis(CompositeCandidate const* cand);

  double MT(Candidate const* cand1, Candidate const* cand2);

  bool IsFilterMatched(Candidate const* cand, std::vector<TriggerObject *> const& objs, std::string const& filter, double const& max_dr);


  template<class T>
  double PF04IsolationVal(T const* cand, double const& dbeta) {
    double iso =  cand->dr04_pfiso_charged_all() 
                  + std::max(cand->dr04_pfiso_neutral() + cand->dr04_pfiso_gamma() - dbeta * cand->dr04_pfiso_pu(), 0.0);
    iso = iso / cand->pt();
    return iso;
  }
  
  template<class T>
  bool PF04Isolation(T const* cand, double const& dbeta, double const& cut) {
    double iso =  PF04IsolationVal(cand, dbeta);
    return (iso < cut);
  }
  bool PF04IsolationEBElec(Electron const* cand, double const& dbeta, double const& cut_barrel, double const& cut_endcap);

  template<class T>
  bool PF04IsolationEB(T const* cand, double const& dbeta, double const& cut_barrel, double const& cut_endcap) {
    double iso =  PF04IsolationVal(cand, dbeta);
    if (fabs(cand->eta()) < 1.479) {
      return (iso < cut_barrel);
    } else {
      return (iso < cut_endcap);
    }
  }

  
  template<class T>
  bool PF03Isolation(T const* cand, double const& dbeta, double const& cut) {
    double iso =  cand->dr03_pfiso_charged_all() 
                  + std::max(cand->dr03_pfiso_neutral() + cand->dr03_pfiso_gamma() - dbeta * cand->dr03_pfiso_pu(), 0.0);
    iso = iso / cand->pt();
    return (iso < cut);
  }

  template<class T>
  bool PF03HadIsolation(T const* cand, double const& dbeta, double const& cut) {
    double iso =  cand->dr03_pfiso_charged() 
                  + std::max(cand->dr03_pfiso_neutral() + cand->dr03_pfiso_gamma() - dbeta * cand->dr03_pfiso_pu(), 0.0);
    iso = iso / cand->pt();
    return (iso < cut);
  }

  template<class T>
  bool PF03HadIsolationVal(T const* cand, double const& dbeta) {
    double iso =  cand->dr03_pfiso_charged() 
                  + std::max(cand->dr03_pfiso_neutral() + cand->dr03_pfiso_gamma() - dbeta * cand->dr03_pfiso_pu(), 0.0);
    iso = iso / cand->pt();
    return iso ;
  }


  bool ElectronZbbIso(Electron const* elec, bool is_data, double const& rho, double const& cut);

  template <class T>
  bool DBeamspotLessThan(T const* cand, double const& cut){
    return (fabs(cand->dxy_beamspot()) < cut);
  }

  template <class T>
  double DeltaZ(T const* cand, double const& vz){
    return fabs(cand->vz() - vz);
  }

  bool PairPtSelection(CompositeCandidate const* cand, double const& jetpt1, double const& jetpt2);
  bool OrderedPairPtSelection(CompositeCandidate const* cand, double const& jetpt1, double const& jetpt2, bool const& aboveorbelow);
  bool PairMassInRange(CompositeCandidate const* cand, double const& mLow, double const& mHigh);
  bool PairEtaProdLessThan(CompositeCandidate const* cand, double const& max);
  bool PairDEtaLessThan(CompositeCandidate const* cand, double const& max);
  bool PairDPhiLessThan(CompositeCandidate const* cand, double const& max);
  bool PairAbsDPhiLessThan(CompositeCandidate const* cand, double const& max);

  bool PairOneWithPt(CompositeCandidate const* cand, double const& ptMin);

  bool PairOppCharge(CompositeCandidate const& cand);
  
  bool PairOppSign(CompositeCandidate const* cand);
  bool PairSameSign(CompositeCandidate const* cand);
  double PairMass(CompositeCandidate const* cand);
  double PairMassDiff(CompositeCandidate const* cand, double const &mass);

  bool MuonTight(Muon const* muon);
  bool MuonIso(Muon const* muon);


  template<class T, class U>
  bool EtaProdLessThan(std::pair<T,U> const& p, double const& max) {
    if ((p.first->eta() * p.second->eta()) < max) return true;
    return false;
  }

  template<class T, class U>
  double EtaProd(std::pair<T,U> const& p) {
    return (p.first->eta() * p.second->eta()) ;
  }

  template<class T, class U>
  bool DEtaLessThan(std::pair<T,U> const& p, double const& max) {
    if (fabs(p.first->eta() - p.second->eta()) < max) return true;
    return false;
  }

  template<class T, class U>
  bool DPhiLessThan(std::pair<T,U> const& p, double const& max) {
    if (ROOT::Math::VectorUtil::DeltaPhi(p.first->vector(), p.second->vector()) < max) return true;
    return false;
  }

  template<class T, class U>
  bool MassLessThan(std::pair<T,U> const& p, double const& max) {
    if ((p.first->vector() + p.second->vector()).M() < max) return true;
    return false;
  }

  template<class T> 
  bool MinDRToCollection(Candidate const* cand, 
    std::vector<T> const& coll, double const& cut) {
    BOOST_FOREACH(T const& ele, coll) {
      if (ROOT::Math::VectorUtil::DeltaR(cand->vector(), ele->vector()) < cut) return false;
    }
    return true;
  }


  template<class T, class U> 
  bool FoundIdInCollection(T const* cand, 
    std::vector<U *> const& coll) {
    BOOST_FOREACH(U * ele, coll) {
      if (ele->id() == cand->id()) return true;
    }
    return false;
  }

  template<class T, class U>
  bool DRLessThan(std::pair<T,U> const& cPair, double const& dr) {
    if (DR(cPair.first,cPair.second) < dr) return true;
    return false;
  }

  template<class T, class U>
  double DR(T const& cand1, U const& cand2) {
    return ROOT::Math::VectorUtil::DeltaR(cand1->vector(),cand2->vector());
  }

  template<class T, class U>
  bool DRCompare(std::pair<T,U> const& p1, std::pair<T,U> const& p2) {
      double d1 = DR (p1.first,p1.second);
      double d2 = DR (p2.first,p2.second);
      return (d1 < d2);
  }

  template<class T>
  double ScalarPtSum(std::vector<T> const& vec) {
    double result = 0.0;
    for (unsigned i = 0; i < vec.size(); ++i) {
      result += vec[i]->pt();
    }
    return result;
  }

  template<class T>
  double ScalarEnergySum(std::vector<T> const& vec) {
    double result = 0.0;
    for (unsigned i = 0; i < vec.size(); ++i) {
      result += vec[i]->energy();
    }
    return result;
  }


  std::vector<Track *> GetTracksAtVertex(std::vector<Track *> const& trks, std::vector<Vertex *> const& vtxs, unsigned idx, double const& dz);
  

  bool MassDiffCompare(Candidate const* p1, Candidate const* p2, double const& mass);

  std::vector<GenParticle *> ExtractStableDaughters(GenParticle * part, std::vector<GenParticle *> const& input);

  std::vector<GenParticle *> ExtractDaughters(GenParticle * part, std::vector<GenParticle *> const& input);

  std::vector<GenParticle *> ExtractDaughtersRecursive(GenParticle * part, std::vector<GenParticle *> const& input);

  std::vector<GenJet> BuildTauJets(std::vector<GenParticle *> const& parts, bool include_leptonic);

  ROOT::Math::PtEtaPhiEVector reconstructWboson(Candidate const*  lepton, Candidate const* met);

  

  /*
  //---------------------------------------------------------------------------
  //Typedefs
  //---------------------------------------------------------------------------
  typedef std::pair<GenParticle*, GenParticle*> GenPGenP;
  typedef std::pair<GenJet*, GenJet*>           GenJGenJ;
  typedef std::pair<GenJet*, GenParticle*>      GenJGenP;
  typedef std::pair<GenParticle*, Electron*>    GenPElec;
  typedef std::pair<Electron*, Electron*>       ElecElec;
  typedef std::pair<GenParticle*, Muon*>        GenPMuon;
  typedef std::pair<Muon*, Muon*>               MuonMuon;
  typedef std::pair<Jet*, GenJet*>              RecJGenJ;

  //---------------------------------------------------------------------------
  //STL-like
  //---------------------------------------------------------------------------
  template <class T, class U> void erase_if(T & t, U pred) {
    t.erase(std::remove_if(t.begin(), t.end(), pred), t.end());
  }

  //---------------------------------------------------------------------------
  //Predicate (may require binding for STL)
  //---------------------------------------------------------------------------
  bool HasPdgId(GenParticle const* part, int pdgid, bool ignoreSign);
    
  
  bool MinPtMaxEtaConst(GenJet const* cand, double const& minPt, double const& maxEta);
  
  bool InEcalGap(Electron const* electron);
  
  bool ElectronIdByWP(Electron const* electron, std::string const& wp);
  
  bool ElectronIsoByWP(Electron const* electron,std::string const& wp);
  
  
  
  bool LeptonDBLessThan(Lepton const* lepton, double const& cut);
  
  bool BTaggedHE(Jet const* jet);
  
  bool BTaggedHP(Jet const* jet);
  
  bool PairMassInRange(Candidate const* cand1, Candidate const* cand2, 
                       double const& mLow, double const& mHigh);
  
  bool GenPairOppCharge(GenParticle const* part1, GenParticle const* part2);
  
  bool LeptonPairOppCharge(Lepton const* lepton1, Lepton const* lepton2);
  
  bool PartonJetFlavour(GenJet const* genJet, unsigned const& flavour);
  
  template<class T> 
    bool MinDRToCollection(Candidate const* cand, 
                      std::vector<T> const& coll, double const& cut) {
    BOOST_FOREACH(T const& ele, coll) {
      if (DR(cand, ele) < cut) return false;
    }
    return true;
  }
  



  //---------------------------------------------------------------------------
  //Comparator (may require binding for STL)
  //---------------------------------------------------------------------------
  inline bool PtComparator(Candidate const* c1, Candidate const* c2) {
    return (c1->Pt() < c2->Pt());
  }

  template<class T, class U>
  bool DRCompare(std::pair<T,U> const& p1, std::pair<T,U> const& p2) {
      double d1 = DR (p1.first,p1.second);
      double d2 = DR (p2.first,p2.second);
      return (d1 < d2);
  }
  

  //---------------------------------------------------------------------------
  //Mutator
  //---------------------------------------------------------------------------
  void JetEnergyUncertainty(Jet * jet, bool shift_up);
  
  void JetEnergyShift(Jet * jet, double unc);
  
  void SmearResolution(Jet * jet, TRandom2 * rand, double jerUnc);
  

  //---------------------------------------------------------------------------
  //Matching
  //---------------------------------------------------------------------------
  double DR(Candidate const* cand1, Candidate const* cand2);
  
  template<class T, class U>
    std::vector< std::pair<T,U> > MatchByDR(std::vector<T> const& c1,
                                              std::vector<U> const& c2,
                                              double const& maxDR,
                                              bool const& uniqueFirst,
                                              bool const& uniqueSecond) {
      std::vector< std::pair<T,U> > pairVec = MakePairs(c1,c2);
      erase_if(pairVec, !boost::bind(DRLessThan<T,U>, _1, maxDR));
      std::sort(pairVec.begin(), pairVec.end(), DRCompare<T,U>);
      if (uniqueFirst && uniqueSecond) {
        std::vector< std::pair<T,U> > uPairVec;
        std::vector<T> fVec;
        std::vector<U> sVec;
        std::pair<T,U> aPair;
        BOOST_FOREACH(aPair, pairVec) {
          bool inFVec = std::count(fVec.begin(),fVec.end(),aPair.first);
          bool inSVec = std::count(sVec.begin(),sVec.end(),aPair.second);
          if (!inFVec && !inSVec) {
            uPairVec.push_back(aPair);
            fVec.push_back(aPair.first);
            sVec.push_back(aPair.second);
          }
        }
        return uPairVec;
      } else if (uniqueFirst) {
        std::vector< std::pair<T,U> > uPairVec;
        std::vector<T> fVec;
        std::pair<T,U> aPair;
        BOOST_FOREACH(aPair, pairVec) {
          bool inFVec = std::count(fVec.begin(),fVec.end(),aPair.first);
          if (!inFVec) {
            uPairVec.push_back(aPair);
            fVec.push_back(aPair.first);
          }
        }
        return uPairVec;
      } else if (uniqueSecond) {
        std::vector< std::pair<T,U> > uPairVec;
        std::vector<U> sVec;
        std::pair<T,U> aPair;
        BOOST_FOREACH(aPair, pairVec) {
          bool inSVec = std::count(sVec.begin(),sVec.end(),aPair.second);
          if (!inSVec) {
            uPairVec.push_back(aPair);
            sVec.push_back(aPair.second);
          }
        }
        return uPairVec;
      }
      return pairVec;
    }


  //---------------------------------------------------------------------------
  //Collection Generators
  //---------------------------------------------------------------------------
  std::vector<GenParticle*> MakeFinalBQuarksCollection(
      std::vector<GenParticle*> const& partVec);
  
  std::vector<GenParticle*> MakeInitialBQuarksCollection(
      std::vector<GenParticle*> const& partVec);

  std::vector<GenParticle*> MakeFinalBHadronsCollection(
      std::vector<GenParticle*> const& partVec);
  
  //Searches a collection of GenParticles for a specific pdgId which, when
  //found, initiates a search over the subsequent particles for any daughters.
  //If found, these are added to the returned vector. 
  std::vector<GenParticle*> MakeDaughterCollection(
      std::vector<GenParticle*> const& partVec, unsigned const& pdgid);
  
  std::vector<GenParticle*> MakeDaughterCollectionMCATNLO(
      std::vector<GenParticle*> const& partVec, unsigned const& pdgid);

  std::vector<GenParticle*> GetStatus1Muons(
      std::vector<GenParticle*> const& partVec);
  
  //---------------------------------------------------------------------------
  //Helpers
  //---------------------------------------------------------------------------
  template <class T>
    std::vector<T*> MakePtrVector(std::vector<T> & vec) {
      std::vector<T*> ptrVec(vec.size(),0);
      for (unsigned i = 0; i < vec.size(); ++i) ptrVec[i] = &(vec[i]);
      return ptrVec;
    }

  template <class T, class U>
    std::vector<T> ExtractFirst(std::vector< std::pair<T,U> > const& pairVec) {
      std::vector<T> vec;
      vec.resize(pairVec.size());
      for (unsigned i = 0; i < vec.size(); ++i) vec[i] = (pairVec[i].first);
      return vec;
    }

  template <class T, class U>
    std::vector<U> ExtractSecond(std::vector< std::pair<T,U> > const& pairVec) {
      std::vector<U> vec;
      vec.resize(pairVec.size());
      for (unsigned i = 0; i < vec.size(); ++i) vec[i] = (pairVec[i].second);
      return vec;
    }

  template <typename T>
    std::vector< std::pair<T,T> > MakePairs(std::vector<T> const& collection) {
      unsigned n = collection.size();
      std::vector< std::pair<T,T> > pairVec;
      if (n == 0) return pairVec;
      pairVec.resize((n*(n-1))/2);
      unsigned vecIndex = 0;
      for (unsigned i = 0; i < (n-1); ++i) {
        for (unsigned j = (i+1); j < n; ++j, ++vecIndex) {
          pairVec[vecIndex] = (std::pair<T,T>(collection[i],collection[j]));
        }
      }
      return pairVec;
    }
  template <class T, class U>
    std::vector< std::pair<T,U> > MakePairs(std::vector<T> const& collection1,
                                            std::vector<U> const& collection2) {
      unsigned n = collection1.size();
      unsigned m = collection2.size();
      std::vector< std::pair<T,U> > pairVec;
      pairVec.resize(n*m);
      unsigned vecIndex = 0;
      for (unsigned i = 0; i < n; ++i) {
        for (unsigned j = 0; j < m; ++j, ++vecIndex) {
          pairVec[vecIndex] = (std::pair<T,U>(collection1[i],collection2[j]));
        }
      }
      return pairVec;
    }

  bool IsBflavPdgId(int pdgid);


  double ScaleReso(double pT, double eta, double jerUnc);

*/
} // namepsace
#endif
