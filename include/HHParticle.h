/*
 * Class for definition of particles
 */

#ifndef V4PARTICLE_H_
#define V4PARTICLE_H_

#include <Rtypes.h>
#include <TString.h>
#include "HHPID.h"

class HHParticle {
public:
  HHParticle();

  Double_t M()                  {return(m_m);}
  Double_t Width() const        {return(m_width);}
  Int_t nBR() const             {return(m_nBR);}
  TString Name() const          {return(m_name);}
  Int_t Color() const           {return(m_color);}
  HHPID::pid ID1 (Int_t iBR) const   {return(m_id1[iBR]);}
  HHPID::pid ID2 (Int_t iBR) const   {return(m_id2[iBR]);}
  Double_t BR (Int_t iBR) const {return(m_br[iBR]);}

  void Set(TString name, Double_t m, Double_t w, Int_t nbr, Int_t color=1) ;
  void SetDecay (Int_t iBR, HHPID::pid id1, HHPID::pid id2,  Double_t br);
  void SetMass(Double_t m);
  void Print(int id=-1);

private:
  TString m_name;
  Double_t m_m;
  Double_t m_width ;
  Int_t m_nBR ;
  Int_t m_color;
  const static Int_t m_maxBR = 3 ;
  HHPID::pid m_id1[m_maxBR];
  HHPID::pid m_id2[m_maxBR];
  Double_t m_br[m_maxBR];
};

#endif /* HHPARTICLE_H_ */
