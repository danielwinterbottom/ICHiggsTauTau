/*
 * HHEventRecord.h
 *
 *  Created on: 16.06.2014
 */

#ifndef EVENTRECORD_H_
#define EVENTRECORD_H_

#include "HHV4Vector.h"
#include "HHParticleList.h"
#include <vector>
#include <TString.h>


class HHEventRecord{
public:
  enum entry{
    undef = -1,
//    UE = 0,    // underlying event
//    ISR = 1,   // ISR gluon jet
    MET = 0,   // MET

    H = 1,     // H
    htau = 2,  // h --> tau1 tau2
    hb = 3,    // h --> b1 b2
    tau1 = 4,  // tau1
    tau2 = 5,  // tau2
    b1 = 6,    // b1
    b2 = 7,    // b2
    tauvis1   = 8,   // mu1
    tauinvis1 = 9,  // nu1
    tauvis2   = 10,  // mu2
    tauinvis2 = 11  // nu2
  };

  HHEventRecord (HHParticleList* particlelist);
  HHEventRecord (const HHEventRecord& eventrecord, TString suffix="");
  ~HHEventRecord ();

  Int_t AddInitialEntry (HHPID::pid id);
  void MakeHEvent ();
  void DecayChain ();
  Int_t GetNEntries () const;
  HHV4Vector* GetEntry (Int_t i) const;
  HHParticleList* GetParticleList () const;
  HHV4Vector* UpdateEntry (Int_t i);
  HHV4Vector* UpdateEntry (entry i) {int temp = i; return UpdateEntry(temp);};
  void Print(TString name="",Int_t mode=1);
  void EventDisplayXY(Int_t style);
  void Recombine();
  void UpdateMothers(Int_t ivDaughter);


private:
  Int_t AddEntry (HHPID::pid id, Int_t origin);
  std::vector< HHV4Vector* > * m_eventrecord;
  HHParticleList* m_particlelist;
};


#endif /* EVENTRECORD_H_ */
