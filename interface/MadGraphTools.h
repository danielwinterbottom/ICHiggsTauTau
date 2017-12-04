
#pragma once

#include <algorithm>
#include <cstdlib>

#include <boost/algorithm/string/replace.hpp>

#include <Python.h> // https://www.codeproject.com/Articles/11805/Embedding-Python-in-C-C-Part-I

#include <Math/Boost.h>
#include <Math/LorentzVector.h>
#include <Math/PxPyPzE4D.h>
#include <TDatabasePDG.h>



// needs four-momenta as px, py, pz, E (e.g. for incoming partons with pT=0)
// https://github.com/cms-cvs-history/SimDataFormats-GeneratorProducts/blob/master/interface/LesHouches.h#L128-L263
// taken from https://github.com/KappaAnalysis/Kappa/blob/master/DataFormats/interface/KParticle.h#L263-L275
class LHEParticle 
{
public:
	typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>> CartesianRMFLV;
	
	LHEParticle() {};
	virtual ~LHEParticle() {};

	CartesianRMFLV p4; // PUP
	int pdgId; // IDUP
};


class MadGraphTools
{
public:
	typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>> CartesianRMFLV;
	
	MadGraphTools(float mixingAngleOverPiHalf, std::string madgraphProcessDirectory, std::string madgraphParamCard, float alphaS,
	              bool madGraphSortingHeavyBQuark=false, bool mg5aMCv2p5OrOlder=false);
	virtual ~MadGraphTools();
	
	template<class TLHEParticle>
	double GetMatrixElementSquared(std::vector<TLHEParticle*>& lheParticles, std::vector<int> const& bosonPdgIds={25}) const // the vector is sorted in-place to match MadGraph ordering scheme
	{
		// sorting of LHE particles for MadGraph
		if (m_madGraphSortingHeavyBQuark)
		{
			std::sort(lheParticles.begin(), lheParticles.begin()+2, &MadGraphTools::MadGraphParticleOrderingHeavyBQuark<TLHEParticle>);
			std::sort(lheParticles.begin()+3, lheParticles.end(), &MadGraphTools::MadGraphParticleOrderingHeavyBQuark<TLHEParticle>);
		}
		else
		{
			std::sort(lheParticles.begin(), lheParticles.begin()+2, &MadGraphTools::MadGraphParticleOrderingLightBQuark<TLHEParticle>);
			std::sort(lheParticles.begin()+3, lheParticles.end(), &MadGraphTools::MadGraphParticleOrderingLightBQuark<TLHEParticle>);
		}
	
		std::vector<CartesianRMFLV> particleFourMomenta = MadGraphTools::BoostToHiggsCMS<TLHEParticle>(lheParticles, bosonPdgIds);
		std::vector<int> particlePdgIds = MadGraphTools::GetPdgIds<TLHEParticle>(lheParticles);

		// construct Python list of four-momenta
		PyObject* pyParticleFourMomenta = PyList_New(0);
		for (std::vector<CartesianRMFLV>::const_iterator particleLV = particleFourMomenta.begin(); particleLV != particleFourMomenta.end(); ++particleLV)
		{
			PyObject* pyParticleFourMomentum = PyList_New(0);
			PyList_Append(pyParticleFourMomentum, PyFloat_FromDouble(particleLV->E()));
			PyList_Append(pyParticleFourMomentum, PyFloat_FromDouble(particleLV->Px()));
			PyList_Append(pyParticleFourMomentum, PyFloat_FromDouble(particleLV->Py()));
			PyList_Append(pyParticleFourMomentum, PyFloat_FromDouble(particleLV->Pz()));
			PyList_Append(pyParticleFourMomenta, pyParticleFourMomentum);
		}
	
		//construct list of particle pdgs
		PyObject* pyParticlePdgIds = PyList_New(0);
		for (std::vector<int>::const_iterator particlePdgId = particlePdgIds.begin(); particlePdgId != particlePdgIds.end(); ++particlePdgId)
		{
			PyList_Append(pyParticlePdgIds, PyInt_FromLong(*particlePdgId));
		}
	
		// call MadGraphTools.matrix_element_squared
		PyObject* pyMethodName = PyString_FromString("matrix_element_squared");
		PyObject* pyMatrixElementSquared = PyObject_CallMethodObjArgs(m_pyMadGraphTools, pyMethodName, pyParticleFourMomenta, pyParticlePdgIds, NULL);
		PyErr_Print();
		double matrixElementSquared = -1.0;
		if (pyMatrixElementSquared != nullptr)
		{
			matrixElementSquared = PyFloat_AsDouble(pyMatrixElementSquared);
			Py_DECREF(pyMatrixElementSquared);
		}
	
		// clean up
		Py_DECREF(pyParticleFourMomenta);
		Py_DECREF(pyParticlePdgIds);
		Py_DECREF(pyMethodName);
	
		return matrixElementSquared;
	}
	
	static TDatabasePDG* GetDatabasePDG(std::string pdgDatabaseFilename="$ROOTSYS/etc/pdg_table.txt");
	
	template<class TLHEParticle>
	static std::string GetProcess(std::vector<TLHEParticle*>& lheParticles, // the vector is sorted in-place to match MadGraph ordering scheme
	                              TDatabasePDG* databasePDG, bool madGraphSortingHeavyBQuark=false, std::vector<int> const& bosonPdgIds={25})
	{
		// sorting of LHE particles for MadGraph
		if (madGraphSortingHeavyBQuark)
		{
			std::sort(lheParticles.begin(), lheParticles.begin()+2, &MadGraphTools::MadGraphParticleOrderingHeavyBQuark<TLHEParticle>);
			std::sort(lheParticles.begin()+3, lheParticles.end(), &MadGraphTools::MadGraphParticleOrderingHeavyBQuark<TLHEParticle>);
		}
		else
		{
			std::sort(lheParticles.begin(), lheParticles.begin()+2, &MadGraphTools::MadGraphParticleOrderingLightBQuark<TLHEParticle>);
			std::sort(lheParticles.begin()+3, lheParticles.end(), &MadGraphTools::MadGraphParticleOrderingLightBQuark<TLHEParticle>);
		}
		
		std::string process = "";
		for (typename std::vector<TLHEParticle*>::iterator lheParticle = lheParticles.begin(); lheParticle != lheParticles.end(); ++lheParticle)
		{
			process += (std::string((std::find(bosonPdgIds.begin(), bosonPdgIds.end(), std::abs((*lheParticle)->pdgId)) != bosonPdgIds.end()) ? "_" : "") +
			            std::string(databasePDG->GetParticle((*lheParticle)->pdgId)->GetName()));
		}
		return process;
	}

private:

	// pdgParticle->GetName() has no specific order
	// madgraph sorts particle before antiparticle
	// puts gluons first
	// up type quarks second
	// downtype quarks third
	// => the order is: g u c d s b u_bar c_bar d_bar s_bar b_bar
	template<class TLHEParticle>
	static bool MadGraphParticleOrderingLightBQuark(TLHEParticle* lheParticle1, TLHEParticle* lheParticle2)
	{
		int pdgId1 = std::abs(lheParticle1->pdgId);
		int pdgId2 = std::abs(lheParticle2->pdgId);
	
		if ((lheParticle1->pdgId < 0) && (lheParticle2->pdgId > 0))
		{
			return false;
		}
		else if ((lheParticle1->pdgId > 0) && (lheParticle2->pdgId < 0))
		{
			return true;
		}
		else
		{
			if (pdgId1 == 21) // gluon
			{
				return true;
			}
			else if (pdgId2 == 21) // gluon
			{
				return false;
			}
			else if (pdgId1 == 2) // up quark
			{
				return true;
			}
			else if (pdgId2 == 2) // up quark
			{
				return false;
			}
			else if (pdgId1 == 4) // charm quark
			{
				return true;
			}
			else if (pdgId2 == 4) // charm quark
			{
				return false;
			}
			else if (pdgId1 == 1) // down quark
			{
				return true;
			}
			else if (pdgId2 == 1) // down quark
			{
				return false;
			}
			else if (pdgId1 == 3) // strange quark
			{
				return true;
			}
			else if (pdgId2 == 3) // strange quark
			{
				return false;
			}
			else if (pdgId1 == 5) // bottom quark
			{
				return true;
			}
			else if (pdgId2 == 5) // bottom quark
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	
	// pdgParticle->GetName() has no specific order
	// madgraph sorts particle before antiparticle
	// puts gluons first
	// up type quarks second
	// downtype quarks third
	// heavy quarks last => the order is: g u c d s u_bar c_bar d_bar s_bar b b_bar
	template<class TLHEParticle>
	static bool MadGraphParticleOrderingHeavyBQuark(TLHEParticle* lheParticle1, TLHEParticle* lheParticle2)
	{
		int pdgId1 = std::abs(lheParticle1->pdgId);
		int pdgId2 = std::abs(lheParticle2->pdgId);
	
		if ((lheParticle1->pdgId < 0) && (lheParticle2->pdgId > 0) && (pdgId1 != 5) && (pdgId2 == 5)) // bottom quark
		{
			return true;
		}
		else if ((lheParticle1->pdgId > 0) && (lheParticle2->pdgId < 0) && (pdgId1 == 5) && (pdgId2 != 5)) // bottom quark
		{
			return false;
		}
		else
		{
			return MadGraphTools::MadGraphParticleOrderingLightBQuark(lheParticle1, lheParticle2);
		}
	}
	
	template<class TLHEParticle>
	static std::vector<CartesianRMFLV> BoostToHiggsCMS(std::vector<TLHEParticle*> lheParticles, std::vector<int> const& bosonPdgIds={25})
	{
		std::vector<CartesianRMFLV> particleFourMomentaHiggsCMS;
	
		//extract 4-momentum of the higgs boson
		CartesianRMFLV higgsFourMomentum = CartesianRMFLV(0,0,0,1);
		for (typename std::vector<TLHEParticle*>::iterator lheParticle = lheParticles.begin(); lheParticle != lheParticles.end(); ++lheParticle)
		{
			if (std::find(bosonPdgIds.begin(), bosonPdgIds.end(), std::abs((*lheParticle)->pdgId)) != bosonPdgIds.end())
			{
				 higgsFourMomentum = (*lheParticle)->p4;
			}
		}
	
		// Calculate boost to Higgs CMRF and boost particle LV to it.
		CartesianRMFLV::BetaVector betaVector = higgsFourMomentum.BoostToCM();
		ROOT::Math::Boost boost(betaVector);
	
		// boost all particles
		for (typename std::vector<TLHEParticle*>::iterator lheParticle = lheParticles.begin(); lheParticle != lheParticles.end(); ++lheParticle)
		{
			particleFourMomentaHiggsCMS.push_back(boost((*lheParticle)->p4));
		}
		return particleFourMomentaHiggsCMS;
	}
	
	template<class TLHEParticle>
	static std::vector<int> GetPdgIds(std::vector<TLHEParticle*> lheParticles)
	{
		std::vector<int> particlePdgIds;
		for (typename std::vector<TLHEParticle*>::iterator lheParticle = lheParticles.begin(); lheParticle != lheParticles.end(); ++lheParticle)
		{
			particlePdgIds.push_back((*lheParticle)->pdgId);
		}
		return particlePdgIds;
	}
	
	bool m_madGraphSortingHeavyBQuark = false;
	bool m_mg5aMCv2p5OrOlder = false;
	
	PyObject* m_pyMadGraphTools = nullptr;
	
};

