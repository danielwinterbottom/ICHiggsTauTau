#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "boost/lexical_cast.hpp"
#include <boost/algorithm/string.hpp>
#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"
#include "TMatrixD.h"
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/SVFitService.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"


using boost::lexical_cast;



int main(int argc, char* argv[]){
  typedef ROOT::Math::PtEtaPhiEVector Vector;

  if (argc != 2){
    std::cerr << "Need 1 arg: <input>" << std::endl;
    exit(1);
  }

  std::string input_file = argv[1];
  std::ifstream file;
  file.open(input_file.c_str());
  if (!file.is_open()) {
    std::cerr << "Error: File " << input_file << " cannot be opened." << std::endl;
    exit(1);
  }
  std::string line = "";
  ic::SVFitService svfit_service;
  while(std::getline(file, line)) {//while loop through lines
    std::vector<std::string> vec_format;
    boost::split(vec_format, line, boost::is_any_of(":"));
    if (vec_format.size() != 18) {
      std::cerr << "Error: Wrong number of elements" << std::endl;
      exit(1);
    }
    std::string runevt = vec_format[0];
    unsigned mode = lexical_cast<unsigned>(vec_format[1]);
    double l1_pt = lexical_cast<double>(vec_format[2]); 
    double l1_eta = lexical_cast<double>(vec_format[3]); 
    double l1_phi = lexical_cast<double>(vec_format[4]); 
    double l1_energy = lexical_cast<double>(vec_format[5]);
    ic::Candidate c1;
    c1.set_vector(Vector(l1_pt,l1_eta,l1_phi,l1_energy));
    double l2_pt = lexical_cast<double>(vec_format[6]); 
    double l2_eta = lexical_cast<double>(vec_format[7]); 
    double l2_phi = lexical_cast<double>(vec_format[8]); 
    double l2_energy = lexical_cast<double>(vec_format[9]); 
    ic::Candidate c2;
    c2.set_vector(Vector(l2_pt,l2_eta,l2_phi,l2_energy));
    double met_pt = lexical_cast<double>(vec_format[10]); 
    double met_eta = lexical_cast<double>(vec_format[11]); 
    double met_phi = lexical_cast<double>(vec_format[12]); 
    double met_energy = lexical_cast<double>(vec_format[13]); 
    double xx_sig = lexical_cast<double>(vec_format[14]); 
    double yx_sig = lexical_cast<double>(vec_format[15]); 
    double xy_sig = lexical_cast<double>(vec_format[16]); 
    double yy_sig = lexical_cast<double>(vec_format[17]);
    ic::Met met;
    met.set_vector(Vector(met_pt,met_eta,met_phi,met_energy));
    met.set_xx_sig(xx_sig);
    met.set_yx_sig(yx_sig);
    met.set_xy_sig(xy_sig);
    met.set_yy_sig(yy_sig);
    double mass = 0.0;
    if (mode == 0 || mode == 1) {
      mass = svfit_service.SVFitMassLepHad(&c1, &c2, &met);
    } else if (mode == 2) {
      mass = svfit_service.SVFitMassLepLep(&c1, &c2, &met);
    }

    std::cout << runevt << ":" << mass << std::endl;
  }

  return 0;
}




