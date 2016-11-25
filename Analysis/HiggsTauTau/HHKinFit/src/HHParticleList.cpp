/*
 * HHParticleList.cpp
 *
 *  Created on: 17.06.2014
 */

#include "../include/HHParticleList.h"
#include "../include/HHEventRecord.h"
#include <iostream>

HHParticleList::HHParticleList()
{
  // create list of new and of known particles
  // (name, mass, width,nBR, color)
  m_particlelist[HHPID::H].Set("H", 350., 0., 1, kGreen);
  m_particlelist[HHPID::h1].Set("htau", 125., 0., 1, kRed + 1); //  two higgs for different decays
  m_particlelist[HHPID::h2].Set("hb", 100., 0., 1, kBlue + 1);
  // Standard Model
  m_particlelist[HHPID::e].Set("e", 0.000511, 0., 0, kRed - 4);
  m_particlelist[HHPID::mu].Set("mu", 0.106, 0., 0, kRed - 9);
  m_particlelist[HHPID::tau].Set("tau", 1.777, 0., 1, kRed - 10);
  m_particlelist[HHPID::nu].Set("nu", 0., 0., 0, kRed - 5);
  m_particlelist[HHPID::q].Set("q", 0., 0., 0, kBlack);
  m_particlelist[HHPID::c].Set("c", 1.2, 0., 0, kBlack);
  m_particlelist[HHPID::b].Set("b", 4.2, 0., 0, kBlue -8);
  m_particlelist[HHPID::top].Set("top", 173., 0., 0, kBlue - 10);
  m_particlelist[HHPID::gamma].Set("gamma", 0., 0., 0, kCyan);
  m_particlelist[HHPID::Z].Set("Z", 91.1876, 0., 0, kCyan - 3);
  m_particlelist[HHPID::W].Set("W", 80.385, 0., 0, kCyan - 2);
  m_particlelist[HHPID::gluon].Set("gluon", 0., 0., 0, kGray + 2);
  m_particlelist[HHPID::higgs].Set("higgs", 125., 0., 0, kMagenta + 3);

  m_particlelist[HHPID::UE].Set("UE", 0., 0., 0, kGray);         // soft underlying event
  m_particlelist[HHPID::gluon].Set("ISR", 0., 0., 0, kGray);     // ISR gluon
  m_particlelist[HHPID::MET].Set("MET", 0., 0., 0, kGray);       // MET of the event


  m_particlelist[HHPID::tauvis].Set("tauvis", 0., 0., 0, kGray);
  m_particlelist[HHPID::tauinvis].Set("tauinvis", 0., 0., 0, kGray);

  // define decays
  m_particlelist[HHPID::H].SetDecay(0, HHPID::h1, HHPID::h2, 1.00);   // (idBR, id1,id2, BR)
  m_particlelist[HHPID::h1].SetDecay(0, HHPID::tau, HHPID::tau, 1.00);
  m_particlelist[HHPID::h2].SetDecay(0, HHPID::b, HHPID::b, 1.00);
  m_particlelist[HHPID::tau].SetDecay(0, HHPID::tauvis, HHPID::tauinvis, 1.00); // ?tau implement 3-body decay!!
}

HHParticle*
HHParticleList::GetParticleInfo(HHPID::pid id)
{
  if (id < m_v4maxPartList)
    return &m_particlelist[id];
  else {
    std::cout << "WARNING: ParticleID not found in V4ParticleList."
        << std::endl;
  }
  return &m_particlelist[0];
}

void
HHParticleList::Print(TString name)
{
  std::cout << "V4ParticleList " << name << std::endl;
  for (int i = 1; i < m_v4maxPartList; i++)
    m_particlelist[i].Print(i);
}

void
HHParticleList::UpdateMass(HHPID::pid id, Double_t m)
{
  m_particlelist[id].SetMass(m);
}

HHV4Vector*
HHParticleList::GenerateV4(HHPID::pid id)
{
  HHV4Vector* temp;
  if (id < m_v4maxPartList) {
    temp = new HHV4Vector(m_particlelist[id].M(), 0, 0, m_particlelist[id].M());
    temp->SetID(id);
    temp->SetName(m_particlelist[id].Name());
    temp->SetMother(HHEventRecord::undef);
  }
  else
    temp = new HHV4Vector();

  return temp;
}
