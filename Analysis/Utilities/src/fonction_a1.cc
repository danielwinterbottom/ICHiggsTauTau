#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/fonction_a1.h"

#include <iostream>
#include <cmath>
#include "TLorentzVector.h"
#include "TVector3.h"
#include <vector>

//Random function
#include "stdlib.h"
#include "time.h"
#include "boost/functional/hash.hpp"

using std::cin;
using std::cout; using std::endl;
using std::string;
using std::vector;


void quadratic(const double &a, const double &b, const double &c, bool isReal, double &x_0, double &x_1)
{
  (void)isReal;
  double D = b*b - 4.0*a*c;
  if(D >= 0){
    isReal = true;
    x_0 = (-b - sqrt(D)) / (2.0 * a);
    x_1 = (-b + sqrt(D)) / (2.0 * a);
  }
  else{
    isReal = false;
    x_0 = -b / (2.0 * a);
    x_1 = -b / (2.0 * a);
  }
}

void quadratic_alternate(const double &a, const double &b, const double &c, bool isReal, double &x_0, double &x_1)
{
  (void)isReal;
  double D = b*b - 4.0*a*c;
  if(D >= 0){
    isReal = true;
    double q = -0.5 * (b + copysign( sqrt(D) , b ));
    x_0 = c / q;
    x_1 = q / a;
  }
  else{
    isReal = false;
    double q = -0.5 * b;
    x_0 = c / q;
    x_1 = q / a;
  }
}


vector<TLorentzVector> tauMomentumSolutions(const TVector3 &tauDir, const TLorentzVector &a1LV, bool isReal)
{
  double tauMass = 1.77682; // GeV
  double thetaGJ = a1LV.Angle(tauDir); // TVector4.Angle(TVector 3)
  TVector3 tauDirUnit = tauDir.Unit();
  double a = 4.0 * ( a1LV.M2() + pow(a1LV.P(), 2.0)  * pow(sin(thetaGJ), 2.0) ) ;
  double b = -4.0 * ( a1LV.M2() + pow(tauMass, 2.0) ) * a1LV.P() * cos(thetaGJ) ;
  double c = 4.0 * pow(tauMass, 2.0)  * ( a1LV.M2() + pow(a1LV.P(), 2.0) ) - pow( ( a1LV.M2() + pow(tauMass, 2.0) ) , 2.0) ;
  double tauMomentumSmall, tauMomentumLarge;
  quadratic_alternate(a, b, c, isReal, tauMomentumSmall, tauMomentumLarge);
  double tauMomentumMean = (tauMomentumSmall + tauMomentumLarge) / 2.0;
  TLorentzVector tauSmall, tauLarge, tauMean;
  vector<TLorentzVector> solutions;
  tauSmall.SetPxPyPzE(tauMomentumSmall * tauDirUnit.Px(),
                      tauMomentumSmall * tauDirUnit.Py(),
                      tauMomentumSmall * tauDirUnit.Pz(),
                      sqrt( pow(tauMass, 2.0) + pow(tauMomentumSmall, 2.0) ) );
  solutions.push_back(tauSmall);
  tauLarge.SetPxPyPzE(tauMomentumLarge * tauDirUnit.Px(),
                      tauMomentumLarge * tauDirUnit.Py(),
                      tauMomentumLarge * tauDirUnit.Pz(),
                      sqrt( pow(tauMass, 2.0) + pow(tauMomentumLarge, 2.0) ) );
  solutions.push_back(tauLarge);
  tauMean.SetPxPyPzE(tauMomentumMean * tauDirUnit.Px(),
                     tauMomentumMean * tauDirUnit.Py(),
                     tauMomentumMean * tauDirUnit.Pz(),
                     sqrt( pow(tauMass, 2.0) + pow(tauMomentumMean, 2.0) ) );
  solutions.push_back(tauMean);
  return solutions;
}
		


double GetThetaGJ(const TVector3 &tauDir, const TLorentzVector &a1LV)
{
  return a1LV.Angle(tauDir);
}




double GetThetaGJMax(const TLorentzVector &a1LV)
{
  double tauMass = 1.77682; //GeV
  return asin( ( pow(tauMass,2) - a1LV.M2() ) / ( 2 * tauMass * a1LV.P() ) );
}

bool isPhysical(const TVector3 &tauDir, const TLorentzVector &a1LV)
{
  return ( GetThetaGJ(tauDir, a1LV)  <= GetThetaGJMax(a1LV) ) ? true : false;
}  



// =====================================================================
// Return an integer from [a,b] (a<b)
// =====================================================================
// Initialize function rand() one time with : "srand(time(NULL));" 
int rand_a_b(const int a, const int b)
{
  return rand() % (b + 1 - a) + a;
}

// =====================================================================
// Return a float from [a,b] (a<b)
// =====================================================================
double frand_a_b(const double a, const double b)
{
  return ( rand() / (double)RAND_MAX ) * (b - a) + a;
}

// =====================================================================
// Resolution cone 
// =====================================================================
TLorentzVector RandomT4VectorInsideResolutionCone(const double res, const TLorentzVector &tau)
{ // resolution res in microns !!!!!!!!!
  double tauFlightDistance = 87.03 * tau.Beta() * tau.Gamma(); // in microns
  double coneAngleMax = atan(res / tauFlightDistance);
  // Generate points on the spherical cap around the north pole
  double u = frand_a_b(cos(coneAngleMax) , 1);
  double theta = frand_a_b(0 , 2 * TMath::Pi());
  double x = sqrt(1 - u * u) * cos(theta);
  double y = sqrt(1 - u * u) * sin(theta);
  double z = u;
  TVector3 randomVector(x,y,z);
  // Find the rotation axis and rotation angle
  TVector3 tauDirUnit = tau.Vect() * (1 / tau.Vect().Mag());
  TVector3 zAxis(0,0,1); 
  double rotationAngle = acos(tauDirUnit.Dot(zAxis));
  TVector3 rotationAxis = zAxis.Cross(tauDirUnit);
  TVector3 rotationAxisUnit = rotationAxis * (1 / rotationAxis.Mag());

  randomVector.Rotate(rotationAngle, rotationAxisUnit); // rotate around rotationAxisUnit
  
  TVector3 T3VectorInside = randomVector * tau.Vect().Mag();
  TLorentzVector T4VectorInside(T3VectorInside, tau.E()) ;
  return T4VectorInside;

}


TVector3 RandomT3VectorInsideResolutionCone(const double res, const double tauFlightDistance, const TVector3 &tau)
{ // resolution res in mm !!!!!!!!!!!
  double coneAngleMax = atan(res / tauFlightDistance);
  // Generate points on the spherical cap around the north pole
  double u = frand_a_b(cos(coneAngleMax * TMath::Pi() / 180) , 1);
  double theta = frand_a_b(0 , 2 * TMath::Pi());
  double x = sqrt(1 - u * u) * cos(theta);
  double y = sqrt(1 - u * u) * sin(theta);
  double z = u;
  TVector3 randomVector(x,y,z);
  // Find the rotation axis and rotation angle
  TVector3 tauDirUnit = tau * (1 / tau.Mag());
  TVector3 zAxis(0,0,1); 
  double rotationAngle = acos(tauDirUnit.Dot(zAxis));
  TVector3 rotationAxis = zAxis.Cross(tauDirUnit);
  TVector3 rotationAxisUnit = rotationAxis * (1 / rotationAxis.Mag());

  randomVector.Rotate(rotationAngle, rotationAxisUnit); // rotate around rotationAxisUnit
  
  TVector3 T3VectorInside = randomVector * tau.Mag();
  return T3VectorInside;

}


// =====================================================================
// Best couple of solution with Z0 mass constraint
// =====================================================================

void tauPairConstraint(const TVector3 &tau1Dir, const TLorentzVector &a1LV1, bool is1Real, TLorentzVector &tau1PairConstraintLV, 
		       const TVector3 &tau2Dir, const TLorentzVector &a1LV2, bool is2Real, TLorentzVector &tau2PairConstraintLV)
{
  double Hmass = 125.10; //91.1876; // GeV
  vector<TLorentzVector> tau1Solutions, tau2Solutions;
  tau1Solutions = tauMomentumSolutions(tau1Dir, a1LV1, is1Real);
  tau2Solutions = tauMomentumSolutions(tau2Dir, a1LV2, is2Real);
  
  int const numberPairs(4);
  double massPairs[numberPairs];
  double massConstraint[numberPairs];

  massPairs[0] = ( tau1Solutions.at(0) + tau2Solutions.at(0) ).M(); // tau1:small solution -- tau2:small solution
  massPairs[1] = ( tau1Solutions.at(0) + tau2Solutions.at(1) ).M(); // tau1:small solution -- tau2:large solution
  massPairs[2] = ( tau1Solutions.at(1) + tau2Solutions.at(0) ).M(); // tau1:large solution -- tau2:small solution
  massPairs[3] = ( tau1Solutions.at(1) + tau2Solutions.at(1) ).M(); // tau1:large solution -- tau2:large solution
  
  massConstraint[0] = fabs( massPairs[0] - Hmass );
  massConstraint[1] = fabs( massPairs[1] - Hmass );
  massConstraint[2] = fabs( massPairs[2] - Hmass );
  massConstraint[3] = fabs( massPairs[3] - Hmass );
  
  double bestMassConstraint = massConstraint[0];
  double bestCouple = 0;
  for(int i=1; i<4; i++)
    {
      if (massConstraint[i] < bestMassConstraint) 
	{
	  bestMassConstraint = massConstraint[i];
	  bestCouple = i;
	}
    }

  if(bestCouple == 0)
    {
      tau1PairConstraintLV = tau1Solutions.at(0);
      tau2PairConstraintLV = tau2Solutions.at(0);
    }
  
  if(bestCouple == 1)
    {
      tau1PairConstraintLV = tau1Solutions.at(0);
      tau2PairConstraintLV = tau2Solutions.at(1);
    }
  
  if(bestCouple == 2)
    {
      tau1PairConstraintLV = tau1Solutions.at(1);
      tau2PairConstraintLV = tau2Solutions.at(0);
    }
  
  if(bestCouple == 3)
    {
      tau1PairConstraintLV = tau1Solutions.at(1);
      tau2PairConstraintLV = tau2Solutions.at(1);
    }
  
}


vector<TLorentzVector> tauPairMomentumSolutions(const TVector3 &tau1Dir, const TLorentzVector &a1RefitLV1, const TLorentzVector &a1LV1, bool is1Real, const TVector3 &tau2Dir, const TLorentzVector &a1RefitLV2, const TLorentzVector &a1LV2, bool is2Real, bool isRefit)
{
  vector<TLorentzVector> solutions, tau1Solutions, tau2Solutions;
  TLorentzVector tau1PairConstraintLV, tau2PairConstraintLV;

  if(isRefit)
    {
      tauPairConstraint(tau1Dir, a1RefitLV1, is1Real, tau1PairConstraintLV, tau2Dir, a1RefitLV2, is2Real, tau2PairConstraintLV);
      tau1Solutions = tauMomentumSolutions(tau1Dir, a1RefitLV1, is1Real);
      tau2Solutions = tauMomentumSolutions(tau2Dir, a1RefitLV2, is2Real);
    }
  else
    {
      tauPairConstraint(tau1Dir, a1LV1, is1Real, tau1PairConstraintLV, tau2Dir, a1LV2, is2Real, tau2PairConstraintLV);
      tau1Solutions = tauMomentumSolutions(tau1Dir, a1LV1, is1Real);
      tau2Solutions = tauMomentumSolutions(tau2Dir, a1LV2, is2Real);
    }
  
  std::copy(tau1Solutions.begin(), tau1Solutions.end(), std::back_inserter(solutions));
  solutions.push_back(tau1PairConstraintLV);
  solutions.insert(solutions.end(), tau2Solutions.begin(), tau2Solutions.end());
  solutions.push_back(tau2PairConstraintLV);
  return solutions;
}

TVector3 GetRefittedPV(std::vector<size_t> hashes, TVector3 PVNominal, std::vector<double> PVRefit_X , std::vector<double> PVRefit_Y, std::vector<double> PVRefit_Z ,std::vector<size_t> VertexHash1, std::vector<size_t> VertexHash2, bool &isRefit)
{ 
  // find the vertex among the refitted vertices
  for (unsigned int ivertex =0; ivertex<PVRefit_X.size(); ivertex++){
    size_t selectionHash = 0;
    boost::hash_combine(selectionHash, VertexHash1.at(ivertex));
    boost::hash_combine(selectionHash, VertexHash2.at(ivertex));
    if ( std::find(hashes.begin(), hashes.end(), selectionHash) != hashes.end() ){
      //cout<<"NoBS Hash Matching!!!"<<endl;
      isRefit=true;
      return TVector3(PVRefit_X.at(ivertex), PVRefit_Y.at(ivertex), PVRefit_Z.at(ivertex));
    }
  } // loop over refitted vertices collection
  isRefit=false;
  return PVNominal;
}


TVector3 GetRefittedPV(std::vector<size_t> hashes, TVector3 PVNominal, std::vector<double> PVRefit_X , std::vector<double> PVRefit_Y, double PV_Z ,std::vector<size_t> VertexHash1, std::vector<size_t> VertexHash2, bool &isRefit)
{ 
  // find the vertex among the refitted vertices
  for (unsigned int ivertex =0; ivertex<PVRefit_X.size(); ivertex++){
    size_t selectionHash = 0;
    boost::hash_combine(selectionHash, VertexHash1.at(ivertex));
    boost::hash_combine(selectionHash, VertexHash2.at(ivertex));
    if ( std::find(hashes.begin(), hashes.end(), selectionHash) != hashes.end() ){
      //cout<<"NoBS Hash Matching!!!"<<endl;
      isRefit=true;
      return TVector3(PVRefit_X.at(ivertex), PVRefit_Y.at(ivertex), PV_Z);
    }
  } // loop over refitted vertices collection
  isRefit=false;
  return PVNominal;
}
