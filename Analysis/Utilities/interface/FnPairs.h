#ifndef ICHiggsTauTau_Utilities_FnPairs_h
#define ICHiggsTauTau_Utilities_FnPairs_h
#include <algorithm>
#include <vector>
#include <string>
#include <utility>
#include <cmath>
//#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include "UserCode/ICHiggsTauTau/interface/Objects.hh"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"

//#include "TRandom2.h"

//#include "UserCode/HbbAnalysis/interface/HbbEvent.hh"


namespace ic {

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
  
  template<class T, class U>
  bool MassDiffCompare(std::pair<T,U> const& p1, std::pair<T,U> const& p2, 
      double const& mass) {
      double m1 = ( p1.first->GetTLorentzVector() + 
                    p1.second->GetTLorentzVector()).M();
      double m2 = ( p2.first->GetTLorentzVector() + 
                    p2.second->GetTLorentzVector()).M();
      double md1 = fabs(m1 - mass);
      double md2 = fabs (m2 - mass);
      return (md1 < md2);
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


  bool IsBflavPdgId(int pdgid);


  double ScaleReso(double pT, double eta, double jerUnc);

*/
} // namepsace
#endif
