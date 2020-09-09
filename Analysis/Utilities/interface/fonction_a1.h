#ifndef A1SOLVER_H
#define A1SOLVER_H

#include <vector>
#include "TLorentzVector.h"
#include "TVector3.h"



void quadratic(const double &a, const double &b, const double &c, bool isReal, double &x_0, double &x_1);

void quadratic_alternate(const double &a, const double &b, const double &c, bool isReal, double &x_0, double &x_1);


std::vector<TLorentzVector> tauMomentumSolutions(const TVector3 &tauDir, const TLorentzVector &a1LV, bool isReal);

double GetThetaGJ(const TVector3 &tauDir, const TLorentzVector &a1LV);

double GetThetaGJMax(const TLorentzVector &a1LV);

bool isPhysical(const TVector3 &tauDir, const TLorentzVector &a1LV);


// =====================================================================
// Return an integer from [a,b] (a<b)
// =====================================================================
// Initialize function rand() one time with : "srand(time(NULL));" 
int rand_a_b(const int a, const int b);

// =====================================================================
// Return a float from [a,b] (a<b)
// =====================================================================
double frand_a_b(const double a, const double b);

TLorentzVector RandomT4VectorInsideResolutionCone(const double res, const TLorentzVector &tau);

TVector3 RandomT3VectorInsideResolutionCone(const double res, const double tauFlightDistance, const TVector3 &tau);

void tauPairConstraint(const TVector3 &tau1Dir, const TLorentzVector &a1LV1, bool is1Real, TLorentzVector &tau1PairConstraintLV, 
		       const TVector3 &tau2Dir, const TLorentzVector &a1LV2, bool is2Real, TLorentzVector &tau2PairConstraintLV);

std::vector<TLorentzVector> tauPairMomentumSolutions(const TVector3 &tau1Dir, const TLorentzVector &a1RefitLV1, const TLorentzVector &a1LV1, bool is1Real, const TVector3 &tau2Dir, const TLorentzVector &a1RefitLV2, const TLorentzVector &a1LV2, bool is2Real, bool isRefit);

TVector3 GetRefittedPV(std::vector<size_t> hashes, TVector3 PVNominal, std::vector<double> PVRefit_X , std::vector<double> PVRefit_Y ,std::vector<double> PVRefit_Z ,std::vector<size_t> VertexHash1, std::vector<size_t> VertexHash2, bool &isRefit);

TVector3 GetRefittedPV(std::vector<size_t> hashes, TVector3 PVNominal, std::vector<double> PVRefit_X , std::vector<double> PVRefit_Y ,double PV_Z ,std::vector<size_t> VertexHash1, std::vector<size_t> VertexHash2, bool &isRefit);








#endif
