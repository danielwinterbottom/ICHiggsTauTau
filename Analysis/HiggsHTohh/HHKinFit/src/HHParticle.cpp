#include "../include/HHParticle.h"
#include "../include/PSTools.h"

#include <iostream>

HHParticle::HHParticle()
    : m_name(""), m_m(0.), m_width(0.), m_nBR(0), m_color(1)
{
}

void
HHParticle::Set(TString name, Double_t m, Double_t w, Int_t nbr, Int_t color)
{
  m_name = name;
  m_m = m;
  m_width = w;
  m_nBR = nbr;
  m_color = color;
}

void
HHParticle::SetDecay(Int_t iBR, HHPID::pid id1, HHPID::pid id2, Double_t br)
{
  m_id1[iBR] = id1;
  m_id2[iBR] = id2;
  m_br[iBR] = br;
}

void
HHParticle::SetMass(Double_t m)
{
  m_m = m;
}

void
HHParticle::Print(int id)
{
  if (id >= 0) {
    PSTools::coutf(2, id);
    std::cout << " ";
  }
  PSTools::coutf(14, TString("V4particle"));
  PSTools::coutf(12, m_name);
  PSTools::coutf(10, m_m);
  PSTools::coutf(6, m_width);
  PSTools::coutf(6, m_nBR);
  PSTools::coutf(6, m_color);
  std::cout << std::endl;
}
