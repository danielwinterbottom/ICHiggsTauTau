// taken from: https://github.com/CMS-HTT/HiggsCPinTauDecays/blob/master/ImpactParameter/src/ImpactParameter.cc


#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/ImpactParameter.h"

#include "TMatrix.h"
#include "Math/Functor.h"
#include "Math/BrentMinimizer1D.h"
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "TError.h"


//typedef ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<float> > RMPoint;

// Variables used during the fit
// The minimizer doesn't like it if these are members of the class and thus
// they are declared here as global
// Possible Fix: Give them as arguments to the functions instead of using
// global variables start with a capital letter
double MagneticField = -999;
TVector3 ReferencePoint(-999,-999,-999);
TVector3 PrimaryVertex(-999,-999,-999);
double QOverP = -999;
double Lambda = -999;
double Phi = -999;
double V_z = -999;

double ImpactParameter::CalculateIPSignificanceHelical(TVector3 IP, ROOT::Math::SMatrix<float,3,3, ROOT::Math::MatRepStd< float, 3, 3 >> IPCovariance){
	// Conversion from TVector3 to SVector
	ROOT::Math::SVector<float, 3> IPNormalized(IP(0), IP(1), IP(2));
	// Jacobi Matrix to get uncertainty on the magnitude of the IP
	IPNormalized = IPNormalized.Unit();
	double errorIP = sqrt( ROOT::Math::Dot(IPNormalized, IPCovariance * IPNormalized) );
	double IPSignificance = IP.Mag() / errorIP;
	return IPSignificance;
}

double ImpactParameter::CalculateIPSignificanceTangential(TVector3 IP, SMatrixSym3D PVCovariance){
	double sigmaIP = ImpactParameter::CalculatePCADifferece(PVCovariance, IP);
	return IP.Mag()/sigmaIP;
}

double Sign(double x){
	return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}

double getRadius(double qOverP, double lambda, double B){
	return TMath::Sin( TMath::PiOver2() - lambda ) / (B * qOverP); //Radius;
}

double PointOnHelix_x(double x, double qOp, double l, double p){
	double pars[5];
	pars[0] = qOp;
	pars[1] = l;
	pars[2] = p;
	pars[3] = ReferencePoint.x();
	pars[4] = MagneticField;
	return pars[3] + TMath::Cos(pars[1]) / pars[4] / pars[0] * Sign(pars[0]) * (TMath::Sin(pars[2]) * (1 - TMath::Cos(x)) + TMath::Cos(pars[2]) * TMath::Sin(x));
}

double PointOnHelix_y(double x, double qOp, double l, double p){
	double pars[5];
	pars[0] = qOp;
	pars[1] = l;
	pars[2] = p;
	pars[3] = ReferencePoint.y();
	pars[4] = MagneticField;
	return pars[3] + TMath::Cos(pars[1]) / pars[4] / pars[0] * Sign(pars[0]) * (TMath::Sin(pars[2]) * TMath::Sin(x) - TMath::Cos(pars[2]) * (1 - TMath::Cos(x)));
	// return pars[3] - TMath::Cos(pars[1]) / pars[4] / pars[0] * Sign(pars[0]) * (TMath::Cos(pars[2]) * (1 - TMath::Cos(x)) - TMath::Sin(pars[2]) * TMath::Sin(x));
}

double PointOnHelix_z(double x, double qOp, double l){
	double pars[4];
	pars[0] = qOp;
	pars[1] = l;
	pars[2] = ReferencePoint.z();
	pars[3] = MagneticField;
	return pars[2] + x * TMath::Sin(pars[1]) / pars[3] / std::abs(pars[0]);
}

double DistancePVToHelix(double x, double qOp, double l, double p){
	return pow(PointOnHelix_x(x, qOp, l, p) - PrimaryVertex[0], 2) + pow(PointOnHelix_y(x, qOp, l, p) - PrimaryVertex[1], 2) + pow(PointOnHelix_z(x, qOp, l) - PrimaryVertex[2], 2);
}

double minuitFunction(const double *xx ){
	long double x = xx[0];
	return DistancePVToHelix(x,QOverP,Lambda,Phi);
}

TVector3 ImpactParameter::CalculatePCA(double B, std::vector<float> h_param, RMPoint ref, RMPoint PrV){
	QOverP = h_param[0];
	Lambda = h_param[1];
	Phi = h_param[2];
	MagneticField = B;
	ReferencePoint.SetXYZ(ref.x(),ref.y(),ref.z());
	PrimaryVertex.SetXYZ(PrV.x(),PrV.y(),PrV.z());

	this->SetRecoDxy(h_param[3]);
	this->SetRecoDsz(h_param[4]);

	//minimizing the distance between the helix and the primary vertex PV
	double x_best = 0.0;
	ROOT::Math::Minimizer *min = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Combined");
	ROOT::Math::Functor f(&minuitFunction,1);
    gErrorIgnoreLevel = kError;
    // min->SetPrintLevel(0); // doesn't work ???
	min->SetFunction(f);
	min->SetVariable(0,"x",1e-14, 1e-16);
	min->SetTolerance(1e-15);
	min->Minimize();

	const double *xs = min->X();
	x_best=xs[0];
	this->SetXBest(x_best);
        delete min;
	TVector3 res(PointOnHelix_x(x_best, QOverP, Lambda, Phi) - PrimaryVertex.x(), PointOnHelix_y(x_best, QOverP, Lambda, Phi) - PrimaryVertex.y(), PointOnHelix_z(x_best, QOverP, Lambda) - PrimaryVertex.z());
	return res;
}

double ImpactParameter::CalculatePCADifferece(SMatrixSym3D cov_PV, TVector3 IP)
{
	TVector3 n = IP.Unit();
	const int dim=3;
	ROOT::Math::SVector<double, dim> Sn(n.x(),n.y(),n.z());
	double alpha = TMath::Sqrt((ROOT::Math::Dot(Sn,cov_PV*Sn)));
	return alpha;
}


ROOT::Math::SMatrix<double,3,3, ROOT::Math::MatRepStd< double, 3, 3 >> ImpactParameter::CalculatePCACovariance(ROOT::Math::SMatrix<double,5,5, ROOT::Math::MatRepSym<double,5>> helixCovariance, SMatrixSym3D primaryVertexCovariance)
{
	double Dxy = GetRecoDxy();
	double Dsz = GetRecoDsz();
	double xBest = GetXBest();

	ROOT::Math::SMatrix<double,3,5, ROOT::Math::MatRepStd< double, 3, 5 >> jacobiPointOnHelix;
	// Derivatives of pointOnHelix with respect to
	// qOverP
	jacobiPointOnHelix(0, 0) =- ((TMath::Cos(Lambda) * Sign(QOverP) * ((1 - TMath::Cos(xBest)) * TMath::Sin(Phi) + TMath::Cos(Phi) * TMath::Sin(xBest))) / (MagneticField * pow(QOverP,2)));
	jacobiPointOnHelix(1, 0) =  (TMath::Cos(Lambda) * Sign(QOverP) * (TMath::Cos(Phi) * (1 - TMath::Cos(xBest)) - TMath::Sin(Phi) * TMath::Sin(xBest))) / (MagneticField * pow(QOverP,2));
	jacobiPointOnHelix(2, 0) =- ((xBest * TMath::Sin(Lambda)) / (MagneticField * pow(QOverP,2))) * Sign(QOverP);
	// jacobiPointOnHelix(2, 0) =- ((xBest * TMath::Sin(Lambda)) / (MagneticField * pow(QOverP,2)));
	// Lambda
	jacobiPointOnHelix(0, 1) =- ((Sign(QOverP) * TMath::Sin(Lambda) * ((1 - TMath::Cos(xBest)) * TMath::Sin(Phi) + TMath::Cos(Phi) * TMath::Sin(xBest))) / (MagneticField * QOverP));
	jacobiPointOnHelix(1, 1) =  (Sign(QOverP) * TMath::Sin(Lambda) * (TMath::Cos(Phi) * (1 - TMath::Cos(xBest)) - TMath::Sin(Phi) * TMath::Sin(xBest))) / (MagneticField * QOverP);
	jacobiPointOnHelix(2, 1) =  (xBest * TMath::Cos(Lambda)) / (MagneticField * QOverP) * Sign(QOverP) + Dsz / TMath::Cos(Lambda) * TMath::Tan(Lambda);
	// Phi
	jacobiPointOnHelix(0, 2) = - (TMath::Cos(Phi) * Dxy) + (TMath::Cos(Lambda) * Sign(QOverP) * (TMath::Cos(Phi) * (1 - TMath::Cos(xBest)) - TMath::Sin(Phi) * TMath::Sin(xBest))) / (MagneticField * QOverP);
	jacobiPointOnHelix(1, 2) = - (Dxy * TMath::Sin(Phi)) - (TMath::Cos(Lambda) * Sign(QOverP) * (-((1 - TMath::Cos(xBest)) * TMath::Sin(Phi)) - TMath::Cos(Phi) * TMath::Sin(xBest))) / (MagneticField * QOverP);
	jacobiPointOnHelix(2, 2) = 0;
	// dxy
	jacobiPointOnHelix(0, 3) = - TMath::Sin(Phi);
	jacobiPointOnHelix(1, 3) =   TMath::Cos(Phi);
	jacobiPointOnHelix(2, 3) = 0;
	// dsz
	jacobiPointOnHelix(0, 4) = 0;
	jacobiPointOnHelix(1, 4) = 0;
	jacobiPointOnHelix(2, 4) = 1 / TMath::Cos(Lambda);

	ROOT::Math::SMatrix<double,3,3, ROOT::Math::MatRepStd< double, 3, 3 >> pointOnHelixCovariance = jacobiPointOnHelix * helixCovariance * ROOT::Math::Transpose(jacobiPointOnHelix);
	ROOT::Math::SMatrix<double,6,6, ROOT::Math::MatRepStd< double, 6, 6 >> pointOnHelixPVCovariance;
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 6; j++){
			switch( ((i < 3) * (j < 3))? 1:0 | ((i >= 3) * (j>=3))? 2:0 ){
				case 0: break;
				case 1: pointOnHelixPVCovariance(i,j) = pointOnHelixCovariance(i, j); break;
				case 2: pointOnHelixPVCovariance(i,j) = primaryVertexCovariance(i-3, j-3); break;
			}
		}
	}

	ROOT::Math::SMatrix<double,3,6, ROOT::Math::MatRepStd< double, 3, 6 >> jacobiIP;
	// Derivative of the IP with respect to
	// PointOnHelix
	jacobiIP(0, 0) = 1;
	jacobiIP(1, 1) = 1;
	jacobiIP(2, 2) = 1;
	// PV
	jacobiIP(0, 3) = -1;
	jacobiIP(1, 4) = -1;
	jacobiIP(2, 5) = -1;

	ROOT::Math::SMatrix<double,3,3, ROOT::Math::MatRepStd< double, 3, 3 >> CovIP = jacobiIP * pointOnHelixPVCovariance * Transpose(jacobiIP);
	return CovIP;
}

// Calculate shortest distance between the track and a point - gen level.
// When distance between track and PV -> shortest distance is the IP vector.
TVector3 ImpactParameter::CalculateShortestDistance(RMFLV p4, RMPoint vertex, RMPoint* pv){

	TVector3 k, p, IP;

	if(vertex.x() != 0 && vertex.y() != 0 && vertex.z() != 0) {
		k.SetXYZ(vertex.x() - pv->x(), vertex.y() - pv->y(), vertex.z() - pv->z());
	}
	else k.SetXYZ(-999, -999, -999);

	p.SetXYZ(p4.Px(), p4.Py(), p4.Pz());

	if ( p.Mag() != 0 && k.x() != -999 && (k.x()!=0 && k.y()!=0 && k.z()!=0) ) {
		IP = k - (p.Dot(k) / p.Mag2()) * p;
	}
	else IP.SetXYZ(-999, -999, -999);

	return IP;

}

// Calculate the shortest distance between a track and a point - reco level.
// When distance between track and PV => shortest distance is the IP vector.
TVector3 ImpactParameter::CalculateShortestDistance(RMFLV p4, RMPoint ref, RMPoint pv){

	TVector3 k, p, IP;
	k.SetXYZ(ref.x() - pv.x(), ref.y() - pv.y(), ref.z() - pv.z());

	p.SetXYZ(p4.Px(), p4.Py(), p4.Pz());

	if (p.Mag() != 0) IP = k - (p.Dot(k) / p.Mag2()) * p;
	else IP.SetXYZ(-999, -999, -999);

	return IP;

}

