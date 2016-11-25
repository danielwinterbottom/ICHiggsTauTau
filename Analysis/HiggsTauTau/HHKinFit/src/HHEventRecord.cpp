/*
 * HHEventRecord.cpp
 *
 *  Created on: 17.06.2014
 */

#include "../include/HHEventRecord.h"
#include "../include/PSTools.h"
#include <iostream>

HHEventRecord::HHEventRecord(HHParticleList* particlelist)
{
  m_particlelist = particlelist;
  m_eventrecord = new std::vector<HHV4Vector*>();
  MakeHEvent();
}

HHEventRecord::HHEventRecord(const HHEventRecord& eventrecord, TString suffix)
{
//  std::cout << "copy V4EventRecord with " << eventrecord.GetNEntries()
//            << " entries and suffix '" << suffix << "'." << std::endl;
  m_particlelist = eventrecord.GetParticleList();
  m_eventrecord = new std::vector<HHV4Vector*>();
  for (Int_t index = 0; index < eventrecord.GetNEntries(); index++) {
    HHV4Vector* temp = eventrecord.GetEntry(index);
    m_eventrecord->push_back(new HHV4Vector(*temp, suffix));
  }
}

void
HHEventRecord::MakeHEvent()
{
//  AddInitialEntry(HHPID::UE);
//  AddInitialEntry(HHPID::gluon);
  AddInitialEntry(HHPID::MET);
  AddInitialEntry(HHPID::H);
  DecayChain();
}

HHEventRecord::~HHEventRecord()
{
  for (Int_t index = 0; index < GetNEntries(); index++) {
    delete UpdateEntry(index);
  }
  delete m_eventrecord;
}

Int_t
HHEventRecord::AddInitialEntry(HHPID::pid pid)
{
  HHV4Vector* entry = m_particlelist->GenerateV4(pid);
  m_eventrecord->push_back(entry);
  Int_t newindex = m_eventrecord->size() - 1;
  return (newindex);
}

void
HHEventRecord::Print(TString name, Int_t mode)
{
  std::cout << "V4EventRecord " << name << std::endl;
  if (mode == 0) {
    PSTools::coutf(4, TString(""));
    std::cout << " ";
    PSTools::coutf(12, TString("name"));
    PSTools::coutf(4, TString("ID"));
    PSTools::coutf(11, TString("  E"));
    PSTools::coutf(11, TString("  Px"));
    PSTools::coutf(11, TString("  Py"));
    PSTools::coutf(11, TString("  Pz"));
    PSTools::coutf(11, TString("  M"));
    std::cout << std::endl;
  }
  if (mode == 1) {
    PSTools::coutf(4, TString(""));
    std::cout << " ";
    PSTools::coutf(12, TString("name"));
    PSTools::coutf(4, TString("ID"));
    PSTools::coutf(11, TString("  E"));
    PSTools::coutf(11, TString("  Pt"));
    PSTools::coutf(11, TString("  Eta"));
    PSTools::coutf(11, TString("  Phi"));
    PSTools::coutf(11, TString("  M"));
    std::cout << std::endl;
  }

  for (Int_t index = 0; index < GetNEntries(); index++){
    if (mode == 0) m_eventrecord->at(index)->PrintEPxPyPzM(index);
    if (mode == 1) m_eventrecord->at(index)->PrintEPtEtaPhiM(index);
  }
}


HHV4Vector*
HHEventRecord::UpdateEntry(Int_t i)
{
  return m_eventrecord->at(i);
}

HHParticleList*
HHEventRecord::GetParticleList() const
{
  return m_particlelist;
}

Int_t
HHEventRecord::AddEntry(HHPID::pid pid, Int_t origin)
{
  HHV4Vector* entry = m_particlelist->GenerateV4(pid);
  entry->SetMother(origin);
  m_eventrecord->push_back(entry);
  Int_t newindex = m_eventrecord->size() - 1;
  m_eventrecord->at(origin)->AddDaughter(newindex);
  return (newindex);
}

void
HHEventRecord::DecayChain()
{
  for (Int_t index = 0; index < GetNEntries(); index++) { // decay loop
    HHPID::pid pid = m_eventrecord->at(index)->ID();
    Int_t nBR = m_particlelist->GetParticleInfo(pid)->nBR();

    if (nBR > 0) {
      //TODO: select decay mode according to BR ??? to be implemented
      Int_t iBR = 0;  // up to now only first one taken
      HHPID::pid pid1 = m_particlelist->GetParticleInfo(pid)->ID1(iBR);
      HHPID::pid pid2 = m_particlelist->GetParticleInfo(pid)->ID2(iBR);
      AddEntry(pid1, index);
      AddEntry(pid2, index);
    }
    else {
      m_eventrecord->at(index)->SetDaughters();
    }
  }
}

Int_t
HHEventRecord::GetNEntries() const
{
  return m_eventrecord->size();
}

HHV4Vector*
HHEventRecord::GetEntry(Int_t i) const
{
  return m_eventrecord->at(i);
}

void
HHEventRecord::EventDisplayXY(Int_t style)
{        // Event Display in x-y view
//  std::cout << "V4EventDisplayXY ------------" << std::endl;
  for (Int_t index = GetNEntries() - 1; index >= 0; index--) {
    HHPID::pid pid = GetEntry(index)->ID();
    Int_t color = m_particlelist->GetParticleInfo(pid)->Color();
    m_eventrecord->at(index)->Draw(color, style);
  }
}

void
HHEventRecord::Recombine()
{
  // recombine mother particles from daughters
  // relies on order of decays from top to bottom
  //  std::cout << "V4RecombineDecays ------------" << nv4 << std::endl;

  for (Int_t index = 0; index < GetNEntries(); index++) { // reset all particles with daughters
    HHV4Vector* vector = GetEntry(index);
    if (vector->nDaughter() > 0)
      vector->SetEEtaPhiM(0., 0., 0., 0.);
  }
  for (Int_t index = GetNEntries() - 1; index >= 0; index--) {
    HHV4Vector* vector = GetEntry(index);
    Int_t imother = vector->Mother();
    if (imother >= 0) {
      HHV4Vector* motherentry = GetEntry(imother);
      UpdateEntry(imother)->Sum(*motherentry, *vector);
    }
  }
}

void
HHEventRecord::UpdateMothers(Int_t ivdaughter)
{
  // recombine mother particles from daughter
  Int_t ivmother = GetEntry(ivdaughter)->Mother();
  //  std::cout << "V4UpdateMothers ------------ nv4 " << nv4
  //       << "  ivDaughter " << ivDaughter << "  Mother " << Mother << std::endl;

  if (ivmother < 0)
    return;

  HHV4Vector* motherentry = GetEntry(ivmother);
  UpdateEntry(ivmother)->SetEEtaPhiM(0., 0., 0., 0.);
  for (Int_t idaugh = 0; idaugh < motherentry->nDaughter(); idaugh++) {
    Int_t iv4daugh = motherentry->Daughter(idaugh);
    HHV4Vector* daughterentry = GetEntry(iv4daugh);
    UpdateEntry(ivmother)->Sum(*motherentry, *daughterentry);
    //      std::cout << "  Mother " << Mother
    //     << "  iv4daugh " << iv4daugh << "  E_daugh " << V4[iv4daugh].E() << std::endl;
  }
  UpdateMothers(ivmother);
}
